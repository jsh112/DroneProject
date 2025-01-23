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
    return this->ThrottlePW;
}
int Drone::getRollPulseWidth(){
    return this->RollPW;
}
int Drone::getPitchPulseWidth(){
    return this->PitchPW;
}
int Drone::getYawPulseWidth(){
    return this->YawPW;
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