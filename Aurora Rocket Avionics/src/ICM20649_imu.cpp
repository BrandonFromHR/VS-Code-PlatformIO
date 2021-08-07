#include "ICM20649_imu.h"

#define FLOAT_PRECISION 5

void ICM20649_imu::init(SD_Storage* sd_card)
{
  if(!begin_SPI(ICM_CS))
  {
    Serial.println("Failed to find ICM20649 chip");
    sd_card->write_evr(CRITICAL,IMU,"Failed to find ICM20649 chip");
    while(1)
    { delay(10); }
  }
  Serial.println("Found ICM20649");
  sd_card->write_evr(NORMAL,IMU,"Found ICM20649");
  setAccelRange(ICM20649_ACCEL_RANGE_30_G);
  Serial.print("Accelerometer range set to: ");
  switch(getAccelRange()) {
  case ICM20649_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    sd_card->write_evr(NORMAL,IMU,"Accelerometer range set to: +-4G");
    break;
  case ICM20649_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    sd_card->write_evr(NORMAL,IMU,"Accelerometer range set to: +-8G");
    break;
  case ICM20649_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    sd_card->write_evr(NORMAL,IMU,"Accelerometer range set to: +-16G");
    break;
  case ICM20649_ACCEL_RANGE_30_G:
    Serial.println("+-30G");
    sd_card->write_evr(NORMAL,IMU,"Accelerometer range set to: +-30G");
    break;
  }

  setGyroRange(ICM20649_GYRO_RANGE_4000_DPS);
  Serial.print("Gyro range set to: ");
  switch (getGyroRange()) {
  case ICM20649_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    sd_card->write_evr(NORMAL,IMU,"Gyroscope range set to: +-500 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    sd_card->write_evr(NORMAL,IMU,"Gyroscope range set to: +-1000 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    sd_card->write_evr(NORMAL,IMU,"Gyroscope range set to: +-2000 degrees/s");
    break;
  case ICM20649_GYRO_RANGE_4000_DPS:
    Serial.println("4000 degrees/s");
    sd_card->write_evr(NORMAL,IMU,"Gyroscope range set to: +-4000 degrees/s");
    break;
  }

  setAccelRateDivisor(0);
  uint16_t accel_divisor = getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);
  sd_card->write_evr(NORMAL,IMU,"Accelerometer rate is approximately "
                                + String(accel_rate));

  setGyroRateDivisor(0);
  uint8_t gyro_divisor = getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);
  Serial.println();
  sd_card->write_evr(NORMAL,IMU,"Gyroscope rate is approximately "
                                + String(gyro_rate));

  this->sd_card = sd_card;

  init_datalog();

  this->sd_card->write_evr(NORMAL,FSW,"IMU initialization complete");
  Serial.println("IMU initialization complete");
}


void ICM20649_imu::init_datalog()
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
      data_titles += "X-Axis Acceleration (m/s^2)";
      data_titles += ",";
      data_titles += "Y-Axis Acceleration (m/s^2)";
      data_titles += ",";
      data_titles += "Z-Axis Acceleration (m/s^2)";
      data_titles += ",";
      data_titles += "X-Axis Gyroscope (rad/s)";
      data_titles += ",";
      data_titles += "Y-Axis Gyroscope (rad/s)";
      data_titles += ",";
      data_titles += "Z-Axis Gyroscope (rad/s)";
      data_titles += ",";
      data_titles += "Temperature (Celsius)";
      sd_card->write_line(datalog_filename,data_titles);
      String note;
      note += "Created File ";
      note += String(datalog_filename);
      sd_card->write_evr(NORMAL,STORAGE,note);
      break; // jump out of loop
    }
  }
}


