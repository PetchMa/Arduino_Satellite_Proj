#include <Wire.h>
#include "Adafruit_MCP9808.h"
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

//It works!!!!
#include <RH_ASK.h>

#include <SPI.h> // Not actually used but needed to compile

int pin8 = 8;
int sensor = A0;
int sensorValue = 0;
RH_ASK driver(2000, 4, 2, 0); 
void setup() {
  Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  Serial.println("MCP9808 demo");

  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
   Serial.println("Found MCP9808!");
  tempsensor.setResolution(3); 
  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    // Debugging only
  #endif
      if (!driver.init())
  #ifdef RH_HAVE_SERIAL
           Serial.println("init failed");
  #else
    ;
  #endif
  pinMode(pin8,OUTPUT);
  
}

void loop() {
  tempsensor.wake();
  float c = tempsensor.readTempC();
  sensorValue = analogRead(sensor);
  Serial.println(sensorValue, DEC);
  Serial.println(c, 4); 
  
  char msg1[8] = "";
  dtostrf(c, 6, 2, msg1);
  driver.send((uint8_t *)msg1, strlen(msg1));
  driver.waitPacketSent();
  delay(200);
  char msg2 [4] ="";
  dtostrf(sensorValue, 2, 2, msg2);
  driver.send((uint8_t *)msg2, strlen(msg2));
  driver.waitPacketSent();
  
  delay(2000);
  tempsensor.shutdown_wake(1); 
  delay(200);
}
