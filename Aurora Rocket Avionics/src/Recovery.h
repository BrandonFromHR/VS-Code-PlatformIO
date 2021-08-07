#pragma once
#include <stdint.h>
#include "PhysicsSim.h"
#include "SD_Storage.h"

#define MAIN_DEPLOY_ALTITUDE 200 // meters
#define MIN_DEPLOY_ALTITUDE 300 // meters
#define DROGUE_2_DELAY 1000 // milliseconds
#define PYRO_PERIOD 500 // milliseconds
#define DEPLOY_BUFFER 30 // meters

class Recovery
{
private:
  /* PARACHUTE INDEX
  * 0 = drogue chute 1
  * 1 = drogue chute 2
  * 2 = main chute
  */
  bool parachute_deployed[3] = {false};
  uint32_t parachute_deploy_time[3];
  uint8_t parachute_pyro_pin[3] = {2,3,4};
  /* Deployment Stage
  * 0 = wait for apogee, Enable Drogue 1 Pyro, wait for disable
  * 1 = wait, Enable Drogue 2 Pyro, wait for disable
  * 2 = wait, Enable Main Pyro, wait for disable
  * 3 = coast till touchdown
  */
  uint8_t deployment_stage = 0;
  SD_Storage* sd_card;
public:
  void init(SD_Storage* sd_card);
  bool is_parachute_deployed();
  bool is_parachute_deployed(uint8_t index);
  uint32_t get_parachute_deploy_time(uint8_t index);
  void run(PhysicsSim* pSim);
  void run(BMP180_altimeter* altimeter);
  void pyro_enable(uint8_t index);
  void pyro_disable(uint8_t index);
  uint8_t get_deployment_stage();
  void increment_deployment_stage();
};
