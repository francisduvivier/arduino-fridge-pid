#include <Wire.h>
#include <Adafruit_SSD1306.h>


//DISPLAY SSD1036
#define SCREEN_WIDTH 128 // OLED ssd1306Scherm width, in pixels
#define SCREEN_HEIGHT 32 // OLED ssd1306Scherm height, in pixels
#define OLED_RESET    -1 // \Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 ssd1306Scherm(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//TEMP DHT22
int wantedTemp = 10;
#include "DHT.h"
#define DHTPIN 11
#define DHTTYPE DHT22

//POTENTIO METER
#define POTPIN A2

//RELAY
#define RELPIN 2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  ssd1306Scherm.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ssd1306Scherm.clearDisplay();
  ssd1306Scherm.display();
  pinMode(RELPIN, OUTPUT);
  pinMode(POTPIN, INPUT);
  dht.begin();
}

void loop() {
  float temp = dht.readTemperature(false);
  delay(500);
  int potVal = analogRead(POTPIN);
  wantedTemp = potVal / 30 -5;
  int offTemp = wantedTemp - 1;
  int onTemp = wantedTemp + 1;
  bool state = false;
  if (temp < offTemp) {
    state = false;
  } else if (temp > onTemp) {
    state = true;
  }

  ssd1306Scherm.setCursor(0, 0);
  ssd1306Scherm.setRotation(0);
  ssd1306Scherm.clearDisplay();
  ssd1306Scherm.setTextColor(WHITE);

  // Show temp setting from POT
  ssd1306Scherm.setTextSize(1);
  ssd1306Scherm.setCursor(2, 0);
  ssd1306Scherm.print("wanted:");

  ssd1306Scherm.print(wantedTemp, DEC);
  ssd1306Scherm.print("C->");


  // Show wanted pot
  ssd1306Scherm.print(offTemp, DEC);
  ssd1306Scherm.print("::");
  ssd1306Scherm.print(onTemp, DEC);

  // Show TEMP
  ssd1306Scherm.setTextSize(2);
  ssd1306Scherm.setCursor(3, 9);
  String tempString = String(temp, 1);
  ssd1306Scherm.print(tempString);
  ssd1306Scherm.print("C::");
  ssd1306Scherm.print(state? "ON": "OFF");

  ssd1306Scherm.setTextSize(1);
  ssd1306Scherm.setCursor(27, 23);
  ssd1306Scherm.print("ko-lab.space");
  ssd1306Scherm.display();
}
