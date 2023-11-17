#include <Arduino.h>

// Rob Tilaart's MCP4725 library https://github.com/RobTillaart/MCP4725
#include <MCP4725.h>
// Bolder flight sbus library: https://github.com/bolderflight/sbus
#include "CandT.hpp"
#include "Wire.h"
#include "joy.hpp"
#include "powerchairout.hpp"
#include "sbus.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <INA226.h>
#include <PID_v1.h>
#include <max6675.h>

/**
 * firmware for the Cislune Excavators BTILC rover.
 *
 * TODOs:
 * - need to have some startup safety checkout and not enable anything if that fails
 * - error LEDs
 * - addressable LED control
 * - dig mode with feedback
 * - fault modes and fault clearing
 *
 */

// ------------- Defines ----------------
#define REVA

#define SBUS_PIN 35
#define SBUS_THROTTLE_CHANNEL 1
#define SBUS_STEERING_CHANNEL 0
// #define SBUS_THROTTLE_LEFT_CHANNEL 0
// #define SBUS_THROTTLE_RIGHT_CHANNEL 2
// TODO(HEIDT) this is a nasty hack!  The direction is switched for some reason, fix!
#define SBUS_THROTTLE_LEFT_CHANNEL 2
#define SBUS_THROTTLE_RIGHT_CHANNEL 0

#define SBUS_DISARM_CHANNEL 10

// Joystick management
// I2C addresses of DACs
#define YDAC_ADDR 0x60
#define XDAC_ADDR 0x61
#define JOY_KEEPALIVE_TIME (1000UL * 60UL) // keepalive every 60 seconds
#define JOY_KEEPALIVE_AMOUNT 0.05

// watchdog pin
#define WATCHDOG_PIN 6

// Timing and status
#define LOOP_PERIOD 25 // attempt to run at 20ms intervals (e.g. 40 hz)
#define LED_DELAY 250  // minimum time between LED blinks

#ifdef REVA
#define LED_STAT_PIN 11
#define LED_ERR_PIN 10
#define NEOPIXEL_PIN 12
#else
#define LED_STAT_PIN 26
#define LED_ERR_PIN 9
#define NEOPIXEL_PIN 33
#endif

#define NUM_NEOPIXELS 18
#define BRIGHTNESS 255
#define NEOPIXEL_STEP_TIME 200
#define NEOPIXEL_BAR_0_OFFSET 0
#define NEOPIXEL_BAR_1_OFFSET 10
#define NEOPIXEL_HIGHBEAM_OFFSET 8

// ------------- Declarations ---------------

MCP4725 y_dac(YDAC_ADDR, &Wire);
MCP4725 x_dac(XDAC_ADDR, &Wire);

bfs::SbusRx sbus_rx(&Serial8);
bfs::SbusData data;
// JoyToJoyTranslator translator(&sbus_rx, SBUS_THROTTLE_CHANNEL, SBUS_STEERING_CHANNEL);
DifferentialToJoyTranslator translator(&sbus_rx, SBUS_THROTTLE_LEFT_CHANNEL,
                                       SBUS_THROTTLE_RIGHT_CHANNEL);
JoystickEmu powerchair_out(XDAC_ADDR, YDAC_ADDR, &Wire);
// LED Blink type definitions for errors
const uint8_t led_status_err[] = {1, 0, 1, 0, 1, 0, 1, 0};
unsigned long last_blink = 0;
int led_status_index = 0;
bool last_led_status = false;

// Addressable LEDs
Adafruit_NeoPixel pixels(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRBW + NEO_KHZ800);
uint8_t pixel_count = 0;
unsigned long last_pixel_update = 0;
bool highbeams_on = false;

String debug_data;


// ------------- Function prototypes -----------

bool is_close(float a, float b, float MAX_DIFF = 0.001) {
    if (abs(a - b) < MAX_DIFF) {
        return true;
    } else {
        return false;
    }
}

void setup_LEDS() {
    // pinMode(LED_STAT_PIN, OUTPUT);
    // pinMode(LED_ERR_PIN, OUTPUT);
    // pixels.begin();
    // pixels.show();
    // pixels.setBrightness(0);
    // pixels.fill(0xFFFFFFFF);
    // pixels.show();
    // delay(500);
    // pixels.clear();
    // pixels.show();
}

