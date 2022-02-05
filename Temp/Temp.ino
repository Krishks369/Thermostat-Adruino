#include <DHT.h>

float humidity;
float tempC;
int setTime = 500;
int dt = 1000;
int sensePin = 7; // Can be used at any pin

#define Type DHT11
DHT dht(sensePin, Type);

void setup()
{
  Serial.begin(9600);
  dht.begin();
}
void loop()
{
  humidity = dht.readHumidity();
  tempC = dht.readTemperature();
  Serial.print("Humidity:");
  Serial.println(humidity);
  Serial.print("Temp:");
  Serial.println(tempC);
  delay(dt);
}
