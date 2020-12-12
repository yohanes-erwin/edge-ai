// Author: Erwin Ouyang, aiotedge.tech
// Date  : 11 Jan 2020

#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Check library setting
#if (SSD1306_LCDHEIGHT != 64)    // 128 x 64 pixel display
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define DHTPIN  4
#define DHTTYPE DHT11
#define I2C_SDA 33
#define I2C_SCL 32
#define OLED_ADDR 0x3C

static const unsigned char PROGMEM temperature_bmp[] =
{
  B00000011, B11001111,
  B00000110, B01100000,
  B00000100, B00101111,
  B00000100, B00100000,
  B00000100, B00101111,
  B00000101, B10100000,
  B00000101, B10101111,
  B00000101, B10100000,
  B00001101, B10110000,
  B00011011, B11011000,
  B00110111, B11101100,
  B00101111, B11110100,
  B00101111, B11110100,
  B00110111, B11101100,
  B00011000, B00011000,
  B00001111, B11110000
};
static const unsigned char PROGMEM humidity_bmp[] =
{
  B00000000, B00000000,
  B00000111, B11100000,
  B00011111, B11111000,
  B00111001, B10011100,
  B00110001, B10001100,
  B01100011, B11000110,
  B01100111, B11100110,
  B01111110, B01111110,
  B01111110, B01111110,
  B01100111, B11100110,
  B01100011, B11000110,
  B00110001, B10001100,
  B00111001, B10011100,
  B00011111, B11111000,
  B00000111, B11100000,
  B00000000, B00000000,
};

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 oled;
int temp;

void setup()
{
  // Initialize serial and I2C
  Serial.begin(115200);

  // Initialize DHT11
  dht.begin();
  
  // OLED initialization
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.clearDisplay();
}

void loop()
{
  int t = (int)dht.readTemperature();
  if (isnan(t))
    temp = temp;
  else
    temp = t;

  String users = predictNumberofUsers();
  
  oled.clearDisplay();
  oled.drawBitmap(32, 16, temperature_bmp, 16, 16, 1);
  oled.drawBitmap(32, 36, humidity_bmp, 16, 16, 1);
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.setCursor(55, 16);
  oled.printf("%d%cC", temp, (char)247);
  oled.setCursor(55, 36);
  oled.printf("%s", users);
  oled.display();
  
  delay(1000);
}

String predictNumberofUsers()
{
  float theta_0 = 25;
  float theta_1 = 1.25;
  float h = theta_0 + theta_1*temp;

  return String(round(h), 0);
}

