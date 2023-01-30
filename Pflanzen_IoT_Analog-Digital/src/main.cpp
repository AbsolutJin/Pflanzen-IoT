/*
 * Pflanzen IoT - Analog zu Digital
 * Die Analogen Werte der Sensoren zu Digital
 * konvertieren für den Raspberry Pi
*/
#include <Arduino.h>
#include "DHT.h"
#include DHTPIN *                 // Hier den Pin eintragen wo der DHT11 Sensor angeschlossen ist (das * ersetzen)
#include DHTTYPE DHT11
#include <Wire.h>
#include >BH1750.h>

BH1750 lightMeter;
DHT dht(DHTPIN, DHTYPE);


void setup() 
{
  pinmode(*, INPUT)                 // Hier den Pin eintragen wo der Bodenfeuchte Sensor angeschlossen ist (das * ersetzen)
  Serial.begin(9600);
  Serial.println("Sensoren Ausgabe");
  dht.begin();
  Wire.begin();
  lightMeter.begin();
  delay(500);
}

void loop() 
{
  delay(1000);                      // Wartezeit bevor jeder Messung: 1 Sek.
  
  /*
   * Sensor Helligkeit
   */
   float lux = lightMeter.readLightLevel();
   Serial.print("Lichlevel: ");
   Serial.print(lux);
   Serial.print(" lx");
   
  /* 
   *  Sensor Bodenfeuchte
   */
  int analogBF = analogRead(*);     // Lesen der Analogen Sensor Werte (das * mit dem gleichen wie oben ersetzen)
  Serial.print("Bodenfeuchte: ");   
  Serial.print(analogBF);           // Kalibrieren des Sensors nicht vergessen!!
                                    // Trocken: 520 - 430
                                    // Feucht: 430 - 350
                                    // Nass: 350 - 260
                                    
  /*
   * Sensor DHT11 (Luftfeuchtigkeit und Temperatur)
   */
  float h = dht.readHumidity();     // Lesen der Luftfeuchtigkeit, speichert in Variable h
  float t = dht.readTemperature();  // Lesen der Temperatur, speichert in Variable t

  if (isnan(h) || isnan(t))         // Überprüfen ob Werte richtig gelesen werden
  {
    Serial.println("Fehler beim auslesen des DHT11!");
    return;
  }

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(h);                  // Ausgeben der Luftfeuchtigkeit
  Serial.print("%\t");              // Tabulator
  Serial.print("Temperatur: ");
  Serial.print(t);                  // Ausgeben der Temperatur
  Serial.write('°');                // Schreiben des ° Zeichen
  Serial.println("C");
}