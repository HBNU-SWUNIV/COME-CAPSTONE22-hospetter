#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HttpClient.h>
#include "soc/rtc.h"]
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

//Period Setting, unit : second
#define TEMPERATURE_PERIOD 1
#define BREATH_PERIOD 0.1
#define SEND_JSON_PERIOD 5

int breathcounter;
float temp;
float gy906_data;
String rfid_data;
int hx711_prev_data = 0;
int hx711_now_data = 0;
String hx711_now_status = "none";
String hx711_prev_status = "none";
int gy906_counter = 0;
String gy906_string = "";

//JSON data Settings
int status_first;
String jsondata = "";
StaticJsonBuffer<300> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void hx711(void *param){
  while(1){
  }
}

void gy906(void *param){
  while(1){
  }
}
void rfid_func(void *param){
  while(1){
  }
}

void post_func(void *param){
  while(1){
    root["breath"] = random(15, 30);
    root["temp"] = (random(2,3) * 10)+ (0.1 * random(0, 9));
    root["rfid"] = "410100008177906";
    
    if (status_first == 0){
      Serial.println("First Sending... delay for 2ec");
      delay(2000);
    }
    Serial.println("===== Sending JSON data to Server =====");
    root.printTo(jsondata);

    //sending json data
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("json sending start");
      HTTPClient http;   
     
      http.begin("http://192.168.45.20:3000/api/get_sensor_data");              // Specify destination for HTTP request
      http.addHeader("Content-Type",
                     "application/json");  // Specify content-type header
      
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
    
    root["breath"] = 0;
    root["temp"] = "";
    root["rfid"] = rfid_data;
    jsondata = "";
    
    status_first = 1;
    
    delay(SEND_JSON_PERIOD * 1000);
  }
}

void setup() {
  breathcounter = 0;
  // wifi provisioning code
  // station mode, esp32 can operate station + ap mode simultaneously
  WiFi.mode(WIFI_STA);
  
  Serial.begin(115200);

  WiFiManager wm;
  //if you wanna reset settings on rebooting device, using this code
  //wm.resetSettings();
  bool res;
  res = wm.autoConnect("Settings"); // password protected ap

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
  hx711_prev_data = 0;
  hx711_now_data = 0;
  hx711_now_status = "none";
  hx711_prev_status = "none";
  
  //gy906 init
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

  //collect the data using multi-thread
  xTaskCreatePinnedToCore ( hx711,"hx711", 10000, NULL, 0, &hx711_handler, CORE1 );
  xTaskCreatePinnedToCore ( gy906,"gy906", 10000, NULL, 0, &gy906_handler, CORE2 );
  xTaskCreatePinnedToCore ( rfid_func,"rfid", 10000, NULL, 0, &rfid_handler, CORE1 );

  //send the data using multi-thread
  xTaskCreatePinnedToCore ( post_func,"post_func", 10000, NULL, 0, &post_handler, CORE2 );

  Serial.println();
  Serial.println("setup complete");

}

void loop() {
  //don't use loop block
  sleep(99999999999999999999999999);
}
