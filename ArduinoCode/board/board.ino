#include <FastLED.h>
#define LED_WIDE 20
#define LED_HIGH 40
#define NUM_LEDS LED_WIDE*LED_HIGH
#define DATA_PIN 16
enum class State {NotConnected, ConnectionEstablished, AwaitingData, RecievingData, SendingData};
State currentState = State::ConnectionEstablished;
//unsigned int ledWide = 0;
//unsigned int ledHigh = 0;
//unsigned int totalLEDs = 0;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // put your setup code here, to run once:
  Serial.begin(9600);
  currentState = State::ConnectionEstablished;
}

void loop() {
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
          //Serial.println("endNewDims");
          currentState = State::AwaitingData;
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
      //Serial.println("state 3");
      byte dataPacket[(NUM_LEDS*3)+4];
      Serial.readBytes(dataPacket, (NUM_LEDS*3)+4);
      if(dataPacket[0]=='S' && dataPacket[1]=='T') {
        int ledIdx = 0;
        for(int i = 0; i<NUM_LEDS*3; i+=3) {
          CRGB pixelColor;
          pixelColor.red = dataPacket[i+2];
          pixelColor.blue = dataPacket[i];
          pixelColor.green = dataPacket[i+1];
          leds[ledIdx] = pixelColor;
          ledIdx++;
        }
        int lastIndex = (NUM_LEDS*3)+3;
        if(dataPacket[lastIndex-1] == 'E' && dataPacket[lastIndex]=='N') {
          currentState = State::AwaitingData;
        }
      }
      break;
    }
  }
  char reset[5];
  Serial.readBytes(reset, 5);
  if(reset[0]=='R'&&reset[1]=='E'&&reset[2]=='S'&&reset[3]=='E'&&reset[4]=='T') {
    currentState = State::ConnectionEstablished;
  }
}
