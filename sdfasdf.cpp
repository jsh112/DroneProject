#include <Arduino.h>
#include <Servo.h>
#include "Arduino_BMI270_BMM150.h"
#include <math.h>

// ESC 핀 정의
const int escPin1 = 9; // ESC1 핀
const int escPin2 = 8; // ESC2 핀
const int escPin3 = 7; // ESC3 핀
const int escPin4 = 6; // ESC4 핀

const int r9dThrottlePin = 2; // R9D 스로틀 신호 입력 핀
const int r9dOnePin = 3; // R9D 1 channel 신호 입력 핀
const int r9dTwoPin = 4; // R9D 2 channel 신호 입력 핀

// Throttle PWM 신호 범위 (마이크로초)
const int inputMin = 1150; // 입력 신호 최소값
const int inputMax = 1950; // 입력 신호 최대값
const int outputMin = 1000; // 변환된 신호 최소값
const int outputMax = 2000; // 변환된 신호 최대값

// Throttle PWM
unsigned long esc1_PWM = 900;
unsigned long esc2_PWM = 900;
unsigned long esc3_PWM = 900;
unsigned long esc4_PWM = 900;


// 1 channel PWM
unsigned long esc1_One = 1525;
unsigned long esc2_One = 1525;
unsigned long esc3_One = 1525;
unsigned long esc4_One = 1525;

// 2 channel PWM
unsigned long esc1_Two = 1530;
unsigned long esc2_Two = 1530;
unsigned long esc3_Two = 1530;
unsigned long esc4_Two = 1530;

Servo esc1, esc2, esc3, esc4;

// Throttle PWM 신호 계산 변수
volatile unsigned long Throttle_startTime = 0; // HIGH 시작 시간
volatile unsigned long Throttle_pulseWidth = 0; // HIGH 지속 시간

// 1 Channel PWM 신호 계산 변수
volatile unsigned long One_startTime = 0; // HIGH 시작 시간
volatile unsigned long One_pulseWidth = 0; // HIGH 지속 시간

// 1 Channel PWM 신호 계산 변수
volatile unsigned long Two_startTime = 0; // HIGH 시작 시간
volatile unsigned long Two_pulseWidth = 0; // HIGH 지속 시간



void Throttle_ISR() {
  // RISING 또는 FALLING 이벤트를 감지하여 HIGH 시간 계산
  if (digitalRead(r9dThrottlePin) == HIGH) {
    Throttle_startTime = micros(); // RISING: HIGH 상태 시작 시간 기록
  } else {
    Throttle_pulseWidth = micros() - Throttle_startTime; // FALLING: HIGH 지속 시간 계산
  }
}

void One_ISR() {
  // RISING 또는 FALLING 이벤트를 감지하여 HIGH 시간 계산
  if (digitalRead(r9dOnePin) == HIGH) {
    One_startTime = micros(); // RISING: HIGH 상태 시작 시간 기록
  } else {
    One_pulseWidth = micros() - One_startTime; // FALLING: HIGH 지속 시간 계산
  }
}

void Two_ISR() {
  // RISING 또는 FALLING 이벤트를 감지하여 HIGH 시간 계산
  if (digitalRead(r9dTwoPin) == HIGH) {
    Two_startTime = micros(); // RISING: HIGH 상태 시작 시간 기록
  } else {
    Two_pulseWidth = micros() - Two_startTime; // FALLING: HIGH 지속 시간 계산
  }
}

void calibrateESC() {
  Serial.println("ESC 보정을 시작합니다...");
  esc1.writeMicroseconds(outputMax);
  esc2.writeMicroseconds(outputMax);
  esc3.writeMicroseconds(outputMax);
  esc4.writeMicroseconds(outputMax);
  delay(2000); // 최대값 신호 대기

  esc1.writeMicroseconds(outputMin);
  esc2.writeMicroseconds(outputMin);
  esc3.writeMicroseconds(outputMin);
  esc4.writeMicroseconds(outputMin);
  delay(2000); // 최소값 신호 대기

  Serial.println("ESC 보정이 완료되었습니다.");
}

// 가속도, 자이로, 자기장 변수 선언
float ax, ay, az;   // 가속도 
float gx, gy, gz;   // 자이로 
float mx, my, mz;   // 자기장 

