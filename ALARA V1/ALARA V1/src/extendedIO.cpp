#include "extendedIO.h"

void pinModeExtended(char register_letter, uint8_t register_pin, bool data_direction)
{
  // Find the correct pin control register based on the register letter and pin
  uint32_t PCR = 0x40049000 + (register_letter - 'A') * 0x1000 + register_pin*4;
  // set the pin control register to the PCR mux
  (*(volatile uint32_t *)PCR) = PORT_PCR_MUX(0x1);

  // Set the data direction register based on the data_direction input
  uint32_t PDDR = 0x400FF014 + (register_letter - 'A') * 0x40;
  if(data_direction) // Flip data direction bit to 1 (OUTPUT)
  { (*(volatile uint32_t *)PDDR) |= (1 << register_pin); }
  else // Flip data direction bit to 0 (INPUT)
  { (*(volatile uint32_t *)PDDR) &= !(1 << register_pin); }
}

void digitalWriteExtended(char register_letter, uint8_t register_pin, bool data)
{
  if(data)
  {       
    uint32_t PSOR = 0x400FF004 + (register_letter - 'A') * 0x40;
    (*(volatile uint32_t *)PSOR) = (1<<register_pin);
  }
  else
  {
    uint32_t PCOR = 0x400FF008 + (register_letter - 'A') * 0x40;
    (*(volatile uint32_t *)PCOR) = (1<<register_pin);
  }
}

bool digitalReadExtended(char register_letter, uint8_t register_pin)
{
  uint32_t PDIR = 0x400FF010 + (register_letter - 'A') * 0x40;
  return (*(volatile uint32_t *)PDIR)>>register_pin;
}