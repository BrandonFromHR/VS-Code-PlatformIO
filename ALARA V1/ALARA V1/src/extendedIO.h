#pragma once
#include <Arduino.h>

void pinModeExtended(char register_letter, uint8_t register_pin, bool data_direction);
void digitalWriteExtended(char register_letter, uint8_t register_pin, bool data);
bool digitalReadExtended(char register_letter, uint8_t register_pin);

// Extended Digital Pin Definitions
#define ED1 'E',6
#define ED2 'E',7
#define ED3 'E',8
#define ED4 'E',9
#define ED5 'E',12
#define ED6 'E',27
#define ED7 'E',28
#define ED8 'A',4
#define ED9 'A',6
#define ED10 'A',7
#define ED11 'A',8
#define ED12 'A',9
#define ED13 'A',10
#define ED14 'A',11
#define ED15 'A',24
#define ED16 'A',25
#define ED17 'A',27
#define ED18 'B',6
#define ED19 'B',7
#define ED20 'B',8
#define ED21 'B',9
#define ED22 'C',12
#define ED23 'C',13
#define ED24 'C',14
#define ED25 'C',15
#define ED26 'C',16
#define ED27 'C',17
#define ED28 'C',18
#define ED29 'C',19
#define ED30 'D',10