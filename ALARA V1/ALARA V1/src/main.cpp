#include "extendedIO/extendedIO.h"
#include "ms5607/ms5607.h"
#include "kx134_1211/kx134_1211.h"
#define BUZZER_PIN 16








/*


#define NRF_CS 28
#define NRF_CE 27
#define NRF_IRQ 65

#define LORA_CS 24
#define LORA_RST 25
#define LORA_INT 26
 
#define TEST_PIN BUZZER_PIN

void setup()
{  
  pinModeExtended(BUZZER_PIN,OUTPUT);  
  analogWrite(BUZZER_PIN,0);

  pinModeExtended(TEST_PIN,OUTPUT);

  Serial.begin(9600);
  delay(1000);
}

void loop()
{

}


*/


















/*
// KX134-1211 basic code, gets the module to give out it's WHO_AM_I ID
// Should be equal to 0x46


kx134_1211 acc;

void setup()
{  
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  pinMode(KX134_CS_PIN,OUTPUT);
  SPI.begin();

  Serial.begin(9600);
  delay(5000);
  //Serial.println("Starting Init for ACC");
  //acc.init_asyncronous();
  //Serial.println("Ending Init for ACC");
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, KX134_SPI_MODE));
  digitalWrite(KX134_CS_PIN,0);
  Serial.println(SPI.transfer(KX134_1211_WHO_AM_I|(1<<7)),HEX);
  Serial.println(SPI.transfer(0x00),HEX);
  digitalWrite(KX134_CS_PIN,1);
  SPI.endTransaction();
  delay(10000);
}

void loop()
{

}
*/

















/*
#include "BMI085/BMI088.h"

Bmi088Accel accel(SPI);
Bmi088Gyro gyro(SPI);

void setup() 
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  int status;
  // USB Serial to print data
  Serial.begin(115200);
  while(!Serial) {}
  // start the sensors
  status = accel.begin();
  if (status < 0) {
    Serial.println("Accel Initialization Error");
    Serial.println(status);
    while (1) {}
  }
  status = gyro.begin();
  if (status < 0) {
    Serial.println("Gyro Initialization Error");
    Serial.println(status);
    while (1) {}
  }
}

void loop() 
{
  // read the accel
  accel.readSensor();
  // read the gyro
  gyro.readSensor();
  // print the data
  Serial.print(accel.getAccelX_mss());
  Serial.print("\t");
  Serial.print(accel.getAccelY_mss());
  Serial.print("\t");
  Serial.print(accel.getAccelZ_mss());
  Serial.print("\t");
  Serial.print(gyro.getGyroX_rads());
  Serial.print("\t");
  Serial.print(gyro.getGyroY_rads());
  Serial.print("\t");
  Serial.print(gyro.getGyroZ_rads());
  Serial.print("\t");
  Serial.print(accel.getTemperature_C());
  Serial.print("\n");
  // delay to help with printing
  delay(20);
}

*/












/*
  SD card test 
   
 This example shows how use the utility libraries on which the'
 SD library is based in order to get info about your SD card.
 Very useful for testing a card when you're not sure whether its working or not.
 	
 The circuit:
  * SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila, pin 7 on Teensy with audio board
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila, pin 14 on Teensy with audio board
 ** CS - depends on your SD card shield or module - pin 10 on Teensy with audio board
 		Pin 4 used here for consistency with other Arduino examples

 
 created  28 Mar 2011
 by Limor Fried 
 modified 9 Apr 2012
 by Tom Igoe
 */
 // include the SD library:

 /*
#include <SD.h>
#include <SPI.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;

void setup()
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }


  Serial.print("\nInitializing SD card...");

  pinMode(chipSelect,OUTPUT);

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  if (volumesize < 8388608ul) {
    Serial.print("Volume size (bytes): ");
    Serial.println(volumesize * 512);        // SD card blocks are always 512 bytes
  }
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 2;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  //Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  //root.openRoot(volume);
  
  // list all files in the card with date and size
  //root.ls(LS_R | LS_DATE | LS_SIZE);
}


void loop() {
  
}

*/
















