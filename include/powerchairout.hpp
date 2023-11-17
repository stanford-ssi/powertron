#ifndef POWERCHAIROUT_H_
#define POWERCHAIROUT_H_

#include "Adafruit_MCP4725.h"
#include "Wire.h"

class iPowerChairOut {
  public:
    virtual void powerchairout(float x, float y) = 0;
    virtual void init() = 0;
};

class JoystickEmu : iPowerChairOut {
  public:
    void powerchairout(float x, float y);
    JoystickEmu(const uint8_t xdac_addr, const uint8_t ydac_addr, TwoWire *wire = &Wire)
        : XDAC_ADDR(xdac_addr), YDAC_ADDR(ydac_addr) {}

    void init();
    void write_dac(int val, int addr) const;
    void joystick_out(float joy_x, float joy_y);

  private:
    Adafruit_MCP4725 y_dac;
    Adafruit_MCP4725 x_dac;
    const uint8_t XDAC_ADDR;
    const uint8_t YDAC_ADDR;
};

#endif