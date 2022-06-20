#include <AccelStepper.h>
#include <FastLED.h>
#include <MsTimer2.h>

#define rxPin 0

unsigned long previousMillis = 0;
const long interval = 5;
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
int read_data_parse = 0;

bool flag = 0;

// 스텝 모터 2개 핀 설정.
// Motor X
int m1DirPin = 4;
int m1StepPin = 5;
// Motor Y
int m2DirPin = 7;
int m2StepPin = 6;
int M1en = 8; //Motor X enable pin
int M2en = 12; //Motor Y enable pin

unsigned int motorSpeed = 800 ;
unsigned long motorStep = 1 ;

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(m1DirPin, OUTPUT);
  pinMode(m1StepPin, OUTPUT) ;
  pinMode(m2DirPin, OUTPUT);
  pinMode(m2StepPin, OUTPUT) ;
  pinMode(M1en, OUTPUT);
  pinMode(M2en, OUTPUT);
  digitalWrite(m1DirPin, 0);
  digitalWrite(m2DirPin, 0) ;
  digitalWrite(m1StepPin, 0);
  digitalWrite(m2StepPin, 0) ;

  Serial.begin(9600);                                        // 시리얼 모니터 속도 정의
  Serial.setTimeout(20);

  // 패스트 라이브러리를 활용하여, 도트스타 스트립 설정 초기화
  FastLED.addLeds<DOTSTAR, 9, 10, BGR>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(255);
}

void loop() {
  reception_valueISR();
  //  unsigned long currentMillis = millis();

  //    if (flag == 0) {
  //      if (currentMillis - previousMillis >= interval) {
  //        previousMillis = currentMillis;
  //        digitalWrite(M1en, HIGH);
  //        digitalWrite(M2en, LOW);
  //        digitalWrite(m1StepPin, 1);
  //        delayMicroseconds(1000);
  //        digitalWrite(m1StepPin, 0) ;
  //        delayMicroseconds(1000) ;
  //      }
  //    } else {
  //      if (currentMillis - previousMillis >= interval) {
  //        previousMillis = currentMillis;
  //        digitalWrite(M1en, LOW);
  //        digitalWrite(M2en, HIGH);
  //        digitalWrite(m2StepPin, 1) ;
  //        delayMicroseconds(1000);
  //        digitalWrite(m2StepPin, 0);
  //        delayMicroseconds(1000);
  //      }
  //    }
}

void reception_valueISR() {     // RF통신으로 센서 값 수신
  if (Serial.available() > 0) {

    String read_data = Serial.readStringUntil('\n');  // rf통신으로 수신 받는 센서 값 string 타입으로 받기.

    read_data_parse = read_data.toInt();          // string 타임 값을 int형으로 변환하기.
    Serial.println(read_data_parse);

    // Dotstar에 센서 값 파싱.
    leds[read_data_parse] = CRGB::White;
    FastLED.show();

    // Dotstar on되는 범위 제한.
    if (read_data_parse >= 27) {
      leds[24] = CRGB::White;
    } else if (read_data_parse <= -10) {
      leds[3] = CRGB::White;
    }

    // 센서 값에 따른 모터 속도 변화.
    if (read_data_parse <= 7) {
      motorStart2_fast();
//      flag = 1;
    } else if (read_data_parse >= 25) {
      motorStart1_fast();
//      flag = 0;
    }
  } else {
    fadeToBlackBy(leds, NUM_LEDS, 5);
    FastLED.show();
    //    motorStop();
  }
}

void motorStop() {
  digitalWrite(M1en, LOW); // Low Level Enable
  digitalWrite(M2en, LOW); // Low Level Enable
  digitalWrite(m1DirPin, LOW);
  digitalWrite(m2DirPin, LOW);
  digitalWrite(m1StepPin, LOW);
  digitalWrite(m2StepPin, LOW) ;
}


// Slow function
void motorStart1_slow() {
  digitalWrite(M1en, HIGH);
  digitalWrite(M2en, LOW);
  digitalWrite(m1StepPin, 1);
  delayMicroseconds(1000);
  digitalWrite(m1StepPin, 0) ;
  delayMicroseconds(1000) ;
}
void motorStart2_slow() {
  digitalWrite(M1en, LOW);
  digitalWrite(M2en, HIGH);
  digitalWrite(m2StepPin, 1) ;
  delayMicroseconds(1000);
  digitalWrite(m2StepPin, 0);
  delayMicroseconds(1000) ;
}


// Fast function
void motorStart1_fast() {
  digitalWrite(M1en, HIGH);
  digitalWrite(M2en, LOW);
  digitalWrite(m1StepPin, 1);
  delayMicroseconds(5);
  digitalWrite(m1StepPin, 0) ;
  delayMicroseconds(5) ;
}
void motorStart2_fast() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(M1en, LOW);
    digitalWrite(M2en, HIGH);
    digitalWrite(m2StepPin, 1) ;
    delayMicroseconds(5);
    digitalWrite(m2StepPin, 0);
    delayMicroseconds(5);
  }
}
