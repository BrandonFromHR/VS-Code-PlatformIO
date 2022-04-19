#include <Arduino.h>
#include <SPI.h>
#include "ms5607.h"

#define RING_FREQUENCY 2700 // Hz
#define PERIOD 100 // milliseconds
#define DUTY_CYCLE 0.05

const uint16_t period_us = 1000000/RING_FREQUENCY;

/*
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
*/

uint32_t last_count = 0;
uint32_t count = 0;


MS5607 bps;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  
  bps.init();
  bps.set_D1_OSR(CONV_D1_4096);
  bps.set_D2_OSR(CONV_D2_4096);
}

double temperature = 0;
double pressure = 0;
double altitude = 0;

void loop()
{   
  
  Serial.print("Temp ");
  Serial.print(bps.get_temp()); 
  Serial.print(" centicelsius - Pressure ");
  Serial.print(bps.get_pressure());
  Serial.print(" Pa - Altitude ");
  Serial.print(bps.get_altitude());
  Serial.println(" centimeters");
  
  delay(1000);
  
  /*
  temperature += bps.get_temp();
  pressure += bps.get_pressure();
  altitude += bps.get_altitude();
  
  count++;

  if(millis() >= last_count + 1000)
  {
    temperature /= count;
    pressure /= count;
    altitude /= count;

    last_count = millis();
    Serial.print("Count = ");
    Serial.print(count);

    Serial.print("  Temperature = ");
    Serial.print(temperature);
    Serial.print("  Pressure = ");
    Serial.print(pressure);
    Serial.print("  Altitude = ");
    Serial.println(altitude);

    temperature = 0;
    pressure = 0;
    altitude = 0;
    count = 0;
    
  } 
  */
}
