#include "SevSeg.h" //https://wokwi.com/projects/342943329015562835
#include "DHT.h"

#define DHTPIN 14
#define DHTTYPE DHT11


SevSeg sevseg;
DHT dht(DHTPIN, DHTTYPE);
  
#define PATTERN_CHANGE_TIME 1000
unsigned long timer = millis() - PATTERN_CHANGE_TIME;

byte sensorSelector = 1;
byte numberofSensors = 2;

void setup() {
  Serial.begin(9600);
  
  byte numDigits = 4;   
  byte digitPins[] = {13, 10, 9, 7}; //Digits: 1,2,3,4 <--put one resistor (ex: 220 Ohms, or 330 Ohms, etc, on each digit pin)
  byte segmentPins[] = {12, 8, 5, 3, 2, 11, 6, 4}; //Segments: A,B,C,D,E,F,G,Period
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  

  // Initialize 4 digits 7 segments display
  Serial.println("Initializing display...");
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);

  // Initialize DHT11 sensors
  Serial.println("Initializing sensors...");
  dht.begin();
}

void loop() {
  
  if (millis() > (timer + PATTERN_CHANGE_TIME)) {
    switch(sensorSelector){
      case 1:
        sevseg.setChars((String(round(dht.readTemperature()))+"*C").c_str());
        break;
      case 2:
        sevseg.setChars((" "+String(round(dht.readHumidity()*10)/10)+"h").c_str());
        break;
    }
    sensorSelector = ((sensorSelector++)%numberofSensors)+1;
    timer = millis();
  }
  sevseg.refreshDisplay();
}
