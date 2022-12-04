#include <DallasTemperature.h>
#include <OneWire.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define BT_RXD 8
#define BT_TXD 7
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

SoftwareSerial bluetooth(BT_RXD, BT_TXD);
float get_temp_data();
void send_data_using_bluetooth();

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
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
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
      analogRead(A0);
  }
  //Wait for a bit to keep serial data from saturating
  //delay(1);
  if(counter % 25 == 0){
    sensors_event_t accelEvent;
    accel.getEvent(&accelEvent);

    x = accelEvent.acceleration.x;
    y = accelEvent.acceleration.y;
    z = accelEvent.acceleration.z;

    hz_x += x*x;
    hz_y += y*y;
    hz_z += z*z;

    loop_c++; 
  }
  if(loop_c == 32){
    float temp_data;
    temp_data = get_temp_data();
    send_data_using_bluetooth(temp_data);
    
    Serial.print("X : "); Serial.print(hz_x); 
    Serial.print("   Y : "); Serial.print(hz_y);
    Serial.print("   Z : "); Serial.print(hz_z);
    Serial.print("\n");
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
  Serial.print("temperature : "); Serial.println(temp_data);
  
  return temp_data;
}

void send_data_using_bluetooth(float data){
  int int_data = (int)data;
  int float_data = (data - int_data)*10;
  bluetooth.write(int_data);
  bluetooth.write(float_data);
}
