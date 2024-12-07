#include <FastLED.h>
#define LED_WIDE 25
#define LED_HIGH 11
#define NUM_LEDS LED_WIDE*LED_HIGH
#define DATA_PIN 2
enum class State {NotConnected, ConnectionEstablished, AwaitingData, RecievingData, SendingData, TESTREAD};
State currentState = State::ConnectionEstablished;
//unsigned int ledWide = 0;
//unsigned int ledHigh = 0;
//unsigned int totalLEDs = 0;
CRGB leds[NUM_LEDS];
unsigned int colorIdx = 0u;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  FastLED.clearData();
  // put your setup code here, to run once:
  Serial.begin(115200);
  //currentState = State::TESTREAD;

}

  inline int ConvertEndian(int val) {
    return ((val>>24)&0x000000FF) | ((val >> 8) & 0x0000FF00) | ((val << 8) & 0x00FF0000) | ((val<<24)&0xFF000000);
  }  
//x
void loop() {
  //Test loop
  //for(int i = 0; i<NUM_LEDS; i++) {
  //  leds[i] = CRGB::BurlyWood;
  //  FastLED.show();
  //}
  switch(currentState) {
    case (State::ConnectionEstablished):
    {
      if(Serial.available()>0) {
        char get[3];
        Serial.readBytes(get, 3);
        if(get[0]=='G'&&get[1]=='E'&&get[2]=='T') {
          //Serial.println("newDims");
          char dimensions[4];
          dimensions[0] = (char)LED_WIDE;
          dimensions[1] = 'W';
          dimensions[2] = (char)LED_HIGH;
          dimensions[3] = 'H';
          Serial.write(dimensions, 4);
          fill_solid(leds,NUM_LEDS,CRGB::Black);        
        }
        else if(get[0]=='S'&&get[1]=='T'&&get[2]=='0') {
          currentState = State::RecievingData;
          break;
        }
        else {
          //TODO: implement video support
          //likely need chrono
        }
      }
      break;
    }
    case (State::AwaitingData): {
      //Serial.println("state2");
      if(Serial.available()>0) {
        currentState = State::RecievingData;
      }
      break;
    }
    case (State::RecievingData): {
      if(Serial.available()>0) { 
        if(colorIdx>=NUM_LEDS) {
          currentState = State::ConnectionEstablished;
          //FastLED.show();
          Serial.println("write success");
          //Serial.flush();
          colorIdx=0u;
        }
        char color [3];
        Serial.readBytes(color, 3);
        leds[colorIdx] = CRGB((int)color[2], (int)color[1], (int)color[0]);
        colorIdx++;
        FastLED.show();
        
      }
      break;
    }
    //case State::TESTREAD:
    //{
     // char color[3];
     // if(Serial.available()>0) {
     //   Serial.readBytes(color, 3);
     //   CRGB color3;
     //   color3.red = color[0];
     //   color3.blue = color[1];
     //   color3.green = color[2];
     //   leds[30] = color3;
     //   FastLED.show();
     // }
     // break;
    //}
  }
}
