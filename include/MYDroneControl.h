#ifndef MYDRONECONTROL_H
#define MYDRONECONTROL_H

#include <Arduino.h>
#include <Servo.h>

extern Servo ESCFrontLeft, ESCFrontRight, ESCBackLeft, ESCBackRight;

// Pin Definition
/* ---------------------------------------------------------- */

// Throttle -> Drone Height
// Rudder (Yaw) -> Drone Rotation horizontally left and right
// Elevator (Pitch) -> Forward and Backward movement
// Aileron (Roll) -> Left and Right movement

// Ch1 : Roll (Aileron)
// Ch2 : Pitch (Elevator)
// Ch3 : Throttle
// Ch4 : Yaw (Rudder)

constexpr uint8_t THROTTLE_PIN = 4;
constexpr uint8_t ROLL_PIN = 5;
constexpr uint8_t PITCH_PIN = 6;
constexpr uint8_t YAW_PIN = 7;

// Define the motor locations Pins
constexpr uint8_t FRONT_LEFT_PIN = 9;
constexpr uint8_t FRONT_RIGHT_PIN = 10;
constexpr uint8_t BACK_LEFT_PIN = 11;
constexpr uint8_t BACK_RIGHT_PIN = 12;

/* ---------------------------------------------------------- */

// Using volatile variables to avoid the compiler optimization
// These variables are used in the ISR functions
// Using extern to declare the variables in the other file
extern volatile size_t ThrottlePulseWidth;
extern volatile size_t PitchPulseWidth;
extern volatile size_t RollPulseWidth;
extern volatile size_t YawPulseWidth;

const int baudRate = 115200;
const int minValue = 1000;
const int maxValue = 1400;

// Minimum and maximum pulse widths for each channel (adjust these based on calibration)
const int minThrottle = 1084;
const int maxThrottle = 1918;
const int minRoll = 1088;
const int maxRoll = 1919;
const int minPitch = 1116;
const int maxPitch = 1926;
const int minYaw = 1088;
const int maxYaw = 1919;

// Define the esc objects
/**
 * FrontLeft ESC : CW Direction
 * FrontRight ESC : CCW Direction
 * BackLeft ESC : CW Direction
 * BackRight ESC : CCW Direction
 */
/**
 * @brief hyperparameters of ISR functions
 */

void Throttle_ISR(void);
void Roll_ISR(void);
void Pitch_ISR(void);
void Yaw_ISR(void);

void motor_init(void);
// void ReceiverCalibration(void);
// void ControlDrone(void);

#endif // MYDRONECONTROL_H
