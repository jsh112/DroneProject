#include <Arduino.h>
#include <Servo.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2Cdev.h"
#include "MPU9250_9Axis_MotionApps41.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define OUTPUT_READABLE_YAWPITCHROLL
#define OUTPUT_TEAPOT

#define INTERRUPT_PIN 2 // use pin 2 on Arduino Uno & most boards

#define MPU9250_IMU_ADDRESS 0x68

/**
 * @brief Information about the Servo.h library functions:
 * 
 * - servo.writeMicroseconds(us):  
 *   Sets the servo pulse width in microseconds.  
 *   @param us: Pulse width in microseconds (int).
 * 
 * - servo.attach(pin):  
 *   Attaches the servo to the specified pin.  
 *   @param pin: The pin to which the servo is connected.  
 * 
 * - servo.attach(pin, min, max):  
 *   Attaches the servo to the specified pin and defines the pulse width range.  
 *   @param pin: The pin to which the servo is connected.  
 *   @param min: Minimum pulse width in microseconds (default is 544).  
 *   @param max: Maximum pulse width in microseconds (default is 2400).  
 */

/**
 * @brief Pin usage on Teensy 4.1 for the current configuration:
 * 
 * - ESC (PWM pins): {9, 10, 11, 12}  
 * - Receiver (PWM pins): {3, 4, 5, 6}  
 */


// Define the esc objects
Servo Escs[4];

// Enum class to represent the motor Locations
enum class MotorLocation
{
    FrontLeftPin = 9,
    FrontRightPin,
    BackLeftPin,
    BackRightPin
};

const int MortorPins[] = {
    static_cast<int>(MotorLocation::FrontLeftPin),
    static_cast<int>(MotorLocation::FrontRightPin),
    static_cast<int>(MotorLocation::BackLeftPin),
    static_cast<int>(MotorLocation::BackRightPin)};

// Enum class to represent the UAV contorl axes
enum class UAVControl
{
    ThrottlePin = 3,
    RollPin,
    PitchPin,
    YawPin
};

// Define constants
const int baudRate = 115200;
const int minValue = 1000;
const int maxValue = 1800;

// Define the functions

void motor_init()
{
    // Syncronize with Serial to baudRate
    Serial.begin(baudRate);

    // Attach the pins to the esc objects
    for (size_t i = 0; i < 4; ++i)
        Escs[i].attach(MortorPins[i], minValue, maxValue);

    for (Servo &const Esc : Escs)
        Esc.writeMicroseconds(1000);
}

//hi
// Setup Function block
void setup()
{
    motor_init();

}

// Loop Function block
void loop()
{
}