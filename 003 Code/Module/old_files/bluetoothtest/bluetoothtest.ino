#include <SoftwareSerial.h>        // 블루투스 시리얼 통신 라이브러리 추가
#define BT_RXD 13
#define BT_TXD 12
SoftwareSerial bluetooth(BT_RXD, BT_TXD);        // 블루투스 설정 BTSerial(Tx, Rx)
void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);                         // 블루투스 통신 시작
}
void loop() {
  if (bluetooth.available()) {        // 블루투스에서 보낸 내용은 시리얼모니터로 전송
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) {           // 시리얼모니터에서 보낸 내용은 블루투스로 전송
    bluetooth.write(Serial.read());
  }
}
