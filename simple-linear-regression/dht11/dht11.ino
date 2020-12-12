#include <DHT.h>

#define DHTPIN  4
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  delay(2000);
  
  float temp = dht.readTemperature();
  
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");
}
