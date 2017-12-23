#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ButtonSetup";

ESP8266WebServer server(80);

void rootRoute() {
  String rootHtml = "::index.html::";
  server.send(200, "text/html", rootHtml);
}

void scriptRoute() {
  String script = "::script.js::";
  server.send(200, "text/javascript", script);
}

void scanRoute() {
  int n = WiFi.scanNetworks();
  String ssids = "[\n";
  for (int i = 0; i < n; i++) {
    int rssi = (2 * (WiFi.RSSI(i) + 100));
    if (rssi > 100) rssi = 100;
    if (rssi < 0) rssi = 0;
    String ssid = WiFi.SSID(i);
    ssid.replace("\"","\\\"");
    ssids += (String) "  {\n    \"ssid\": \"" + ssid + "\",\n";
    ssids += (String) "    \"secured\": " + (WiFi.encryptionType(i) != ENC_TYPE_NONE) + ",\n";
    ssids += (String) "    \"signal\": " + rssi + ",\n";
    ssids += (String) "    \"mac\": \"" + WiFi.BSSIDstr(i) + "\",\n";
    ssids += (String) "    \"channel\": " + WiFi.channel(i) + "\n  }";
    ssids += (i == n - 1) ? "\n" : ",\n";
  }
  ssids += "]";
  server.send(200, "text/json", ssids);
}

void connectRoute() {
  String ssid = server.arg("ssid");
  server.send(200, "text/html", "ssid: " + ssid + "\n" + "pass: " + server.arg("pass"));
}

void setup() {
  delay(1000);
  WiFi.softAP(ssid);

  server.on("/", rootRoute);
  server.on("/scan", scanRoute);
  server.on("/script.js", scriptRoute);
  server.on("/connect", HTTP_POST, connectRoute);
  server.begin();
  Serial.begin(115200);
}

void loop() {
  server.handleClient();
}
