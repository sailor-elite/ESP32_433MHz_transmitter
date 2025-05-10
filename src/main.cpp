#include <Arduino.h>
#include "BluetoothSerial.h"
#include <ELECHOUSE_CC1101_SRC_DRV.h>

//SPI Pins
#define SCK_PIN 18
#define SS_PIN 5
#define MISO_PIN 19
#define MOSI_PIN 23


// BT initialisation
BluetoothSerial SerialBT;

// buffer for incoming strings
const int n = 61;
byte buffer[n] = "";


// default CC1101 settings
float MHZ_def = 433.92;
int Modulation_def = 2;
int CCMode_def = 1;
int PA_def = 0;
int SyncMode_def = 3;
int CRC_def = 3;
float DRate_def = 2.4;


// only for debugging
bool debugMode = false;


void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32PID");
  ELECHOUSE_cc1101.setSpiPin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setCCMode(CCMode_def);
  ELECHOUSE_cc1101.setModulation(Modulation_def);
  ELECHOUSE_cc1101.setMHZ(MHZ_def);
  ELECHOUSE_cc1101.setPA(PA_def);
  ELECHOUSE_cc1101.setSyncMode(SyncMode_def);
  ELECHOUSE_cc1101.setCrc(CRC_def);
  ELECHOUSE_cc1101.setDRate(DRate_def);
}


// fuction to send chars throug CC1101 - one char at a time
void sendChar(int Data)
{
  buffer[Data] = '\0';
  //only for debugging
  if (debugMode == true)
  {
    Serial.println(Data);
    Serial.println((char*)buffer);
    Serial.print("Buffer: ");
  }
  for (int i = 0; i<Data; i++)
  {
    byte temp_byte[1] = {buffer[i]};
    ELECHOUSE_cc1101.SendData(temp_byte, 1, 100);
    //only for debugging
    if (debugMode == true)
    {
    Serial.println(buffer[i]);
    }
  }
}

void loop() {
  if (SerialBT.available()){
    int Data = SerialBT.readBytesUntil('\n', buffer, n - 1);
    sendChar(Data);
    }
}
