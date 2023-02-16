#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

#define DHTPIN D1     
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

#define SensorPin A0  
BH1750 lightMeter;

unsigned long prevTime = 0;
const int interval = 5000;  // Intervall in Millisekunden

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  prevTime = millis();
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
      String output = "Temperatur: " + String(t) + " Grad Celsius\t" + "Luftfeuchtigkeit: " + String(h) + " %";
      Serial.println(output);
    }
    float sensorValue = analogRead(SensorPin);
    Serial.println("Bodenfeuchte: " + String(sensorValue));
    float lux = lightMeter.readLightLevel();
    Serial.println("Lichtlevel: " + String(lux) + " lx");
  }
}