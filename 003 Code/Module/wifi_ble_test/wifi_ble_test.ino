#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoBLE.h>

const char* ssid = "DfXLabPros_2.4G";
const char* pass = "thdtnrms1!";
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
  if(!BLE.begin()){
    Serial.println("BLE start failed");
    while(1);
  }

  //try to set BLE device
  BLE.setLocalName("ArduinoNano33IoTBLE");
  BLE.setAdvertisedServiceUuid("abf55695-4242-4cd8-af5b-cdb4a069e6b8");

  //start Advertising
  BLE.advertise();
  Serial.println("Blutooth device is activated. Waiting for connections");
  
}

void loop() {
  //wait for BLE central. if connection is success, Serial print the BLE central address
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central : ");
    Serial.println(central.address());

    //BLE is connected
    while (central.connected()) {
      Serial.println("Do something");
      delay(1000);
    }
  }
}
