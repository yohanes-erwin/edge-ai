// Author: Erwin Ouyang, aiotedge.tech
// Date  : 11 Jan 2020

#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Huawei-E5573";
const char* password = "huaweie5573";

AsyncWebServer server(80);
int temp = 0;
bool req_root = false;

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);

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
    req_root = true;
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css");
  });
  server.on("/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/jquery-3.3.1.min.js");
  });
  server.on("/popper.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/popper.min.js");
  });
  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.js");
  });
  server.on("/predict", HTTP_GET, [](AsyncWebServerRequest *request) {
    req_root = false;
    if (request->params() == 1)
    {
      AsyncWebParameter* p = request->getParam(0);
      if (p->name() == "temperature")
      {
        temp = p->value().toInt();
      }
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Start server
  server.begin();
}
 
void loop()
{  
}

// Replaces placeholder with LED state value
String processor(const String& var)
{
  if (var == "RESULT")
  {
    if (req_root)
      return String(0);
    else
      return predictNumberofUsers(temp);
  }
  return String();
}

String predictNumberofUsers(int temp)
{
  float theta_0 = 25;
  float theta_1 = 1.25;
  float h = theta_0 + theta_1*temp;

  return String(round(h), 0);
}

