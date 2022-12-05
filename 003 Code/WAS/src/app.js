const express = require('express')
const app = express()
const https = require('httpolyglot')
const fs = require('fs')
const mediasoup = require('mediasoup')
const path = require('path')
var cors = require('cors');

//여기서부터 js파일을 모듈로 불러옴. 모듈을 불러오는것은 실행이 아님에 유의
const config = require('./config')
const Room = require('./Room')
const Peer = require('./Peer')

const options = {
  //key: fs.readFileSync(path.join(__dirname, config.sslKey), 'utf-8'),
  //cert: fs.readFileSync(path.join(__dirname, config.sslCrt), 'utf-8')
  key: fs.readFileSync(path.join(__dirname, "./rootca.key"), 'utf-8'),
  cert: fs.readFileSync(path.join(__dirname, "./rootca.crt"), 'utf-8')
}

//https 서버 생성
const httpsServer = https.createServer(options, app)
//socketio를 사용하여 웹소켓을 좀더 편하게
const io = require('socket.io')(httpsServer)

//퍼블릭 폴더 설정. public 디렉토리
app.use(express.static(path.join(__dirname, '..', 'public')))

app.use(cors())

//https서버 생성. config.listenport이므로 config.js의 listenport부분을 모듈로 해서 가져올 수 있음
httpsServer.listen(config.listenPort, () => {
  console.log('Listening on https://' + config.listenIp + ':' + config.listenPort)
})

// all mediasoup workers
// 워커들(생성자?)를 담아둘 배열과 그 인덱스
let workers = []
let nextMediasoupWorkerIdx = 0

/**
 * 룸인덱스 형식
 * roomList
 * {
 *  room_id: Room {
 *      id:
 *      router:
 *      peers: {
 *          id:,
 *          name:,
 *          master: [boolean],
 *          transports: [Map],
 *          producers: [Map],
 *          consumers: [Map],
 *          rtpCapabilities:
 *      }
 *  }
 * }
 */
let roomList = new Map()

;(async () => {
  await createWorkers()
})()

async function createWorkers() {
  let { numWorkers } = config.mediasoup
  /** 해당부분은 다음과 같음
    numWorkers: Object.keys(os.cpus()).length,
    컴퓨터 코어정보, 코어갯수(length)
  */

  // 해당부분은 아마도 사용자의 코어갯수를 활용하여 worker를 만드는 부분인듯?
  for (let i = 0; i < numWorkers; i++) {
    let worker = await mediasoup.createWorker({
      logLevel: config.mediasoup.worker.logLevel,
      logTags: config.mediasoup.worker.logTags,
      rtcMinPort: config.mediasoup.worker.rtcMinPort,
      rtcMaxPort: config.mediasoup.worker.rtcMaxPort
    })
    
    //worker가 죽으면(연결끊기면)
    worker.on('died', () => {
      console.error('mediasoup worker died, exiting in 2 seconds... [pid:%d]', worker.pid)
      setTimeout(() => process.exit(1), 2000)
    })

    // 워커를 넣는다 앞부분의 워커리스트에
    workers.push(worker)

    // log worker resource usage
    /*setInterval(async () => {
            const usage = await worker.getResourceUsage();

            console.info('mediasoup Worker resource usage [pid:%d]: %o', worker.pid, usage);
        }, 120000);*/
  }
}

