#include <Arduino.h>
#include <LittleFS.h>

int8_t file_find(char* file_name);
void file_delete(char* file_name);
void file_write_line(char* file_name, char* data);
void file_read(char* file_name);

LittleFS_QSPIFlash my_fs;

void setup()
{
  delay(5000);
  Serial.begin(1000000);
  if(my_fs.begin())
  {
    Serial.println("my_fs initialized successfully");
  }
  else
  {
    Serial.println("my_fs FAILED to initialize");
    while(1){} // stop program from running
  }

  Serial.print("my_fs size = ");
  Serial.print(my_fs.totalSize());
  Serial.println("");

  file_find("EVR_000.csv");
  file_find("EVR_001.csv");

  file_find("ALT_000.csv");
  file_find("ALT_001.csv");

  file_find("IMU_000.csv");
  file_find("IMU_001.csv");

  file_find("GPS_000.csv");
  file_find("GPS_001.csv");

  file_find("SIM_000.csv");
  file_find("SIM_001.csv");

  file_delete("ALT_000.csv");

  file_write_line("ALT_001.csv","ALT,DUMMY_WRITE,0.01");

  file_read("ALT_001.csv");
}

void loop()
{

}

int8_t file_find(char* file_name)
{
  if(my_fs.exists(file_name))
  {
    Serial.print(file_name);
    Serial.println(" - exists");
    return 1;
  }
  else
  {
    Serial.print(file_name);
    Serial.println(" - does not exist");
    return 0;
  }
}

void file_delete(char* file_name)
{
  if(file_find(file_name))
  {
    Serial.print("Deleting ");
    Serial.println(file_name);
    my_fs.remove(file_name);
  }
}

void file_write_line(char* file_name, char* data)
{
  if(!file_find(file_name))
  {
    Serial.print("Creating file ");
    Serial.println(file_name);
  }
  Serial.print("Opening file ");
  Serial.println(file_name);
  if(File my_file = my_fs.open(file_name,FILE_WRITE))
  {
    Serial.print("Writing to file ");
    Serial.println(file_name);
    my_file.println(data);
    my_file.close();
  }
  else
  {
    Serial.print("Could not open file ");
    Serial.println(file_name);
  }
}

void file_read(char* file_name)
{
  if(file_find(file_name))
  {
    File my_file = my_fs.open(file_name);
    while (my_file.available())
    {
      Serial.write(my_file.read());
    }
  }
}