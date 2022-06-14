#include <SoftwareSerial.h>
#include <FastLED.h>

SoftwareSerial HC12(2,3);                                    // HC-12 TX Pin은 D2(RX용)에, HC-12 RX Pin을 D3(TX용)

#define NUM_STRIPS 2
#define NUM_LEDS_PER_STRIP 15                                // led 동작 개수 
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

CRGB source1[NUM_LEDS];
CRGB source2[NUM_LEDS];
CRGB output[NUM_LEDS];

uint8_t blendAmount = 0;
uint8_t patternCounter = 0;
uint8_t source1Pattern = 0;
uint8_t source2Pattern = 1;
bool useSource1 = false;

float roll;
int brightness = 0;

unsigned long previousMillis = 0;
const long interval = 1000;

bool ledState = 0;

void setup() {
  Serial.begin(9600);                                        // 시리얼 모니터 속도 정의
  HC12.begin(9600);                                          // HC12 시리얼 통신 속도 정의

  // 패스트 라이브러리를 활용하여, 도트스타 스트립 설정 초기화 
//  FastLED.addLeds<DOTSTAR, 4, 5, BGR>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<DOTSTAR, 4, 5, GRB>(output, NUM_LEDS);
  FastLED.setBrightness(50);

//  headphone_Wearing();
for (int i = 0; i < 1000; i++) {
    EVERY_N_MILLISECONDS(1) {
      blend(source1, source2, output, NUM_LEDS, blendAmount);   // Blend between the two sources
    }
    runPattern(source1Pattern, source1);                  // Run both patterns simultaneously  
    FastLED.show();
  }
}
 
void loop() {
  unsigned long currentMillis = millis();
  
//  if (HC12.available()) {                                     // HC-12에 수신 데이터가 존재하는 경우
//    recive_value();
    
//    headphone_Wearing();

  
 }

void runPattern(uint8_t pattern, CRGB *LEDArray) {
  switch (pattern) {
    case 0:
      movingDots(LEDArray);
      break;
  }
}

void movingDots(CRGB *LEDarray) {
  
  uint16_t posBeat  = beatsin16(30, 0, NUM_LEDS - 1, 0, 0);
  uint16_t posBeat2 = beatsin16(60, 0, NUM_LEDS - 1, 0, 0);

  uint16_t posBeat3 = beatsin16(30, 0, NUM_LEDS - 1, 0, 32767);
  uint16_t posBeat4 = beatsin16(60, 0, NUM_LEDS - 1, 0, 32767);

  LEDarray[(posBeat + posBeat2) / 2]  = CRGB::White;
  LEDarray[(posBeat3 + posBeat4) / 2]  = CRGB::White;

  fadeToBlackBy(LEDarray, NUM_LEDS, 10);
}

void recive_value() {     // RF통신으로 센서 값 수신 
//    roll = HC12.read();
    Serial.write(HC12.read());                                      // HC-12 모듈의 출력 내용을 읽어 시리얼 모니터로 전송
}

void headphone_Wearing() {
//  if(currentMillis - previousMillis >= interval) {
//        for (int brightness = 0; brightness < 255; brightness++) {
//            previousMillis = currentMillis;
//            FastLED.setBrightness(brightness);
//            leds[15] = CRGB::White;
//            FastLED.show();
//        }
//      }
//      if(currentMillis - previousMillis >= interval) {
//        for (int brightness = 255; brightness > 0; brightness--) {
//            previousMillis = currentMillis;
//            FastLED.setBrightness(brightness);
//            leds[15] = CRGB::White;
//            FastLED.show();
//        }
//      }
    for (int i = 0; i < 10; i++) {
        for(int brightness = 0; brightness < 255; brightness = brightness+3) {
//          FastLED.setBrightness(brightness);
          leds[15 + i] = CRGB::White;
          leds[15 - i] = CRGB::White;
          fadeToBlackBy(leds, NUM_LEDS, 10);
          delay(0.1);
          FastLED.show();
        }
      }
   for (int i = 0; i < 10; i++) {
          leds[15 + i] = CRGB::White;
          leds[15 - i] = CRGB::White;
          for (int i = 0; i < 100; i++) {
          fadeToBlackBy(leds, NUM_LEDS, 10);
          delay(1);
          FastLED.show();
          }
   }
   FastLED.setBrightness(0);
}


//for (int j = 0; j < 10; j++) {
//    static uint8_t hue = 0;
//    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
//      FastLED.showColor(CHSV(hue++, 255, 255));
//      delay(10);
//    }
//  }
//  for (int j = 0; j < 10; j++) {
//    for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
//      leds[i] = CRGB::Blue;
//      leds[i+1] = CRGB::Green;
//      leds[i+2] = CRGB::Red;
//      FastLED.show();
//      leds[i] = CRGB::Black;
//      leds[i+1] = CRGB::Black;
//      leds[i+2] = CRGB::Black;
//      delay(50);
//    }
//  }
