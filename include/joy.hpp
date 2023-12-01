#include <Arduino.h>

// Bolder flight sbus library: https://github.com/bolderflight/sbus
#include "sbus.h"

class JoyTranslator {
  public:
    virtual void get_sbus_joy(float &joy_x_out, float &joy_y_out) = 0;
};

class DifferentialToJoyTranslator : JoyTranslator {
  public:
    DifferentialToJoyTranslator(bfs::SbusRx *sbus_rx) : sbus_rx_(sbus_rx) {}
    void get_sbus_joy(float &joy_x_out, float &joy_y_out);
  private:
    
    bfs::SbusData data;
    bfs::SbusRx *sbus_rx_;
};

// class JoyToJoyTranslator : JoyTranslator {
//     public:
//     JoyToJoyTranslator(bfs::SbusRx *sbus_rx, int SBUS_THROTTLE_CHANNEL, int SBUS_STEERING_CHANNEL) : 
//         sbus_rx_(sbus_rx) {}
//     void get_sbus_joy(float &joy_x_out, float &joy_y_out);
//   private:
//     bfs::SbusData data;
//     bfs::SbusRx *sbus_rx_;
// };
