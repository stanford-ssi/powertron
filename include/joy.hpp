#include <Arduino.h>

// Bolder flight sbus library: https://github.com/bolderflight/sbus
#include "sbus.h"

class JoyTranslator {
  public:
    virtual void get_sbus_joy(float &joy_x_out, float &joy_y_out) = 0;
};

class DifferentialToJoyTranslator : JoyTranslator {
  public:
    DifferentialToJoyTranslator(bfs::SbusRx *sbus_rx, int SBUS_LEFT_CHANNEL, int SBUS_RIGHT_CHANNEL) : 
        SBUS_LEFT_CHANNEL_(SBUS_LEFT_CHANNEL), SBUS_RIGHT_CHANNEL_(SBUS_RIGHT_CHANNEL),
        sbus_rx_(sbus_rx) {}
    void get_sbus_joy(float &joy_x_out, float &joy_y_out);
  private:
    void differential_to_joy(float diff_left, float diff_right, float &joy_x_out, float &joy_y_out);
    int SBUS_LEFT_CHANNEL_;
    int SBUS_RIGHT_CHANNEL_;
    
    bfs::SbusData data;
    bfs::SbusRx *sbus_rx_;
};

class JoyToJoyTranslator : JoyTranslator {
    public:
    JoyToJoyTranslator(bfs::SbusRx *sbus_rx, int SBUS_THROTTLE_CHANNEL, int SBUS_STEERING_CHANNEL) : 
        SBUS_THROTTLE_CHANNEL_(SBUS_THROTTLE_CHANNEL), SBUS_STEERING_CHANNEL_(SBUS_STEERING_CHANNEL),
        sbus_rx_(sbus_rx) {}
    void get_sbus_joy(float &joy_x_out, float &joy_y_out);
  private:
    int SBUS_THROTTLE_CHANNEL_;
    int SBUS_STEERING_CHANNEL_;
    
    bfs::SbusData data;
    bfs::SbusRx *sbus_rx_;
};