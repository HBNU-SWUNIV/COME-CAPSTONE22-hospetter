#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoBLE.h>
#include <ArduinoJson.h>
#include <Thread.h>
#include <ThreadController.h>

//setting thread
ThreadController controll = ThreadController();
Thread sensor_thread = Thread();

//setting ssid, password
char ssid[] = "DfXLabPros_2.4G";
char pass[] = "thdtnrms1!";
int keyIndex = 0;

//setting status
int status = WL_IDLE_STATUS;

//api server address
char server[] = "192.168.45.13";

//audio settings
const int sampleWindow = 47;
unsigned int sample;
int sample_rate = 0;


WiFiClient client;
String sensor_type;
int sensor_data;

//json생성
DynamicJsonBuffer jsonBuffer(200);
JsonObject& root = jsonBuffer.createObject();
JsonArray& data = root.createNestedArray("data");
String jsondata = "";

void sensorCallback(){
  
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  sample = analogRead(0);
  Serial.println(sample);

  data.add(sample);

  sample_rate += 1;

  if (sample_rate == 16000){
    root.printTo(jsondata);
  
    /*
    Serial.println("\nStarting connection to server...");
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      client.println("POST /api/get_sensor_data HTTP/1.1");
      client.println("Host: ArduinoNanoIoT33");
      client.println("Content-Type: application/json");
      //client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(jsondata.length());
      client.println();
      client.print(jsondata);
      client.println();
  
      Serial.println("[Sended Data]");
      Serial.println(jsondata);
    }
    
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
    }*/

    Serial.print(jsondata);

    sample_rate = 0;
    jsondata = "";
    
  }
}

void setup() {
  Serial.begin(9600);

  //wifi module initiation
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  //if wifi is connected
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(5000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  sensor_type = "SENSOR_TYPE";
  sensor_data = 30;

  //thread setup
  sensor_thread.onRun(sensorCallback);
  sensor_thread.setInterval(47);
  
  controll.add(&sensor_thread);
}

void loop() {
  controll.run();
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
