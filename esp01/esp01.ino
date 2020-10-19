#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
const uint32_t connectTimeoutMs = 5000;

#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

String inComing = "";
const String startIndexHtml = "<indexHtml>";
const String stopIndexHtml = "</indexHtml>";
unsigned int startIndexHtmlLength = startIndexHtml.length();
String indexHtml = "";
const String startJqueryMinJs = "<jqueryMinJs>";
const String stopJqueryMinJs = "</jqueryMinJs>";
unsigned int startJqueryMinJsLength = startJqueryMinJs.length();
String jqueryMinJs = "";
const String startBootstrapMinCss = "<bootstrapMinCss>";
const String stopBootstrapMinCss = "</bootstrapMinCss>";
unsigned int startBootstrapMinCssLength = startBootstrapMinCss.length();
String bootstrapMinCss = "";
const String startBootstrapThemeMinCss = "<bootstrapThemeMinCss>";
const String stopBootstrapThemeMinCss = "</bootstrapThemeMinCss>";
unsigned int startBootstrapThemeMinCssLength = startBootstrapThemeMinCss.length();
String bootstrapThemeMinCss = "";
const String startBootstrapMinJs = "<bootstrapMinJs>";
const String stopBootstrapMinJs = "</bootstrapMinJs>";
unsigned int startBootstrapMinJsLength = startBootstrapMinJs.length();
String bootstrapMinJs = "";

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

  while (Serial.available() > 0) {
    inComing = Serial.readStringUntil('\n');
    if (inComing != "") {
      readSerialData();
    }
  }
}

void readSerialData() {
  inComing.trim();
  unsigned long dataStart = 0;
  unsigned long dataStop = 0;
  if (inComing.startsWith(startIndexHtml) and inComing.endsWith(stopIndexHtml)) {
    dataStart = inComing.indexOf(startIndexHtml) + startIndexHtmlLength;
    dataStop = inComing.indexOf(stopIndexHtml);
    indexHtml = inComing.substring(dataStart, dataStop);
  } else  if (inComing.startsWith(startJqueryMinJs) and inComing.endsWith(stopJqueryMinJs)) {
    dataStart = inComing.indexOf(startJqueryMinJs) + startJqueryMinJsLength;
    dataStop = inComing.indexOf(stopJqueryMinJs);
    jqueryMinJs = inComing.substring(dataStart, dataStop);
  } else  if (inComing.startsWith(startBootstrapMinCss) and inComing.endsWith(stopBootstrapMinCss)) {
    dataStart = inComing.indexOf(startBootstrapMinCss) + startBootstrapMinCssLength;
    dataStop = inComing.indexOf(stopBootstrapMinCss);
    bootstrapMinCss = inComing.substring(dataStart, dataStop);
  } else  if (inComing.startsWith(startBootstrapThemeMinCss) and inComing.endsWith(stopBootstrapThemeMinCss)) {
    dataStart = inComing.indexOf(startBootstrapThemeMinCss) + startBootstrapThemeMinCssLength;
    dataStop = inComing.indexOf(stopBootstrapThemeMinCss);
    bootstrapThemeMinCss = inComing.substring(dataStart, dataStop);
  } else  if (inComing.startsWith(startBootstrapMinJs) and inComing.endsWith(stopBootstrapMinJs)) {
    dataStart = inComing.indexOf(startBootstrapMinJs) + startBootstrapMinJsLength;
    dataStop = inComing.indexOf(stopBootstrapMinJs);
    bootstrapMinJs = inComing.substring(dataStart, dataStop);
  }
}

void checkWiFiConnect() {
  while (WiFi.status() != WL_CONNECTED) {
    if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
      Serial.print("WiFi connected: ");
      Serial.print(WiFi.SSID());
      Serial.print(" ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi not connected!");
    }
  }
}

void handleRoot() {
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
  if (server.hasHeader("User-Agent")) {
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
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

void handleNotFound() {
  server.send(404, "text/html", "<h1>404 error: File not found</h1>");
}
