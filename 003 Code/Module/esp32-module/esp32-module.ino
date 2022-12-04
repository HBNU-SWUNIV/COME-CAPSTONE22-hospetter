#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HttpClient.h>
#include "soc/rtc.h"
#include "HX711.h"
#include <Adafruit_MLX90614.h>
#include <HardwareSerial.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <HTTPClient.h>
#include <HttpClient.h>

//define core
#define CORE1 0
#define CORE2 1

//sensor handler
TaskHandle_t hx711_handler;
TaskHandle_t gy906_handler;
TaskHandle_t rfid_handler;

//post handler
TaskHandle_t post_handler;

//hx411 wiring
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
long scale_data;
long hx711_prev_data;
long hx711_counter;
String breath_stat;
int threshold;

HX711 scale;

//gy906 wiring
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float gy906_data;
String gy906_string;
int gy906_counter;

//rfid wiring
HardwareSerial rfid(2);
String rfid_data;

//Period Setting, unit : second
#define TEMPERATURE_PERIOD 3
#define BREATH_PERIOD 0.1
#define SEND_JSON_PERIOD 20

//JSON data Settings
int status_first;
String jsondata = "";
StaticJsonBuffer<300> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void hx711(void *param){
  
  while(1){
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
        hx711_counter += 1;
        Serial.println(hx711_counter);
        breath_stat = "deflation";
        hx711_prev_data = scale_data;
        root["breath"] = hx711_counter;
        }
      }
    }  
  }
}
void gy906(void *param){
  while(1){
    gy906_data = mlx.readObjectTempC()+2;
    Serial.print("temperature = ");
    Serial.print(gy906_data);
    Serial.println("*C");
    root["temp"] = gy906_data;
    
    gy906_counter += 1;
    delay(TEMPERATURE_PERIOD * 1000);
  }
}
void rfid_func(void *param){
  while(1){
    if(rfid.available() > 0 && rfid_data == ""){
      String reading = rfid.readStringUntil('\n\r');
      //String reading = rfid.readString(); //Reading RFID value
      if(reading == " "){
        Serial.println("reading value is empty. rfid is not updated");
      }
      else{
        rfid_data = reading.substring(1);
        rfid_data.trim();
        Serial.print("RFID reading value : ");
        Serial.println(rfid_data);
        
        root["rfid"] = rfid_data;
        delay(5 * 1000);
      }
    }
  }
}
void post_func(void *param){
  while(1){
    if (status_first == 0){
      Serial.println("First Sending... delay for 2ec");
      delay(2000);
    }
    //Serial.println("===== Sending JSON data to Server =====");
    //for test
    //root["rfid"] = "410100008131982";
    root.printTo(jsondata);
    root["breath"] = 0;

    //sending json data
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("json sending start");
      HTTPClient http;   

      //for the test
      //http.begin("http://httpbin.org/post");
      http.begin("https://arc1el.iptime.org:3000/api/get_sensor_data");              // Specify destination for HTTP request
      http.addHeader("Content-Type",
                     "application/x-www-form-urlencoded");  // Specify content-type header
      
      String httpRequestData = jsondata;
      int httpResponseCode = http.POST(httpRequestData);    // Send the actual POST request
      
      if(httpResponseCode>0){                               // if data sending is sucessful
      
        String response = http.getString();                 // Get the response to the request
        
        Serial.println(httpResponseCode);                   // Print return code
        Serial.println(response);                           // Print request answer
      
      }
      else{
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
      http.end();  // free 
      Serial.println("==== Sending the data Successfully ====");
    }
    
    Serial.println(jsondata);
    
    
    delay(SEND_JSON_PERIOD * 1000);
    root["rfid"] = rfid_data;
    jsondata = "";
    

    //initalize data
    hx711_counter = 0;
    gy906_counter = 0;

    status_first = 1;
  }
}

void setup() {

  // wifi provisioning code
  // station mode, esp32 can operate station + ap mode simultaneously
  WiFi.mode(WIFI_STA);
  
  Serial.begin(115200);

  WiFiManager wm;
  //if you wanna reset settings on rebooting device, using this code
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("HospetterSettings"); // password protected ap

  if(!res) {
      Serial.println("connection-failed");
      // ESP.restart();
  }
  // if connection is failed
  else {
      Serial.println("connection-success");
  }

  status_first = 0;

  //hx711 init
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  hx711_prev_data = 99999999;
  scale_data = 0;
  hx711_counter = 0;
  breath_stat = "";
  threshold = 1000;
  
  //gy906 init
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  
  gy906_counter = 0;
  gy906_string = "";
  /*
  xTaskCreatePinnedToCore (
  Task1Code,      // task function name
  "task1",        // task name
  10000,          // stack size(word)
  NULL,           // task parameter
  0,              // task priority
  &Task1,         // task handler adress
  0 );            // executing core number
  */

  //rfid init
  rfid.begin(9600, SERIAL_8N1, 36, 13);
  //for the test
  
  rfid_data = "";
  

  //collect the data using multi-thread
  xTaskCreatePinnedToCore ( hx711,"hx711", 10000, NULL, 0, &hx711_handler, CORE2 );
  xTaskCreatePinnedToCore ( gy906,"gy906", 10000, NULL, 0, &gy906_handler, CORE2 );
  xTaskCreatePinnedToCore ( rfid_func,"rfid", 10000, NULL, 0, &rfid_handler, CORE2 );

  //send the data using multi-thread
  xTaskCreatePinnedToCore ( post_func,"post_func", 10000, NULL, 0, &post_handler, CORE1 );

  Serial.println();
  Serial.println("setup complete");

}

void loop() {
  //don't use loop block
  sleep(99999999999999999999999999);
}
