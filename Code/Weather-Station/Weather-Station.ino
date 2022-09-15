#include "SevSeg.h"
#include "DHT.h"

#define DHTPIN 14
#define DHTTYPE DHT11


//Create an instance of the object.
SevSeg sevseg;
DHT dht(DHTPIN, DHTTYPE);

#define PATTERN_CHANGE_TIME 1000
unsigned long timer = millis() - PATTERN_CHANGE_TIME;

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;   
  byte digitPins[] = {13, 10, 9, 7}; //Digits: 1,2,3,4 <--put one resistor (ex: 220 Ohms, or 330 Ohms, etc, on each digit pin)
  byte segmentPins[] = {12, 8, 5, 3, 2, 11, 6, 4}; //Segments: A,B,C,D,E,F,G,Period
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  sevseg.setBrightness(90);
  
  // Initialise la capteur DHT11
  dht.begin();
}

void loop() {

  
  // sur le moniteur serie
  //Serial.println("Temperature = " + String(dht.readTemperature())+" °C");
  //Serial.println("Humidite = " + String(dht.readHumidity())+" %");
  char temp[5] = "15*C";//String(round(dht.readTemperature()))+"*C";
  if (millis() > (timer + PATTERN_CHANGE_TIME)) {
    sevseg.setNumber(dht.readTemperature());
    Serial.println(dht.readTemperature());
    //Serial.println("Temperature = " + String(int(round(dht.readTemperature()))));
    timer = millis();
  }
  sevseg.refreshDisplay();
  //sevseg.refreshDisplay();
}
