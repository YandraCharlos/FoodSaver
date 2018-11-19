#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

// Set these to run example.
#define FIREBASE_HOST "https://mantuls-project-riot.firebaseio.com"
#define FIREBASE_AUTH "KK13T9Tw6m64SZuNxsuGcorgtqjgWTrb3Ume6qwC"
#define WIFI_SSID "bla bla bla"
#define WIFI_PASSWORD "pass"

float data_berat = 50.0;
String id_device = "id_device_1";

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  // get value
  Serial.print("data now: ");
  Serial.println(Firebase.getFloat("/restaurants/smart-scale/" + id_device + "/scale_data"));
  delay(1000);

  // append data value
  Firebase.pushFloat("/restaurants/smart-scale/" + id_device + "/scale_data", data_berat);
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  data_berat = data_berat + 1.00;
  delay(1000);

}