/*

#define ADDR_PIN1 ED22
#define ADDR_PIN2 ED23
#define ADDR_PIN3 ED24
#define ADDR_PIN4 ED25

uint8_t device_address = 0;

void setup()
{  
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  delay(1000);

  Serial.begin(9600);

  delay(1000);
  Serial.println("Serial started");

  delay(1000);

  pinMode(ADDR_PIN1,INPUT);
  pinMode(ADDR_PIN2,INPUT);
  pinMode(ADDR_PIN3,INPUT);
  pinMode(ADDR_PIN4,INPUT);

  if(digitalRead(ADDR_PIN1) == 1)
  {
    device_address |= (1<<0);
  }
  if(digitalRead(ADDR_PIN2) == 1)
  {
    device_address |= (1<<1);
  }
  if(digitalRead(ADDR_PIN3) == 1)
  {
    device_address |= (1<<2);
  }
  if(digitalRead(ADDR_PIN4) == 1)
  {
    device_address |= (1<<3);
  }
  Serial.print("Device address = ");
  Serial.println(device_address);

  Serial.print("Reading ");
  Serial.print(digitalRead(ADDR_PIN4));
  Serial.print(digitalRead(ADDR_PIN3));
  Serial.print(digitalRead(ADDR_PIN2));
  Serial.print(digitalRead(ADDR_PIN1));

}


void loop()
{

}

*/











/*

#include <LittleFS.h>

// Flash chip on Teensy Audio Shield or Prop Shield
LittleFS_SPIFlash myfs;

#define MUX_S0 43
#define MUX_S1 46
#define MUX_S2 44
#define MUX_OE 45

void printDirectory(FS &fs);
void printDirectory(File dir, int numSpaces);
void printSpaces(int num);

void setup()
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  pinMode(MUX_S0,OUTPUT);
  pinMode(MUX_S1,OUTPUT);
  pinMode(MUX_S2,OUTPUT);
  pinMode(MUX_OE,OUTPUT);
  
  digitalWrite(MUX_S0,LOW);
  digitalWrite(MUX_S1,LOW);
  digitalWrite(MUX_S2,LOW);
  digitalWrite(MUX_OE,HIGH);
  
  SPI.begin();

  delay(2000);
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  { }; // wait for Arduino Serial Monitor

  Serial.println("Initializing Flash Chip");
  
  if (!myfs.begin(MUX_OE,SPI)) {
    Serial.println("initialization failed!");
    while(1);
  }

  Serial.print("Space Used = ");
  Serial.println(myfs.usedSize());
  Serial.print("Filesystem Size = ");
  Serial.println(myfs.totalSize());

  if(!myfs.exists("flash2.cpp"))
  {
    // open the file.
    File dataFile = myfs.open("flash2.cpp", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println("data,1,2,3,apple,banana");
      dataFile.close();      
      Serial.println("wrote to flash2.cpp");
    } else {
      // if the file isn't open, pop up an error:
      Serial.println("error opening flash2.cpp");
    }
  }

  printDirectory(myfs);
}


void loop()
{
  
}


void printDirectory(FS &fs) {
  Serial.println("Directory\n---------");
  printDirectory(fs.open("/"), 0);
  Serial.println();
}

void printDirectory(File dir, int numSpaces) {
   while(true) {
     File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     printSpaces(numSpaces);
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numSpaces+2);
     } else {
       // files have sizes, directories do not
       printSpaces(36 - numSpaces - strlen(entry.name()));
       Serial.print("  ");
       Serial.print(entry.size(), DEC);
       Serial.println();
     }
     entry.close();
   }
}

void printSpaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

*/















