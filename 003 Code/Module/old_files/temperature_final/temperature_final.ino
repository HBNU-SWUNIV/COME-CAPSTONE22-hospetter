#include <DallasTemperature.h>
#include <OneWire.h>
#include<SoftwareSerial.h>

#define BT_RXD 8
#define BT_TXD 7
#define TEMP_18B20 2

OneWire oneWire(TEMP_18B20);
DallasTemperature sensors(&oneWire);
float get_temp_data();
void send_data_using_bluetooth();

void setup() {
  Serial.begin(9600);
}

void loop() {
  float temp_data;
  temp_data = get_temp_data();
  delay(1000);
}

float get_temp_data(){
  sensors.requestTemperatures();
  float temp_data = sensors.getTempCByIndex(0);
  Serial.print("temperature : "); Serial.println(temp_data);
  
  return temp_data;
}
