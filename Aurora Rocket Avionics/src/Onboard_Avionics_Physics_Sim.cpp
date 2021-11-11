#include <Arduino.h>
#include "ICM20649_imu.h"
#include "BMP180_altimeter.h" // excluded for sim version of Flight Software
#include "PhysicsSim.h"
#include "Recovery.h"
#include "GT_U7_GPS.h"
#include "SD_Storage.h"

void timer_8Hz_func();
void timer_32Hz_func();
void timer_100Hz_func();
void sim_physics();
void buzzer_cry();
void print_csv_data_sim();
void print_csv_data_flight();
void print_csv_element(bool data);
void print_csv_element(uint8_t data);
void print_csv_element(uint32_t data);
void print_csv_element(double data);
void print_csv_end(bool data);
void print_csv_end(uint8_t data);
void print_csv_end(uint32_t data);
void print_csv_end(double data);
void dump_data();

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

IntervalTimer timer_8Hz; // Serial print
IntervalTimer timer_32Hz; // Altimeter & GPS
IntervalTimer timer_100Hz; // IMU, RECOVERY

uint32_t timer_8Hz_period = 0;
uint32_t timer_32Hz_period = 0;
uint32_t timer_100Hz_period = 0;

bool data_dumped = false;

void setup()
{
  SPI.setClockDivider(150);
  Serial.begin(500000);
  PRIMARY_STORAGE.init("BISD",BISD_CS,&gps); // primary storage should be XTSD in flight
  PRIMARY_STORAGE.remove_all_files(); // only use right before flight, not during flight

  physicsSim.init(&PRIMARY_STORAGE);
  altimeter.init(&PRIMARY_STORAGE);
  imu.init(&PRIMARY_STORAGE);
  recovery.init(&PRIMARY_STORAGE);
  gps.init(&PRIMARY_STORAGE);

  SPI.usingInterrupt(timer_8Hz);
  SPI.usingInterrupt(timer_32Hz);
  SPI.usingInterrupt(timer_100Hz);
  timer_8Hz.begin(timer_8Hz_func,125000); // 8 Hz interrupt
  timer_8Hz.priority(130);
  timer_32Hz.begin(timer_32Hz_func,31250); // 32 Hz interrupt
  timer_32Hz.priority(129);
  timer_100Hz.begin(timer_100Hz_func,10000); // 100 Hz interrupt
  timer_100Hz.priority(128);

  PRIMARY_STORAGE.write_evr(NORMAL,FSW,"FSW setup complete");
  Serial.println("FSW setup complete");
}

void loop()
{
  /*
  if(recovery.get_recovery_stage() == 4)
  {
    dump_data();
    recovery.data_just_dumped();
  }
  */
}

void timer_8Hz_func()
{
  uint32_t start_time = micros();

  //print_csv_data_sim();
  print_csv_data_flight();

  timer_8Hz_period = micros() - start_time;
}

void timer_32Hz_func()
{
  uint32_t start_time = micros();

  if(recovery.get_recovery_stage() < 4)
  {
    altimeter.update_data();
    gps.update_data(); // wasn't working well in 2Hz timer
    sim_physics();
  }

  timer_32Hz_period = micros() - start_time;
}

void timer_100Hz_func()
{
  uint32_t start_time = micros();

  if(recovery.get_recovery_stage() < 4)
  {
    imu.update_data();
  }  

  recovery.run(&altimeter); // MUST USE THIS IN FLIGHT

  //recovery.run(&physicsSim); // ABSOLUTELY DO NOT USE THIS IN FLIGHT, SIM ONLY!!!

  timer_100Hz_period = micros() - start_time;
}

void sim_physics()
{
  bool parachute_deployed[3] = {
    recovery.is_parachute_deployed(0),
    recovery.is_parachute_deployed(1),
    recovery.is_parachute_deployed(2)};
  physicsSim.sim_physics(parachute_deployed);
}

void print_csv_data_sim()
{
  print_csv_element(physicsSim.get_altitude_data_avg_recent());  
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
  print_csv_element(recovery.get_recovery_stage());
  print_csv_element(gps.get_latitude());
  print_csv_element(gps.get_longitude());  
  print_csv_element(timer_8Hz_period);
  print_csv_element(timer_32Hz_period);
  print_csv_end(timer_100Hz_period);
  Serial.println();
}

void print_csv_data_flight()
{
  print_csv_element(altimeter.get_altitude_data_avg_recent());
  print_csv_element(imu.get_avg_accel(0));
  print_csv_element(imu.get_avg_accel(1));
  print_csv_element(imu.get_avg_accel(2));
  print_csv_element(imu.get_avg_gyro(0));
  print_csv_element(imu.get_avg_gyro(1));
  print_csv_element(imu.get_avg_gyro(2));
  print_csv_element(recovery.is_parachute_deployed(0));
  print_csv_element(recovery.is_parachute_deployed(1));
  print_csv_element(recovery.is_parachute_deployed(2));
  print_csv_element(recovery.get_recovery_stage());
  print_csv_element(gps.get_latitude());
  print_csv_element(gps.get_longitude());  
  print_csv_element(timer_8Hz_period);
  print_csv_element(timer_32Hz_period);
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
{  Serial.print(int(data)); }

void print_csv_end(uint8_t data)
{  Serial.print(data); }

void print_csv_end(uint32_t data)
{  Serial.print(data); }

void print_csv_end(double data)
{  Serial.print(data,10); }

void dump_data()
{
  Serial.println("Dumping data");
  SECONDARY_STORAGE.init("BISD",BISD_CS,&gps);
  
  // removes all files from card
  SECONDARY_STORAGE.remove_all_files();

  char file_subname[NUM_DATAFILES][SUBNAME_SIZE] = 
  {
    "EVR","IMU","ALT","GPS"
  };
  for(char i4 = '0'; i4 <= '9'; i4++)
  {
    for(char i5 = '0'; i5 < '9'; i5++)
    {
      for(char i6 = '0'; i6 < '9'; i6++)
      {
        for(int i = 0; i < NUM_DATAFILES; i++)
        {          
          char filename[12];
          filename[0] = file_subname[i][0];
          filename[1] = file_subname[i][1];
          filename[2] = file_subname[i][2];
          filename[3] = '_';
          filename[4] = i4;
          filename[5] = i5;
          filename[6] = i6;
          filename[7] = '.';
          filename[8] = 'c';
          filename[9] = 's';
          filename[10] = 'v';
          filename[11] = '\0';
      
          File dataFile = PRIMARY_STORAGE.open(filename);
          String fileData;
          // if the file is available, write to it:
          if (dataFile)
          {
            while (dataFile.available())
            {
              char temp = dataFile.read();
              fileData += temp;
            }
            File dumpFile = SECONDARY_STORAGE.open(filename,FILE_WRITE);
            if(dumpFile)
            {
              dumpFile.print(fileData);
            }
            else
            { 
              String error = "Error opening ";
              error += filename;              
              error += " on BISD";
              Serial.println(error);
              i4 = '9' + 1;
              i5 = '9' + 1;
              i6 = '9' + 1;
              break;
            }     
            dataFile.close();
            dumpFile.close();
          }  
          // if the file isn't open, pop up an error:
          else
          { 
            String error = "Error opening ";
            error += filename;
            error += " on XTSD";
            Serial.println(error);
            i4 = '9' + 1;
            i5 = '9' + 1;
            i6 = '9' + 1;
            break;
          }     
        }
      }
    }
  }
}