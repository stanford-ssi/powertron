#include <Arduino.h>
#include "controllers.hpp"

ControllerProfile build_controller(ControllerType_t controller) {
    switch (controller) {
    case FLYSKY:
        return ControllerProfile {
            FLYSKY_LEFT_MIN, FLYSKY_RIGHT_MIN,
            FLYSKY_LEFT_MAX, FLYSKY_RIGHT_MAX,
            FLYSKY_REVERSED
        };
    }
}

