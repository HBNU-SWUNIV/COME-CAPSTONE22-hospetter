#include <SoftwareSerial.h>
SoftwareSerial bluetooth(8, 7); // 블루투스 모듈을 선언합니다.
void setup() 
{
  Serial.begin(9600);
  bluetooth.begin(9600);
}
void loop() {
  if (bluetooth.available()) { // 블루투스로 값이 들어오면 출력합니다.
    Serial.write(bluetooth.read());
  }
  if (Serial.available()) { // 시리얼모니터로 입력된 값을 블루투스로 전송합니다.
    bluetooth.write(Serial.read());
  }
}
