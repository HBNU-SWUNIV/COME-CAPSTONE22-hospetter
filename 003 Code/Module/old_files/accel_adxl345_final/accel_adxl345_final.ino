 #include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
/* Assign a unique ID to this sensor at the same time */ 
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
float AccelMinX = 0; 
float AccelMaxX = 0; 
float AccelMinY = 0; 
float AccelMaxY = 0; 
float AccelMinZ = 0; 
float AccelMaxZ = 0;
float x = 0;
float y = 0;
float z = 0;

int counter = 0;

void setup(void) {
  Serial.begin(9600);
  Serial.println("ADXL345 Accelerometer Calibration");
  Serial.println("");
/* Initialise the sensor */ 
if(!accel.begin())
  {
    accel.setDataRate(ADXL345_DATARATE_800_HZ);
  /* There was a problem detecting the ADXL345 ... check your connections */ 
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!"); while(1);
  }
}
void loop(void) {
  /* Get a new sensor event */
  if(counter % 25 == 0){
    sensors_event_t accelEvent;
    accel.getEvent(&accelEvent);

    x = accelEvent.acceleration.x;
    y = accelEvent.acceleration.y;
    z = accelEvent.acceleration.z;
    
    Serial.print("X : "); Serial.print(x); 
    Serial.print("   Y : "); Serial.print(y);
    Serial.print("   Z : "); Serial.print(z);
    Serial.print("\n");
  }
  counter++;
}