void setup_joy() {
    powerchair_out.init();
    sbus_rx.Begin();
}

void setup_watchdog() {
    // pinMode(WATCHDOG_PIN, OUTPUT);
    // digitalWrite(WATCHDOG_PIN, LOW);
}

void setup() {
    Serial.begin(2000000);
    setup_joy();
    setup_LEDS();
}

void handle_joystick(drive_data_t *drive_data) {
    static unsigned long last_nonzero_command = millis();
    float joy_x = 0.0;
    float joy_y = 0.0;
    drive_data->has_error = false;
    drive_data->error_message = F("");

    // If the servo's aren't ready, just write to middle
    // TODO(Heidt) should probably fire a timeout or reset to middle when no signal in
    // TODO(Heidt) handle the failsafe, would be nice to setup for this
    if (sbus_rx.Read()) {
        translator.get_sbus_joy(joy_x, joy_y);
        bfs::SbusData data = sbus_rx.data();
        drive_data->status = data.ch[SBUS_DISARM_CHANNEL];
        // never drive if the disarm switch is on

        // if the joystick hasn't moved in a certian time, just tweak a bit to keep the controller
        // on
        if (!is_close(joy_x, 0.0) || !is_close(joy_y, 0.0)) {
            last_nonzero_command = millis();
        }

        if (millis() - last_nonzero_command > JOY_KEEPALIVE_TIME) {
            joy_x = JOY_KEEPALIVE_AMOUNT;
            last_nonzero_command = millis();
        }
        powerchair_out.powerchairout(joy_x, joy_y);
        drive_data->x = joy_x;
        drive_data->y = joy_y;
    } else {
        powerchair_out.powerchairout(0, 0);
        drive_data->has_error = true;
        drive_data->error_message = F("No data from SBUS,");
    }
}

void handle_telem(system_data_t *system_data) {
    StaticJsonDocument<1024> doc;
    doc["type"] = ID_TELEM;
    JsonObject drive_telem = doc.createNestedObject("drive_telem");
    JsonObject drum_telem_fore = doc.createNestedObject("drum_telem_fore");
    JsonObject drum_telem_aft = doc.createNestedObject("drum_telem_aft");
    JsonObject system_telem = doc.createNestedObject("system_telem");

    drive_telem["joy_x"] = system_data->drive_data.x;
    drive_telem["joy_y"] = system_data->drive_data.y;
    drive_telem["drive_status"] = system_data->drive_data.status;

    drum_telem_fore["speed"] = system_data->drum_data.fore_drum_speed;
    drum_telem_fore["position"] = fmod(system_data->drum_data.fore_drum_position, 1.0);
    drum_telem_fore["height"] = system_data->lift_data.fore_lift_position;
    drum_telem_fore["torque"] = system_data->drum_data.fore_drum_torque;
    drum_telem_fore["average_torque"] = system_data->drum_data.fore_drum_average_torque;
    drum_telem_fore["current"] = system_data->drum_data.fore_drum_current;
    drum_telem_fore["lift_current"] = system_data->lift_data.fore_current;

    drum_telem_aft["speed"] = system_data->drum_data.aft_drum_speed;
    drum_telem_aft["position"] = fmod(system_data->drum_data.aft_drum_position, 1.0);
    drum_telem_aft["height"] = system_data->lift_data.aft_lift_position;
    drum_telem_aft["torque"] = system_data->drum_data.aft_drum_torque;
    drum_telem_aft["average_torque"] = system_data->drum_data.aft_drum_average_torque;
    drum_telem_aft["current"] = system_data->drum_data.aft_drum_current;
    drum_telem_aft["lift_current"] = system_data->lift_data.aft_current;

    JsonArray thermal_array = doc.createNestedArray("thermal_telem");
    for (int i = 0; i < NUM_THERMAL_SENSORS; i++) {
        JsonObject thermal_data = thermal_array.createNestedObject();
        thermal_data["name"] = thermal_sensor_names[i];
        thermal_data["temperature"] = system_data->thermistors.temperatures[i];
    }

    // motor temperatures
    JsonObject fore_motor_thermal = thermal_array.createNestedObject();
    fore_motor_thermal["name"] = F("FORE_MOTOR_TEMP");
    fore_motor_thermal["temperature"] = system_data->drum_data.fore_drum_motor_temperature;

    JsonObject aft_motor_thermal = thermal_array.createNestedObject();
    aft_motor_thermal["name"] = F("AFT_MOTOR_TEMP");
    aft_motor_thermal["temperature"] = system_data->drum_data.aft_drum_motor_temperature;

    system_telem["system_voltage"] = system_data->lift_data.voltage;
    system_telem["system_current"] = system_data->power_data.current_draw;
    system_telem["system_power"] = system_data->power_data.total_power_usage;
    system_telem["drive_errors"] = "";
    system_telem["drum_errors"] = "";
    system_telem["lift_errors"] = "";
    system_telem["command_errors"] = "";
    system_telem["system_mode"] = "NONE";
    system_telem["hardfault"] = false;
    system_telem["debug_data"] = debug_data;

    if (system_data->drive_data.has_error) {
        system_telem["drive_errors"] = system_data->drive_data.error_message;
    }
    if (system_data->drum_data.has_error) {
        system_telem["drum_errors"] = system_data->drum_data.error_message;
    }
    if (system_data->lift_data.has_error) {
        system_telem["lift_errors"] = system_data->lift_data.error_message;
    }
    if (system_data->command_data.has_error) {
        system_telem["command_errors"] = system_data->command_data.error_message;
    }
    if (debug_data != "") {
        system_telem["debug_data"] = debug_data;
    }
    // serializeJson(doc, Serial);
    // Serial.println();
}

