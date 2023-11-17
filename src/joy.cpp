#include <Arduino.h>
#include <ArduinoJson.h>
#include "joy.hpp"
#include <cstdio>


#define COS_NEG45 (0.70710678118)
#define SIN_NEG45 (-0.70710678118)

void DifferentialToJoyTranslator::differential_to_joy(float diff_left, float diff_right, float &joy_x_out, float &joy_y_out) {
    // because of the mapping between these two, things get a little wonky at the corners, hence the scaling terms
    joy_x_out = (COS_NEG45 * diff_left + SIN_NEG45 * diff_right) * (1.0/0.78);
    joy_y_out = (-1.0 * SIN_NEG45 * diff_left + COS_NEG45 * diff_right) * (1.0/1.21);

    // left right is reversed
    joy_x_out = -1.0 * constrain(joy_x_out, -1.0, 1.0);
    joy_y_out = constrain(joy_y_out, -1.0, 1.0);

    char buffer[500];
    sprintf(buffer, "joy x: %.4f; joy y: %.4f\n\n\n", joy_x_out, joy_y_out);
    Serial.println(buffer);

}

void DifferentialToJoyTranslator::get_sbus_joy(float &joy_x_out, float &joy_y_out) {
    data = sbus_rx_->data();
    int left = data.ch[SBUS_LEFT_CHANNEL_];
    int right = data.ch[SBUS_RIGHT_CHANNEL_];

    char buffer[500];

    // TODO(HEIDT) replace magic numbers with calibration operation or #defines
    float left_float = (float)(left - 1000.0) / 900.0;
    float right_float = (float)(right - 1000.0) / 900.0;  
    left_float = constrain(left_float, -1.0, 1.0);
    right_float = constrain(right_float, -1.0, 1.0);

    sprintf(buffer, "left channel: %d; right channel: %d;", left, right);
    Serial.println(buffer);
    sprintf(buffer, "left norm: %.4f; right norm: %.4f", left_float, right_float);
    Serial.println(buffer);

    differential_to_joy(left_float, right_float, joy_x_out, joy_y_out);
}

void JoyToJoyTranslator::get_sbus_joy(float &joy_x_out, float &joy_y_out) {
    data = sbus_rx_->data();
    int throttle = data.ch[SBUS_THROTTLE_CHANNEL_];
    int steering = data.ch[SBUS_STEERING_CHANNEL_];

    joy_y_out = (float)(throttle - 1000.0) / 828.0;
    joy_x_out = (float)(steering - 1000.0) / 828.0;  
    joy_y_out = constrain(joy_y_out, -1.0, 1.0);
    joy_x_out = constrain(joy_x_out, -1.0, 1.0);
}
