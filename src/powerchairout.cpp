#include "powerchairout.hpp"

#define VOLTAGE_MIN (1.0)
#define VOLTAGE_MAX (4.0)
#define VOLTAGE_DIFF (VOLTAGE_MAX - VOLTAGE_MIN)
#define DAC_ZERO (4096 / 2)
#define DAC_LSB_PER_VOLT (4096.0 / 5.0)

void JoystickEmu::powerchairout(float x, float y) { joystick_out(x, y); }

inline unsigned int map_joy(float joy) {
    // convert range -1.0 to 1.0 to 0.0-1.0
    joy = (joy + 1.0) / 2.0;
    // convert to the desired voltage
    float voltage = (joy * VOLTAGE_DIFF) + VOLTAGE_MIN;
    // convert to adc value, round to nearest
    unsigned int adc_val = (unsigned int)((voltage * DAC_LSB_PER_VOLT) + 0.5);
    return adc_val;
}

void JoystickEmu::write_dac(int val, int addr) const {
    Wire.beginTransmission(addr);
    Wire.write(64);              // cmd to update the DAC
    Wire.write(val >> 4);        // the 8 most significant bits...
    Wire.write((val & 15) << 4); // the 4 least significant bits...
    Wire.endTransmission();
}

void JoystickEmu::joystick_out(float joy_x, float joy_y) {
    unsigned int joy_x_dacval = map_joy(joy_x);
    unsigned int joy_y_dacval = map_joy(joy_y);

    x_dac.setVoltage(joy_x_dacval, false);
    y_dac.setVoltage(joy_y_dacval, false);
}

void JoystickEmu::init() {
    Wire.begin();
    // TODO(Heidt) might be a hardware problem, but the following code doesn't get a response
    // from the DAC,
    //  although it writes properly.
    //  Set both DACs to zero point and write this to EEPROM so they output that voltage at
    //  ever startup
    x_dac.begin(XDAC_ADDR);
    y_dac.begin(YDAC_ADDR);

    x_dac.setVoltage(DAC_ZERO, true);
    y_dac.setVoltage(DAC_ZERO, true);
}