void handle_LEDS() {
    // TODO(HEIDT) create enum of LED statuses
    last_led_status = !last_led_status;
    digitalWrite(LED_STAT_PIN, last_led_status);

    if (millis() - last_pixel_update > NEOPIXEL_STEP_TIME) {
        pixels.clear();
        pixels.setPixelColor(pixel_count + NEOPIXEL_BAR_0_OFFSET, pixels.Color(0, 255, 0, 0));
        pixels.setPixelColor(NEOPIXEL_BAR_1_OFFSET + (7 - pixel_count), pixels.Color(255, 0, 0, 0));

        pixel_count++;
        pixel_count %= 8;
        if (highbeams_on) {
            pixels.setPixelColor(NEOPIXEL_HIGHBEAM_OFFSET, pixels.Color(255, 255, 255, 255));
            pixels.setPixelColor(NEOPIXEL_HIGHBEAM_OFFSET + 1, pixels.Color(255, 255, 255, 255));
        }

        pixels.show();
        last_pixel_update = millis();
    }

    // if (millis() - last_blink > LED_DELAY) {
    //     led_status_index++;
    //     led_status_index %= (sizeof(led_status_err) / sizeof(led_status_err[0]));
    //     digitalWrite(LED_ERR_PIN, led_status_err[led_status_index]);
    //     last_blink = millis();
    // }
}

void delay_loop_period(unsigned long loop_end_time, unsigned long loop_start_time) {
    if (loop_end_time - loop_start_time < LOOP_PERIOD) {
        unsigned long time_to_delay = LOOP_PERIOD - (loop_start_time - loop_end_time);
        delay(time_to_delay);
    }
}

void handle_setup_data(system_data_t *data) {
    data->drum_data.fore_drum_average_torque = 0.0;
    data->drum_data.aft_drum_average_torque = 0.0;
}

void loop() {
    unsigned long loop_start_time = millis();
    static bool data_initialized = false;
    static system_data_t system_data;
    debug_data = String("");
    
    if(!data_initialized) {
        handle_setup_data(&system_data);
        data_initialized = true;
    }
    handle_joystick(&system_data.drive_data);
    handle_LEDS();

    handle_telem(&system_data);

    unsigned long loop_end_time = millis();
    delay_loop_period(loop_end_time, loop_start_time);
}
