# WAS(Web Application Service)

## 1. 원격진단(화상진단)
Hospetter에서 의사와 환자 간의 화상진단이 이루어져야 함.
이는, mediasoup-webrtc를 사용하여 구현

## 2. 웹 서비스
backend: node.js를 이용하여 hospetter의 회원을 생성 및 관리하며, 각 회원에게 맞는 정보 제공
frontend: 주로 bootstrap을 이용하여 디자인

### contents

**mediasoup-webrtc**
webrtc service using mediasoup

연결이 발생하면(무조건 초기화 실행)

```
io.on('connection', (socket) => {
    해당 룸의 id가 이미 존재하는지 확인

    존재하지 않는다면,
        룸 생성

        getMideasoupWorker로부터 워커 받아옴

        룸 id 설정

        소켓정보전달
})
```