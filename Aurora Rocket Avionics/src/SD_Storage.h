#pragma once
#include <SD.h>
#include <SPI.h>
#include "GT_U7_GPS.h"

// Subsystems
#define FSW "FSW"
#define IMU "IMU"
#define ALTIMETER "ALTIMETER"
#define RECOVERY "RECOVERY"
#define GPS1 "GPS"
#define STORAGE "STORAGE"
#define SIM "SIM"


// Event Report (EVR) TYPES
#define NORMAL "NORMAL" // An normal, expected event happened
#define CONTROL "CONTROL" // A controling event happened (Ex. Pyro Firing)
#define WARNING "WARNING" // An unexpected event happened
#define CRITICAL "CRITICAL" // A disasterous event happened

// Chip Select Pins
#define XTSD_CS 36
#define BISD_CS BUILTIN_SDCARD

class GT_U7_GPS; // declaration for circular dependencies

class SD_Storage : public SDClass
{
private:
  String name;
  uint8_t csPin;

  char evr_filename[13] = "EVR_00.csv";

  GT_U7_GPS* gps;
public:
  void init(String name, uint8_t cs_pin, GT_U7_GPS* gps);
  void init_evr_file();
  void write_line(char filename[13], String dataString);
  void write_evr(String type, String subsystem, String note);
  void read(char filename[13]);
};
