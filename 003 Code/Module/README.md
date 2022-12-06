# Module
<img src="https://user-images.githubusercontent.com/65393001/205955853-7d799390-c73f-4e29-9adc-ee058e86dbce.JPG" width="30%" height="30%">
Hospetter는 실시간으로 동물의 상태를 모듈을 통해 자가진단 및 확인이 가능하다.

## Contents

### esp32-gy906
<img src="https://user-images.githubusercontent.com/65393001/205956031-500fe533-1f1e-46d4-851b-1a7109846e90.jpg" width="30%" height="30%">
비접촉 적외선 온도센서 모듈

### esp32-hx711
<img src="https://user-images.githubusercontent.com/65393001/205956187-c10efabc-e5a4-4c69-9f24-6b1f18d2a9b5.jpg" width="30%" height="30%">
증폭 adc 전류 (장력 측정)센서 모듈

### esp32-rfid
<img src="https://user-images.githubusercontent.com/65393001/205956260-30deda1a-9e49-46a1-8fb0-64ed7de00600.jpg" width="30%" height="30%">
RFID 식별 모듈

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
* 체온, 움직임 확인 등 여러 센서를 하나의 모듈 안에서 사용하므로 이를 조정하는 코드 작성

* 각 모듈을 초기화
```
// hx711 init
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  hx711_prev_data = 0;
  hx711_now_data = 0;
  hx711_counter = 0;
  hx711_now_status = "none";
  hx711_prev_status = "none";

// gy906 init
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
  gy906_counter = 0;
  gy906_string = "";

// rfid init
  rfid.begin(9600, SERIAL_8N1, 36, 13);
```

* 각 모듈을 통해 데이터 수집
```
  xTaskCreatePinnedToCore ( hx711,"hx711", 10000, NULL, 0, &hx711_handler, CORE1 );
  xTaskCreatePinnedToCore ( gy906,"gy906", 10000, NULL, 0, &gy906_handler, CORE2 );
  xTaskCreatePinnedToCore ( rfid_func,"rfid", 10000, NULL, 0, &rfid_handler, CORE1 );
```

* 모은 데이터 전송
```
  xTaskCreatePinnedToCore ( post_func,"post_func", 10000, NULL, 0, &post_handler, CORE2 );
```

### esp32-wifiprovision
와이파이가 제공되는 환경에서 사용자는 손쉽게 디바이스 설정 가능
