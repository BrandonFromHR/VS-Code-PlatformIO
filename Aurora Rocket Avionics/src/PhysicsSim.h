#pragma once
#include "BMP180_altimeter.h"
#include "SD_Storage.h"

#define GRAVITY 9.81 // m/s^2

#define I500_THRUST 500.0 // Newtons
#define I500_BURNTIME 1.2 // seconds

#define WET_MASS 2.7 // Kg
#define PROP_MASS 0.312 // Kg
#define DRY_MASS WET_MASS - PROP_MASS // Kg

#define AIR_DENSITY 1.1 // Kg/m^3
#define DROGUE_DRAG_COEFF 1.0
#define MAIN_DRAG_COEFF 1.0

#define LAUNCH_TIME 10000 // milliseconds

class PhysicsSim
{
private:
  double real_altitude = 0.0; // meters
  double mass = WET_MASS; // Kg
  double velocity = 0.0; // m/s
  double acceleration = 0.0; // m/s^2

  double altitude_data[ALT_SAMPLES] = {0.0};

  bool apogee_detected = false;

  // time in us since last physics calculation
  uint32_t lastPhysCalc = LAUNCH_TIME*1000;

  SD_Storage* sd_card;
  char datalog_filename[13] = "SIM_000.csv";
public:
  void init(SD_Storage* sd_card);
  void init_datalog();

  void sim_physics(bool parachute_deployed[3]);

  double get_altitude_data();
  double get_altitude_data_avg();
  double get_altitude_data_avg_recent();

  double get_velocity();
  double get_acceleration();
};
