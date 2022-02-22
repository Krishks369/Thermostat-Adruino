#include <DHT.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
#include "./data.h"

float humidity;
float tempC;
int setTime = 500;
int dt = 1000;
int sensePin = 7;
float optTempC = 25.0;
int onFlag = 0;

#define Type DHT11
DHT dht(sensePin, Type);
IRsendRaw mySender;

void setup()
{
    Serial.begin(9600);
    dht.begin();
    delay(setTime);
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
            mySender.send(rawDataOFF, RAW_DATA_LEN, 38);
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
            mySender.send(rawDataON, RAW_DATA_LEN, 38);
            delay(dt);
            mySender.send(rawDataMODE, RAW_DATA_LEN, 38);
            delay(dt);
            onFlag = 1;
            if (tempC > optTempC)
            {
                for (int i = 0; i < (tempC - optTempC); i++)
                {
                    mySender.send(rawDataTEMPDEC, RAW_DATA_LEN, 38);
                    delay(1000);
                }
            }
        }
    }
}
