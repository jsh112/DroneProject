#include "MYDroneControl.h"

void Throttle_ISR() {
    static uint32_t lastThrottleTime = 0;
    uint32_t startTime = micros();

    if (digitalRead(THROTTLE_PIN) == HIGH) {
        lastThrottleTime = startTime;
    } else {
        ThrottlePulseWidth = startTime - lastThrottleTime;
    }
}

// ISR for roll pulse measurement
void Roll_ISR() {
    static uint32_t lastRollTime = 0;
    uint32_t startTime = micros();

    if (digitalRead(ROLL_PIN) == HIGH) {
        lastRollTime = startTime;
    } else {
        RollPulseWidth = startTime - lastRollTime;
    }
}

// ISR for pitch pulse measurement
void Pitch_ISR() {
    static uint32_t lastPitchTime = 0;
    uint32_t startTime = micros();

    if (digitalRead(PITCH_PIN) == HIGH) {
        lastPitchTime = startTime;
    } else {
        PitchPulseWidth = startTime - lastPitchTime;
    }
}

// ISR for yaw pulse measurement
void Yaw_ISR() {
    static uint32_t lastYawTime = 0;
    uint32_t startTime = micros();

    if (digitalRead(YAW_PIN) == HIGH) {
        lastYawTime = startTime;
    } else {
        YawPulseWidth = startTime - lastYawTime;
    }
}