void ICM20649_imu::update_data()
{
  increment_index();
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  getEvent(&accel, &gyro, &temp);
  accel_data[0][index] = accel.acceleration.x;
  accel_data[1][index] = accel.acceleration.y;
  accel_data[2][index] = accel.acceleration.z;
  gyro_data[0][index] = gyro.gyro.x;
  gyro_data[1][index] = gyro.gyro.y;
  gyro_data[2][index] = gyro.gyro.z;
  temp_data[index] = temp.temperature;
  double data[8] =
  {
    micros(),
    accel.acceleration.x,
    accel.acceleration.y,
    accel.acceleration.z,
    gyro.gyro.x,
    gyro.gyro.y,
    gyro.gyro.z,
    temp.temperature
  };
  String csv_data;
  for(int i = 0; i < 8; i++)
  {
    csv_data += String(data[i],4);
    if(i < 7)
    { csv_data += ","; }
  }
  sd_card->write_line(datalog_filename,csv_data);
  if(get_accel_magnitude() > 196)
  {
    sd_card->write_evr(WARNING,IMU,"ACCELERATION > 20g DETECTED!");
  }
}


void ICM20649_imu::increment_index()
{
  index++;
  if(index >= DATA_POINTS)
  { index = 0; }
}


float ICM20649_imu::get_last_accel_data(uint8_t axis_index)
{ return accel_data[axis_index][index]; }


float ICM20649_imu::get_avg_accel(uint8_t axis_index)
{
  float avg_accel = 0.0;
  for(int i = 0; i < DATA_POINTS; i++)
  { avg_accel += accel_data[axis_index][i] / float(DATA_POINTS); }
  return avg_accel;
}


float ICM20649_imu::get_last_gyro_data(uint8_t axis_index)
{ return gyro_data[axis_index][index]; }


float ICM20649_imu::get_avg_gyro(uint8_t axis_index)
{
  float avg_gyro = 0.0;
  for(int i = 0; i < DATA_POINTS; i++)
  { avg_gyro += gyro_data[axis_index][i] / float(DATA_POINTS); }
  return avg_gyro;
}


float ICM20649_imu::get_last_temp_data()
{ return temp_data[index]; }


float ICM20649_imu::get_avg_temp()
{
  float avg_temp = 0.0;
  for(int i = 0; i < DATA_POINTS; i++)
  { avg_temp += temp_data[i] / float(DATA_POINTS); }
  return avg_temp;
}


float ICM20649_imu::get_accel_magnitude()
{
  float acc_x_sqrd = pow(accel_data[0][index],2);
  float acc_y_sqrd = pow(accel_data[1][index],2);
  float acc_z_sqrd = pow(accel_data[2][index],2);
  return sqrt(acc_x_sqrd + acc_y_sqrd + acc_z_sqrd);
}


float ICM20649_imu::get_accel_magnitude(uint8_t specific_index)
{
  float acc_x_sqrd = pow(accel_data[0][specific_index],2);
  float acc_y_sqrd = pow(accel_data[1][specific_index],2);
  float acc_z_sqrd = pow(accel_data[2][specific_index],2);
  return sqrt(acc_x_sqrd + acc_y_sqrd + acc_z_sqrd);
}


float ICM20649_imu::get_avg_accel_magnitude()
{
  float avg_accel = 0.0;
  for(int i = 0; i < DATA_POINTS; i++)
  { avg_accel += get_accel_magnitude(i) / float(DATA_POINTS); }
  return avg_accel;
}


float ICM20649_imu::get_gyro_magnitude()
{
  float gyro_x_sqrd = pow(gyro_data[0][index],2);
  float gyro_y_sqrd = pow(gyro_data[1][index],2);
  float gyro_z_sqrd = pow(gyro_data[2][index],2);
  return sqrt(gyro_x_sqrd + gyro_y_sqrd + gyro_z_sqrd);
}


float ICM20649_imu::get_gyro_magnitude(uint8_t specific_index)
{
  float gyro_x_sqrd = pow(gyro_data[0][specific_index],2);
  float gyro_y_sqrd = pow(gyro_data[1][specific_index],2);
  float gyro_z_sqrd = pow(gyro_data[2][specific_index],2);
  return sqrt(gyro_x_sqrd + gyro_y_sqrd + gyro_z_sqrd);
}


float ICM20649_imu::get_avg_gyro_magnitude()
{
  float avg_gyro = 0.0;
  for(int i = 0; i < DATA_POINTS; i++)
  { avg_gyro += get_gyro_magnitude(i) / float(DATA_POINTS); }
  return avg_gyro;
}
