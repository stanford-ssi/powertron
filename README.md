# BTILC Low Level Controller
This is the location of the Teensy code that handles a few different tasks.  It is built on platformio and intended to run on a Teensy 4.1

The tasks it performs (i.e. the requirements that drive the design) are:
- Read in SBUS signals from the flight computer
- Convert the SBUS signals to DAC outputs to simulate the joystick
- Read the SBUS channel 10 which contains the ARM/DISARM state of the flight computer
- Control two linear actuators
  - provide position feedback and control
  - provide homing operations
- Read force data from two inline load cells
- Control two drum motor controllers
  - provide speed feedback and control
  - provide torque (current) data 
- Communicate with a host PC
  - report data from actuators, load cells, and drum motor controllers
  - receive commands from host PC to control drums and linear actuators
- Enter a safe mode when the state of the system becomes disarmed