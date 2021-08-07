#pragma once
#include <stdint.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "SD_Storage.h"

#define ICM_CS 37
#define AXES 3
#define DATA_POINTS 100

class ICM20649_imu : public Adafruit_ICM20649
{
private:
  // axis indexes are 0 = X, 1 = Y, 2 = Z

  // stores past 100 samples of acceleration data for all 3 axes
  float accel_data[AXES][DATA_POINTS] = {{0.0}};

  // stores past 100 samples of gyroscopic data for all 3 axes
  float gyro_data[AXES][DATA_POINTS] = {{0.0}};

  // stores past 100 samples of temperature data from IMU
  float temp_data[DATA_POINTS] = {{0.0}};

  uint8_t index = 0; // keeps track of current data point in arrays

  SD_Storage* sd_card;
  char datalog_filename[13] = "IMU_00.csv";
public:
  void init(SD_Storage* sd_card);
  void init_datalog();

  void update_data();
  void increment_index();

  float get_last_accel_data(uint8_t axis_index);
  float get_avg_accel(uint8_t axis_index);

  float get_last_gyro_data(uint8_t axis_index);
  float get_avg_gyro(uint8_t axis_index);

  float get_last_temp_data();
  float get_avg_temp();

  float get_accel_magnitude();
  float get_accel_magnitude(uint8_t specific_index);
  float get_avg_accel_magnitude();

  float get_gyro_magnitude();
  float get_gyro_magnitude(uint8_t specific_index);
  float get_avg_gyro_magnitude();
};
