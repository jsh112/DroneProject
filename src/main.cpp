#include <Arduino.h>
#include "Drone.hpp"

// Declare Drone Class
Drone drone;

// ================================================================
// ===                    MAIN PROGRAM SETUP                    ===
// ================================================================

void setup() {
    Serial.begin(baudRate);
    while(!Serial);
    pinMode(drone.PIN_THROTTLE, INPUT);
    pinMode(drone.PIN_ROLL, INPUT);
    pinMode(drone.PIN_PITCH, INPUT);
    pinMode(drone.PIN_YAW, INPUT);

    attachInterrupt(digitalPinToInterrupt(drone.PIN_THROTTLE), drone.ThrottleISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(drone.PIN_ROLL), drone.RollISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(drone.PIN_PITCH), drone.PitchISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(drone.PIN_YAW), drone.YawISR, CHANGE);
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    /* Serial.print("Mapped Throttle: ");
    Serial.println(throttle);
    Serial.print("Mapped Roll: ");
    Serial.println(roll);
    Serial.print("Mapped Pitch: ");
    Serial.println(pitch);
    Serial.print("Mapped Yaw: ");
    Serial.println(yaw);
    Serial.println(); */

    int Throttle = drone.getThrottlePulseWidth();
    int Roll = drone.getRollPulseWidth();
    int Pitch = drone.getPitchPulseWidth();
    int Yaw = drone.getYawPulseWidth();

    Throttle = map(Throttle, minThrottle, maxThrottle, 1000, 1500);
    Roll = map(Roll, minRoll, maxRoll, -200, 200);
    Pitch = map(Pitch, minPitch, maxPitch, -200, 200);
    Yaw = map(Yaw, minYaw, maxYaw, -200, 200);

    // Estimate the pose centered
    if (abs(Roll) < 10) {
        Serial.println("Roll is centered");
    }
    if (abs(Pitch) < 10) {
        Serial.println("Pitch is centered");
    }
    if (abs(Yaw) < 10) {
        Serial.println("Yaw is centered");
    }

    int MotorFrontLeft = Throttle + Roll + Pitch + Yaw;
    int MotorFrontRight = Throttle - Roll + Pitch - Yaw;
    int MotorBackLeft = Throttle + Roll - Pitch - Yaw;
    int MotorBackRight = Throttle - Roll - Pitch + Yaw;

    MotorFrontLeft = constrain(MotorFrontLeft, minValue, maxValue);
    MotorFrontRight = constrain(MotorFrontRight, minValue, maxValue);
    MotorBackLeft = constrain(MotorBackLeft, minValue, maxValue);
    MotorBackRight = constrain(MotorBackRight, minValue, maxValue);

    Serial.print("FrontLeft Motor PWM: ");
    Serial.println(MotorFrontLeft);
    Serial.print("FrontRight Motor PWM: ");
    Serial.println(MotorFrontRight);
    Serial.print("BackLeft Motor PWM: ");
    Serial.println(MotorBackLeft);
    Serial.print("BackRight Motor PWM: ");
    Serial.println(MotorBackRight);
    // Write motor speeds to ESCs
    drone.updateMotorPulseWidth(MotorFrontLeft, MotorFrontRight, MotorBackLeft, MotorBackRight);


    delay(1000);  // Delay for readability
}

// 드디어 성공