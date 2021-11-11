#pragma once
#include <stdint.h>
#include "PhysicsSim.h"
#include "SD_Storage.h"

#define MAIN_DEPLOY_ALTITUDE 200 // meters
#define STAGE_1_ALTITUDE 200 // meters
#define PYRO_PERIOD 1000 // milliseconds
#define DEPLOY_BUFFER 30 // meters
#define TOUCHDOWN_TOLERANCE 2

#define BUZZER_STEP 250
#define BUZZER_MAX 5000
#define BUZZER_MIN 250
#define BUZZER_PIN 5
#define BUZZER_PWM 10

#define CHIRP_FREQ 1500
#define CHIRP_PERIOD 1000 // in ms

#define PYRO_JUMPER_PIN 40

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
  * 0 = Waiting for launch, waiting for 100m altitude
  * 1 = Launched, wait for apogee, Enable Drogue 1 & 2 Pyros, wait for disable
  * 2 = wait, Enable Main Pyro, wait for disable
  * 3 = wait for touchdown, recent altitude within 5m of altitude avg
  * 4 = Touched down, dump data to sd, cry for help
  * 5 = data is dumped, continue crying
  */
  uint8_t recovery_stage = 0;
  SD_Storage* sd_card;

  bool buzzer_increase_freq = true;
  uint32_t buzzer_frequency = 2500;

  bool chirp_on = false;
  uint32_t last_chirp = 0;

  bool data_dumped = false;
public:
  void init(SD_Storage* sd_card);
  bool is_parachute_deployed();
  bool is_parachute_deployed(uint8_t index);
  uint32_t get_parachute_deploy_time(uint8_t index);
  void run(PhysicsSim* pSim);
  void run(BMP180_altimeter* altimeter);
  void pyro_enable(uint8_t index);
  void pyro_disable(uint8_t index);
  uint8_t get_recovery_stage();
  void increment_recovery_stage();
  void buzzer_cry();
  void buzzer_chirp(uint8_t divider);
  
  void data_just_dumped();
};
