#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// library DHT sensor library by Adafruit
#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

int buttonPin = 8;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

 
  dht.begin();
  pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

//  lcd.noBacklight();
  bool pressed = digitalRead(buttonPin); // read button

  if (!pressed) { 
    lcd.noBacklight();
    return; 
  }

  lcd.backlight();

  float f = dht.readTemperature(true);
  float t = dht.readTemperature(); // read humidity 
  float h = dht.readHumidity();    // read temperature

  if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      delay(1000);
      return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  float humidityPercents = h;
  float temperatureCelsius = t;
  float heatIndexCelsius = hic;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  lcd.print("Temp: " + String(temperatureCelsius) + "C");
  delay(1200);
  lcd.clear();
  
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print(String(humidityPercents) + "%");
  delay(1200);
  lcd.clear();
  
  lcd.print("Heat Index: ");
  lcd.setCursor(0, 1);
  lcd.print(String(heatIndexCelsius));
  delay(1200);
  lcd.clear();


  if (heatIndexCelsius > 50) {
    lcd.print("Seriuos Risk of");
    lcd.setCursor(0, 1);
    lcd.print("heatstroke");
  } else if (heatIndexCelsius > 29 && heatIndexCelsius < 50){
    lcd.print("Intermediate risk of");
    lcd.setCursor(0, 1);
    lcd.print("heatstroke on sport");
  } else {
    lcd.print("No risk. You can only");
    lcd.setCursor(0, 1);
    lcd.print("get a bit tired");
  }

  delay(1200);
  lcd.clear();
}
