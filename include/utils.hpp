#pragma once
#include <Arduino.h>
#include <cmath>

void differential_to_joy(float wl, float wr, float &joy_x_out, float &joy_y_out);

