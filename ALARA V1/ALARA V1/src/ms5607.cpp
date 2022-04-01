#include "ms5607.h"


void MS5607::init() // Initialize the sensor
{
    pinMode(CS_PIN, OUTPUT);

    // Reset MS5607 after powerup to ensure PROM load to internal register
    reset();
    read_PROM();
}


void MS5607::reset() // Reset the sensor
{
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(RESET); // send reset command
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();
    delay(10); // wait for reset to complete
}


void MS5607::read_PROM() // Load the PROM data for calibration
{
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C1); // throw away first byte read
    C1 += SPI.transfer(PROM_READ_C1)*256;
    C1 += SPI.transfer(PROM_READ_C1);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C2); // throw away first byte read
    C2 += SPI.transfer(PROM_READ_C2)*256;
    C2 += SPI.transfer(PROM_READ_C2);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C3); // throw away first byte read
    C3 += SPI.transfer(PROM_READ_C3)*256;
    C3 += SPI.transfer(PROM_READ_C3);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C4); // throw away first byte read
    C4 += SPI.transfer(PROM_READ_C4)*256;
    C4 += SPI.transfer(PROM_READ_C4);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C5); // throw away first byte read
    C5 += SPI.transfer(PROM_READ_C5)*256;
    C5 += SPI.transfer(PROM_READ_C5);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(PROM_READ_C6); // throw away first byte read
    C6 += SPI.transfer(PROM_READ_C6)*256;
    C6 += SPI.transfer(PROM_READ_C6);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();
}
 

int32_t MS5607::get_temp()
{
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(CONV_D2_4096);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    delay(10);

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);  
    SPI.transfer(ADC_READ); // throw away first byte read
    uint8_t temp_data[3] = {0};

    for(int i = 0; i < 3; i++)
    { // transfer all 3 bytes of the temperature data
        temp_data[i] = SPI.transfer(ADC_READ);
    }

    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    D2 = temp_data[0]*65536 + temp_data[1]*256 + temp_data[2]; // combine data
    dT = (int64_t)D2 - (int64_t)C5*256*1.011;
    TEMP = 2000 + (int64_t)dT*(int64_t)C6/8388608; 
    return TEMP;
}


uint32_t MS5607::get_pressure()
{
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);
    SPI.transfer(CONV_D1_4096);
    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    delay(10);

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN,0);  
    SPI.transfer(ADC_READ); // throw away first byte read
    uint8_t pres_data[3] = {0};

    for(int i = 0; i < 3; i++)
    { // transfer all 3 bytes of the temperature data
        pres_data[i] = SPI.transfer(ADC_READ);
    }

    digitalWrite(CS_PIN,1);
    SPI.endTransaction();

    D1 = pres_data[0]*65536 + pres_data[1]*256 + pres_data[2]; // combine data
    OFF = (int64_t)C2*131072 + ((int64_t)C4*(int64_t)dT)/64;
    SENS = (int64_t)C1*65536 + ((int64_t)C3*(int64_t)dT)/128;
    PRES = ((int64_t)D1*SENS/2097152 - OFF)/32768;
    return PRES;
}


void MS5607::print_all()
{
    Serial.print("C1:");
    Serial.print(C1);
    Serial.print(" C2:");
    Serial.print(C2);
    Serial.print(" C3:");
    Serial.print(C3);
    Serial.print(" C4:");
    Serial.print(C4);
    Serial.print(" C5:");
    Serial.print(C5);
    Serial.print(" C6:");
    Serial.print(C6);

    Serial.print(" D1:");
    Serial.print(D1);
    Serial.print(" D2:");
    Serial.print(D2);

    Serial.print(" dT:");
    Serial.print(dT);
    Serial.print(" TEMP:");
    Serial.print(TEMP);

    Serial.print(" OFF:");
    Serial.print(OFF);
    Serial.print(" SENS:");
    Serial.print(SENS);
    Serial.print(" PRES:");
    Serial.println(PRES);
}


double MS5607::get_altitude()
{
    double altitude = (2.408058-pow((double)PRES/1000.0,1.0/5.256))/0.00005074287;
    if(altitude > 11000)
    {
        altitude = (1.7028-log(PRES/22.65))/0.000157;
    }
    return altitude;
}