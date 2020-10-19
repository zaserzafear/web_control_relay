#include <SPI.h>
#include <SD.h>
const byte sdCardModule = 4;
File myFile;
const char *pathIndexHtml = "index.html";
const char *pathJqueryMinJs = "js/jquery.min.js";
const char *pathBootstrapMinCss = "css/bootstrap.min.css";
const char *pathBootstrapThemeMinCss = "css/bootstrap-theme.min.css";
const char *pathBootstrapMinJs = "js/bootstrap.min.js";
const char *pathLoginCsv = "login.csv";
const char*  pathWebFile[] = {pathIndexHtml, pathJqueryMinJs, pathBootstrapMinCss, pathBootstrapThemeMinCss, pathBootstrapMinJs, pathLoginCsv};

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

  Serial.println("Initializing SD card...");
  initSdCard();

  Serial.println("Check Web File...");
  checkWebFile();
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
  } else if (inComing.equalsIgnoreCase(pathIndexHtml)) {
    readWebFile(pathIndexHtml);
  } else if (inComing.equalsIgnoreCase(pathJqueryMinJs)) {
    readWebFile(pathJqueryMinJs);
  } else if (inComing.equalsIgnoreCase(pathBootstrapMinCss)) {
    readWebFile(pathBootstrapMinCss);
  } else if (inComing.equalsIgnoreCase(pathBootstrapThemeMinCss)) {
    readWebFile(pathBootstrapThemeMinCss);
  } else if (inComing.equalsIgnoreCase(pathBootstrapMinJs)) {
    readWebFile(pathBootstrapMinJs);
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

void initSdCard() {
  if (!SD.begin(sdCardModule)) {
    Serial.println("initialization failed !");
  } else {
    Serial.println("initialization done.");
  }
}

void checkWebFile() {
  unsigned int fileWebCount = (sizeof(pathWebFile) / sizeof(pathWebFile[0]));
  Serial.print("File Web Count = ");
  Serial.println(fileWebCount);
  for (unsigned int i = 0; i < fileWebCount; i++) {
    char *fileWebName = pathWebFile[i];
    if (SD.exists(fileWebName)) {
      Serial.print("SUCCESS - Found ");
      Serial.print(fileWebName);
      Serial.println(" file.");
    } else {
      Serial.print("ERROR - Can't find ");
      Serial.print(fileWebName);
      Serial.println(" file.");
    }
  }
}

String readWebFile(String pathFile) {
  String fileContent = "";
  myFile = SD.open(pathFile);
  if (myFile) {
    while (myFile.available() > 0) {
      fileContent = myFile.readStringUntil("\r\n");
    }
  }
  myFile.close();
  return fileContent;
}

String checkLogin(String username, String password) {
  String fileContent = "";
  myFile = SD.open(pathFile);
  if (myFile) {
    while (myFile.available() > 0) {
      fileContent = myFile.readStringUntil("\r\n");
    }
  }
  myFile.close();
  return fileContent;
}
