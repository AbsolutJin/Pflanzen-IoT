#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

#define DHTPIN D6 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

#define SensorPin A0  
BH1750 lightMeter;

unsigned long prevTime = 0;
const int interval = 5000;  // Intervall in Millisekunden

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "FRITZ!Box 6591 Cable MV"
#define WIFI_PASSWORD "36235131873529065677"

#define API_KEY "AIzaSyB89OCTWyh9TxRbFQI2aALi2XPdHiBkCBA"
#define DATABASE_URL "https://pflanzen-iot-48551-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  prevTime = millis();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Verbinden zu ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Verbunden mit IP: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Erfolgreich");
    signupOK = true;
  }
  else
  {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() 
{
  if (millis() - prevTime >= interval) 
  {
    prevTime = millis();
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();
    if (isnan(newT) || isnan(newH))
    {
      Serial.println("DHT Sensor lesen fehlgeschlagen!");
    }
    else
    {
      t = newT;
      h = newH;
    }
    float sensorValue = analogRead(SensorPin);
    float lux = lightMeter.readLightLevel();

    if (Firebase.ready() && signupOK )
    {
      if (Firebase.RTDB.setFloat(&fbdo,"Luftfeuchtigkeit",h))
      {
        Serial.print("Luftfeuchtigkeit: ");
        Serial.println(h);
      }
      else
      {
        Serial.println("FEHLSCHLAG");
        Serial.println("Grund: " + fbdo.errorReason());
      }
      if (Firebase.RTDB.setFloat(&fbdo,"Temperatur",t))
      {
        Serial.print("Temperatur: ");
        Serial.println(t);
      }
      else
      {
        Serial.println("FEHLSCHLAG");
        Serial.println("Grund: " + fbdo.errorReason());
      }
      if (Firebase.RTDB.setFloat(&fbdo,"Bodenfeuchte",sensorValue))
      {
        Serial.print("Bodenfeuchte: ");
        Serial.print(sensorValue);
      }
      else
      {
        Serial.println("FEHLSCHLAG");
        Serial.println("Grund: " + fbdo.errorReason());
      }
      if (Firebase.RTDB.setFloat(&fbdo,"Lichtlevel",lux))
      {
        Serial.print("Lichtlevel: ");
        Serial.print(lux);
      }
      else
      {
        Serial.println("FEHLSCHLAG");
        Serial.println("Grund: " + fbdo.errorReason());
      }
    }
  }
}