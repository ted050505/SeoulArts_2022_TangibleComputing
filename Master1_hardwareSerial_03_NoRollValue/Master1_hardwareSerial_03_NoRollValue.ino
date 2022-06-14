#include <SoftwareSerial.h>
#include <FastLED.h>
#include <MsTimer2.h>

SoftwareSerial HC12(2,3);                                    // HC-12 TX Pin은 D2(RX용)에, HC-12 RX Pin을 D3(TX용)

#define NUM_LEDS  30
#define NUM_STRIPS 1
#define NUM_LEDS_PER_STRIP 30                                // led 동작 개수 
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
CRGB leds[NUM_LEDS];

char roll;
int lednum = 0;
bool sensor_value_true = false;
String value;
int cnt = 0;

char Buffer[20];

void setup() {
  Serial.begin(9600);                                        // 시리얼 모니터 속도 정의
  HC12.begin(9600);                                          // HC12 시리얼 통신 속도 정의

  // 패스트 라이브러리를 활용하여, 도트스타 스트립 설정 초기화 
  FastLED.addLeds<DOTSTAR, 9, 10, BGR>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(100);

   MsTimer2::set(1, reception_valueISR);
   MsTimer2::start();
}
 
void loop() {
  if (HC12.available() > 0) {
    sensor_value_true = true;
  }else{
    sensor_value_true = false;
  }
  
//  headphone_Wearing_start();
  Serial.print(roll);
  Serial.print(",");
  Serial.println(sensor_value_true);

  if (sensor_value_true) { 
//    uint16_t sinBeat = beatsin16(35, 0, NUM_LEDS - 1, 0, 0);
    leds[15] = CRGB::White;
//    fadeToBlackBy(leds, NUM_LEDS, 1);
    FastLED.show();
  }else{
    for (int i=0; i < 255; i++) {
      fadeToBlackBy(leds, NUM_LEDS, 5);
      FastLED.show();
    }
  }
}

void reception_valueISR() {     // RF통신으로 센서 값 수신 
    if(HC12.available() > 0) { 
//      instring = HC12.readStringUntil('\n');
//      instring1 = instring.toFloat();
      roll = Serial.write(HC12.read());                                      // HC-12 모듈의 출력 내용을 읽어 시리얼 모니터로 전송

//      char data = HC12.read();
//      
//      if (data == '\n') {
//        Buffer[cnt] = '\0';      
//      }
//        Buffer[cnt++] = data;
//        
//        sscanf(Buffer,"%d",&value);
//        Serial.println(value);

        
//      value = HC12.read();   
//      Serial.print(value);
    }
}

void headphone_Wearing_start() {
  leds[15] = CRGB::White;
  FastLED.show();
    for (int i = 0; i <= 15; i++) {
       leds[15 + i] = CRGB::White;
       leds[15 - i] = CRGB::White;
       FastLED.show();
       if (i==14) {
         for (int u = 0; u <= 255; u++) {
          fadeToBlackBy(leds, NUM_LEDS, 5);
          FastLED.show();
          }
       }
  }
}
