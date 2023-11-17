#ifndef CANDT_H_
#define CANDT_H_

#include "Arduino.h"

/*
 * Command and telemetry definitions for BTILC rover
 * Commands:
 *   - set drum speed
 *   - set drum position
 *   - set drum mode (dig, idle)
 *
 * 
 * Telemetry:
 *   - drum status
 *   - thermals
 *   - system status
*/

#define NUM_THERMAL_SENSORS 5
#define ID_SET_FORE_DRUM_SPEED 0x00
#define ID_SET_FORE_DRUM_HEIGHT 0x01
#define ID_PING 0x03
#define ID_SET_AFT_DRUM_SPEED 0x04
#define ID_SET_AFT_DRUM_HEIGHT 0x05
#define ID_SET_DRUM_MODE 0x06
#define ID_SET_DRUM_HEIGHTS 0x07
#define ID_CLEAR_DRUM_ERRORS 0x08
#define ID_CLEAR_HARDFAULT 0x09
#define ID_TELEM 0x10
#define ID_TELEM_DRUM_STATUS 0x11
#define ID_TELEM_THERMAL_STATUS 0x12
#define ID_TELEM_SYSTEM_STATUS 0x13
#define ID_PING_RESP 0x14
#define ID_TOGGLE_HIGHBEAMS 0x15
#define ID_STOP_LIFTS 0x16
#define ID_ENABLE_DISABLE_DRUMS 0x17
#define ID_ZERO_DRUMS 0x18
#define ID_SET_LIFT_MODE 0x19
#define ID_SET_RELAY 0x1A


String thermal_sensor_names[] = {
  "FORE_LIFT",
  "BATTERY_BAY",
  "POWERCHAIR_DRIVE",
  "NUC",
  "AFT_LIFT",
};

struct __attribute__((packed)) command_drum_speed_t {
  uint8_t drum;
  float speed;
};

struct __attribute__((packed)) command_drum_height_t {
  uint8_t drum;
  float position;
};

/*
 * Drum Modes:
 * 0 - idle - receives commands and translates
 * 1 - dig - self adjusts position within some bounds
 * 
*/
struct __attribute__((packed)) command_drum_mode_t {
    uint8_t drum;
    uint8_t drum_mode;
};


// -------------------- Telemetry Definitions -----------------------


struct __attribute__((packed)) echo_response_t {
  char resp[32];
};

// ----------------------- Internal message definitions ------------------------
struct lift_data_t {
  float fore_lift_position; // in meters
  float aft_lift_position;  // in meters
  float fore_current;
  float aft_current;
  float temp1;  // in C
  float temp2;  // in C
  float voltage;
  bool has_error;
  String error_message;
};

struct drum_data_t {
  // in Amps
  float fore_drum_current;
  float aft_drum_current;
  // in Nm
  float fore_drum_torque;
  float aft_drum_torque;
  // in Nm
  float fore_drum_average_torque;
  float aft_drum_average_torque;
  // in RPS
  float fore_drum_speed;
  float aft_drum_speed;
  // in turns
  float fore_drum_position;
  float aft_drum_position;
  // all temperatures in degrees Celcius
  float fore_drum_motor_temperature; 
  float aft_drum_motor_temperature; 
  float fore_drum_controller_temperature;
  float aft_drum_controller_temperature;

  bool has_error;
  String error_message;
};

struct thermistor_data_t {
  float temperatures[NUM_THERMAL_SENSORS];
};

struct drive_data_t {
  float x;
  float y;
  uint16_t status;
  
  bool has_error;
  String error_message;
};

struct fan_data_t {
  bool has_error;
  String error_message;
};

struct command_data_t {
  bool has_error;
  String error_message;
};

struct power_data_t {
  float current_draw;
  float total_power_usage;
};

struct system_data_t {
  command_data_t command_data;
  lift_data_t lift_data;
  drum_data_t drum_data;
  thermistor_data_t thermistors;
  drive_data_t drive_data;
  fan_data_t fan_data;
  power_data_t power_data;
  bool hardfault;
};

#endif

