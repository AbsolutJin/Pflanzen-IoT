/*
* Pflanzen Iot - Senosorwerte von Analog zu Digital
*/

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

/*
* DHT Sensor 
*/
#define DHTPIN D1     // Position des Pins für den Temperatur und Feuchtigkeitssensor [DHT11]
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

/*
* Bodenfeuchte Sensor
*/
#define SensorPin A0  // Poistion des Pins für den Bodenfeuchte Sensor [Capacitive Soil Moisture Sensor v1.2]

/*
* Lichtensor
*/
BH1750 lightMeter;

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  Wire.begin();
  lightMeter.begin();
}

void loop() 
{
  /*
  * DHT Sensor
  */
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
    Serial.print("Temperatur: ");
    Serial.print(t);
    Serial.print(" Grad Celsius");
    Serial.print("%\t"); 
    Serial.print("Luftfeuchtigkeit: ");
    Serial.print(h);
    Serial.println(" %");
  }

/*
* Bodenfeuchte Sensor
*/
float sensorValue = analogRead(SensorPin);
Serial.print("Bodenfeuchte: ");
Serial.println(sensorValue);
  
/*
* Licht Sensor
*/
float lux = lightMeter.readLightLevel();
Serial.print("Lichtlevel: ");
Serial.print(lux);
Serial.print(" lx");

delay(5000);
}