# WAS(Web Application Service)

## 1. 원격진단(화상진단)
Hospetter에서 의사와 환자 간의 화상진단이 이루어져야 함.
이는, mediasoup-webrtc를 사용하여 구현

## 2. 웹 서비스
- backend: node.js를 이용하여 hospetter의 회원을 생성 및 관리하며, 각 회원에게 맞는 정보 제공
- frontend: 주로 bootstrap을 이용하여 디자인

## Contents

### mediasoup-webrtc

**app.js**

- 연결 생성
```
io.on('connection', (socket) => {
  socket.on('createRoom', async ({ room_id }, callback) => {
    if (roomList.has(room_id)) {
      callback('already exists')
    } else {
      console.log('Created room', { room_id: room_id })

      let worker = await getMediasoupWorker()
      roomList.set(room_id, new Room(room_id, worker, io))
      callback(room_id)
    }
  })
```

- 연결 응답
```
  socket.on('join', ({ room_id, name }, cb) => {
    console.log('User joined', {
      room_id: room_id,
      name: name
    })

    if (!roomList.has(room_id)) {
      return cb({
        error: 'Room does not exist'
      })
    }

    roomList.get(room_id).addPeer(new Peer(socket.id, name))
    socket.room_id = room_id
    cb(roomList.get(room_id).toJson())
  })
```
