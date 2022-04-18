#include "ESP8266WiFi.h"
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include "WiFiClient.h"
#include <ThingSpeak.h>
#include "./data.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino_JSON.h>

const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed); // Set the GPIO to be used to sending the message.

float humidity;
float tempC;
int setTime = 500;
int dt = 1000;
int sensePin = 7;
float optTempC = 25.0;
int onFlag = 0;
int remTemp = 24;
#define DHTPIN 12
#define DHTTYPE DHT11

String serverName = "https://api.thingspeak.com/channels/1699188/feeds.json?api_key=VDUJ37N3H8E12E7H&results=2";

// WiFi parameters to be configured
const char *ssid = "SSID";        // Write here your router's username
const char *password = "PASSWORD"; // Write here your router's passward

unsigned long myChannelNumber = 1699188;
const char *myWriteAPIKey = "3VQM1YOXAA4UYOI0";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup()
{
    Serial.begin(9600) x;
    dht.begin();
    delay(setTime);

    irsend.begin();
#if ESP8266
    Serial.begin(9600, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
    Serial.begin(9600, SERIAL_8N1);
#endif // ESP8266

    // Connect to WiFi
    WiFi.begin(ssid, password);

    // while wifi not connected yet, print '.'
    // then after it connected, get out of the loop
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // print a new line, then print WiFi connected and the IP address
    Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());
    ThingSpeak.begin(client);
    dht.begin();
}

void loop()
{
    humidity = dht.readHumidity();
    tempC = dht.readTemperature();
    delay(dt);
    Serial.println(tempC);
    if (tempC < optTempC + 2.5 && tempC > optTempC - 2.5)
    {

        Serial.println("Temperature is Ok");
        if (onFlag == 0)
        {
            Serial.println("OFF");
        }
        else if (onFlag)
        {
            Serial.println("ON");
            irsend.sendRaw(rawDataOFF, RAW_DATA_LEN, 38);
            remTemp = 24;
            delay(dt);
            onFlag = 0;
        }
    }
    else
    {
        Serial.println("Temperature not in Optimum Level");
        if (onFlag == 1)
        {
            Serial.println("ON");
        }
        else
        {
            Serial.println("Sent Signal");
            irsend.sendRaw(rawDataON, RAW_DATA_LEN, 38);
            delay(dt);
            onFlag = 1;
            if (tempC > optTempC)
            {
                for (int i = 0; i < (tempC - optTempC); i++)
                {
                    irsend.sendRaw(rawDataTEMPDEC, RAW_DATA_LEN, 38);
                    if (remTemp > 16)
                    {
                        remTemp = remTemp - 1;
                    }
                    delay(1000);
                }
            }
        }
    }
    delay(2000);

    ThingSpeak.setField(1, tempC);
    ThingSpeak.setField(2, humid ity);
    ThingSpeak.setField(3, remTemp);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    // wifi
    delay(20000);
}