// Pitch, roll, yaw
float pitch;
float roll;
float yaw;

// pitch, roll 오프셋. - pitch랑 roll을 0으로 만들려고
float pitchOffset = 0;
float rollOffset = 0;
float yawOffset = 0;
bool offsetFlag = false;

// 저주파 필터
float filteredYaw = 0; // 필터링된 초기값
/*
float filteredPitch = 0;
float filteredRoll = 0;
*/

// 필터 계수
const float alpha = 0.1;
//const float filterAlpha = 0.2;

// 360도 경계 문제를 해결하는 함수
float wrapYaw(float angle) {
  if (angle > 180) {
    angle -= 360;
  } else if (angle < -180) {
    angle += 360;
  }
  return angle;
}

// 9축 센서 정보로 Yaw, Pitch, Roll 데이터 계산하는 함수
void calculateOrientation() {
  // Pitch (x축) -> 오른쪽이 올라가면 + 왼쪽이 올라가면 -
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / M_PI;
  // Roll (y축)
  roll = atan2(-ay, az) * 180 / M_PI;
  // Yaw (z축) - 자기장 데이터 사용
  yaw = atan2(-my, mx) * 180 / M_PI;

  if(!offsetFlag){
    pitchOffset = pitch;
    rollOffset = roll;
    yawOffset = yaw;
    offsetFlag = true;
  }

  // offset 적용
  pitch -= pitchOffset;
  roll -= rollOffset;

  //filteredPitch = filterAlpha * pitch + (1 - filterAlpha) * filteredPitch;
  //filteredRoll = filterAlpha * roll + (1 - filterAlpha) * filteredRoll;

  yaw = yaw - yawOffset - 90;

  // Yaw 값을 -180도 ~ 180도 범위로 변환
  yaw = wrapYaw(yaw);

  // 저역 통과 필터 적용
  float diff = wrapYaw(yaw - filteredYaw); // 경계 문제 보정
  filteredYaw += alpha * diff;

  // 필터링 후 Yaw 값도 -180도 ~ 180도 범위 유지
  filteredYaw = wrapYaw(filteredYaw);
}

void setup() {
  // ESC 핀 초기화
  esc1.attach(escPin1, outputMin, outputMax);
  esc2.attach(escPin2, outputMin, outputMax);
  esc3.attach(escPin3, outputMin, outputMax);
  esc4.attach(escPin4, outputMin, outputMax);

  // 핀 모드 설정
  pinMode(r9dThrottlePin, INPUT); // R9D 스로틀 입력 핀
  attachInterrupt(digitalPinToInterrupt(r9dThrottlePin), Throttle_ISR, CHANGE); // 인터럽트 핸들러 등록

  // 핀 모드 설정
  pinMode(r9dOnePin, INPUT); // R9D 스로틀 입력 핀
  attachInterrupt(digitalPinToInterrupt(r9dOnePin), One_ISR, CHANGE); // 인터럽트 핸들러 등록


// 핀 모드 설정
  pinMode(r9dTwoPin, INPUT); // R9D 스로틀 입력 핀
  attachInterrupt(digitalPinToInterrupt(r9dTwoPin), Two_ISR, CHANGE); // 인터럽트 핸들러 등록

  // 시리얼 모니터 시작
  Serial.begin(115200);

  // 센서 초기화
  if (!IMU.begin()) {
    //Serial.println("Failed to initialize BMI270/BMM150!");
    while (1);
  }

  // ESC 보정 수행
  calibrateESC();

  // 초기화: 최소 신호 전송
  esc1.writeMicroseconds(outputMin);
  esc2.writeMicroseconds(outputMin);
  esc3.writeMicroseconds(outputMin);
  esc4.writeMicroseconds(outputMin);

  delay(2000); // ESC 초기화 대기
}

