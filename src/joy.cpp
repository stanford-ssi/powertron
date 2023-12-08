#include <Arduino.h>
#include <ArduinoJson.h>
#include "joy.hpp"
#include "controllers.hpp"
#include <cstdio>
#include <cmath>

#define COS_NEG45 (0.70710678118)
#define SIN_NEG45 (-0.70710678118)

#define CONTROLLER FLYSKY
#define DEBUG_PRINT true

void DifferentialToJoyTranslator::get_sbus_joy(float &joy_x_out, float &joy_y_out) {
    data = sbus_rx_->data();

    ControllerProfile ctrl = build_controller(CONTROLLER);

    // check if autopilot is on and if so return
    // if autopilot is on we will hard code a translation scheme

    if (data.ch[ctrl.autopilot_channel-1] == ctrl.autopilot_value) {
        ctrl = build_controller(PIXHAWK);
    } 


    int left = data.ch[ctrl.left_channel-1];
    int right = data.ch[ctrl.right_channel-1];

    char buffer[1000];

    // if system is disarmed, do nothing
    if (data.ch[ctrl.arm_channel-1] != ctrl.arm_value) {
        joy_x_out = 0;
        joy_y_out = 0;

        Serial.println("disarmed, returning 0s");

        return;
    }

    float left_scaled = 0;
    float right_scaled = 0;

    float left_norm = ctrl.left_max - ctrl.left_min;
    float right_norm = ctrl.right_max - ctrl.right_min;

    if (ctrl.reversed) {
        left_scaled = ((-(float) (left-ctrl.left_max))/left_norm)*2-1;
        right_scaled = ((-(float) (right-ctrl.right_max))/right_norm)*2-1;
    } else {
        left_scaled = ((float)(left-ctrl.left_min)/left_norm)*2-1;
        right_scaled = ((float)(left-ctrl.right_min)/right_norm)*2-1;
    }

    left_scaled = constrain(left_scaled, -1.0, 1.0);
    right_scaled = constrain(right_scaled, -1.0, 1.0);

    /// TODO harden to prevent buffer overflows
    if (DEBUG_PRINT) {
        sprintf(buffer, "(1) %d (2) %d (3) %d (4) %d", data.ch[0], data.ch[1], data.ch[2], data.ch[3]);
        Serial.println(buffer);
        sprintf(buffer, "(5) %d (6) %d (7) %d (8) %d", data.ch[4], data.ch[5], data.ch[6], data.ch[7]);
        Serial.println(buffer);
        sprintf(buffer, "(9) %d (10) %d (11) %d (12) %d", data.ch[8], data.ch[9], data.ch[10], data.ch[11]);
        Serial.println(buffer);
        sprintf(buffer, "(13) %d (14) %d (15) %d (16) %d", data.ch[12], data.ch[13], data.ch[14], data.ch[15]);
        Serial.println(buffer);
        sprintf(buffer, "num_ch %d", data.NUM_CH);
        Serial.println(buffer);
        sprintf(buffer, "ch_17 %d ch_18 %d", data.ch17, data.ch18);
        Serial.println(buffer);
    }


    sprintf(buffer, "left channel (%d): %d; right channel (%d): %d;", ctrl.left_channel, left, ctrl.right_channel, right);
    Serial.println(buffer);

    if (abs(left_scaled) < ctrl.dead_percentage) {
        left_scaled = 0;
    }
    if (abs(right_scaled) < ctrl.dead_percentage) {
        right_scaled = 0;
    }

    sprintf(buffer, "left norm: %.4f; right norm: %.4f", left_scaled, right_scaled);
    Serial.println(buffer);

    joy_x_out = right_scaled;
    joy_y_out = left_scaled;
}

// void JoyToJoyTranslator::get_sbus_joy(float &joy_x_out, float &joy_y_out) {
//     data = sbus_rx_->data();
//     int throttle = data.ch[SBUS_THROTTLE_CHANNEL_];
//     int steering = data.ch[SBUS_STEERING_CHANNEL_];

//     joy_y_out = (float)(throttle - 1000.0) / 828.0;
//     joy_x_out = (float)(steering - 1000.0) / 828.0;  
//     joy_y_out = constrain(joy_y_out, -1.0, 1.0);
//     joy_x_out = constrain(joy_x_out, -1.0, 1.0);
// }
