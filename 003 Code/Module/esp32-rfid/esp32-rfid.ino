#include <HardwareSerial.h>
HardwareSerial mySerial(2); //3개의 시리얼 중 2번 채널을 사용


//36 pin is rx
void setup() {
  //115200
    Serial.begin(115200); //기존의 기본 시리얼
    mySerial.begin(9600, SERIAL_8N2, 36, 13); //추가로 사용할 시리얼. RX:12 / TX:13번 핀 사용
}

void loop() {
    if(mySerial.available() > 0){
      String command = mySerial.readStringUntil('\n'); //추가 시리얼의 값을 수신하여 String으로 저장
      Serial.println(command); //기본 시리얼에 추가 시리얼 내용을 출력
    }
}
