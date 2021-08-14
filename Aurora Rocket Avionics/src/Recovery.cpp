#include "Recovery.h"


void Recovery::init(SD_Storage* sd_card)
{
  // setup pyro pins for output
  for(int i = 0; i < 3; i++)
  {
    pinMode(parachute_pyro_pin[i],OUTPUT);
  }

  analogWriteFrequency(BUZZER_PIN,buzzer_frequency);  
  analogWrite(BUZZER_PIN,0);

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
  if(recovery_stage == 0 && analogRead(PYRO_JUMPER_PIN) > 50)
  {
    buzzer_chirp(8); // brief chirps
  }
  else if(recovery_stage > 0 && recovery_stage < 4 && analogRead(PYRO_JUMPER_PIN) > 50)
  {
    buzzer_chirp(2); // long chirps, system is "in flight"
  }
  else if(recovery_stage >= 0 && recovery_stage < 4)
  {
    analogWrite(BUZZER_PIN,0);
  }
  switch(recovery_stage)
  {
  case 0: // Waiting for launch
    if(pSim->get_altitude_data_avg() > STAGE_1_ALTITUDE)
    {
      increment_recovery_stage();
    }
    break;
  case 1: // wait for apogee, Enable Drogue Pyros, wait for disable
    if(pSim->get_altitude_data_avg_recent() - DEPLOY_BUFFER <
       pSim->get_altitude_data_avg() &&
       pSim->get_altitude_data_avg() >= STAGE_1_ALTITUDE &&
       !parachute_deployed[0])
    {
      pyro_enable(0);      
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 1 Parachute deployed at "
                        + String(pSim->get_altitude_data_avg_recent()) + " meters");
      pyro_enable(1);
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 2 Parachute deployed at "
                        + String(pSim->get_altitude_data_avg_recent()) + " meters");
    }
    else if(parachute_deployed[0] &&
            millis() >= parachute_deploy_time[0] + PYRO_PERIOD)
    {
      pyro_disable(0);
      pyro_disable(1);      
      increment_recovery_stage();
    }
    break;
  case 2: // wait, Enable Main Pyro, wait for disable
    if(pSim->get_altitude_data_avg_recent() < MAIN_DEPLOY_ALTITUDE + DEPLOY_BUFFER &&
       !parachute_deployed[2])
    {
      pyro_enable(2);
      sd_card->write_evr(NORMAL,RECOVERY,"Main Parachute deployed at "
                        + String(pSim->get_altitude_data_avg_recent()) + " meters");
    }
    else if(parachute_deployed[2] &&
            millis() > parachute_deploy_time[2] + PYRO_PERIOD)
    {
      pyro_disable(2);      
      increment_recovery_stage();
    }
    break;
  case 3: // coast till touchdown
    if(pSim->get_altitude_data_avg() < 25)
    {
      if(pSim->get_altitude_data_avg_recent() < pSim->get_altitude_data_avg() + TOUCHDOWN_TOLERANCE &&
        pSim->get_altitude_data_avg_recent() > pSim->get_altitude_data_avg() - TOUCHDOWN_TOLERANCE)
      {
        sd_card->write_evr(NORMAL,RECOVERY,"Landing detected!");
        sd_card->write_evr(CONTROL,RECOVERY,"Beginning cry for help!");
        increment_recovery_stage();
      }
    }
    break;
  case 4: // touched down, dump data, cry for help
    buzzer_cry();
    if(data_dumped)
    {
      increment_recovery_stage();
    }
    break;
  case 5: // cry for help forever
    buzzer_cry();
    break;
  }
}


