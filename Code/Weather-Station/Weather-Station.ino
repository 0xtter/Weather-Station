#include "SevSeg.h" //https://wokwi.com/projects/342943329015562835
#include "DHT.h"
#include <Sleep_n0m1.h>

#define DHTPIN 14
#define DHTTYPE DHT11

Sleep sleep;
SevSeg sevseg;
DHT dht(DHTPIN, DHTTYPE);
  
#define PATTERN_CHANGE_TIME 5000
#define LOADING_PATTERN "----"

unsigned long timer = millis() - PATTERN_CHANGE_TIME;


const int button = 19;
const int sensorAlim = 16;

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

  pinMode(button,INPUT);
  pinMode(sensorAlim,OUTPUT);
}

void loop() {
  float temp;
  float hum;
  
  digitalWrite(16,HIGH);
  sevseg.setChars(LOADING_PATTERN);
  do{
    temp = dht.readTemperature();
    hum = dht.readHumidity();
    sevseg.refreshDisplay();
  }while(isnan(temp) || isnan(hum));
  
  String stringTemp = String(round(dht.readTemperature()))+"*C";
  String stringHum = " "+String(round(dht.readHumidity()*10)/10)+"h";
  
  timer = millis();
  sevseg.setChars(stringTemp.c_str());
  
  while(millis() < (timer + PATTERN_CHANGE_TIME)){
    if (millis() > (timer + PATTERN_CHANGE_TIME/2)){
      sevseg.setChars(stringHum.c_str());
    }
    sevseg.refreshDisplay();
  }
  sevseg.blank();
  digitalWrite(sensorAlim,LOW);
  delay(100); //delay to allow serial to fully print before sleep

  sleep.pwrDownMode(); //set sleep mode

  //Sleep till interrupt pin equals a particular state.
  //In this case "low" is state 0.
  sleep.sleepPinInterrupt(button,LOW); //(interrupt Pin Number, interrupt State)
  
}
