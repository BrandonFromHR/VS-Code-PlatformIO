#pragma once
#include "BMP180_altimeter.h"
#include "SD_Storage.h"

#define GRAVITY 9.81 // m/s^2

#define AURORA_MASS 2.066 // Kg

#define I500_THRUST 500.0 // Newtons
#define I500_BURNTIME 1.24 // seconds
#define I500_WET_MASS 0.576 // Kg
#define I500_PROP_MASS 0.312 // Kg

#define J435_THRUST 435 // Newtons
#define J435_BURNTIME 1.7 // seconds
#define J435_WET_MASS 0.624 // Kg
#define J435_PROP_MASS 0.352 // Kg

#define K535_THRUST 535 // Newtons
#define K535_BURNTIME 2.8 // seconds
#define K535_WET_MASS 1.264 // Kg
#define K535_PROP_MASS 0.745 // Kg

#define K455_THRUST 455 // Newtons
#define K455_BURNTIME 5.1 // seconds
#define K455_WET_MASS 2.286 // Kg
#define K455_PROP_MASS 1.361 // Kg

#define SIM_THRUST K455_THRUST // Newtons
#define SIM_BURNTIME K455_BURNTIME // seconds
#define SIM_WET_MASS AURORA_MASS + K455_WET_MASS // Kg
#define SIM_PROP_MASS K455_PROP_MASS // Kg
#define SIM_DRY_MASS SIM_WET_MASS - SIM_PROP_MASS // Kg

#define AIR_DENSITY 1.1 // Kg/m^3
#define DROGUE_DRAG_COEFF 1.0
#define MAIN_DRAG_COEFF 1.0
#define AURORA_DRAG_COEFF 0.0013

#define LAUNCH_TIME 5000 // milliseconds

class PhysicsSim
{
private:
  double real_altitude = 0.0; // meters
  double mass = SIM_WET_MASS; // Kg
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
