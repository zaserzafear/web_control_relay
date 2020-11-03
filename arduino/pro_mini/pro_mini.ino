#include <SPI.h>
#include <SD.h>
void initSdCard() {
  const byte sdCardChipSelect = 4;
  Sd2Card card;
  if (!card.init(SPI_HALF_SPEED, sdCardChipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
  Serial.print("Card type : ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
  SdVolume volume;
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }
  Serial.print("Volume type is : FAT");
  Serial.println(volume.fatType(), DEC);
  uint32_t volumesize;
  volumesize = volume.blocksPerCluster();
  volumesize *= volume.clusterCount();
  volumesize /= 2;
  Serial.print("Volume size (Kb) : ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb) : ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb) : ");
  Serial.println((float)volumesize / 1024.0);
}
void readFromSdCard(String filePath = "test.txt") {
  File myFile;
  myFile = SD.open(filePath);
  if (myFile) {
    Serial.print(filePath);
    Serial.println(" : ");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.print("error opening ");
    Serial.println(filePath);
  }
}

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX | TX
String inComing = "";

unsigned long runTime = 0;
unsigned long previousTime = 0;
unsigned long timeInterval = 1000;

const byte powerPin = 14; //A0
const String powerOn = "power";
const byte restartPin = 15; //A1
const String powerRestart = "restart";
const byte clearBiosPin = 16; //A2
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
  Serial.println();
  Serial.println("Initializing SD card...");
  initSdCard();
  readFromSdCard();
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
