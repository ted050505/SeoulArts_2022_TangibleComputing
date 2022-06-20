#include <AccelStepper.h>
#include <FastLED.h>
#include <MsTimer2.h>

#define HALFSTEP 8  //Half-step mode (8 step control signal sequence)
////#define STEPS 200
#define rxPin 0

#define mtrPin1  8     // IN1 on the ULN2003 driver 1
#define mtrPin2  9     // IN2 on the ULN2003 driver 1
#define mtrPin3  10     // IN3 on the ULN2003 driver 1
#define mtrPin4  11     // IN4 on the ULN2003 driver 1

#define mtrPin_1  4     // IN1 on the ULN2003 driver 2
#define mtrPin_2  5     // IN2 on the ULN2003 driver 2
#define mtrPin_3  6     // IN3 on the ULN2003 driver 2
#define mtrPin_4  7     // IN4 on the ULN2003 driver 2

AccelStepper stepper1(HALFSTEP, mtrPin1, mtrPin3, mtrPin2, mtrPin4);
AccelStepper stepper2(HALFSTEP, mtrPin_1, mtrPin_3, mtrPin_2, mtrPin_4);

int speedLeft = 500;
int speedRight = 500;
int leftWheelForeward = 1;
int leftWheelBackward = -1;
int rightWheelForeward = -1;
int rightWheelBackward = 1;

unsigned long previousMillis = 0;
const long interval = 1000;
const long interval_2 = 1000;

#define NUM_LEDS  30
#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 30                                // led 동작 개수 
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
CRGB leds[NUM_LEDS];

long baud = 9600;
char roll;
int lednum = 0;
bool sensor_value_true = false;
int read_data_parse_old = 0;

bool motor_turn = false;

// 스텝 모터 2개 핀 설정.
// Motor X
int m1DirPin = 4;
int m1StepPin = 5;
// Motor Y
int m2DirPin = 7;
int m2StepPin = 6;
int M1en = 8; //Motor X enable pin
int M2en = 12; //Motor Y enable pin

void setup() {
  pinMode(rxPin, INPUT);
  stepper1.setMaxSpeed(2000.0);
  stepper2.setMaxSpeed(2000.0);

  Serial.begin(9600);                                        // 시리얼 모니터 속도 정의
  Serial.setTimeout(20);

  // 패스트 라이브러리를 활용하여, 도트스타 스트립 설정 초기화
  FastLED.addLeds<DOTSTAR, 9, 10, BGR>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(250);
}

void loop() {
  reception_valueISR();
}

void reception_valueISR() {     // RF통신으로 센서 값 수신
  if (Serial.available() > 0) {

    String read_data = Serial.readStringUntil('\n');  // rf통신으로 수신 받는 센서 값 string 타입으로 받기.

    int read_data_parse = read_data.toInt();          // string 타임 값을 int형으로 변환하기.
    Serial.println(read_data_parse);
    read_data_parse_old = read_data_parse;

    // Dotstar에 센서 값 파싱.
    leds[read_data_parse] = CRGB::White;
    FastLED.show();

    // Dotstar on되는 범위 제한.
    if (read_data_parse >= 27) {
      leds[24] = CRGB::White;
    }else if (read_data_parse <= -10) {
      leds[3] = CRGB::White;
    }
    
    // 센서 값에 따른 모터 속도 변화.
    if (read_data_parse <= 7) {
        motorStart2_fast();
      }
    } else if (read_data_parse >= 25) {
      motorStart1_fast();
    } else {
      //        motorStop();
    }
  } else {
    fadeToBlackBy(leds, NUM_LEDS, 5);
    FastLED.show();
    motorStop();
  }
}

void motorStop() {
}


// Slow function
void motorStart1_slow() {
  stepper1.move(leftWheelForeward);
  stepper2.move(rightWheelForeward);
  stepper1.setSpeed(500);
  stepper2.setSpeed(500);
  stepper1.runSpeedToPosition();
  stepper2.runSpeedToPosition();
  //delay(10);
}
void motorStart2_slow() {
}


// Fast function
void motorStart1_fast() {
}
void motorStart2_fast() {
}
