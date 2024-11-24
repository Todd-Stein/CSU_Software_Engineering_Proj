#include <FastLED.h>
#define MAX_LED 1024
#define DATA_PIN 16
enum class State {NotConnected, ConnectionEstablished, RecievingData, SendingData, ChangeLED, TEST_DoNothing, TEST_ReadSerial};
State currentState = State::ConnectionEstablished;
unsigned int ledWide = 0;
unsigned int ledHigh = 0;
unsigned int totalLEDs = 0;
CRGB leds[MAX_LED];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, MAX_LED);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  //currentState = State::TEST_ReadSerial;
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(currentState) {
    case State::NotConnected:
    {
      Serial.begin(9600);
      break;
    }
    case State::ConnectionEstablished:
    {
        if (Serial.available()>0) {
        char initialStream [5];
        Serial.readBytes(initialStream, 5);
        if(initialStream[0]=='S' && initialStream[1]== 'T' && (char)initialStream[2]=='A' && (char)initialStream[3]=='R' && (char)initialStream[4]=='T') {
          Serial.println("Start data stream.");
          currentState = State::RecievingData;
          byte newDims [2];
          Serial.readBytes(newDims, 2);
          ledWide = (unsigned int)newDims[0];
          ledHigh = (unsigned int)newDims[1];
          Serial.println("New dimensions ");
          Serial.print(ledWide);
          Serial.print("\n");
          Serial.print(ledHigh);
          Serial.print("\n\n\n");
          totalLEDs = ledWide*ledHigh;
        }
        //break;
      }
      Serial.println("No data");
      break;
    }
    case State::ChangeLED:
    { 
      //delete [] leds;
      //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, totalLEDs);
      currentState = State::RecievingData;
      break;
    }
    case State::RecievingData:
    {
      byte colors [totalLEDs*3];
      int ledIdx = 0;
      Serial.readBytes(colors, totalLEDs*3);
      for(int i = 0; i<totalLEDs*3; i+=3) {
        delay(2000);
        Serial.println("next color");
        Serial.print(i);
        Serial.print(totalLEDs);
        CRGB color;
        color.red = colors[i+2];
        color.blue = colors[i];
        color.green = colors[i+1];
        leds[ledIdx] = color;
        ledIdx++;
      }
      Serial.println("colors set");
      currentState = State::SendingData;
      break;
    }
    case State::SendingData:
    {
      Serial.write("finished recieving data");
      currentState = State::ConnectionEstablished;
      break;
    }
    case State::TEST_DoNothing:
    {
      delay(1000);
      break;
    }
    case State::TEST_ReadSerial:
      char data[100];
      while (Serial.available()>0) {
        Serial.readBytes(data, 20);
        for(int i = 0; i<20; i++) {
          Serial.println((char)data[i]);
        }
      }
  }
}
