#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

#include <Wire.h>

#define I2C_Address 0x53

// ADXL345 register address
#define POWER_CTL 0x2D
#define DATA_FORMAT 0x31
#define X_axis 0x32
#define Y_axis 0x34
#define Z_axis 0x36

#define Range_2g 0
#define Range_4g 1
#define Range_8g 2
#define Range_16g 3

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup(){
    Wire.begin();   // 센서 작동 시작
    Serial.begin(9600); // 통신속도 9600 설정
    adxl = initADXL345(Range_2g);
    adxl.setDataRate(ADXL345_DATARATE_30_HZ);
}

void loop(){
    printAxis();
}

// I2C 인터페이스를 통해 축을 읽음
int readAxis(byte a){
    int data;

    Wire.beginTransmission(I2C_Address);
    Wire.write(a);
    Wire.endTransmission();

    Wire.beginTransmission(I2C_Address);
    Wire.requestFrom(I2C_Address, 2);

    if(Wire.available()){
        data = (int)Wire.read();
        data = data | (Wire.read() << 8);
    }else{
        data = 0;
    }

    Wire.endTransmission();
    
    return data;
}

// Init ADXL345 
void initADXL345(byte r){
    Wire.beginTransmission(I2C_Address);

    // 감도설정
    Wire.write(DATA_FORMAT);
    Wire.write(r);
    Wire.endTransmission();

    // 측정모드로 전환
    Wire.beginTransmission(I2C_Address);
    Wire.write(POWER_CTL);
    Wire.write(0x08);
    Wire.endTransmission();
}

void printAxis(){
    // 3축 출력
    Serial.print(" X: ");
    Serial.print(readAxis(X_axis));
    Serial.print("  Y: ");
    Serial.print(readAxis(Y_axis));
    Serial.print("  Z: ");
    Serial.print(readAxis(Z_axis));
    Serial.print("\n==============================");
    Serial.println();

    delay(1000);
}
