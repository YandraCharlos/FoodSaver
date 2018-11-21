#include <Arduino.h>
#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IOXhop_FirebaseESP32.h>

// define firebase param
#define FIREBASE_HOST "https://mantuls-project-riot.firebaseio.com"
#define FIREBASE_AUTH "KK13T9Tw6m64SZuNxsuGcorgtqjgWTrb3Ume6qwC"

//define wifi param
#define WIFI_SSID "Go the Extra Mile"
#define WIFI_PASSWORD "G0beyondexpectation!"

//define load cell param
#define DT 17 // FOR ESP32
#define SCK 16
#define calibration_factor 21700.0 // can use "-21700" if you get negatif value

//define Firebase param
String id_device = "001";

//define LCD param
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

HX711 scale(DT, SCK);

float weight = 0.0; // variabel for scale
float val;

void connect_to_wifi(const char *ssid, const char *password)
{
  int dot = 0;
  Serial.print("connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
    dot++;
    if (dot == 60)
    {
      dot = 0;
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WiFi CONNECTED");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }
  else
  {
    Serial.println("Failed to connect ..");
    connect_to_wifi(ssid, password);
  }
}

void setup()
{
  Serial.begin(115200);
  connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
  //lcd.begin();
  lcd.init();
  lcd.backlight();
  scale.set_scale(calibration_factor);
  scale.tare(); //Reset the scale to 0
}

void loop()
{
  weight = scale.get_units();
  Serial.println(weight, 3);
  //val = weight - 38.650; // FIRST VALUE - 38.650

  if (weight < 0)
  {
    Serial.print("Berat ");
    Serial.println("0.00");
    lcd.setCursor(0, 0);
    lcd.print("Measurement !!!!");
    lcd.setCursor(0, 1);
    lcd.print("0.00 Kg !");
    delay(1000);
    lcd.clear();
  }
  else
  {
    Serial.print("Berat : ");
    //Serial.println((val * 2), 3);
    Serial.println(weight, 3);
    lcd.setCursor(0, 0);
    lcd.print("Measurement !!!!");
    lcd.setCursor(0, 1);
    lcd.print(weight);
    lcd.print(" Kg");
    Firebase.setFloat("/restaurants/smart-scale/" + id_device + "/scale_data/weight", weight);
    delay(1000);
    lcd.clear();
  }
  delay(500);
}