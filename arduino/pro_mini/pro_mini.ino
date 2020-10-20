#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX | TX
String inComing = "";

unsigned long runTime = 0;
unsigned long previousTime = 0;
unsigned long timeInterval = 1000;

const byte powerPin = 14;
const String powerOn = "power";
const byte restartPin = 15;
const String powerRestart = "restart";
const byte clearBiosPin = 16;
const String clearBios = "clear";
const unsigned long intervalPowerButton = 255;
const unsigned long intervalClearBiosJumper = 5000;

void setPin() {
  pinMode(powerPin, OUTPUT);
  pinMode(restartPin, OUTPUT);
  pinMode(clearBiosPin, OUTPUT);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Serial.begin(115200);");

  mySerial.begin(9600);
  Serial.println("mySerial.begin(9600)");

  Serial.println("Set Pin Mode...");
  setPin();
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
  Serial.println("Power.");
  digitalWrite(powerPin, 1);
  delay(intervalPowerButton);
  digitalWrite(powerPin, 0);
}

void relayPowerRestart() {
  Serial.println("Restart.");
  digitalWrite(restartPin, 1);
  delay(intervalPowerButton);
  digitalWrite(restartPin, 0);
}

void relayClearBios() {
  Serial.println("Clear BIOS.");
  digitalWrite(clearBiosPin, 1);
  delay(intervalClearBiosJumper);
  digitalWrite(clearBiosPin, 0);
}
