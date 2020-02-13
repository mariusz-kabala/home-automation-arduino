#include "EspMQTTClient.h"
#include "DHT.h"

#define DEVICEID "5"
#define DHTPIN 22
#define MOISTUREPIN 32
#define DHTTYPE DHT11
#define TIME_TO_SLEEP  60
#define uS_TO_S_FACTOR 1000000ULL

EspMQTTClient client(
  "@Home0",         // WIFI SID
  "your-pass-goes-here",     // WIFI pass
  "192.168.0.109",  // MQTT Broker server ip
  "",               // MQTT login
  "",               // MQTT pass
  "esp32Plants",    // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
float m;

void setup()
{
  Serial.begin(115200);
 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output

  pinMode(DHTPIN, INPUT);
  pinMode(MOISTUREPIN, INPUT); 
  dht.begin();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

void onConnectionEstablished()
{
  client.publish("home/esp32/plants/" + String(DEVICEID), "{\"humidity\": " + String(h) + ", \"temperature\": " + String(t) + ", \"moisture\": " + String(m) + "}");


  esp_deep_sleep_start();
}

void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  m = analogRead(MOISTUREPIN);
  
  client.loop();  
}
