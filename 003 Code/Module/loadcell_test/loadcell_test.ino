#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
int prev_status;
int now_status;
int last_reading;
int count;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  count = 0;
  prev_status = 0;
  now_status = 0;
  last_reading = 0;
}

void loop() {

  if (scale.is_ready()) {
    long reading = scale.read();
    last_reading = reading;
    
    if (last_reading > reading + 100000) {
      prev_status = now_status;
      now_status = 1;
      Serial.println(reading);
      //Serial.print(now_status);
      //Serial.println(prev_status);
    }
    else{
      prev_status = now_status;
      now_status = 0;
      Serial.println(reading);
      //Serial.print(now_status);
      //Serial.println(prev_status);
    }

  if(prev_status != now_status){
    count ++;
    if(count%2 == 0){
      Serial.print("호흡수 : ");
      Serial.println(count/2);
    }
  }

  

    
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1000);
  
}
