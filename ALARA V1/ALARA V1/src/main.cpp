#include <Arduino.h>
#include <SPI.h>
#include "ms5607.h"
#include "extendedIO.h"

/*
uint32_t counter[2] = {0};
uint8_t pinOn[2] = {0};

char register_letter[2] = {'E','E'};
uint8_t register_pin[2] = {6,7};

void setup()
{
  Serial.begin(1000000);
  pinModeExtended(ED1,OUTPUT);
  pinModeExtended(ED2,OUTPUT); 
}


uint32_t printCounter = 0;
*/

/*
#define RING_FREQUENCY 2700 // Hz
#define PERIOD 100 // milliseconds
#define DUTY_CYCLE 0.05

const uint16_t period_us = 1000000/RING_FREQUENCY;
*/


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
  pinMode(13,OUTPUT);
  pinMode(10,OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  
  bps.init(BME_CS_PIN,OSR_4096);
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
}