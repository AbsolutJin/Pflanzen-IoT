/*
* Pflanzen Iot - Senosorwerte von Analog zu Digital
*/

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

/*
* DHT Sensor 
*/
#define DHTPIN 5     // Position des Pins für den Temperatur und Feuchtigkeitssensor [DHT11]
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

/*
* Bodenfeuchte Sensor
*/
#define SensorPin A0  // Poistion des Pins für den Bodenfeuchte Sensor [Capacitive Soil Moisture Sensor v1.2]

void setup() 
{
  Serial.begin(115200);
  dht.begin();
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
    Serial.println("Temperatur: ");
    Serial.print(t);
    Serial.print(" Grad Celsius");
    Serial.println("Luftfeuchtigkeit: ");
    Serial.print(h);
    Serial.print(" %");
  }

/*
* Bodenfeuchte Sensor
*/
float sensorValue = analogRead(SensorPin);
Serial.println("Bodenfeuchte: ");
Serial.print(sensorValue);
  
/*
* Licht Sensor
*/


delay(2000);
}