#include <Arduino.h>
#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define DT 13     // FOR ESP32
#define SCK 12  
#define calibration_factor 21700.0  // can use "-21700" if you get negatif value
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);




HX711 scale(DT, SCK);

float weight = 0.0; // variabel for scale
float val;

void setup() {
  Serial.begin(9600);
  //lcd.begin();
  lcd.init();
  lcd.backlight();
  scale.set_scale(calibration_factor);
  scale.tare();  //Reset the scale to 0
}

void loop() {
  weight = scale.get_units();
  Serial.println(weight, 3);
  //val = weight - 38.650; // FIRST VALUE - 38.650

  if (weight < 0) {
    Serial.print("Berat " );
    Serial.println ("0.00");
    lcd.setCursor(0, 0);
    lcd.print("Measurement !!!!");
    lcd.setCursor(0, 1);
    lcd.print("0.00 Kg !");
    delay (1000);
    lcd.clear();
    } else {
    Serial.print("Berat : ");
    //Serial.println((val * 2), 3);
    Serial.println(weight, 3);
    lcd.setCursor(0, 0);
    lcd.print("Measurement !!!!");
    lcd.setCursor(0, 1);
    lcd.print(weight);
    lcd.print(" Kg");
    delay (1000);
    lcd.clear();
  }
  delay (500);
}