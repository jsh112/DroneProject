#include "Drone.hpp"

volatile size_t Drone::ThrottlePW = 1083;
volatile size_t Drone::RollPW = 1500;
volatile size_t Drone::PitchPW = 1529;
volatile size_t Drone::YawPW = 1504;

Drone::Drone() : 
    ESCFrontLeft(), ESCFrontRight(), ESCBackLeft(), ESCBackRight() {
    ESCFrontLeft.attach(Motor::FRONT_LEFT, minValue, maxValue);
    ESCFrontRight.attach(Motor::FRONT_RIGHT, minValue, maxValue);
    ESCBackLeft.attach(Motor::BACK_LEFT, minValue, maxValue);
    ESCBackRight.attach(Motor::BACK_RIGHT, minValue, maxValue);
}

void Drone::attachReceiverInterrupts()
{
    pinMode(PIN_THROTTLE, INPUT);
    pinMode(PIN_ROLL, INPUT);
    pinMode(PIN_PITCH, INPUT);
    pinMode(PIN_YAW, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_THROTTLE), ThrottleISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ROLL), RollISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_PITCH), PitchISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_YAW), YawISR, CHANGE);
}

// setter
void Drone::setThrottlePulseWidth(size_t pw){
    ThrottlePW = pw;
}
void Drone::setRollPulseWidth(size_t pw){
    RollPW = pw;
}
void Drone::setPitchPulseWidth(size_t pw){
    PitchPW = pw;
}
void Drone::setYawPulseWidth(size_t pw){
    YawPW = pw;
}

// getter
int Drone::getThrottlePulseWidth(){
    return ThrottlePW;
}
int Drone::getRollPulseWidth(){
    return RollPW;
}
int Drone::getPitchPulseWidth(){
    return PitchPW;
}
int Drone::getYawPulseWidth(){
    return YawPW;
}

// ISR Functions
void Drone::ThrottleISR()
{
    static uint32_t StartTime;
    if (digitalRead(PIN_THROTTLE))
    {
        StartTime = micros();
    }
    else
    {
        setThrottlePulseWidth(micros() - StartTime);
    }
}
void Drone::RollISR()
{
    static uint32_t StartTime;
    if (digitalRead(PIN_ROLL))
    {
        StartTime = micros();
    }
    else
    {
        setRollPulseWidth(micros() - StartTime);
    }
}
void Drone::PitchISR()
{
    static uint32_t StartTime;
    if (digitalRead(PIN_PITCH))
    {
        StartTime = micros();
    }
    else
    {
        setPitchPulseWidth(micros() - StartTime);
    }
}
void Drone::YawISR()
{
    static uint32_t StartTime;
    if (digitalRead(PIN_YAW))
    {
        StartTime = micros();
    }
    else
    {
        setYawPulseWidth(micros() - StartTime);
    }
}

void Drone::updateMotorPulseWidth(int& fl, int& fr, int& bl, int& br){
    this->setMotorSignal(this->ESCFrontLeft, fl);
    this->setMotorSignal(this->ESCFrontRight, fr);
    this->setMotorSignal(this->ESCBackLeft, bl);
    this->setMotorSignal(this->ESCBackRight, br);
}


void Drone::setMotorSignal(Servo& servo, int& MotorSig){
    servo.writeMicroseconds(MotorSig);
}