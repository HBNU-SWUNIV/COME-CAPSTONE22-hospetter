if (location.href.substr(0, 5) !== 'https') location.href = 'https' + location.href.substr(4, location.href.length - 4)

$( window ).on( "load", function() {
  console.log( "ready!" );
  var http = new XMLHttpRequest();
  var url = "https://arc1el.iptime.org:3000/api/recent_rfid";

  http.open("GET", url, true);
  
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      console.log("res : ", res);
      result = JSON.parse(res);
      rfid = result[0].rfid;
      console.log(rfid);
      input = document.getElementById('setrfid_input');
      input.value = rfid
  }
}
  http.send(null);
  
});


const socket = io()

let producer = null

roomidInput.value = "123"
nameInput.value = 'user_' + Math.round(Math.random() * 1000)

socket.request = function request(type, data = {}) {
  return new Promise((resolve, reject) => {
    socket.emit(type, data, (data) => {
      if (data.error) {
        reject(data.error)
      } else {
        resolve(data)
      }
    })
  })
}

let rc = null

function httpGet(theUrl){
  var http = new XMLHttpRequest();
  var url = theUrl;

  http.open("GET", url, true);
  
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      console.log("res : ", res);
      result = JSON.parse(res);

      var table = document.getElementById('table1')
      for (var i=0; i < result.length; i++)
      {
          var row = `<tr>
                      <td>${result[i].date}</td>
                      <td>${result[i].name}</td>
                      <td>${result[i].sex}</td>
                      <td>${result[i].kind}</td>
                      <td>${result[i].breath}</td>
                      <td>${result[i].temp}</td>
                      <td>${result[i].owner}</td>
                      <td>${result[i].rfid}</td>
                  </tr>`
          table.innerHTML += row
      }
      console.log("get the table successfully");
      }
  }
  http.send(null);
}

function animal_status(){
  console.log("show animal status");

  var http = new XMLHttpRequest();
  var url = "https://arc1el.iptime.org:3000/api/animal_status";

  http.open("GET", url, true);
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      result = JSON.parse(res)[0];
      console.log("res : ", result);

      //chart for breath
      var canvas1 = '<canvas id="breath-chart" width="300" height="380"></canvas>'
      div1 = document.getElementById('breath_div');
      div1.innerHTML = canvas1;

      new Chart(document.getElementById("breath-chart"), {
        type: 'doughnut',
        data: {
          labels: ["정상", "주의", "위험"],
          datasets: [
            {
              backgroundColor: ["#13bd7e", "#ffbe4c","#f53e50"],
              data: [result.breath1, result.breath2, 10]
            }
          ]
        },
        options: {
          title: {
              display: true,
              text: '호흡수'
          },
          legend: {
            display: false,
          }
      }
      });

      //chart for temp
      var canvas2 = '<canvas id="temp-chart" width="300" height="380"></canvas>'
      div2 = document.getElementById('temp_div');
      div2.innerHTML = canvas2;
      
      new Chart(document.getElementById("temp-chart"), {
        type: 'doughnut',
        data: {
          labels: ["정상", "주의", "위험"],
          datasets: [
            {
              backgroundColor: ["#13bd7e", "#ffbe4c","#f53e50"],
              data: [result.temp2, result.temp1, 10]
            }
          ]
        },
        options: {
            title: {
                display: true,
                text: '체온'
            },
            legend: {
              display: false,
            }
        }
      });
    }
  }
  http.send(null);
}

function home(){
  document.getElementById('table1').innerHTML = "";
  document.getElementById('breath_div').innerHTML = "";
  document.getElementById('temp_div').innerHTML = "";
  document.getElementById("total_h2").innerHTML = "Total Data"


  httpGet("https://arc1el.iptime.org:3000/api/recent_data");
  hide(monitor_breath_div)
  reveal(main_status)
  document.getElementById("indiv_h2").innerHTML = "Individual Data";
  animal_status();
  individual_data();
}