void Recovery::run(BMP180_altimeter* altimeter)
{
  if(recovery_stage == 0 && analogRead(PYRO_JUMPER_PIN) > 50)
  {
    buzzer_chirp(16); // brief chirps
  }
  else if(recovery_stage > 0 && recovery_stage < 4 && analogRead(PYRO_JUMPER_PIN) > 50)
  {
    buzzer_chirp(2); // long chirps, system is "in flight"
  }
  else if(recovery_stage >= 0 && recovery_stage < 4)
  {
    analogWrite(BUZZER_PIN,0);
  }
  switch(recovery_stage)
  {
  case 0: // Waiting for launch
    if(altimeter->get_altitude_data_avg() > STAGE_1_ALTITUDE)
    {
      increment_recovery_stage();
    }
    break;
  case 1: // wait for apogee, Enable Drogue Pyros, wait for disable
    if(altimeter->get_altitude_data_avg_recent() - DEPLOY_BUFFER <
       altimeter->get_altitude_data_avg() &&
       altimeter->get_altitude_data_avg() >= STAGE_1_ALTITUDE &&
       !parachute_deployed[0])
    {
      pyro_enable(0);      
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 1 Parachute deployed at "
                        + String(altimeter->get_altitude_data_avg_recent()) + " meters");
      pyro_enable(1);
      sd_card->write_evr(NORMAL,RECOVERY,"Drogue 2 Parachute deployed at "
                        + String(altimeter->get_altitude_data_avg_recent()) + " meters");
    }
    else if(parachute_deployed[0] &&
            millis() >= parachute_deploy_time[0] + PYRO_PERIOD)
    {
      pyro_disable(0);
      pyro_disable(1);      
      increment_recovery_stage();
    }
    break;
  case 2: // wait, Enable Main Pyro, wait for disable
    if(altimeter->get_altitude_data_avg_recent() < MAIN_DEPLOY_ALTITUDE + DEPLOY_BUFFER &&
       !parachute_deployed[2])
    {
      pyro_enable(2);
      sd_card->write_evr(NORMAL,RECOVERY,"Main Parachute deployed at "
                        + String(altimeter->get_altitude_data_avg_recent()) + " meters");
    }
    else if(parachute_deployed[2] &&
            millis() > parachute_deploy_time[2] + PYRO_PERIOD)
    {
      pyro_disable(2);      
      increment_recovery_stage();
    }
    break;
  case 3: // coast till touchdown
    if(altimeter->get_altitude_data_avg() < 25)
    {
      if(altimeter->get_altitude_data_avg_recent() < altimeter->get_altitude_data_avg() + TOUCHDOWN_TOLERANCE &&
        altimeter->get_altitude_data_avg_recent() > altimeter->get_altitude_data_avg() - TOUCHDOWN_TOLERANCE)
      {
        sd_card->write_evr(NORMAL,RECOVERY,"Landing detected!");
        sd_card->write_evr(CONTROL,RECOVERY,"Beginning cry for help!");
        increment_recovery_stage();
      }
    }
    break;
  case 4: // touched down, dump data, cry for help
    buzzer_cry();
    if(data_dumped)
    {
      increment_recovery_stage();
    }
    break;
  case 5: // cry for help forever
    buzzer_cry();
    break;
  }
}


void Recovery::pyro_enable(uint8_t index)
{
  digitalWrite(parachute_pyro_pin[index],HIGH);
  parachute_deployed[index] = true;
  parachute_deploy_time[index] = millis();
  if(index < 2)
  { sd_card->write_evr(CONTROL,RECOVERY,"PYRO ENABLED! Drogue Parachute " +
                                         String(index)); }
  else
  { sd_card->write_evr(CONTROL,RECOVERY,"PYRO ENABLED! Main Parachute"); }
}


void Recovery::pyro_disable(uint8_t index)
{
  digitalWrite(parachute_pyro_pin[index],LOW);
  if(index < 2)
  { sd_card->write_evr(CONTROL,RECOVERY,"PYRO DISABLED! Drogue Parachute " +
                                         String(index)); }
  else
  { sd_card->write_evr(CONTROL,RECOVERY,"PYRO DISABLED! Main Parachute"); }

}


uint8_t Recovery::get_recovery_stage()
{
  return recovery_stage;
}


void Recovery::increment_recovery_stage()
{
  recovery_stage++;
  sd_card->write_evr(NORMAL,RECOVERY,
    "Recovery deployment stage incremented to stage " + String(recovery_stage));
}


void Recovery::buzzer_cry()
{
  if(buzzer_increase_freq)
  {
    buzzer_frequency += BUZZER_STEP;
    if(buzzer_frequency >= BUZZER_MAX)
    {
      buzzer_frequency = BUZZER_MAX;
      buzzer_increase_freq = false;
    }    
  }
  else
  {
    buzzer_frequency -= BUZZER_STEP;
    if(buzzer_frequency <= BUZZER_MIN)
    {
      buzzer_frequency = BUZZER_MIN;
      buzzer_increase_freq = true;
    }    
  }  
  analogWriteFrequency(BUZZER_PIN,buzzer_frequency);  
  analogWrite(BUZZER_PIN,BUZZER_PWM);
}


void Recovery::buzzer_chirp(uint8_t divider)
{
  if(millis() > last_chirp + CHIRP_PERIOD)
  {
    last_chirp = millis();
    chirp_on = true;
  }
  else if(millis() > last_chirp + CHIRP_PERIOD/divider)
  {
    chirp_on = false;
  }

  if(chirp_on)
  {
    analogWrite(BUZZER_PIN,BUZZER_PWM);
    analogWriteFrequency(BUZZER_PIN,CHIRP_FREQ);  
  }
  else
  {
    analogWrite(BUZZER_PIN,0);
  }  
}


void Recovery::data_just_dumped()
{
  data_dumped = true;
}