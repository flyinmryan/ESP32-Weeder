#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <ESP32Servo.h>

class PhaseEnableMotor {
public:
    PhaseEnableMotor(int phase_pin, int enable_pin, int channel);
    void forward(float speed = 1.0);
    void backward(float speed = 1.0);
    void stop();

private:
    int _phase_pin;
    int _enable_pin;
    int _channel;
};

class PhaseEnableRobot {
public:
    PhaseEnableRobot(int l_phase, int l_enable, int r_phase, int r_enable);
    void forward(float speed = 1.0);
    void backward(float speed = 1.0);
    void left(float speed = 1.0);
    void right(float speed = 1.0);
    void stop();

private:
    PhaseEnableMotor _left;
    PhaseEnableMotor _right;
};

class AngularServo {
public:
    AngularServo(int pin, int initial_angle, int min_angle, int max_angle, int min_pulse, int max_pulse);
    void setAngle(int angle);
    int getAngle() { return _current_angle; }

private:
    Servo _servo;
    int _current_angle;
    int _min_angle;
    int _max_angle;
};

#endif // ACTUATORS_H
