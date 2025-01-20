#include "DroneControl.hpp"
#include "MPU9250.h"

Servo ESCFrontLeft, ESCFrontRight, ESCBackLeft, ESCBackRight;

void motor_init(){
    Serial.begin(115200);
    ESCFrontLeft.attach(FRONT_LEFT_PIN, minValue, maxValue);
    ESCFrontRight.attach(FRONT_RIGHT_PIN, minValue, maxValue);
    ESCBackLeft.attach(BACK_LEFT_PIN, minValue, maxValue);
    ESCBackRight.attach(BACK_RIGHT_PIN, minValue, maxValue);

    ESCFrontLeft.writeMicroseconds(minValue);
    ESCFrontRight.writeMicroseconds(minValue);
    ESCBackLeft.writeMicroseconds(minValue);
    ESCBackRight.writeMicroseconds(minValue);

    // Define the pins for PWM signal input
    pinMode(THROTTLE_PIN, INPUT);
    pinMode(ROLL_PIN, INPUT);
    pinMode(PITCH_PIN, INPUT);
    pinMode(YAW_PIN, INPUT);

    // Attach interrupts for each channel to measure PWM pulse width
    attachInterrupt(digitalPinToInterrupt(THROTTLE_PIN), Throttle_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROLL_PIN), Roll_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PITCH_PIN), Pitch_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(YAW_PIN), Yaw_ISR, CHANGE);
}