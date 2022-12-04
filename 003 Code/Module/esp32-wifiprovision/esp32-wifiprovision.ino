#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <HttpClient.h>


TaskHandle_t Task1;
TaskHandle_t Task2;
 
//data collecting thread
void Task1Code( void *param)
{
  while (1) {
    Serial.print("task1 code is running on core : ");
    Serial.println(xPortGetCoreID());
    delay(3000);
  }
}

//data sending thread
void Task2Code( void *param)
{
  while (1) {
    Serial.print("task2 code is running on core : ");
    Serial.println(xPortGetCoreID());

    //testing json format
    String jsondata = "";
    
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["tempvalue"] = 26;
    root["humivalue"] = 37;
    root.printTo(jsondata);
    Serial.println(jsondata);
  
    //testing send json data
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("json sending start");
      HTTPClient http;   
     
      http.begin("http://httpbin.org/post");              // Specify destination for HTTP request
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
    }
    delay(10000);
  }
}

void setup() {

  // wifi provisioning code
  // station mode, esp32 can operate station + ap mode simultaneously
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  // wifi manager initialization 
  WiFiManager wm;
  //if you wanna reset settings on rebooting device, using this code
  wm.resetSettings();
  bool res;
  res = wm.autoConnect("Settings"); // password protected ap

  // res is the boolean. if wifi connection is success, res value is true
  if(!res) {
      Serial.println("connection-failed");
      // ESP.restart();
  }
  // if connection is failed
  else {
      Serial.println("connection-success");
  }
  //end of the wifi provisioning

  // multi-threading initalization
  //task1
  xTaskCreatePinnedToCore (
  Task1Code,      // task function name
  "task1",        // task name
  10000,          // stack size(word)
  NULL,           // task parameter
  0,              // task priority
  &Task1,         // task handler adress
  0 );            // executing core number
  //task2
  xTaskCreatePinnedToCore (
  Task2Code,
  "task2",
  10000,
  NULL,
  0,
  &Task1,
  1 );
  // end of the multi-threading intialization
}

void loop() {
  //don't use loop block
  sleep(99999999999999999999999999);
}
