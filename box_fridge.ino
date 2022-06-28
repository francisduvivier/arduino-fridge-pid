#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>


//DISPLAY SSD1036
#define SCREEN_WIDTH 128 // OLED ssd1306Scherm width, in pixels
#define SCREEN_HEIGHT 32 // OLED ssd1306Scherm height, in pixels
#define OLED_RESET    -1 // \Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 ssd1306Scherm(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//TEMP DHT22
float wantedTemp = 10;
float wantedTempRange = 4;
float MAXTEMP = 30;
float MINTEMP = -5;
#define DHTPIN 13
#define DHTTYPE DHT22

//POTENTIO METER
#define POTPIN A0
//This allows to disable the potentiometer by NOT connecting d3 to ground.
#define POTSWITCH 3
#define RANGESWITCH 4

//RELAY
#define RELPIN 9

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  ssd1306Scherm.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  ssd1306Scherm.clearDisplay();
  ssd1306Scherm.display();
  pinMode(RELPIN, OUTPUT);
  pinMode(POTPIN, INPUT);
  pinMode(POTSWITCH, INPUT_PULLUP);
  pinMode(RANGESWITCH, INPUT_PULLUP);
  dht.begin();
}
bool state = false;

void loop() {
  int nbSame = 0;
  int nbSameNeeded = 3;
  float temp = dht.readTemperature(false);
  if (!digitalRead(POTSWITCH) || !digitalRead(RANGESWITCH)) {
    int potVal = analogRead(POTPIN);
    int secondPotRead = -1;
    //Filter out bad potentiometer readings
    while (potVal != secondPotRead) {
      potVal = secondPotRead;
      secondPotRead = analogRead(POTPIN);
    }
    if (!digitalRead(RANGESWITCH)) {
      wantedTempRange = potVal /(1024/10.0);
    } else {
      wantedTemp = potVal / (1024/(MAXTEMP-MINTEMP)) + MINTEMP;
    }

  }
  float lowTemp = wantedTemp - (wantedTempRange / 2);
  float highTemp = wantedTemp + (wantedTempRange / 2);
  if (temp < lowTemp) {
    state = true;
  } else if (temp > highTemp) {
    state = false;
  }
  digitalWrite(RELPIN, state);

  ssd1306Scherm.setCursor(0, 0);
  ssd1306Scherm.setRotation(0);
  ssd1306Scherm.clearDisplay();
  ssd1306Scherm.setTextColor(WHITE);

// Show wanted pot
  ssd1306Scherm.setTextSize(1);
  ssd1306Scherm.setCursor(2, 0);
  ssd1306Scherm.print("wanted:");
  String wantedTempString = String(wantedTemp, 1);

  ssd1306Scherm.print(wantedTempString);
  ssd1306Scherm.print("+-");
  
  String wantedTempMarginString = String(wantedTempRange/2, 1);
  ssd1306Scherm.print(wantedTempMarginString);

  // Show TEMP
  ssd1306Scherm.setTextSize(2);
  ssd1306Scherm.setCursor(3, 9);
  String tempString = String(temp, 1);
  ssd1306Scherm.print(tempString);
  ssd1306Scherm.print("C::");
  ssd1306Scherm.print(state ? "OFF" : "ON");

  ssd1306Scherm.setTextSize(1);
  ssd1306Scherm.setCursor(27, 23);
  ssd1306Scherm.print("ko-lab.space");
  ssd1306Scherm.display();
  delay(100);
}
