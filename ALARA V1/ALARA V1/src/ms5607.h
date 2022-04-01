#pragma once
#include <SPI.h>

#include <Arduino.h>

class MS5607
{
private:
    uint8_t CS_PIN = 10;

    // command bytes
    const uint16_t RESET = 0x1E;          // Reset Command Byte
    
    const uint16_t CONV_D1_256 = 0x40;   // Pressure Convert OSR 256 Command Byte  
    const uint16_t CONV_D1_512 = 0x42;   // Pressure Convert OSR 512 Command Byte  
    const uint16_t CONV_D1_1024 = 0x44;   // Pressure Convert OSR 1024 Command Byte  
    const uint16_t CONV_D1_2048 = 0x46;   // Pressure Convert OSR 2048 Command Byte  
    const uint16_t CONV_D1_4096 = 0x48;   // Pressure Convert OSR 4096 Command Byte  

    const uint16_t CONV_D2_256 = 0x50;   // Temperature Convert OSR 256 Command Byte
    const uint16_t CONV_D2_512 = 0x52;   // Temperature Convert OSR 512 Command Byte
    const uint16_t CONV_D2_1024 = 0x54;   // Temperature Convert OSR 1024 Command Byte
    const uint16_t CONV_D2_2048 = 0x56;   // Temperature Convert OSR 2048 Command Byte
    const uint16_t CONV_D2_4096 = 0x58;   // Temperature Convert OSR 4096 Command Byte

    const uint16_t ADC_READ = 0x00;       // ADC Read Command Byte

    const uint16_t PROM_READ_C1 = 0xA2;
    const uint16_t PROM_READ_C2 = 0xA4;
    const uint16_t PROM_READ_C3 = 0xA6;
    const uint16_t PROM_READ_C4 = 0xA8;
    const uint16_t PROM_READ_C5 = 0xAA;
    const uint16_t PROM_READ_C6 = 0xAC;
    
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
    void init();                // Initialize the sensor
    void reset();               // Reset the sensor
    void read_PROM();           // Load the PROM data for calibration
   
    int32_t get_temp();
    uint32_t get_pressure();
    double get_altitude();

    void print_all();
};