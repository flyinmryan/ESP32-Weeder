#ifndef WEEDER_LOGIC_H
#define WEEDER_LOGIC_H

#include <Arduino.h>
#include "Actuators.h"
#include "esp_camera.h"

struct Point {
    int x;
    int y;
};

class WeederLogic {
public:
    WeederLogic(PhaseEnableRobot& robot, PhaseEnableRobot& zaxis, 
                AngularServo& lid, AngularServo& tilt, 
                AngularServo& roll, AngularServo& swing);

    void sunTracker(int q1, int q2, int q3, int q4);
    bool orientToSun(int targetX, int targetY, int threshold);
    void moveToWeed(int x);
    int classify(camera_fb_t* fb);

private:
    PhaseEnableRobot& _robot;
    PhaseEnableRobot& _zaxis;
    AngularServo& _lid;
    AngularServo& _tilt;
    AngularServo& _roll;
    AngularServo& _swing;

    void gradualMove(AngularServo& servo, int targetAngle);
};

#endif // WEEDER_LOGIC_H
