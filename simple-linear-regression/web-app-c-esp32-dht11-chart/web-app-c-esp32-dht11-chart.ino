// Author: Erwin Ouyang, aiotedge.tech
// Date  : 11 Jan 2020

#include <DHT.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define DHTPIN  4
#define DHTTYPE DHT11

const char* ssid = "Huawei-E5573";
const char* password = "huaweie5573";

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
int temp;

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize DHT11
  dht.begin();

  // Initialize SPIFFS
  if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for web page and libraries
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css");
  });
  server.on("/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/jquery-3.3.1.min.js");
  });
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.js");
  });
  server.on("/popper.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/popper.min.js");
  });
  server.on("/raphael-2.1.4.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/raphael-2.1.4.min.js");
  });
  server.on("/justgage.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/justgage.js");
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/highcharts.js");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHT11Temperature().c_str());
  });
  server.on("/predict", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", predictNumberofUsers().c_str());
  });
    
  // Start server
  server.begin();
}
 
void loop()
{  
}

String readDHT11Temperature()
{
  int t = (int)dht.readTemperature();
  
  if (isnan(t))
    temp = temp;
  else
    temp = t;
  
  return String(temp);
}

String predictNumberofUsers()
{
  float theta_0 = 25;
  float theta_1 = 1.25;
  float h = theta_0 + theta_1*temp;

  return String(round(h), 0);
}

