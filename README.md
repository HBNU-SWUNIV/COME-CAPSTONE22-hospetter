# 한밭대학교 컴퓨터공학과 hospetter팀
#### 반려동물을 위한 웨어러블 디바이스 및 모니터링 시스템 (Wearable devices and monitoring systems for pets)


<img src="https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white"><img src="https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=node.js&logoColor=white"><img src="https://img.shields.io/badge/Express-000000?style=for-the-badge&logo=Express&logoColor=white"><img src="https://img.shields.io/badge/Javascript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=white"><img src="https://img.shields.io/badge/WebRTC-333333?style=for-the-badge&logo=webrtc&logoColor=white"><img src="https://img.shields.io/badge/MySQL-4479A1?style=for-the-badge&logo=mysql&logoColor=white"><img src="https://img.shields.io/badge/Espressif-E7352C?style=for-the-badge&logo=espressif&logoColor=white">

**팀 구성**
- 20197125 김현민
- 20197131 정봉경
- 20197122 최서현

## <u>Teamate</u> Project Background
- ### 필요성
  - 동물병원을 운영함에 있어서 가장 많은 인건비를 차지하는부분이 전문 테크니션을 고용하는 부분이라고 한다. 반려동물의 사이즈에 따라 차이가 있지만 병원에 입원을 해야하는 동물의 경우 주기적으로 생체데이터를 기록해야할 필요가 있고, 적어도 2-4명의 테크니션들이 1마리의 반려동물에 붙어 체온, 심박수, 호흡수 등을 체크하고 추가적인 정밀검사가 필요한 경우 더 많은 테크니션들이 필요하다. 우리는 해당 문제점이 충분히 수요가 있을 것으로 판단, 동물병원에서 사용하는 웨어러블 디바이스를 개발하는 것으로 주제의 방향을 잡았다.
  - 현재 동물병원에서는 주기적으로 입원한 반려동물에 대해 생체정보를 시트에 기록한다. 때문에 동물병원에서는 이를 수집하고 기록하는 일만 전담으로 하는 전문 테크니션을 고용하기도 한다. 대형병원의 경우에는 이에 따른 인건비가 만만치 않다. 때문에 동물병원에서 이를 자동으로 측정해주는 웨어러블 디바이스를 개발하려고 한다. 해당 기기를 통해 동물병원에서의 인건비를 줄이고, 데이터를 자동으로 측정하여 이를 문서화하여 테크니션들의 업무 부담을 줄이고자 한다.
- ### 기존 해결책의 문제점
  - 바이탈신호 기록을 여러명의 테크니션이 담당하여 수기로 진행하며, 1시간 단위로 주기적으로 기록한다.

## System Design
  - ### System Requirements
    - API Server
      * (Hardware) Raspberry Pi4. However, it can run on any Linux os
      * node >= v16.14.2
      * express >= v4.16.1
      * npm >= v16.14.2
      * mysql >= v2.18.1 (mysql-connector)

    - Web Application Server (WAS)
      * (Hardware) Raspberry Pi4. However, it can run on any Linux os
      * node >= v16.14.2
      * express >= v4.17.1
      * npm >= v16.14.2
      * mediasoup >= v3.11.3
      * mediasoup-client >= v3.6.37
      * socket.io >= 4.1.3

    - DB Server
      * (Hardware) Raspberry Pi4. However, it can run on any Linux os
      * mysql-server (8.0.31-0ubuntu0.22.04.1)
    - Module
      * NodeMCU-32S Lua WiFi (ESP-32)
      
## Installing and Running
  - Install API server and WAS
  ```sh
  cd {Project Directory Path(API or WAS)}
  npm install
  ```
  - Running Application
  ```sh
  npm start
  ```
## Case Study
  -  ESP32를 활용한 센서데이터 무선 수집, WiFi Provisioning 사용
  - 호흡수, 체온 측정기능 구현. 데이터베이스 저장
  - 3D프린터를 활용한 하우징 제작
  - MySQL을 사용하는 데이터베이스 구현
  - 데이터베이스서버와 WAS서버 간 데이터 교환을 위한 API서버 구현
  - 모니터링 시스템을 구현하는 WAS서버 제작
  
## Conclusion
  - ### 웨어러블 모듈
    <img src="https://user-images.githubusercontent.com/8403172/205635133-46872ecc-ec18-43c0-af1b-6fb22083978d.jpg" width="70%" height="70%">  
    <img src="https://user-images.githubusercontent.com/8403172/205685603-2df895d7-8547-4cfb-b384-1304420206f2.gif" width="70%" height="70%">  
    <img src="https://user-images.githubusercontent.com/8403172/205685638-5606ba37-9869-4f4c-b674-55e4721618d7.gif" width="70%" height="70%">  
  - ### 모니터링 시스템
    <img src="https://user-images.githubusercontent.com/8403172/205635469-149729bb-95a2-4c0d-9fa4-495818c8fee2.jpg" width="70%" height="70%">  
    <img src="https://user-images.githubusercontent.com/8403172/205686786-5abd97ab-b3ef-4395-921f-71ba0e198f2f.png" width="70%" height="70%">
    
## Project Outcome
- ### 2022년학년도 제 11회 정보기술대학 작품전시회 장려상 수상
     <img src="https://user-images.githubusercontent.com/8403172/205636836-2d109650-7a7a-40fd-a6f2-a71284431e5a.jpg" width="50%" height="50%">
   
## Reference List
  - 이진홍, 유성희(2020), 「코로나19 사태로 비추어 본 반려동물 원격진료 도입 가능성에 대한 법적 고찰」, 『의생명과학과법』23, 법학연구소(원광대), pp.113-134 
  - 정한조, 이정훈, 이지형, 김세윤, 정지원(2020), 「웨어러블 디바이스를 이용한 반려동물 원격 모니터링 시스템」, 『한국컴퓨터정보학회 학술발표논문집』28, 한국컴퓨터정보학회, pp. 281-282
  - 이민형, 유래현, 김수연, 김경호(2021), 「반려동물 생체신호 측정 시스템에 관한 연구」, 『대한전기학회 학술대회 논문집』, 대한전기학회, pp. 405-406
  - 박주연, 윤지윤, 이예진, 박서영, 김두열, 이기석(2021), 「반려동물 생체인식 앱」, 『한국컴퓨터정보학회 학술발표논문집』29, 한국컴퓨터정보학회, pp. 351-354
  - 이병규, 박재곤, 유채은, 유하정(2018), 「IoT를 이용한 반려동물 Healthcare System」, 『한국정보과학회 학술발표논문집』, 한국정보과학회, pp.2226-2228
  - 유경근(2016), 「반려동물 자가진료 금지에 대한 올바른 이해 - 반려동물 자가진료 금지의 의미와 범위, 그리고 목적」, 『대한수의사회지 제 52권』8, 대한수의사회, pp.462-466
  - 김종수. 무선 청진기를 포함하는 웨어러블 장치(WEARABLE DEVICE HAVING WIRELESS STETHOSCOPE), 특허 출원번호 1020180128023, 출원일 2018.10.25., 등록일 2019.03.05.
  - 주식회사 케어식스, 심전도와 심탄도 동시 측정이 가능한 반려견용 심장운동 측정 장치(Dog cardiometry device for simultaneous measurement of EKG and ECG), 특허 출원번호 1020190128260
  - 주식회사 케어식스, 반려견의 활동량과 심박호흡등 생체신호 그리고 임상신호에 기반한 질병예측 방법 및 장치(Disease Prediction Method and Apparatus Based on the Activity of Dogs, Bio-signal), 특허 출원번호 1020190128299


