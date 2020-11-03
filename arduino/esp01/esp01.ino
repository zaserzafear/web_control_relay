#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
void checkWiFiConnect() {
  while (WiFi.status() != WL_CONNECTED) {
    if (wifiMulti.run() == WL_CONNECTED) {
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
  }
}

#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
const String linkIndexHtml = "";
const String linkLoginHtml = "";
const String linkJqueryMinJs = "";
const String linkBootstrapMinCss = "";
const String linkBootstrapMinCssMap = "";
const String linkBootstrapThemeMinCss = "";
const String linkBootstrapThemeMinCssMap = "";
const String linkBootstrapMinJs = "";
void handleRoot() {
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
  }

  String result = readWebData(linkIndexHtml);
  server.send(200, "text/html", result);
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
  server.send(200, "text/javascript", result);
}
void handleBootstrapMinCss() {
  String result = readWebData(linkBootstrapMinCss);
  server.send(200, "text/css", result);
}
void handleBootstrapMinCssMap() {
  String result = readWebData(linkBootstrapMinCssMap);
  server.send(200, "text/css", result);
}
void handleBootstrapThemeMinCss() {
  String result = readWebData(linkBootstrapThemeMinCss);
  server.send(200, "text/css", result);
}
void handleBootstrapThemeMinCssMap() {
  String result = readWebData(linkBootstrapThemeMinCssMap);
  server.send(200, "text/css", result);
}
void handleBootstrapMinJs() {
  String result = readWebData(linkBootstrapMinJs);
  server.send(200, "text/javascript", result);
}

String inComing = "";

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Start Setup.");

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("Note_WIFI", "11111111");

  Serial.println("WiFi connecting");
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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
  readWebData();
}

void loop() {
  //  checkWiFiConnect();
  server.handleClient();

  readSerialData();
}

void readSerialData() {
  while (Serial.available() > 0) {
    inComing = Serial.readStringUntil('\n');
    if (inComing != "") {
      inComing.trim();
      Serial.println(inComing);

      if (inComing == "readWebData") {
        String webData = readWebData();
      }
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
