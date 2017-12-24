#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

const char *ssid = "ButtonSetup";

ESP8266WebServer server(80);

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void scanRoute() {
  int n = WiFi.scanNetworks();
  String ssids = "[\n";
  for (int i = 0; i < n; i++) {
    int rssi = (2 * (WiFi.RSSI(i) + 100));
    if (rssi > 100) rssi = 100;
    if (rssi < 0) rssi = 0;
    String ssid = WiFi.SSID(i);
    ssid.replace("\"", "\\\"");
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
  SPIFFS.begin();
  server.on("/scan", scanRoute);
  server.on("/connect", HTTP_POST, connectRoute);
  server.begin();
  Serial.begin(115200);

  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });
}

void loop() {
  server.handleClient();
}
