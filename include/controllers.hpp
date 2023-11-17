#define FLYSKY_LEFT_MIN 353
#define FLYSKY_LEFT_MAX 995

#define FLYSKY_RIGHT_MIN 353
#define FLYSKY_RIGHT_MAX 1637

#define FLYSKY_REVERSED true

typedef enum ControllerType {
    FLYSKY
} ControllerType_t;

typedef struct ControllerProfile {
    int left_min;
    int right_min;

    int left_max;
    int right_max;

    bool reversed;
};

ControllerProfile build_controller(ControllerType_t controller);


