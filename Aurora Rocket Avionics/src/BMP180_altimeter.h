#pragma once
#include <stdint.h>
#include <SFE_BMP180.h> // for bmp180 pressure and temp sensor
#include "SD_Storage.h"

#define BMP180_FREQUENCY 40
#define ALT_SAMPLES (BMP180_FREQUENCY * 5)
#define RECENT_SAMPLES (BMP180_FREQUENCY * 2)

class BMP180_altimeter : public SFE_BMP180
{
private:
  double baseline = 0.0; // baseline for ground level pressure
  double altitude_data[ALT_SAMPLES] = {0.0};

  SD_Storage* sd_card;
  char datalog_filename[13] = "ALT_000.csv";
public:
  void init(SD_Storage* sd_card);
  void init_datalog();

  void update_data();
  double get_altitude();
  double get_pressure();

  double get_altitude_data();
  double get_altitude_data_avg();
  double get_altitude_data_avg_recent();
};
