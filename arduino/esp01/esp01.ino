String inComing = "";

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
void checkWiFiConnect()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi re connecting...");
    if (wifiMulti.run() == WL_CONNECTED)
    {
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Start Setup.");

  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS not ready");
    return;
  }

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Note_WIFI", "11111111");
  wifiMulti.addAP("Note_WIFI2", "11111111");
  Serial.println("WiFi connecting");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print('.');
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/login.php", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/login.php");
    request->send(SPIFFS, "/login.html", "text/html");
  });
  server.on("/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/js/jquery.min.js");
    request->send(SPIFFS, "/js/jquery.min.js", "text/javascript");
  });
  server.on("/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/css/bootstrap.min.css");
    request->send(SPIFFS, "/css/bootstrap.min.css", "text/css");
  });
  server.on("/css/bootstrap-theme.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/css/bootstrap-theme.min.css");
    request->send(SPIFFS, "/css/bootstrap-theme.min.css", "text/css");
  });
  server.on("/js/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/js/bootstrap.min.js");
    request->send(SPIFFS, "/js/bootstrap.min.js", "text/javascript");
  });
  server.on("/relay.php", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.print("web server : ");
    Serial.println("/relay.php");
    String relayParamName = "relay";
    String relayParamValue = "";

    if (request->hasParam(relayParamName, true))
    {
      AsyncWebParameter *p = request->getParam(relayParamName, true);
      relayParamValue = p->value();
      Serial.println(relayParamValue);
    }

    request->send(200, "text/plain", "param name : " + relayParamName + " // param value : " + relayParamValue);
  });
  server.onNotFound([](AsyncWebServerRequest *request) {
    Serial.println("web server : onNotFound");
    request->send(404, "text/html", "<H1>HTTP ERROR 404</H1>");
  });

  server.begin();
  Serial.println("Server Begin complete!!!");
  Serial.println();
}

void loop()
{
  readSerialData();
  checkWiFiConnect();
}

void readSerialData()
{
  while (Serial.available() > 0)
  {
    inComing = Serial.readStringUntil('\n');
    if (inComing != "")
    {
      inComing.trim();
      Serial.println(inComing);
    }
  }
}
