typedef enum ControllerType {
    FLYSKY,
    FLYSKY_LEFT,
    PIXHAWK,
    ORANGE,
    ORANGE_DIFF,
} ControllerType_t;

struct ControllerProfile {
    int left_min;
    int right_min;

    int left_max;
    int right_max;

    int arm_value;
    int disarm_value;

    int autopilot_value;
    int manual_value;

    bool reversed;
    bool forward_only;
    bool differential;

    int left_channel;
    int right_channel;
    int arm_channel;
    int autopilot_channel;

    double dead_percentage;
};

ControllerProfile build_controller(ControllerType_t controller);