function individual_data(){
  console.log("show individual_data status");

  var http = new XMLHttpRequest();
  var url = "https://arc1el.iptime.org:3000/api/recent_animal_data";

  http.open("get", url, true);
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      result = JSON.parse(res);
      console.log("res : ", result, "len : ", result.length);

      div = document.getElementById('monitor_div')
      contents = ""

      for (i=0; i<=result.length; i++){
        dat = result[i]
        pet_name = dat.name;
        owner_name = dat.owner;
        kind = dat.kind;
        date = dat.date;
        rfid = dat.rfid;
        breath = dat.breath;
        temp = dat.temp;

        contents +=`<div class="individual_div" style="color:white;" onclick="search_func(`+rfid+`)">
        <h6>&nbsp&nbsp;${pet_name}  /  ${owner_name}</h6>              
        <h6>&nbsp&nbsp;${kind}</h6>
        <h6>&nbsp&nbsp;${date}</h6>                
        <h6>&nbsp&nbsp;${breath}회/분&nbsp;${temp}℃</h6></div>`

        div.innerHTML = contents

      }
    }
  }
  http.send(null);
}

function joinRoom(name, room_id) {
  reveal(main_status)
  hide(monitor_breath_div)
  var table = document.getElementById('table1')
  div1 = document.getElementById('breath_div');
  div2 = document.getElementById('temp_div');
  div2.innerHTML = "";
  div1.innerHTML = "";
  table.innerHTML="";
  if (rc && rc.isOpen()) {
    console.log('Already connected to a room')
  } else {
    
    httpGet("https://arc1el.iptime.org:3000/api/recent_data");
    animal_status();
    individual_data();
    initEnumerateDevices();

    rc = new RoomClient(localMedia, remoteVideos, remoteAudios, window.mediasoupClient, socket, room_id, name, roomOpen)

    addListeners();
  }
}

function search_func(data) {
  reveal(monitor_breath_div)
  document.getElementById("monitor_div").innerHTML = "";
  document.getElementById("indiv_h2").innerHTML = "";
  document.getElementById("total_h2").innerHTML = "Monitor";
  //for the test
  if (data == 3840000000261){
    data = "03840000000261";
  }
  console.log("recieved data : ", data);
  json = '{"data" : "' + data + '"}';
  var http = new XMLHttpRequest();
  var url = "https://arc1el.iptime.org:3000/api/search";
  params = json
  http.open("POST", url, true);
  http.setRequestHeader("Content-Type", "application/json");
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      console.log("res : ", res);
      result = JSON.parse(res);
      var table = document.getElementById('table1')
      table.innerHTML = "";
      temp_arr = [];
      breath_arr = [];
      for (var i=0; i < result.length; i++)
      {
          var row = `<tr>
                      <td>${result[i].date}</td>
                      <td>${result[i].name}</td>
                      <td>${result[i].sex}</td>
                      <td>${result[i].kind}</td>
                      <td>${result[i].breath}</td>
                      <td>${result[i].temp}</td>
                      <td>${result[i].owner}</td>
                      <td>${result[i].rfid}</td>
                  </tr>`
          table.innerHTML += row
          temp_arr.push(result[i].temp)
          breath_arr.push(result[i].breath)
      }
      console.log("temp data : ", temp_arr, "breath data : ", breath_arr);
      document.getElementById('breath_div').innerHTML = "";
      document.getElementById('temp_div').innerHTML = "";

      var canvas2 = '<canvas id="monitor_breath" width="400" height="200"></canvas>'
      div2 = document.getElementById('monitor_breath_div');
      div2.innerHTML = canvas2;
      label = Array(100).fill().map((v,i)=> i+1);
      var ctx = document.getElementById('monitor_breath');
      var myChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: label,
          datasets: [
            {
              label: 'Breath',
              data: breath_arr,
              fill: false,
              borderColor: '#877af5',
            },
            {
              label: 'Temperature',
              data: temp_arr,
              fill: false,
              borderColor: '#f53e50',
            },
          ]
        },
        options: {
          interaction: {
            mode: 'index',
            intersect: false,
          },
          stacked: false,
          plugins: {
            title: {
              display: true,
              text: 'Chart.js Line Chart - Multi Axis'
            }
          },
          scales: {
            xAxes: [{
              display: false,
            }],
          }
        }
      });
      hide(main_status)

    }
  }
  http.send(params);
}