/*
MS5607 bps;

void setup()
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);
  Serial.begin(9600);
  SPI.begin();
  delay(5000);
  Serial.println("Setup Delay Done");
  pinMode(ED19,OUTPUT);
  
  //digitalWrite(ED19,0);
  bps.init(BPS_CS_PIN,OSR_256);    
  bps.print_all();
  //digitalWrite(ED19,1);

  Serial.println("Setup Complete");
}

void loop()
{
  Serial.print("Altitude = ");
  digitalWrite(ED19,0);
  bps.update();
  Serial.print(bps.get_altitude());
  digitalWrite(ED19,1);
  Serial.println("");
  delay(1000);
}
*/



















/*
#include <Wire.h> //Needed for I2C to GPS

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_u-blox_GNSS
SFE_UBLOX_GPS myGPS;
long lastTime = 0; //Tracks the passing of 2000ms (2 seconds)

void setup()
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);

  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println("SparkFun Ublox Example");

  Wire.begin();
  if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }
}

void loop()
{
  //Query module only every second. Doing it more often will just cause I2C traffic.
  //The module only responds when a new position is available
  if (millis() - lastTime > 1000)
  {
    uint64_t counter = micros();
    lastTime = millis(); //Update the timer
    
    long latitude = myGPS.getLatitude(250);
    Serial.print(F("Lat: "));
    Serial.print(latitude);

    long longitude = myGPS.getLongitude(250);
    Serial.print(F(" Long: "));
    Serial.print(longitude);

    long altitudeMSL = myGPS.getAltitudeMSL(250);
    Serial.print(F(" AltMSL: "));
    Serial.print(altitudeMSL);
    Serial.print(F(" (mm)"));

    Serial.print(" micros: ");
    Serial.print(micros() - counter);

    Serial.println();

  }
}
*/





















#define RGB1_RED_PIN 71
#define RGB1_GRN_PIN 70
#define RGB1_BLU_PIN 69

#define HPO_DIG1 54
#define HPO_DIG2 51
#define HPO_DIG3 52
#define HPO_DIG4 53
#define HPO_DIG5 55
#define HPO_DIG6 ED30
#define HPO_DIG7 48
#define HPO_DIG8 47
#define HPO_DIG9 ED29
#define HPO_DIG10 ED28

#define HPO_PWM1 5
#define HPO_PWM2 6
#define HPO_PWM3 8
#define HPO_PWM4 7
#define HPO_PWM5 2
#define HPO_PWM6 10
#define HPO_PWM7 9
#define HPO_PWM8 22
#define HPO_PWM9 30
#define HPO_PWM10 29

#define HPO_VD1 A7
#define HPO_VD2 A6
#define HPO_VD3 A0
#define HPO_VD4 A19
#define HPO_VD5 A18
#define HPO_VD6 A17
#define HPO_VD7 A16
#define HPO_VD8 A9
#define HPO_VD9 A1
#define HPO_VD10 A24

void hpo_on(uint8_t output_num);
void hpo_off(uint8_t output_num);
void hpo_switch(uint8_t output_num, bool state);
uint32_t hpo_read(uint8_t output_num);
void RGB_toggle(bool red_on, bool grn_on, bool blu_on);
void tune(uint16_t frequency, uint16_t sound_period, uint16_t delay_period);

void setup()
{
  pinMode(BUZZER_PIN,1);  
  analogWrite(BUZZER_PIN,0);

  pinModeExtended(RGB1_RED_PIN,1);
  pinModeExtended(RGB1_BLU_PIN,1);
  pinModeExtended(RGB1_GRN_PIN,1);

  pinMode(HPO_DIG1,1);
  pinMode(HPO_PWM1,1);
  pinMode(HPO_DIG2,1);
  pinMode(HPO_PWM2,1);
  pinMode(HPO_DIG3,1);
  pinMode(HPO_PWM3,1);
  pinMode(HPO_DIG4,1);
  pinMode(HPO_PWM4,1);
  pinMode(HPO_DIG5,1);
  pinMode(HPO_PWM5,1);
  pinMode(HPO_DIG6,1);
  pinMode(HPO_PWM6,1);
  pinMode(HPO_DIG7,1);
  pinMode(HPO_PWM7,1);
  pinMode(HPO_DIG8,1);
  pinMode(HPO_PWM8,1);
  pinMode(HPO_DIG9,1);
  pinMode(HPO_PWM9,1);
  pinMode(HPO_DIG10,1);
  pinMode(HPO_PWM10,1);

  pinMode(ED15,1);

  analogReadResolution(13);
}

