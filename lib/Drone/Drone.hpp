#pragma once
#ifndef DRONE_HPP
#define DRONE_HPP

#include <Arduino.h>
#include <Servo.h>

constexpr size_t baudRate = 115200;
constexpr size_t minThrottle = 1084;
constexpr size_t maxThrottle = 1918;
constexpr size_t minRoll = 1088;
constexpr size_t maxRoll = 1919;
constexpr size_t minPitch = 1116;
constexpr size_t maxPitch = 1926;
constexpr size_t minYaw = 1088;
constexpr size_t maxYaw = 1919;
static constexpr int minValue = 1000;
static constexpr int maxValue = 1400;

class Drone {
public:
    // Constructor
    Drone();
    //
    enum Pin {
        PIN_THROTTLE = 4,
        PIN_ROLL = 5,
        PIN_PITCH= 6,
        PIN_YAW = 7
    };

    enum Motor {
        FRONT_LEFT = 9,
        FRONT_RIGHT = 10,
        BACK_LEFT = 11,
        BACK_RIGHT = 12
    };
    // Interrupt functions
    static void ThrottleISR();
    static void RollISR();
    static void PitchISR();
    static void YawISR();

    // Getter and Setter
    static void setThrottlePulseWidth(size_t pw);
    static void setRollPulseWidth(size_t pw);
    static void setPitchPulseWidth(size_t pw);
    static void setYawPulseWidth(size_t pw);
    int getThrottlePulseWidth();
    int getRollPulseWidth();
    int getPitchPulseWidth();
    int getYawPulseWidth();
    void updateMotorPulseWidth(int& fl, int& fr, int& bl, int& br);
    void setMotorSignal(Servo& servo, int& pw);
protected:
    // PulseWidth
    static volatile size_t ThrottlePW, RollPW, PitchPW, YawPW;
    // Servo objects
    Servo ESCFrontLeft, ESCFrontRight, ESCBackLeft, ESCBackRight;
    // Constants
    static constexpr size_t InitThrottle = 1083;
    static constexpr size_t InitRoll = 1500;
    static constexpr size_t InitPitch = 1529;
    static constexpr size_t InitYaw = 1504;
};

#endif // DRONE_HPP
