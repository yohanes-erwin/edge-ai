// Author: Erwin Ouyang, aiotedge.tech
// Date  : 25 Jan 2020

#include <DHT.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid = "Huawei-E5573";
const char* password = "huaweie5573";

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
float humidity;

void setup()
{
  // Initialize serial port
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
  server.on("/a076d05399.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/a076d05399.js");
  });
  server.on("/predict_not_rain.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/predict_not_rain.png");
  });
  server.on("/predict_rain.png", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/predict_rain.png");
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHT11Humidity().c_str());
  });
  server.on("/predict", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", predictRain().c_str());
  });
    
  // Start server
  server.begin();
}
 
void loop()
{  
}

String readDHT11Humidity()
{
  float h = dht.readHumidity();
  
  if (isnan(h))
    humidity = humidity;
  else
    humidity = h;
  
  return String((int)humidity);
}

float sigmoid(float x)
{
  return 1/(1 + exp(-x));
}

String predictRain()
{
  float theta_0 = -9.33;
  float theta_1 = 0.18;
  float h = sigmoid(theta_0 + theta_1*humidity);

  return String(round(h), 0);
}

