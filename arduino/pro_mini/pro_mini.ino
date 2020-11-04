#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX | TX
String inComing = "";
const byte powerPin = 14; //A0
const char* powerOn = "power";
const byte restartPin = 15; //A1
const char* powerRestart = "restart";
const byte clearBiosPin = 16; //A2
const char* clearBios = "clear";
const unsigned long intervalPowerButton = 255;
const unsigned long intervalClearBiosJumper = 5000;

void setPin() {
  pinMode(powerPin, OUTPUT);
  pinMode(restartPin, OUTPUT);
  pinMode(clearBiosPin, OUTPUT);
  pinMode(17, OUTPUT);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Serial.begin(9600);");
  mySerial.begin(9600);
  Serial.println("mySerial.begin(9600)");
  Serial.println();
  Serial.println("Set Pin Mode...");
  setPin();
  Serial.println();
}

void loop() {
  while (Serial.available() > 0) {
    inComing = Serial.readStringUntil('\n');
    if (inComing != "") {
      Serial.print("Serial Input = ");
      Serial.println(inComing);
      mySerial.println(inComing);
      readSerialData();
    }
  }

  while (mySerial.available() > 0) {
    inComing = mySerial.readStringUntil('\n');
    if (inComing != "") {
      Serial.print("mySerial Input = ");
      Serial.println(inComing);
      readSerialData();
    }
  }
}

void readSerialData() {
  inComing.trim();
  if (inComing.equalsIgnoreCase(powerOn)) {
    relayPowerOn();
  } else  if (inComing.equalsIgnoreCase(powerRestart)) {
    relayPowerRestart();
  } else if (inComing.equalsIgnoreCase(clearBios)) {
    relayClearBios();
  }
}

void relayPowerOn() {
  Serial.println("relayPowerOn");
  digitalWrite(powerPin, 1);
  digitalWrite(17, 1);
  delay(intervalPowerButton);
  digitalWrite(17, 0);
  digitalWrite(powerPin, 0);
}

void relayPowerRestart() {
  Serial.println("relayPowerRestart");
  digitalWrite(restartPin, 1);
  digitalWrite(17, 1);
  delay(intervalPowerButton);
  digitalWrite(17, 0);
  digitalWrite(restartPin, 0);
}

void relayClearBios() {
  Serial.println("relayPowerRestart");
  digitalWrite(clearBiosPin, 1);
  digitalWrite(17, 1);
  delay(intervalClearBiosJumper);
  digitalWrite(17, 0);
  digitalWrite(clearBiosPin, 0);
}
