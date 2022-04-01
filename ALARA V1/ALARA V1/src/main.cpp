#include <Arduino.h>
#include <SPI.h>
#include "ms5607.h"

#define RING_FREQUENCY 2700 // Hz
#define PERIOD 100 // milliseconds
#define DUTY_CYCLE 0.1

const uint16_t period_us = 1000000/RING_FREQUENCY;

void setup()
{
  pinMode(54,OUTPUT);
}

void loop()
{
  for(int i = 0; i < RING_FREQUENCY*PERIOD/1000.0; i++)
  {
    digitalWrite(54,1);
    delayMicroseconds(period_us*DUTY_CYCLE);
    digitalWrite(54,0);
    delayMicroseconds(period_us*(1.0-DUTY_CYCLE));
  }

  delay(1000 - PERIOD);
}

















/*
MS5607 bps;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  bps.init();
}

void loop()
{ 
  Serial.print("Temp ");
  Serial.print((double)bps.get_temp()/100.0); 
  Serial.print(" degC - Pressure ");
  Serial.print(bps.get_pressure());
  Serial.print(" Pa - Altitude ");
  Serial.print(bps.get_altitude());
  Serial.println(" meters");
  delay(80);
}
*/