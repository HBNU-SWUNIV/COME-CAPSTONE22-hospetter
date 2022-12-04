#include <SPI.h>
#include <WiFiNINA.h>

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
  
}

void loop() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address : ");
  Serial.println(ip);
}
