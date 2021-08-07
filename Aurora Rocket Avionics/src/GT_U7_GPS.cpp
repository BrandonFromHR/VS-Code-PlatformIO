#include "GT_U7_GPS.h"

void GT_U7_GPS::init(SD_Storage* sd_card)
{
  Serial8.begin(9600);
  this->sd_card = sd_card;

  init_datalog();

  this->sd_card->write_evr(NORMAL,FSW,"GPS initialization complete");
  Serial.println("GPS initialization complete");
}


void GT_U7_GPS::init_datalog()
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
      data_titles += "Hour (UTC)";
      data_titles += ",";
      data_titles += "Minute";
      data_titles += ",";
      data_titles += "Second";
      data_titles += ",";
      data_titles += "Date (UTC)";
      data_titles += ",";
      data_titles += "Latitude";
      data_titles += ",";
      data_titles += "Longitude";
      data_titles += ",";
      data_titles += "Altitude (m AMSL)";
      data_titles += ",";
      data_titles += "Speed (m/s)";
      data_titles += ",";
      data_titles += "Course (deg)";
      data_titles += ",";
      data_titles += "Distance (m)";
      sd_card->write_line(datalog_filename,data_titles);
      String note;
      note += "Created File ";
      note += String(datalog_filename);
      sd_card->write_evr(NORMAL,STORAGE,note);
      break; // jump out of loop
    }
  }
}


void GT_U7_GPS::update_data()
{
  while(Serial8.available())
  {
    encode(Serial8.read());
  }
  if(location.isUpdated())
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    hour_data = time.hour();
    minute_data = time.minute();
    second_data = time.second();
    date_data = date.value();
    latitude_data = location.lat();
    longitude_data = location.lng();
    altitude_data = altitude.meters();
    speed_data = speed.mps();
    course_data = course.deg();
    if(foundStartLocation)
    {
      distance_data = distanceBetween(latitude_data,
          longitude_data,start_latitude,start_longitude);
    }
    else
    {
      start_latitude = latitude_data;
      start_longitude = longitude_data;
      foundStartLocation = true;
      String note;
      note += "Found starting location at Latitude: ";
      note += String(start_latitude,6);
      note += " | Longitude: ";
      note += String(start_longitude,6);
      sd_card->write_evr(NORMAL,GPS1,note);
    }

    String csv_data;
    csv_data += String(micros());
    csv_data += ",";
    csv_data += String(hour_data);
    csv_data += ",";
    csv_data += String(minute_data);
    csv_data += ",";
    csv_data += String(second_data);
    csv_data += ",";
    csv_data += String(date_data);
    csv_data += ",";
    csv_data += String(latitude_data,6);
    csv_data += ",";
    csv_data += String(longitude_data,6);
    csv_data += ",";
    csv_data += String(altitude_data);
    csv_data += ",";
    csv_data += String(speed_data);
    csv_data += ",";
    csv_data += String(course_data);
    csv_data += ",";
    csv_data += String(distance_data);
    sd_card->write_line(datalog_filename,csv_data);
  }
}


double GT_U7_GPS::get_latitude()
{
  return latitude_data;
}


double GT_U7_GPS::get_longitude()
{
  return longitude_data;
}


uint8_t GT_U7_GPS::get_hour_data()
{
  return hour_data;
}


uint8_t GT_U7_GPS::get_minute_data()
{
  return minute_data;
}


uint8_t GT_U7_GPS::get_second_data()
{
  return second_data;
}


uint32_t GT_U7_GPS::get_date_data()
{
  return date_data;
}
