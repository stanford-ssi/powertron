#include <Arduino.h>
#include "controllers.hpp"

// THIS MAYBE WRONG BUT
// UP AND DOWN IS LEFT
// LEFT AND RIGHT IS RIGHT
ControllerProfile build_controller(ControllerType_t controller) {
    switch (controller) {
    case FLYSKY_LEFT:
        return ControllerProfile {
            .left_min = 353,
            .right_min = 353,

            .left_max = 995, 
            .right_max = 1637, 

            .arm_value = 1804,
            .disarm_value = 232,

            .reversed = true,

            .left_channel = 1, // throttle
            .right_channel = 3, // steering
            .arm_channel = 6, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0.01,
        };

    case FLYSKY:
        return ControllerProfile {
            .left_min = 353,
            .right_min = 353,

            .left_max = 1637, 
            .right_max = 1637, 

            .arm_value = 1804,
            .disarm_value = 232,

            .autopilot_value = 1804,
            .manual_value = 232,

            .reversed = true,

            .left_channel = 3, // throttle
            .right_channel = 1, // steering
            .arm_channel = 6, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0.02,
        };

    case PIXHAWK:
        return ControllerProfile {
            // we only care about the left channel which is the throttle
            .left_min = 1000,
            .right_min = 1000,

            .left_max = 2000, 
            .right_max = 2000, 

            .arm_value = 2000,
            .disarm_value = 1000,

            .autopilot_value = 2000,
            .manual_value = 1000,

            .reversed = false,

            .left_channel = 3, // throttle
            .right_channel = 1, // steering
            .arm_channel = 6, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0.02,
        };
    }
}

