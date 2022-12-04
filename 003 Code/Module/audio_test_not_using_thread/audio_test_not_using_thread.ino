#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoBLE.h>
#include <ArduinoJson.h>

//setting ssid, password
char ssid[] = "DfXLabPros_2.4G";
char pass[] = "thdtnrms1!";
int keyIndex = 0;

//setting status
int status = WL_IDLE_STATUS;

//api server address
char server[] = "192.168.45.83";

WiFiClient client;
String sensor_type;
int sensor_data;
const int sampleWindow = 47;
unsigned int sample;
int samplerate = 0;

String audio_dat = "{'data' : [";

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  /*
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
  sensor_data = 30;*/
}

void loop() {

  unsigned long startMillis= millis();
  while (millis() - startMillis < 47)
  {
    sample = analogRead(0);
    Serial.println(sample);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
