#pragma once
#include <stdint.h>
#include "TinyGPS++.h"
#include "SD_Storage.h"

class SD_Storage; // declaration for circular dependencies

class GT_U7_GPS : public TinyGPSPlus
{
private:
  double start_latitude = 0.0;
  double start_longitude = 0.0;
  bool foundStartLocation = false;

  uint8_t hour_data = 0;
  uint8_t minute_data = 0;
  uint8_t second_data = 0;
  uint32_t date_data = 0;
  double latitude_data = 0.0;
  double longitude_data = 0.0;
  double altitude_data = 0.0;
  double speed_data = 0.0;
  double course_data = 0.0;
  double distance_data = 0.0;

  SD_Storage* sd_card;
  char datalog_filename[13] = "GPS_00.csv";
public:
  void init(SD_Storage* sd_card);
  void init_datalog();

  void update_data();

  double get_latitude();
  double get_longitude();

  uint8_t get_hour_data();
  uint8_t get_minute_data();
  uint8_t get_second_data();
  uint32_t get_date_data();
};
