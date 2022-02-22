#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
#include "data.h"
IRsendRaw mySender;


void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(1000);
  Serial.println("Sending ON");
  mySender.send(rawDataON, RAW_DATA_LEN, 36);
  Serial.println("Sent Turn ON Aircon");
}
