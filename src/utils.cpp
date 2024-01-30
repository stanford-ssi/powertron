#include "utils.hpp"

void differential_to_joy(float wl, float wr, float &joy_x_out, float &joy_y_out) {
    double magnitude = sqrt((wl*wl)+(wr*wr));
    double arctan = atan(wr/wl);

    // translates to proper unit circle angle, otherwise working with negative angles
    if(arctan<0) {
        arctan = arctan + (M_PI);
    }
    if(wr<0 || (wr==0 && wl<0)) {
        arctan = arctan + (M_PI);
    }

    //scaling factor, basically the size of the vector. 
    //divides the magnitude of the vector by the 
    //maximum possible value it could be at that angle on translated rectangle
    double scalingFactor = 0;
    if((arctan > (M_PI/4) && arctan < (3*M_PI/4)) || (arctan > (5*M_PI/4) && arctan < (7*M_PI/4))) {
        scalingFactor = 1/sin(arctan);
    }
    else {
        scalingFactor = 1/cos(arctan);
    }
    scalingFactor = abs(magnitude/scalingFactor);

    // offset angle, most important operation. joystick is offset by pi/4
    // from the output of the motors
    double theta = arctan + (M_PI/4); 

    // proper translation back. converts theta to associated rectangular coords
    double testX = 0;
    double testY = 0;
    if(theta > 0 && theta <= M_PI/4) {
        testX = 1;
        testY = tan(theta);
    }
    else if(theta > M_PI/4 && theta <= M_PI/2) {
        testX = 1/tan(theta);
        testY = 1;
    }
    else if(theta > M_PI/2 && theta <= 3*M_PI/4) {
        testX = 1/tan(theta);
        testY = 1;
    }
    else if(theta > 3*M_PI/4 && theta <= M_PI) {
        testX = -1;
        testY = tan(theta);
    }
    else if(theta > M_PI && theta <= 5*M_PI/4) {
        testX = -1;
        testY = -1*tan(theta);
    }
    else if(theta > 5*M_PI/4 && theta <= 3*M_PI/2) {
        testX = -1/tan(theta);
        testY = -1;
    }
    else if(theta > 3*M_PI/2 && theta <= 7*M_PI/4) {
        testX = -1/tan(theta);;
        testY = -1;
    }
    else if(theta > 7*M_PI/4 && theta <= 2*M_PI) {
        testX = 1;
        testY = -tan(theta);
    }
    else {
        testX = 1;
        testY = 1;
    }

    /*
    testX = cos(theta);
    testY = sin(theta);
    */

    // rounds values and multiplies by scaling value to get vector of proper length for inputs
    testX = scalingFactor*(round(testX*100000)/100000);
    testY = scalingFactor*(round(testY*100000)/100000);

    joy_x_out = testX;
    joy_y_out = testY;

    // clamp to prevent man cart shoe (i.e. controller failure)
    joy_x_out = constrain(joy_x_out, -0.96, 0.96);
    joy_y_out = constrain(joy_y_out, -0.96, 0.96);
}
