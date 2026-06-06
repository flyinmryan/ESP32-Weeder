#include "Actuators.h"

PhaseEnableMotor::PhaseEnableMotor(int phase_pin, int enable_pin, int channel) 
    : _phase_pin(phase_pin), _enable_pin(enable_pin), _channel(channel) {
    pinMode(_phase_pin, OUTPUT);
    ledcSetup(_channel, 20000, 8); // 20kHz, 8-bit resolution
    ledcAttachPin(_enable_pin, _channel);
    stop();
}

void PhaseEnableMotor::forward(float speed) {
    digitalWrite(_phase_pin, LOW);
    ledcWrite(_channel, (int)(speed * 255));
}

void PhaseEnableMotor::backward(float speed) {
    digitalWrite(_phase_pin, HIGH);
    ledcWrite(_channel, (int)(speed * 255));
}

void PhaseEnableMotor::stop() {
    ledcWrite(_channel, 0);
}

PhaseEnableRobot::PhaseEnableRobot(int l_phase, int l_enable, int r_phase, int r_enable)
    : _left(l_phase, l_enable, 0), _right(r_phase, r_enable, 1) {}

void PhaseEnableRobot::forward(float speed) {
    _left.forward(speed);
    _right.forward(speed);
}

void PhaseEnableRobot::backward(float speed) {
    _left.backward(speed);
    _right.backward(speed);
}

void PhaseEnableRobot::left(float speed) {
    _left.backward(speed);
    _right.forward(speed);
}

void PhaseEnableRobot::right(float speed) {
    _left.forward(speed);
    _right.backward(speed);
}

void PhaseEnableRobot::stop() {
    _left.stop();
    _right.stop();
}

AngularServo::AngularServo(int pin, int initial_angle, int min_angle, int max_angle, int min_pulse, int max_pulse)
    : _current_angle(initial_angle), _min_angle(min_angle), _max_angle(max_angle) {
    _servo.attach(pin, min_pulse, max_pulse);
    _servo.write(_current_angle);
}

void AngularServo::setAngle(int angle) {
    _current_angle = constrain(angle, _min_angle, _max_angle);
    _servo.write(_current_angle);
}
