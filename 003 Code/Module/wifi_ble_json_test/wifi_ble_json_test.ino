#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoBLE.h>
#include <ArduinoJson.h>

const char* ssid = "DfXLabPros_2.4G";
const char* pass = "thdtnrms1!";
const char* server = "192.168.45.25";

String sensor_type;
int sensor_data;
String filename;

WiFiClient client;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  //try wifi module check
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module initiation error");
    while(true);
  }

  //try wifi module initiation
  while (status != WL_CONNECTED) {
    Serial.print("try to connect WPA SSID : ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  //wifi connect success
  Serial.println("connected to Network");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address : ");
  Serial.println(ip);
  
  //try ble module check
  /*
  if(!BLE.begin()){
    Serial.println("BLE start failed");
    while(1);
  }

  //try to set BLE device
  BLE.setLocalName("ArduinoNano33IoTBLE");
  BLE.setAdvertisedServiceUuid("abf55695-4242-4cd8-af5b-cdb4a069e6b8");

  start Advertising
  BLE.advertise();
  Serial.println("Blutooth device is activated. Waiting for connections");
  */
  
  sensor_type = "SENSOR_TYPE";
  sensor_data = 30;
  filename = "test_data";
}

void loop() {
  //wait for BLE central. if connection is success, Serial print the BLE central address
  //BLEDevice central = BLE.central();
  //if (central) {
    //Serial.print("Connected to central : ");
    //Serial.println(central.address());

    //BLE is connected
    //while (central.connected()) {

    

    
      String jsondata = "";
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();

      json["sensor_type"] = sensor_type;
      json["sensor_data"] = sensor_data;
      json["filename"] = filename;
      json.printTo(jsondata);

     /*
      Serial.println("Connect to api server...");
      if (client.connect("http://192.168.45.25", 80)) {
        client.println("POST /sensor HTTP/1.1");
        client.println("Host: http://192.168.45.25");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(postData.length());
        client.println();
        client.print(postData);
      }else {
        Serial.println("Connection Failed...");
      }

      if (client.connected()) {
        Serial.println("client stopping...");
        client.stop();
      }

      Serial.println("send data successfully");  
      Serial.println(jsondata);
      
      
      delay(5000);
      */
      Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
    //}
  //}
}
