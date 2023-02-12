#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define DHTPIN D1     
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float t = 0.0;
float h = 0.0;

#define SensorPin A0  
BH1750 lightMeter;

unsigned long prevTime = 0;
const int interval = 10000;  // Intervall in Millisekunden

const char* ssid = "FRITZ!Box 6591 Cable MV";
const char* wifi_password = "36235131873529065677";

const char* mqtt_server = "192.168.178.8";
const char* humidity_topic = "home/plant/humidity";          
const char* temperature_topic = "home/plant/temperature";        
const char* moisture_topic = "home/plant/moisture";           
const char* lux_topic = "homeplant/lux";                
const char* mqtt_username = "pi";         
const char* mqtt_password = "2103";        
const char* clientID = "client_plant";      

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void connect_MQTT()
{
  Serial.print("Verbinden zu ");
  Serial.print(ssid);

  WiFi.begin(ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi verbunden");
  Serial.print("IP Adresse: ");
  Serial.print(WiFi.localIP());
  
  if (client.connect(clientID, mqtt_username, mqtt_password))
  {
    Serial.println("Verbunden zum MQTT Broker!");
  }
  else
  {
    Serial.println("Verbindung zum MQTT Broker fehlgeschlagen...");
  } 
}

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
    connect_MQTT();
    Serial.setTimeout(2000);
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

    String hs ="Luft: "+String((float)h)+"%";
    String ts ="Temp: "+String((float)t)+"C";
    String bf ="Boden: "+String((float)sensorValue);
    String luxs ="Licht: "+String((float)lux)+"lx";

    if (client.publish(temperature_topic, String(t).c_str()))
    {
      Serial.println("Temperatur gesendet!");
    }
    else
    {
      Serial.println("Temperatur senden fehlgeschlagen. MQTT Verbindung wird wieder hergestellt und erneut versucht!");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10);
      client.publish(temperature_topic, String(t).c_str());
    }

    if (client.publish(humidity_topic, String(h).c_str()))
    {
      Serial.println("Luftfeuchtigkeit gesendet!");
    }
    else
    {
      Serial.println("Luftfeuchtigkeit senden fehlgeschlagen. MQTT Verbindung wird wieder hergestellt und erneut versucht!");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10);
      client.publish(humidity_topic, String(h).c_str());
    }

    if (client.publish(moisture_topic, String(sensorValue).c_str()))
    {
      Serial.println("Bodenfeuchtigkeit gesendet!");
    }
    else
    {
      Serial.println("Bodenfeuchtigkeit senden fehlgeschlagen. MQTT Verbindung wird wieder hergestellt und erneut versucht!");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10);
      client.publish(moisture_topic, String(sensorValue).c_str());
    }

    if (client.publish(lux_topic, String(lux).c_str()))
    {
      Serial.println("Licht gesendet!");
    }
    else
    {
      Serial.println("Licht senden fehlgeschlagen. MQTT Verbindung wird wieder hergestellt und erneut versucht!");
      client.connect(clientID, mqtt_username, mqtt_password);
      delay(10);
      client.publish(lux_topic, String(lux).c_str());
    }
    client.disconnect();
  }
}