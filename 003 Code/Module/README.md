# Module

Hospetter는 실시간으로 동물의 상태를 모듈을 통해 자가진단 및 확인이 가능하다.

## Contents

### esp32-gy906
비접촉 적외선 온도센서 모듈

### esp32-hx711
증폭 adc 전류 센서 모듈

### esp32-module
* 모듈 안에 장착된 센서들의 데이터(json)를 웹 서버와 교류하기 위한 코드

```
String jsondata = "";
StaticJsonBuffer <300> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
```

* 각 모듈에서 모아진 데이터들을 json을 이용해 서버로 전송한다.
```
void post_func(void *param){...}
```

* 데이터 전송이 처음인지 아닌지를 판별하는 변수
```
int status_first;

```


### esp32-multi-thread-sensors
체온, 움직임 확인 등 여러 센서를 하나의 모듈 안에서 사용하므로 이를 조정하는 코드 작성

```
// sensor handler
TaskHandle_t hx711_handler;
TaskHandle_t gy906_handler;
TaskHandle_t rfid_handler;
```

### esp32-test-api
```
```

### esp32-rfid
```
```

### esp32-wifiprovision
```
```