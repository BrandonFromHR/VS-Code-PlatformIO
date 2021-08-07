#include "PhysicsSim.h"


void PhysicsSim::init(SD_Storage* sd_card)
{
  // randomize each iteration of the sim
  randomSeed(analogRead(0));
  this->sd_card = sd_card;

  init_datalog();

  this->sd_card->write_evr(NORMAL,FSW,"Physics Sim initialization complete");
  Serial.println("Physics Sim initialization complete");
}


void PhysicsSim::init_datalog()
{
  // figure out which file name to use based on sd card contents
  for(int i = 0; i < 100; i++)
  {
    datalog_filename[4] = i/10 + '0';
    datalog_filename[5] = i%10 + '0';
    if(!sd_card->exists(datalog_filename)) // found new file name
    {
      String data_titles;
      data_titles += "Clock (us)";
      data_titles += ",";
      data_titles += "Real Altitude (m)";
      data_titles += ",";
      data_titles += "Altitude Data (m)";
      data_titles += ",";
      data_titles += "Velocity (m/s)";
      data_titles += ",";
      data_titles += "Acceleration (m/s^2)";
      sd_card->write_line(datalog_filename,data_titles);
      String note;
      note += "Created File ";
      note += String(datalog_filename);
      sd_card->write_evr(NORMAL,STORAGE,note);
      break; // jump out of loop
    }
  }
}


void PhysicsSim::sim_physics(bool parachute_deployed[3])
{
  double vel2 = velocity*velocity;
  double wind_resistance_coefficient = 0.0012;
  if(millis() < 10000) // rocket on pad
  { // do nothing
  }
  else if(millis() < 10000 + I500_BURNTIME*1000)
  { acceleration = (I500_THRUST - wind_resistance_coefficient*vel2) / mass - GRAVITY; }
  else if(real_altitude > 0)
  {
    mass = DRY_MASS;
    double wind_resistance;
    if(velocity > 0)
    { wind_resistance = -wind_resistance_coefficient*vel2; }
    else
    { wind_resistance = wind_resistance_coefficient*vel2; }
    if(parachute_deployed[0])
    {
      float chute_diameter_inches = 12;
      float chute_radius_meters = chute_diameter_inches/2.0*0.0254;
      float chute_area = 3.14159*chute_radius_meters*chute_radius_meters;
      wind_resistance += AIR_DENSITY * DROGUE_DRAG_COEFF * chute_area / 2.0 * vel2;
    }
    if(parachute_deployed[1])
    {
      float chute_diameter_inches = 12;
      float chute_radius_meters = chute_diameter_inches/2.0*0.0254;
      float chute_area = 3.14159*chute_radius_meters*chute_radius_meters;
      wind_resistance += AIR_DENSITY * DROGUE_DRAG_COEFF * chute_area / 2.0 * vel2;
    }
    if(parachute_deployed[2])
    {
      float chute_diameter_inches = 48;
      float chute_radius_meters = chute_diameter_inches/2.0*0.0254;
      float chute_area = 3.14159*chute_radius_meters*chute_radius_meters;
      wind_resistance += AIR_DENSITY * MAIN_DRAG_COEFF * chute_area / 2.0 * vel2;
    }
    acceleration = wind_resistance / mass - GRAVITY ;
  }
  else
  {
    acceleration = 0;
    velocity = 0;
  }


  for(int i = ALT_SAMPLES - 1; i > 0; i--)
  {
    altitude_data[i] = altitude_data[i-1];
  }

  float timeDelta = (float(micros()) - lastPhysCalc) / 1000000.0;
  velocity += acceleration * timeDelta;
  real_altitude += velocity * timeDelta + 0.5 * acceleration * pow(timeDelta,2);
  float noise = 1.0;
  for(int i = 0; i < 5; i++)
  {
    float randomNum = float(random(-100,50));
    noise *= randomNum;
  }
  noise /= 10000000.0;
  altitude_data[0] = real_altitude + noise;
  lastPhysCalc = micros();

  double data[5] =
  {
    micros(),
    real_altitude,
    altitude_data[0],
    velocity,
    acceleration
  };
  String csv_data;
  for(int i = 0; i < 5; i++)
  {
    csv_data += String(data[i]);
    if(i < 4)
    { csv_data += ","; }
  }
  sd_card->write_line(datalog_filename,csv_data);

  if(velocity < 0.0 && !apogee_detected)
  {
    apogee_detected = true;
    String apogee = real_altitude;
    sd_card->write_evr(NORMAL,SIM,"Apogee Detected at " + apogee + " meters.");
  }
}


double PhysicsSim::get_altitude_data()
{
  return altitude_data[0];
}


double PhysicsSim::get_altitude_data_avg()
{
  double alt_avg = 0.0;
  for(int i = 0; i < ALT_SAMPLES; i++)
  {
    alt_avg += altitude_data[i] / double(ALT_SAMPLES);
  }
  return alt_avg;
}


double PhysicsSim::get_altitude_data_avg_recent()
{
  double recent_alt_avg = 0.0;
  for(int i = 0; i < RECENT_SAMPLES; i++)
  {
    recent_alt_avg += altitude_data[i] / double(RECENT_SAMPLES);
  }
  return recent_alt_avg;
}


double PhysicsSim::get_velocity()
{
  return velocity;
}


double PhysicsSim::get_acceleration()
{
  return acceleration;
}