//연결이 발생하면(무조건 초기화 실행이라고 생각하면 됨)
io.on('connection', (socket) => {

  //크레이트 룸 동작
  socket.on('createRoom', async ({ room_id }, callback) => {

    if (roomList.has(room_id)) {
      callback('already exists')
    } else {

      //룸만들기
      console.log('Created room', { room_id: room_id })

      // getMediasoupWorker로 부터 워커를 받아옴 (await)
      let worker = await getMediasoupWorker()

      // 룸 리스트에 룸아이디와 새로운 룸을 가져옴
      // 이때의 룸은 room.js 에 정의되어있음.
      /*
      constructor(room_id, worker, io) {
        this.id = room_id
        const mediaCodecs = config.mediasoup.router.mediaCodecs
        worker
          .createRouter({
            mediaCodecs
          })
          .then(
            function (router) {
              this.router = router
            }.bind(this)
          )

        this.peers = new Map()
        this.io = io
      } */
      roomList.set(room_id, new Room(room_id, worker, io))
      // 아이디설정하고, 미디어코덱 가져오고, 라우터를 만들고 라우터를 바인딩
      // 생성한 소켓 정보도 넘겨줌
      callback(room_id)
    }
  })

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

  // 해방부분은 RoomClient.js에서 호출한 getproducers에 의해 실행
  socket.on('getProducers', () => {
    if (!roomList.has(socket.room_id)) return
    console.log('Get producers', { name: `${roomList.get(socket.room_id).getPeers().get(socket.id).name}` })

    // send all the current producer to newly joined member
    let producerList = roomList.get(socket.room_id).getProducerListForPeer()

    socket.emit('newProducers', producerList)
  })

  socket.on('getRouterRtpCapabilities', (_, callback) => {
    console.log('Get RouterRtpCapabilities', {
      name: `${roomList.get(socket.room_id).getPeers().get(socket.id).name}`
    })

    try {
      callback(roomList.get(socket.room_id).getRtpCapabilities())
    } catch (e) {
      callback({
        error: e.message
      })
    }
  })

  socket.on('createWebRtcTransport', async (_, callback) => {
    console.log('Create webrtc transport', {
      name: `${roomList.get(socket.room_id).getPeers().get(socket.id).name}`
    })

    try {
      const { params } = await roomList.get(socket.room_id).createWebRtcTransport(socket.id)

      callback(params)
    } catch (err) {
      console.error(err)
      callback({
        error: err.message
      })
    }
  })

  socket.on('connectTransport', async ({ transport_id, dtlsParameters }, callback) => {
    console.log('Connect transport', { name: `${roomList.get(socket.room_id).getPeers().get(socket.id).name}` })

    if (!roomList.has(socket.room_id)) return
    await roomList.get(socket.room_id).connectPeerTransport(socket.id, transport_id, dtlsParameters)

    callback('success')
  })

  socket.on('produce', async ({ kind, rtpParameters, producerTransportId }, callback) => {
    if (!roomList.has(socket.room_id)) {
      return callback({ error: 'not is a room' })
    }

    let producer_id = await roomList.get(socket.room_id).produce(socket.id, producerTransportId, rtpParameters, kind)

    console.log('Produce', {
      type: `${kind}`,
      name: `${roomList.get(socket.room_id).getPeers().get(socket.id).name}`,
      id: `${producer_id}`
    })

    callback({
      producer_id
    })
  })

  socket.on('consume', async ({ consumerTransportId, producerId, rtpCapabilities }, callback) => {
    //TODO null handling
    let params = await roomList.get(socket.room_id).consume(socket.id, consumerTransportId, producerId, rtpCapabilities)

    console.log('Consuming', {
      name: `${roomList.get(socket.room_id) && roomList.get(socket.room_id).getPeers().get(socket.id).name}`,
      producer_id: `${producerId}`,
      consumer_id: `${params.id}`
    })

    callback(params)
  })

  socket.on('resume', async (data, callback) => {
    await consumer.resume()
    callback()
  })

  socket.on('getMyRoomInfo', (_, cb) => {
    cb(roomList.get(socket.room_id).toJson())
  })

  socket.on('disconnect', () => {
    console.log('Disconnect', {
      name: `${roomList.get(socket.room_id) && roomList.get(socket.room_id).getPeers().get(socket.id).name}`
    })

    if (!socket.room_id) return
    roomList.get(socket.room_id).removePeer(socket.id)
  })

  //roomClient.js에 의해 실행
  socket.on('producerClosed', ({ producer_id }) => {
    console.log('Producer close', {
      name: `${roomList.get(socket.room_id) && roomList.get(socket.room_id).getPeers().get(socket.id).name}`
    })

    roomList.get(socket.room_id).closeProducer(socket.id, producer_id)
  })

  socket.on('exitRoom', async (_, callback) => {
    console.log('Exit room', {
      name: `${roomList.get(socket.room_id) && roomList.get(socket.room_id).getPeers().get(socket.id).name}`
    })

    if (!roomList.has(socket.room_id)) {
      callback({
        error: 'not currently in a room'
      })
      return
    }
    // close transports
    await roomList.get(socket.room_id).removePeer(socket.id)
    if (roomList.get(socket.room_id).getPeers().size === 0) {
      roomList.delete(socket.room_id)
    }

    socket.room_id = null

    callback('successfully exited room')
  })
})

// TODO remove - never used?
function room() {
  return Object.values(roomList).map((r) => {
    return {
      router: r.router.id,
      peers: Object.values(r.peers).map((p) => {
        return {
          name: p.name
        }
      }),
      id: r.id
    }
  })
}

/**
 * Get next mediasoup Worker.
 */
function getMediasoupWorker() {
  const worker = workers[nextMediasoupWorkerIdx]

  if (++nextMediasoupWorkerIdx === workers.length) nextMediasoupWorkerIdx = 0

  return worker
}
