#include "Recovery.h"


void Recovery::init(SD_Storage* sd_card)
{
  // setup pyro pins for output
  for(int i = 0; i < 3; i++)
  {
    pinMode(parachute_pyro_pin[i],OUTPUT);
  }
  this->sd_card = sd_card;

  this->sd_card->write_evr(NORMAL,FSW,"Recovery initialization complete");
  Serial.println("Recovery initialization complete");
}


bool Recovery::is_parachute_deployed()
{
  return parachute_deployed;
}


bool Recovery::is_parachute_deployed(uint8_t index)
{
  return parachute_deployed[index];
}


uint32_t Recovery::get_parachute_deploy_time(uint8_t index)
{
  return parachute_deploy_time[index];
}


void Recovery::run(PhysicsSim* pSim)
{
  switch(deployment_stage)
  {
  case 0: // wait for apogee, Enable Drogue 1 Pyro, wait for disable
    if(pSim->get_altitude_data_avg_recent() - DEPLOY_BUFFER <
       pSim->get_altitude_data_avg() &&
       pSim->get_altitude_data_avg() >= MIN_DEPLOY_ALTITUDE &&
       !parachute_deployed[0])
    {
      pyro_enable(0);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO ENABLED! Drogue Parachute 1");
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 1 Parachute deployed at "
                            + String(pSim->get_altitude_data_avg_recent())
                            + " meters");
    }
    else if(parachute_deployed[0] &&
            millis() >= parachute_deploy_time[0] + PYRO_PERIOD)
    {
      pyro_disable(0);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO DISABLED! Drogue Parachute 1");
    }
    break;
  case 1: // wait, Enable Drogue 2 Pyro, wait for disable
    if(millis() >= parachute_deploy_time[0] + DROGUE_2_DELAY &&
       !parachute_deployed[1])
    {
      pyro_enable(1);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO ENABLED! Drogue Parachute 2");
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 2 Parachute deployed at "
                            + String(pSim->get_altitude_data_avg_recent())
                            + " meters");
    }
    else if(parachute_deployed[1] &&
            millis() > parachute_deploy_time[1] + PYRO_PERIOD)
    {
      pyro_disable(1);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO DISABLED! Drogue Parachute 2");
    }
    break;
  case 2: // wait, Enable Main Pyro, wait for disable
    if(pSim->get_altitude_data_avg_recent() < MAIN_DEPLOY_ALTITUDE + DEPLOY_BUFFER &&
       !parachute_deployed[2])
    {
      pyro_enable(2);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO ENABLED! Main Parachute");
      sd_card->write_evr(NORMAL,RECOVERY,"Main Parachute deployed at "
                            + String(pSim->get_altitude_data_avg_recent())
                            + " meters");
    }
    else if(parachute_deployed[2] &&
            millis() > parachute_deploy_time[2] + PYRO_PERIOD)
    {
      pyro_disable(2);
      sd_card->write_evr(CONTROL,RECOVERY,"PYRO DISABLED! Main Parachute");
    }
    break;
  case 3: // coast till touchdown
    // do nothing
    break;
  }
}


void Recovery::run(BMP180_altimeter* altimeter)
{

}


void Recovery::pyro_enable(uint8_t index)
{
  digitalWrite(parachute_pyro_pin[index],HIGH);
  parachute_deployed[index] = true;
  parachute_deploy_time[index] = millis();
}


void Recovery::pyro_disable(uint8_t index)
{
  digitalWrite(parachute_pyro_pin[index],LOW);
  increment_deployment_stage();
}


uint8_t Recovery::get_deployment_stage()
{
  return deployment_stage;
}


void Recovery::increment_deployment_stage()
{
  deployment_stage++;
  sd_card->write_evr(NORMAL,RECOVERY,
    "Recovery deployment stage incremented to stage " + String(deployment_stage));
}
