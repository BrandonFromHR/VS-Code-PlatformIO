/*
MS5607 Barometric Pressure Sensor driver

Written by Brandon Summers
Last updated April 18, 2022
Based on MS5607 datasheet information

Arduino platform friendly - Made specifically to be used with Teensy 3.6
*/

#pragma once
#include <SPI.h>

#include <Arduino.h>

#define BME_SPI_RATE 20000000 // Hz


enum D1_OSR : const uint8_t
{
    CONV_D1_256 = 0x40,   // Pressure Convert OSR 256 Command Byte  
    CONV_D1_512 = 0x42,   // Pressure Convert OSR 512 Command Byte  
    CONV_D1_1024 = 0x44,  // Pressure Convert OSR 1024 Command Byte  
    CONV_D1_2048 = 0x46,  // Pressure Convert OSR 2048 Command Byte  
    CONV_D1_4096 = 0x48   // Pressure Convert OSR 4096 Command Byte  
};

enum D2_OSR : const uint8_t
{
    CONV_D2_256 = 0x50,   // Temperature Convert OSR 256 Command Byte
    CONV_D2_512 = 0x52,   // Temperature Convert OSR 512 Command Byte
    CONV_D2_1024 = 0x54,  // Temperature Convert OSR 1024 Command Byte
    CONV_D2_2048 = 0x56,  // Temperature Convert OSR 2048 Command Byte
    CONV_D2_4096 = 0x58   // Temperature Convert OSR 4096 Command Byte
};

enum PROM_COEF : const uint8_t
{
    PROM_READ_C1 = 0xA2,
    PROM_READ_C2 = 0xA4,
    PROM_READ_C6 = 0xAC,
    PROM_READ_C3 = 0xA6,
    PROM_READ_C4 = 0xA8,
    PROM_READ_C5 = 0xAA
};


class MS5607
{
private:
    uint8_t CS_PIN = 10;

    // command bytes
    const uint16_t RESET = 0x1E;    // Reset Command Byte    
    const uint16_t ADC_READ = 0x00; // ADC Read Command Byte

    // oversampling rates
    D1_OSR d1_osr = CONV_D1_256;
    D2_OSR d2_osr = CONV_D2_256;
    
    // Calibration data from PROM
    int64_t C1 = 0;
    int64_t C2 = 0;
    int64_t C3 = 0;
    int64_t C4 = 0;
    int64_t C5 = 0;
    int64_t C6 = 0;

    int64_t D1 = 0;
    int64_t D2 = 0;

    int64_t dT = 0;
    int64_t TEMP = 0;

    int64_t OFF = 0;
    int64_t SENS = 0;
    int64_t PRES = 0;

public:
    void init();
    void reset(); 

    void set_D1_OSR(D1_OSR new_d1_osr);
    D1_OSR get_D1_OSR();
    void set_D2_OSR(D2_OSR new_d2_osr);
    D2_OSR get_D2_OSR();

    uint16_t get_D1_OSR_delay();
    uint16_t get_D2_OSR_delay();

    void get_PROM();
    void get_coef(int64_t* coef, uint16_t coef_reg);
   
    int32_t get_temp();
    uint32_t get_pressure();
    uint32_t get_altitude();

    void print_all();
};