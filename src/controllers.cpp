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
            .forward_only = false,
            .differential = false,

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
            .forward_only = false,
            .differential = false,

            .left_channel = 3, // throttle
            .right_channel = 1, // steering
            .arm_channel = 6, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0.02,
        };

    case PIXHAWK:
        return ControllerProfile {
            .left_min = 1637,
            .right_min = 1637,

            .left_max = 353, 
            .right_max = 353, 

            .arm_value = 1804,
            .disarm_value = 232,

            .autopilot_value = 1804,
            .manual_value = 232,

            .reversed = true,
            .forward_only = true,
            .differential = false,

            .left_channel = 3, // throttle
            .right_channel = 1, // steering
            .arm_channel = 6, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0.02,
        };
    case ORANGE:
        return ControllerProfile {
            .left_min = 842,
            .right_min = 202,

            .left_max = 1802, 
            .right_max = 1802, 

            .arm_value = 1002,
            .disarm_value = 202,

            .autopilot_value = 1804,
            .manual_value = 232,

            .reversed = false,
            .forward_only = true,
            .differential = false,

            .left_channel = 1, // throttle
            .right_channel = 2, // steering

            .arm_channel = 5, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0,
        };

    case ORANGE_DIFF:
        return ControllerProfile {
            .left_min = 202,
            .right_min = 202,

            .left_max = 1802, 
            .right_max = 1802, 

            .arm_value = 1002,
            .disarm_value = 202,

            .autopilot_value = 1804,
            .manual_value = 232,

            .reversed = false,
            .forward_only = false,
            .differential = true,

            .left_channel = 1, // left motor
            .right_channel = 2, // right motor

            .arm_channel = 5, // 1 indexing
            .autopilot_channel = 5, // 0 indexing

            .dead_percentage = 0,
        };
    }
};

