#include "SD_Storage.h"


void SD_Storage::init(String name, uint8_t cs_pin, GT_U7_GPS* gps)
{
  Serial.print("Searching for ");
  Serial.print(name);
  Serial.print(" card...");
  // see if the card is present and can be initialized:
  if (!begin(cs_pin)) {
    Serial.println(" FAILED, is card present?");
    while (1)
    {
        for(int i = 0; i < 10; i++)
        {
          analogWriteFrequency(5,400);
          analogWrite(5,100);
          delay(40);
          analogWrite(5,0);
          delay(10);
        }
        for(int i = 0; i < 10; i++)
        {
          analogWriteFrequency(5,300);
          analogWrite(5,100);
          delay(40);
          analogWrite(5,0);
          delay(10);
        }
        for(int i = 0; i < 10; i++)
        {
          analogWriteFrequency(5,200);
          analogWrite(5,100);
          delay(40);
          analogWrite(5,0);
          delay(10);
        }
        for(int i = 0; i < 10; i++)
        {
          analogWriteFrequency(5,100);
          analogWrite(5,100);
          delay(40);
          analogWrite(5,0);
          delay(10);
        }
        delay(5000);
    }
  }
  Serial.println(" card found.");

  String note;
  note += name;
  note += " card initialization complete.";

  if(name == "BISD") // change to XTSD after XTSD is implemented
  {
    init_evr_file();
    write_evr(NORMAL,STORAGE,note);
  }
  this->gps = gps;

  Serial.println(note);
}


void SD_Storage::init_evr_file()
{
  // figure out which file name to use based on sd card contents
  for(int i = 0; i < 100; i++)
  {
    evr_filename[4] = i/100 + '0';
    evr_filename[5] = i/10 + '0';
    evr_filename[6] = i%10 + '0';
    if(!exists(evr_filename)) // found new file name
    {
      String data_titles;
      data_titles += "FSW Clock (us)";
      data_titles += ",";
      data_titles += "Second (UTC)";
      data_titles += ",";
      data_titles += "Minute (UTC)";
      data_titles += ",";
      data_titles += "Hour (UTC)";
      data_titles += ",";
      data_titles += "Date (DDMMYY) (UTC)";
      data_titles += ",";
      data_titles += "Type";
      data_titles += ",";
      data_titles += "Subsystem";
      data_titles += ",";
      data_titles += "Note";
      write_line(evr_filename,data_titles);
      String note;
      note += "Created File ";
      note += String(evr_filename);
      write_evr(NORMAL,STORAGE,note);
      break; // jump out of loop
    }
  }
}


void SD_Storage::write_line(char filename[13], String dataString)
{
  File dataFile = open(filename, FILE_WRITE);
  if (dataFile) // file opened
  {
    dataFile.println(dataString);
    dataFile.close();
  }
  else // file didn't open
  {
    String file_name = filename;
    String text = "Error opening " + file_name;
    Serial.println(text);
    write_evr(WARNING,STORAGE,text);
  }
}


void SD_Storage::write_evr(String type, String subsystem, String note)
{
  File dataFile = open(evr_filename, FILE_WRITE);
  if (dataFile) // file opened
  {
    String evr;
    evr += micros();
    evr += ",";
    evr += gps->get_second_data();
    evr += ",";
    evr += gps->get_minute_data();
    evr += ",";
    evr += gps->get_hour_data();
    evr += ",";
    evr += gps->get_date_data();
    evr += ",";
    evr += type;
    evr += ",";
    evr += subsystem;
    evr += ",";
    evr += note;
    dataFile.println(evr);
    dataFile.close();    
    Serial.println(evr);
  }
  else // file didn't open
  {
    String file_name = String(evr_filename);
    String text = "Error opening " + file_name;
    Serial.println(text);
  }
}


void SD_Storage::read(char filename[13])
{
  File dataFile = open(filename);
  if (dataFile)
  {
    // read from the file until there's nothing else in it:
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    // close the file:
    dataFile.close();
  } else {
    // if the file didn't open, print an error:
    String file_name = filename;
    String text = "Error opening " + file_name;
    Serial.println(text);
    write_evr(WARNING,STORAGE,text);
  }
}


void SD_Storage::remove_all_files()
{
// USE THIS FUNCTION WISELY!
// REMOVES ALL FILES FROM CARD!!!

  File root = open("/");
  while(true)
  {
    File nextFile = root.openNextFile();
    if(!nextFile) // no more files exist
    { break; }
    
    const char* fileName = nextFile.name();
    nextFile.close();
    remove(fileName);
  }
}