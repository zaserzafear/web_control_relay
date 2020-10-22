#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
const uint32_t connectTimeoutMs = 5000;
void checkWiFiConnect() {
  if (wifiMulti.run(connectTimeoutMs) != WL_CONNECTED) {
    Serial.println("!!");
  } else {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}


#include <WiFiClient.h>
WiFiClient wifiClient;
#include <ESP8266HTTPClient.h>
HTTPClient httpClient;
String readWebData(String Link = "https://www.google.co.th/") {
  httpClient.begin(wifiClient, Link);
  int httpCode = httpClient.GET();
  String payLoad = httpClient.getString();
  httpClient.end();
  return payLoad;
}

#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
const String linkIndexHtml = "https://drive.google.com/uc?export=download&id=19jHTfveYjz8lz64_wMkonXL7kJmEwdgb";
const String linkLoginHtml = "https://drive.google.com/uc?export=download&id=132b_zs15qLgbJs-HemHpUqT84gM7zHvz";
const String linkJqueryMinJs = "https://drive.google.com/uc?export=download&id=197YSxH7ZgGUKBJ1HhDRJ0cY8KVTvTKBy";
const String linkBootstrapMinCss = "https://drive.google.com/uc?export=download&id=1IAqyMu7ISbN9M5-X3WrwvbqnIeWlr4Zr";
const String linkBootstrapMinCssMap = "https://drive.google.com/uc?export=download&id=1Cxyrildg1I5IDPRkjvwvT4FzCBv-A5GC";
const String linkBootstrapThemeMinCss = "https://drive.google.com/uc?export=download&id=1y33RHBGHvauCW1gurxObcAu9uzZIk7n5";
const String linkBootstrapThemeMinCssMap = "https://drive.google.com/uc?export=download&id=1d73zR0sG4CFH4U4PMuwRQ_NZTiMauJ16";
const String linkBootstrapMinJs = "https://drive.google.com/uc?export=download&id=1O9yRCHjNdCKRmaVQlxPNLA40TcDLY2XQ";
void handleRoot() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  String result = readWebData(linkIndexHtml);
  String content;
  server.send(200, "text/html", content);
}
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }

  if (server.hasArg("DISCONNECT")) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }

  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin") {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }

    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }

  String content = "<html><body><form action='/login' method='POST'>To log in, please use : admin/admin<br>";
  content += "User:<input type='text' name='USERNAME' placeholder='user name'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";

  server.send(200, "text/html", content);
}
void handleJqueryMinJs() {
  String result = readWebData(linkJqueryMinJs);
  String content;
  server.send(200, "text/javascript", content);
}
void handleBootstrapMinCss() {
  String result = readWebData(linkBootstrapMinCss);
  String content;
  server.send(200, "text/css", content);
}
void handleBootstrapMinCssMap() {
  String result = readWebData(linkBootstrapMinCssMap);
  String content;
  server.send(200, "text/css", content);
}
void handleBootstrapThemeMinCss() {
  String result = readWebData(linkBootstrapThemeMinCss);
  String content;
  server.send(200, "text/css", content);
}
void handleBootstrapThemeMinCssMap() {
  String result = readWebData(linkBootstrapThemeMinCssMap);
  String content;
  server.send(200, "text/css", content);
}
void handleBootstrapMinJs() {
  String result = readWebData(linkBootstrapMinJs);
  String content;
  server.send(200, "text/javascript", content);
}

String inComing = "";

void setup() {
  Serial.begin(9600);
  Serial.print("Last Upload = ");
  Serial.print(__DATE__);
  Serial.print(" / ");
  Serial.print(__TIME__);
  Serial.println();

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Note_WIFI", "11111111");

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/js/jquery.min.js", handleJqueryMinJs);
  server.on("/css/bootstrap.min.css", handleBootstrapMinCss);
  server.on("/css/bootstrap.min.css.map", handleBootstrapMinCssMap);
  server.on("/css/bootstrap-theme.min.css", handleBootstrapThemeMinCss);
  server.on("/css/bootstrap-theme.min.css.map", handleBootstrapThemeMinCssMap);
  server.on("/js/bootstrap.min.js", handleBootstrapMinJs);

  server.on("/relay", handleRelay);
  server.onNotFound(handleNotFound);
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  checkWiFiConnect();
  server.handleClient();

  readSerialData();
}

void readSerialData() {
  while (Serial.available() > 0) {
    inComing = Serial.readStringUntil('\n');
    if (inComing != "") {
      inComing.trim();
      Serial.println(inComing);
    }
  }
}

void handleRelay() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if (server.hasArg("relay")) {
    Serial.println(server.arg("relay"));
    return;
  }
}

void handleNotFound() {
  server.send(404, "text/html", "<h1>404 error: File not found</h1>");
}

bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}