void loop()
{
  /*
  RGB_toggle(1,0,0); // Red
  delay(100);
  RGB_toggle(0,1,0); // Green
  delay(100);
  RGB_toggle(0,0,1); // Blue
  delay(100);

  RGB_toggle(1,1,0); // Yellow
  delay(100);
  RGB_toggle(0,1,1); // Turquoise 
  delay(100);
  RGB_toggle(1,0,1); // Purple
  delay(100);
  
  RGB_toggle(1,1,1); // White
  delay(100);
*/

  tune(2000,100,900);
}

void hpo_on(uint8_t output_num)
{
  hpo_switch(output_num,1);
}

void hpo_off(uint8_t output_num)
{
  hpo_switch(output_num,0);
}

void hpo_switch(uint8_t output_num, bool state)
{
  switch(output_num)
  {
  case 1:
    digitalWrite(HPO_DIG1,state);
    digitalWrite(HPO_PWM1,state);
    break;
  case 2:
    digitalWrite(HPO_DIG2,state);
    digitalWrite(HPO_PWM2,state);
    break;
  case 3:
    digitalWrite(HPO_DIG3,state);
    digitalWrite(HPO_PWM3,state);
    break;
  case 4:
    digitalWrite(HPO_DIG4,state);
    digitalWrite(HPO_PWM4,state);
    break;
  case 5:
    digitalWrite(HPO_DIG5,state);
    digitalWrite(HPO_PWM5,state);
    break;
  case 6:
    digitalWrite(HPO_DIG6,state);
    digitalWrite(HPO_PWM6,state);
    break;
  case 7:
    digitalWrite(HPO_DIG7,state);
    digitalWrite(HPO_PWM7,state);
    break;
  case 8:
    digitalWrite(HPO_DIG8,state);
    digitalWrite(HPO_PWM8,state);
    break;
  case 9:
    digitalWrite(HPO_DIG9,state);
    digitalWrite(HPO_PWM9,state);
    break;
  case 10:
    digitalWrite(HPO_DIG10,state);
    digitalWrite(HPO_PWM10,state);
    break;
  }
}

uint32_t hpo_read(uint8_t output_num)
{
  switch(output_num)
  {
  case 1:
      return analogRead(HPO_VD1);
    break;
  case 2:
      return analogRead(HPO_VD2);
    break;
  case 3:
      return analogRead(HPO_VD3);
    break;
  case 4:
      return analogRead(HPO_VD4);
    break;
  case 5:
      return analogRead(HPO_VD5);
    break;
  case 6:
      return analogRead(HPO_VD6);
    break;
  case 7:
      return analogRead(HPO_VD7);
    break;
  case 8:
      return analogRead(HPO_VD8);
    break;
  case 9:
      return analogRead(HPO_VD9);
    break;
  case 10:
      return analogRead(HPO_VD10);
    break;
  }
  return 0;
}

void RGB_toggle(bool red_on, bool grn_on, bool blu_on)
{
  digitalWriteExtended(RGB1_RED_PIN,red_on);
  digitalWriteExtended(RGB1_GRN_PIN,grn_on);
  digitalWriteExtended(RGB1_BLU_PIN,blu_on);
}

void tune(uint16_t frequency, uint16_t sound_period, uint16_t delay_period)
{
    analogWriteFrequency(BUZZER_PIN,frequency);
    analogWrite(BUZZER_PIN,150);
    delay(sound_period);
    analogWrite(BUZZER_PIN,0);
    delay(delay_period);
}