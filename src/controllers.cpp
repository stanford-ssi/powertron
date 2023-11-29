#include <Arduino.h>
#include "controllers.hpp"

// UP AND DOWN IS RIGHT
// LEFT AND RIGHT IS LEFT
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

            .left_channel = 0, // this is channel 1, but 0 indexing :/
            .right_channel = 2, // 0 indexing
            .arm_channel = 5, // 0 indexing
        };

    case FLYSKY:
        return ControllerProfile {
            .left_min = 232,
            .right_min = 240,

            .left_max = 1804, 
            .right_max = 1796, 

            .arm_value = 1804,
            .disarm_value = 232,

            .reversed = true,

            // Put down here what you know of as "channel value" MINUS ONE
            // because 0 indexing
            .left_channel = 3, // this is channel 1, but 0 indexing :/
            .right_channel = 1, // 0 indexing
            .arm_channel = 5, // 0 indexing
        };
    }
}