void loop() {
  // 인터럽트에서 계산된 PWM 신호 읽기
  noInterrupts(); // 데이터 무결성을 위해 인터럽트 비활성화
  unsigned long currentPulseWidth = Throttle_pulseWidth;
  unsigned long currentOne = One_pulseWidth;
  unsigned long currentTwo = Two_pulseWidth;
  interrupts(); // 인터럽트 활성화

  // 입력 신호를 변환 (1150~1950 -> 1000~2000)
  unsigned long mappedPulseWidth = map(currentPulseWidth, inputMin, inputMax, outputMin, outputMax);

  if(currentOne > 1600){ // left - esc2, esc3
    esc1_One = abs(currentOne - 1600);
    esc2_One = 0;
    esc3_One = 0;
    esc4_One = abs(currentOne - 1600);
  }
  else if(currentOne < 1450){ // right - esc1, esc4
    esc1_One = 0;
    esc2_One = abs(1450 - currentOne);
    esc3_One = abs(1450 - currentOne);
    esc4_One = 0;
  }
  else{
    esc1_One = 0;
    esc2_One = 0;
    esc3_One = 0;
    esc4_One = 0;
  }

if(currentTwo > 1600){ // back - esc1, esc2
    esc1_Two = abs(currentTwo - 1600);
    esc2_Two = abs(currentTwo - 1600);
    esc3_Two = 0;
    esc4_Two = 0;
  }
  else if(currentTwo < 1460){ // front - esc3, esc4
    esc1_Two = 0;
    esc2_Two = 0;
    esc3_Two = abs(1460 - currentTwo);
    esc4_Two = abs(1460 - currentTwo);
  }
  else{
    esc1_Two = 0;
    esc2_Two = 0;
    esc3_Two = 0;
    esc4_Two = 0;
  }
  
  // 각 모터 PWM값 설정
  esc1_PWM = mappedPulseWidth + esc1_One + esc1_Two;
  esc2_PWM = mappedPulseWidth + esc2_One + esc2_Two;
  esc3_PWM = mappedPulseWidth + esc3_One + esc3_Two;
  esc4_PWM = mappedPulseWidth + esc4_One + esc4_Two;
  

  // esc1_PWM
  if (esc1_PWM >= outputMin && esc1_PWM <= outputMax) {
    esc1.writeMicroseconds(esc1_PWM);

    // Serial.print("esc1 PWM: ");
    //Serial.println(esc1_PWM);
  } 
  else if (esc1_PWM < outputMin){
    esc1.writeMicroseconds(900);
    //Serial.println("stop");
  }
  else{
    esc1.writeMicroseconds(2000);
    //Serial.println("High");
  }

  // esc2_PWM
  if (esc2_PWM >= outputMin && esc2_PWM <= outputMax) {
    esc2.writeMicroseconds(esc2_PWM);

    //Serial.print("esc2 PWM: ");
    //Serial.println(esc2_PWM);
  } 
  else if (esc2_PWM < outputMin){
    esc2.writeMicroseconds(900);
    //Serial.println("stop");
  }
  else{
    esc2.writeMicroseconds(2000);
    //Serial.println("High");
  }

  // esc3_PWM
  if (esc3_PWM >= outputMin && esc3_PWM <= outputMax) {
    esc3.writeMicroseconds(esc3_PWM);

    //Serial.print("esc3 PWM: ");
    //Serial.println(esc3_PWM);
  } 
  else if (esc3_PWM < outputMin){
    esc3.writeMicroseconds(900);
    //Serial.println("stop");
  }
  else{
    esc3.writeMicroseconds(2000);
    //Serial.println("High");
  }

  // esc4_PWM
  if (esc4_PWM >= outputMin && esc4_PWM <= outputMax) {
    esc4.writeMicroseconds(esc4_PWM);

    //Serial.print("esc4 PWM: ");
    //Serial.println(esc4_PWM);
  } 
  else if (esc4_PWM < outputMin){
    esc4.writeMicroseconds(900);
    //Serial.println("stop");
  }
  else{
    esc4.writeMicroseconds(2000);
    //Serial.println("High");
  }
  
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);
    IMU.readMagneticField(mx, my, mz);

    // Yaw, Pitch, Roll 계산
    calculateOrientation();

    // 데이터 전송 (Yaw, Pitch, Roll)
    Serial.print(filteredYaw); Serial.print(",");
    Serial.print(pitch); Serial.print(",");
    Serial.println(roll);
  }


  // 실전용
  delay(50);
  // test 용
  //delay(1000); // 주기적으로 신호 유지
}
