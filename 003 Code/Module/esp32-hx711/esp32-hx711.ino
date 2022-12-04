#include <Arduino.h>
#include "soc/rtc.h"
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
bool TARE_FLAG = false;

long hx711_prev_data;
long read_data;
int counter;
int scale_data;
int threshold;
String breath_stat;

HX711 scale;

void setup() {
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  hx711_prev_data = 9999999;
  read_data = 0;
  counter = 0;
  scale_data = 0;
  threshold = 400;
  breath_stat = "";
}

void loop() {
  if (scale.is_ready()) {
    
    scale_data = scale.read(); // 20000
    //Serial.print("prev_data : ");
    //Serial.print(hx711_prev_data);
    //Serial.print(" scale : ");
    //Serial.println(scale_data);
    
    if (scale_data > hx711_prev_data){
      hx711_prev_data = scale_data;
      breath_stat = "expansion";
    }
    
    if (scale_data + threshold < hx711_prev_data){
      if (breath_stat = "expansion"){
        counter += 1;
        Serial.println(counter);
        breath_stat = "deflation";
        hx711_prev_data = scale_data;
      }
    }
  }
}
