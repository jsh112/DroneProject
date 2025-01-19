/* #include <Arduino.h>
#include <Servo.h> */
#include "MYDroneControl.h"

// 쓰로틀은 1083이 최솟값
volatile size_t ThrottlePulseWidth = 1083;
volatile size_t RollPulseWidth = 1500;
volatile size_t PitchPulseWidth = 1529;
volatile size_t YawPulseWidth = 1504;

void setup() {
    motor_init();
}

void loop() {
    // Map the pulse widths to a range of -200 to 200, with 0 being the center value
    int Throttle = map(ThrottlePulseWidth, minThrottle, maxThrottle, 1000, 1500);
    int Roll = map(RollPulseWidth, minRoll, maxRoll, -200, 200);
    int Pitch = map(PitchPulseWidth, minPitch, maxPitch, -200, 200);
    int Yaw = map(YawPulseWidth, minYaw, maxYaw, -200, 200);

    // Print the mapped values to see how close they are to 0 (center value)
    Serial.print("Mapped Throttle: ");
    Serial.println(Throttle);
    Serial.print("Mapped Roll: ");
    Serial.println(Roll);
    Serial.print("Mapped Pitch: ");
    Serial.println(Pitch);
    Serial.print("Mapped Yaw: ");
    Serial.println(Yaw);
    Serial.println();

    // Check if the values are centered (close to 0)
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

    // Write motor speeds to ESCs
    ESCFrontLeft.writeMicroseconds(MotorFrontLeft);
    ESCFrontRight.writeMicroseconds(MotorFrontRight);
    ESCBackLeft.writeMicroseconds(MotorBackLeft);
    ESCBackRight.writeMicroseconds(MotorBackRight);

    Serial.print("FrontLeft Motor PWM: ");
    Serial.println(MotorFrontLeft);
    Serial.print("FrontRight Motor PWM: ");
    Serial.println(MotorFrontRight);
    Serial.print("BackLeft Motor PWM: ");
    Serial.println(MotorBackLeft);
    Serial.print("BackRight Motor PWM: ");
    Serial.println(MotorBackRight);

    delay(1000);  // Delay for readability
}
