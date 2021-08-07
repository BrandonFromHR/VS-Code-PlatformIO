#include <Arduino.h>
#include "ICM20649_imu.h"
#include "BMP180_altimeter.h" // excluded for sim version of Flight Software
#include "PhysicsSim.h"
#include "Recovery.h"
#include "GT_U7_GPS.h"
#include "SD_Storage.h"


void timer_50Hz_func();
void timer_100Hz_func();
void do_physics();
void print_csv_data();
void print_csv_element(bool data);
void print_csv_element(uint8_t data);
void print_csv_element(uint32_t data);
void print_csv_element(double data);
void print_csv_end(bool data);
void print_csv_end(uint8_t data);
void print_csv_end(uint32_t data);
void print_csv_end(double data);


BMP180_altimeter altimeter; // create altimeter object

PhysicsSim physicsSim;

ICM20649_imu imu; // create imu object

Recovery recovery;

GT_U7_GPS gps;

SD_Storage XTSD;
SD_Storage BISD;

// should be XTSD for flight
#define PRIMARY_STORAGE BISD
// should be BISD for flight
#define SECONDARY_STORAGE XTSD

IntervalTimer timer_50Hz; // Altimeter
IntervalTimer timer_100Hz; // IMU, RECOVERY

uint32_t timer_50Hz_period = 0;
uint32_t timer_100Hz_period = 0;

void setup()
{
  Serial.begin(1000000);
  PRIMARY_STORAGE.init("BISD",BISD_CS,&gps); // primary storage should be XTSD in flight
  SECONDARY_STORAGE.init("XTSD",XTSD_CS,&gps); // shouldn't be here in flight

  physicsSim.init(&PRIMARY_STORAGE);
  altimeter.init(&PRIMARY_STORAGE);
  imu.init(&PRIMARY_STORAGE);
  recovery.init(&PRIMARY_STORAGE);
  gps.init(&PRIMARY_STORAGE);

  SPI.usingInterrupt(timer_50Hz);
  SPI.usingInterrupt(timer_100Hz);
  timer_50Hz.begin(timer_50Hz_func,20000); // 50 Hz interrupt
  timer_50Hz.priority(129);
  timer_100Hz.begin(timer_100Hz_func,10000); // 100 Hz interrupt
  timer_100Hz.priority(128);

  PRIMARY_STORAGE.write_evr(NORMAL,FSW,"FSW setup complete");
  Serial.println("FSW setup complete");
}

void loop()
{

}

void timer_50Hz_func()
{
  uint32_t start_time = micros();

  altimeter.update_data();
  gps.update_data();
  do_physics();
  //print_csv_data();

  timer_50Hz_period = micros() - start_time;
}

void timer_100Hz_func()
{
  uint32_t start_time = micros();

  //recovery.run(&altimeter);
  recovery.run(&physicsSim);
  imu.update_data();

  timer_100Hz_period = micros() - start_time;
}

void do_physics()
{
  bool parachute_deployed[3] = {
    recovery.is_parachute_deployed(0),
    recovery.is_parachute_deployed(1),
    recovery.is_parachute_deployed(2)};
  physicsSim.sim_physics(parachute_deployed);
}

void print_csv_data()
{
  print_csv_element(physicsSim.get_altitude_data());
  print_csv_element(physicsSim.get_velocity());
  print_csv_element(physicsSim.get_acceleration());
  print_csv_element(altimeter.get_altitude_data());
  print_csv_element(imu.get_last_accel_data(0));
  print_csv_element(imu.get_last_accel_data(1));
  print_csv_element(imu.get_last_accel_data(2));
  print_csv_element(imu.get_last_gyro_data(0));
  print_csv_element(imu.get_last_gyro_data(1));
  print_csv_element(imu.get_last_gyro_data(2));
  print_csv_element(recovery.is_parachute_deployed(0));
  print_csv_element(recovery.is_parachute_deployed(1));
  print_csv_element(recovery.is_parachute_deployed(2));
  print_csv_element(recovery.get_deployment_stage());
  print_csv_element(gps.get_latitude());
  print_csv_element(gps.get_longitude());
  print_csv_element(timer_50Hz_period);
  print_csv_end(timer_100Hz_period);
  Serial.println();
}

void print_csv_element(bool data)
{
  Serial.print(int(data));
  Serial.print(",");
}

void print_csv_element(uint8_t data)
{
  Serial.print(data);
  Serial.print(",");
}

void print_csv_element(uint32_t data)
{
  Serial.print(data);
  Serial.print(",");
}

void print_csv_element(double data)
{
  Serial.print(data,6);
  Serial.print(",");
}

void print_csv_end(bool data)
{
  Serial.print(int(data));
}

void print_csv_end(uint8_t data)
{
  Serial.print(data);
}

void print_csv_end(uint32_t data)
{
  Serial.print(data);
}

void print_csv_end(double data)
{
  Serial.print(data,6);
}
