#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define TEMP_18B20 2

OneWire oneWire(TEMP_18B20);
DallasTemperature sensors(&oneWire);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
float x = 0;
float y = 0;
float z = 0;
int counter = 0;
int loop_c = 0;
float hz_x = 0;
float hz_y = 0;
float hz_z = 0;

// for sensor dat
SoftwareSerial bluetooth1(8, 7);
// for ecg dat
SoftwareSerial bluetooth2(13, 12);

float get_temp_data();
void send_data_using_bluetooth();

void setup() {
  Serial.begin(9600);
  bluetooth1.begin(9600);
  bluetooth2.begin(9600);
  accel.setDataRate(ADXL345_DATARATE_800_HZ);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  
  if(!accel.begin())
  {
  /* There was a problem detecting the ADXL345 ... check your connections */ 
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!"); while(1);
  }
}

void loop() {
  
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    //Serial.println('!');
  }
  else{
   // Sending the ecg data
   int ecg_dat = analogRead(A0);
   Serial.println(ecg_dat);
   send_ecg_using_bluetooth(ecg_dat);
   delay(1);
  }


  if(counter % 25 == 0){
    sensors_event_t accelEvent;
    accel.getEvent(&accelEvent);

    x = accelEvent.acceleration.x;
    y = accelEvent.acceleration.y;
    z = accelEvent.acceleration.z;

    hz_x += x;
    hz_y += y;
    hz_z += z;

    loop_c++; 
  }

  // Sending the data, period is 5sec.
  
  if(loop_c == 32){

    // Send the temperature data. data is divided by int, float data
    float temp_data;
    temp_data = get_temp_data();
    send_data_using_bluetooth((int)temp_data);
    send_data_using_bluetooth(((temp_data - (int)temp_data) * 10));

    // Send the Accel data.
    send_data_using_bluetooth((int)hz_x);
    send_data_using_bluetooth((int)hz_y);
    send_data_using_bluetooth((int)hz_z);
    
    hz_x = 0;
    hz_y = 0;
    hz_z = 0;
    loop_c = 0;
  }
  counter++;
  
}

float get_temp_data(){
  sensors.requestTemperatures();
  float temp_data = sensors.getTempCByIndex(0);
  //Serial.print("temperature : "); Serial.println(temp_data);
  
  return temp_data;
}

void send_data_using_bluetooth(int data){
  bluetooth1.write(data);
}
void send_ecg_using_bluetooth(int data){
  bluetooth2.write(data);
}
