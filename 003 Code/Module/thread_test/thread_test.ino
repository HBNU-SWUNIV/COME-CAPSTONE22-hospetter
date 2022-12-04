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

//testdata
int test_int = 0;

WiFiClient client;
String sensor_type;
int sensor_data;

void sensorCallback(){
  String jsondata = "";
  
  //const size_t capacity = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + 4*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4);
  DynamicJsonBuffer jsonBuffer(200);
  
  JsonObject& root = jsonBuffer.createObject();
  
  JsonObject& SENSOR = root.createNestedObject("SENSOR");
  
  JsonObject& SENSOR_SENSOR01 = SENSOR.createNestedObject("SENSOR01");
  SENSOR_SENSOR01["type"] = "ecg";
  
  JsonArray& SENSOR_SENSOR01_data = SENSOR_SENSOR01.createNestedArray("data");
  SENSOR_SENSOR01_data.add(1);
  SENSOR_SENSOR01_data.add(2);
  SENSOR_SENSOR01_data.add(3);
  
  JsonObject& SENSOR_SENSOR02 = SENSOR.createNestedObject("SENSOR02");
  SENSOR_SENSOR02["type"] = "temp";
  SENSOR_SENSOR02["data"] = 456;
  
  JsonObject& SENSOR_SENSOR03 = SENSOR.createNestedObject("SENSOR03");
  SENSOR_SENSOR03["type"] = "accel";
  SENSOR_SENSOR03["data"] = 789;
  
  JsonObject& SENSOR_SENSOR04 = SENSOR.createNestedObject("SENSOR04");
  SENSOR_SENSOR04["type"] = "gps";
  SENSOR_SENSOR04["data"] = 34;
  
  root.printTo(jsondata);
  
  
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
  sensor_thread.setInterval(10000);
  
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
