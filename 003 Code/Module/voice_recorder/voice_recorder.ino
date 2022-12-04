/*
   File Name: spy-recorder.ino
   Created on: 7-Jan-2021
   Author: Noyel Seth (noyelseth@gmail.com)
*/
/*
   Hardware Pinout Connection
   Arduino Nano        SD Pin
        5v ------------ VCC
        GND ----------- GND
        D10 ----------- CS
        D11 ----------- MOSI
        D12 ----------- MISO
        D13 ----------- SCK
  ________________________________________
   Arduino Nano         MAX9814
        3.3v ----------- VDD
        GND ------------ GND
        A0 -------------  Out
  ________________________________________
   Arduino Nano D2 pin user for Led to notify that record is in process.
*/
/*
   use Link: https://www.arduino.cc/reference/en/libraries/tmrpcm/ TMRpcm library for recording audio using MAX9814
   Recording a WAV file to an SD card is an advanced feature of the TMRpcm library so you must edit the library configuration file in order to use it.
   It simply searches the file "pcmConfig.h" using File Explorer and disables a few lines of code (then saves it).
    1. On Uno or non-mega boards uncomment the line #define buffSize 128
    2. Also uncomment #define ENABLE_RECORDING and #define BLOCK_COUNT 10000UL
*/
#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
TMRpcm audio;
int file_number = 0;
char filePrefixname[50] = "gain";
char exten[10] = ".wav";
const int recordLed = 2;
const int mic_pin = A0;
const int sample_rate = 16000;
#define SD_CSPin 10
// delay function for with serial log.
void wait_min(int mins) {
  int count = 0;
  int secs = mins * 60;
  while (1) {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs) {
      count = 0;
      break;
    }
  }
  Serial.println();
  return ;
}
void wait_secs(int sec) {
  int count = 0;
  int secs = sec;
  while (1) {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs) {
      count = 0;
      break;
    }
  }
  Serial.println();
  return ;
}
void setup() {
  // put your setup code here, to run once:
  //initialises the serial connection between the arduino and any connected serial device(e.g. computer, phone, raspberry pi...)
  Serial.begin(9600);
  //Sets up the pins
  pinMode(mic_pin, INPUT);
  pinMode(recordLed, OUTPUT);
  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
  }
  while (!SD.begin(SD_CSPin)) {
    Serial.print(".");
    delay(500);
  }
  audio.CSPin = SD_CSPin;
}
void loop() {
  Serial.println("####################################################################################");
  char fileSlNum[20] = "";
  itoa(file_number, fileSlNum, 10);
  char file_name[50] = "";
  strcat(file_name, filePrefixname);
  strcat(file_name, fileSlNum);
  strcat(file_name, exten);
  Serial.print("New File Name: ");
  Serial.println(file_name);
  digitalWrite(recordLed, HIGH);
  audio.startRecording(file_name, sample_rate, mic_pin);
  Serial.println("startRecording ");
  // record audio for 2mins. means , in this loop process record 2mins of audio.
  // if you need more time duration recording audio then
  // pass higher value into the wait_min(int mins) function.
  wait_secs(20);
  digitalWrite(recordLed, LOW);
  audio.stopRecording(file_name);
  Serial.println("stopRecording");
  file_number++;
  Serial.println("####################################################################################");
}