function setOwner(rfid, owner){
  console.log("rfid : ", rfid, " owner : ", owner);
  json = '{"rfid" : "' + rfid + '", "owner" : "' + owner + '"}';
  console.log(json)

  var http = new XMLHttpRequest();
  var url = "https://arc1el.iptime.org:3000/api/set_owner";
  params = json
  http.open("POST", url, true);
  http.setRequestHeader("Content-Type", "application/json");
  http.onreadystatechange = function() {//Call a function when the state changes.
  if(http.readyState == 4 && http.status == 200) {
      res = http.responseText;
      console.log("res : ", res);
      if(res == "no"){
        alert("정보를 다시 확인해주세요");
      }else{
        alert("성공적으로 등록되었습니다.")
      }
    }
  }
  http.send(params);
}

function roomOpen() {
  login.className = 'hidden'
  hide(startAudioButton)
  hide(stopAudioButton)
  hide(startVideoButton)
  hide(stopVideoButton)
  hide(startScreenButton)
  hide(stopScreenButton)
  hide(exitButton)
  reveal(devicesButton)
  reveal(mainDiv)
  control.className = ''
  mainDiv.className = ''
  hide(videoMedia)
}

function hide(elem) {
  elem.className = 'hidden'
}

function reveal(elem) {
  elem.className = ''
}

function addListeners() {
  rc.on(RoomClient.EVENTS.startScreen, () => {
    hide(startScreenButton)
    reveal(stopScreenButton)
  })

  rc.on(RoomClient.EVENTS.stopScreen, () => {
    hide(stopScreenButton)
    reveal(startScreenButton)
  })

  rc.on(RoomClient.EVENTS.stopAudio, () => {
    hide(stopAudioButton)
    reveal(startAudioButton)
  })
  rc.on(RoomClient.EVENTS.startAudio, () => {
    hide(startAudioButton)
    reveal(stopAudioButton)
  })

  rc.on(RoomClient.EVENTS.startVideo, () => {
    hide(startVideoButton)
    reveal(stopVideoButton)
  })
  rc.on(RoomClient.EVENTS.stopVideo, () => {
    hide(stopVideoButton)
    reveal(startVideoButton)
  })
  rc.on(RoomClient.EVENTS.exitRoom, () => {
    hide(control)
    hide(devicesList)
    hide(videoMedia)
    hide(devicesButton)
    hide(mainDiv)
    reveal(login)
  })
}

let isEnumerateDevices = false

function initEnumerateDevices() {
  // Many browsers, without the consent of getUserMedia, cannot enumerate the devices.
  if (isEnumerateDevices) return

  const constraints = {
    audio: true,
    video: true
  }

  navigator.mediaDevices
    .getUserMedia(constraints)
    .then((stream) => {
      enumerateDevices()
      stream.getTracks().forEach(function (track) {
        track.stop()
      })
    })
    .catch((err) => {
      console.error('Access denied for audio/video: ', err)
    })
}

function enumerateDevices() {
  // Load mediaDevice options
  navigator.mediaDevices.enumerateDevices().then((devices) =>
    devices.forEach((device) => {
      let el = null
      if ('audioinput' === device.kind) {
        el = audioSelect
      } else if ('videoinput' === device.kind) {
        el = videoSelect
      }
      if (!el) return

      let option = document.createElement('option')
      option.value = device.deviceId
      option.innerText = device.label
      el.appendChild(option)
      isEnumerateDevices = true
    })
  )
}
