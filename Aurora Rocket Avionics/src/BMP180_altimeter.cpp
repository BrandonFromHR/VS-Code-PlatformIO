#include "BMP180_altimeter.h"

void BMP180_altimeter::init(SD_Storage* sd_card)
{
  if (begin())
  {
    Serial.println("BMP180 init success");
  }
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("BMP180 init FAIL\n\n");
    while(1); // Pause forever.
  }

   // Get the baseline pressure:
  baseline = get_pressure();

  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");

  this->sd_card = sd_card;

  init_datalog();

  this->sd_card->write_evr(NORMAL,FSW,"Altimeter initialization complete");
  Serial.println("Altimeter initialization complete");
}


void BMP180_altimeter::init_datalog()
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
      data_titles += "Altitude (m)";
      sd_card->write_line(datalog_filename,data_titles);
      String note;
      note += "Created File ";
      note += String(datalog_filename);
      sd_card->write_evr(NORMAL,STORAGE,note);
      break; // jump out of loop
    }
  }
}


void BMP180_altimeter::update_data()
{
  // shift all data up one index
  for(int i = ALT_SAMPLES - 1; i > 0; i--)
  {
    altitude_data[i] = altitude_data[i-1];
  }
  // get new altitude reading
  altitude_data[0] = get_altitude();

  String csv_data;
  csv_data += String(micros());
  csv_data += ",";
  csv_data += altitude_data[0];
  sd_card->write_line(datalog_filename,csv_data);
}


double BMP180_altimeter::get_altitude()
{
  double a,P;

  // Get a new pressure reading:

  P = get_pressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:

  a = altitude(P,baseline);
  return a;
}


double BMP180_altimeter::get_pressure()
{
  char status;
  double T,P;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = startPressure(0);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else
        {
          Serial.println("Error retrieving pressure measurement\n");
          sd_card->write_evr(WARNING,ALTIMETER,"Error retrieving pressure measurement");
        }
      }
      else
      {
        Serial.println("Error starting pressure measurement\n");
        sd_card->write_evr(WARNING,ALTIMETER,"Error starting pressure measurement");
      }
    }
    else
    {
      Serial.println("Error retrieving temperature measurement\n");
      sd_card->write_evr(WARNING,ALTIMETER,"Error retrieving temperature measurement");
    }
  }
  else
  {
    Serial.println("Error starting temperature measurement\n");
    sd_card->write_evr(WARNING,ALTIMETER,"Error starting temperature measurement");
  }
}


double BMP180_altimeter::get_altitude_data()
{
  return altitude_data[0];
}


double BMP180_altimeter::get_altitude_data_avg()
{
  double alt_avg = 0.0;
  for(int i = 0; i < ALT_SAMPLES; i++)
  {
    alt_avg += altitude_data[i] / double(ALT_SAMPLES);
  }
  return alt_avg;
}


double BMP180_altimeter::get_altitude_data_avg_recent()
{
  double recent_alt_avg = 0.0;
  for(int i = 0; i < RECENT_SAMPLES; i++)
  {
    recent_alt_avg += altitude_data[i] / double(RECENT_SAMPLES);
  }
  return recent_alt_avg;
}
