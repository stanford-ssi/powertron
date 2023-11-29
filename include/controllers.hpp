typedef enum ControllerType {
    FLYSKY,
    FLYSKY_LEFT
} ControllerType_t;

struct ControllerProfile {
    int left_min;
    int right_min;

    int left_max;
    int right_max;

    int arm_value;
    int disarm_value;

    bool reversed;

    int left_channel;
    int right_channel;
    int arm_channel;
};

ControllerProfile build_controller(ControllerType_t controller);


