#include <Arduino.h>
#include <ArduinoJson.h>
#include "joy.hpp"
#include "controllers.hpp"
#include <cstdio>

#define COS_NEG45 (0.70710678118)
#define SIN_NEG45 (-0.70710678118)

#define CONTROLLER FLYSKY

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

    ControllerProfile ctrl = build_controller(CONTROLLER);
    
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

    char buffer[1000];

    left_scaled = constrain(left_scaled, -1.0, 1.0);
    right_scaled = constrain(right_scaled, -1.0, 1.0);

    // sprintf(buffer, "left channel: %d; right channel: %d;", left, right);
    // Serial.println(buffer);
    // sprintf(buffer, "left norm: %.4f; right norm: %.4f", left_scaled, right_scaled);
    // Serial.println(buffer);

    joy_x_out = right_scaled;
    joy_y_out = left_scaled;

    // differential_to_joy(left_float, right_float, joy_x_out, joy_y_out);
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
