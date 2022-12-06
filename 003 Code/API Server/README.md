## API Server

hospetter의 API Server는 DB서버의 데이터를 클라이언트에게 제공하는 기능에 한하여 접근할 수 있도록 합니다.

# Installing and Running
  - Install API server and WAS
  ```sh
  cd {Project Directory Path(API or WAS)}
  npm install
  ```
  - Running Application
  ```sh
  npm start
  ```

# 제공 기능

* **recent_data :: *GET***

    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">

    ```json
     {
          "rfid": "0000000000000",
          "owner": "김현민",
          "name": "나",
          "sex": "남",
          "kind": "인간",
          "neuter": "NO",
          "org": "대전",
          "office": "042",
          "date": "2022-11-30_12:42:28",
          "breath": 186,
          "temp": 24.43
      }, ... // 2 more elements, limit 3
    ```
    </div>
    </details>

* **recent_animal_data :: *GET***

    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
          "rfid": "0000000000000",
          "owner": "정봉경",
          "name": "아롱",
          "sex": "수컷",
          "kind": "푸들",
          "neuter": "중성",
          "date": "2080-11-23_10:37:03",
          "breath": 30,
          "temp": 37.5
      }, // ... more
    ```
    </div>
    </details>
    
* **recent_rfid :: *GET***

    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
          "date": "2022-11-30_13:59:31",
          "breath": 65,
          "temp": 31.19,
          "rfid": ""
      }
    ```
    </div>
    </details>
      
* **animal_status :: *GET***

    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
          "temp1": 4095,
          "temp2": 25256,
          "breath1": 24457,
          "breath2": 4894
      }
    ```
    </div>
    </details>
      
* **get_sensor_data :: *POST***

    <details>
    <summary>Request<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```c
    xTaskCreatePinnedToCore ( post_func,"post_func", 10000, NULL, 0, &post_handler, CORE2 ); //Transfer from Arduino to data api server in json format by POST method 
    ```
    </div>
    </details>
    
    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```js
    "ok"
    ```
    </div>
    </details>
      
* **get_temp_breath :: *POST***

    <details>
    <summary>Request<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
      "type":"temp" //"temp" or "breath"
      ,"rfid":"00000000000000"
      }
    ```
    </div>
    </details>
    
    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    [{"temp":37},{"temp":39.1},{"temp":37.7},{"temp":38.9},{"temp":38.6},{"temp":37},{"temp":37.9},{"temp":38.3},{"temp":38.5},{"temp":38.3},{"temp":38.9},{"temp":39}, {"temp":38.3},{"temp":38.7},{"temp":38.6},{"temp":38.2},...]
    ```
    </div>
    </details>
    
* **get_rfid_info :: *POST***

    <details>
    <summary>Requeste<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
      "rfid":"000000000000000"
      }
    ```
    </div>
    </details>
  
    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {"rfid":"000000000000000"
    ,"owner":"정봉경"
    ,"name":"아롱"
    ,"sex":"수컷"
    ,"kind":"푸들"
    ,"neuter":"중성"
    ,"org":"세종특별자치시"
    ,"office":"044-000-0000"
    }
    ```
    </div>
    </details>

* **search :: *POST***

    <details>
    <summary>Request<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
      "data":"정봉경" //owner name
      }
    ```
    </div>
    </details>
  
    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
    
    ```json
    [{"rfid":"00000000000000","date":"2022-11-23_10:37:03","breath":29,"temp":37,"owner":"정봉경","name":"아롱","sex":"수컷","kind":"푸들","neuter":"중성","org":"세종특별자치시","office":"044-000-0000"},...]
    ```
     /div>
    </details>

* **set_owner :: *POST***

    <details>
    <summary>Request<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```json
    {
      "rfid":"000000000000000"
      ,"owner":"김현민"
      }
    ```
    </div>
    </details>
  
    <details>
    <summary>Response<b>(접기/펼치기)</b></summary>
    <div markdown="1">
      
    ```js
    "ok" or "no" //If the data in the request is registered in the government API, insert it into the DB and respond "ok"
    ```
    </div>
    </details>

### 사용 예제

클라이언트 측에선 아래와 같은 경로로 API Server에 접근합니다.

```javascript
/api/제공 기능
```
아래와 같이 POST 요청과 함께 전송된 데이터를 조합한 SQL 생성 후 DB서버에 접근합니다.

```javascript
console.log(req.body);
  jsondata = req.body; //POST의 body로 넘어온 JSON 데이터
  rfid = jsondata.rfid;
  console.log(rfid);
  
  try{
    sql = "select * from animal where rfid='" + rfid + "' limit 1"
    data = await send_query(sql); //DB서버에 SQL 전송
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
```

## 외부 API Server 활용

```javascript
  //sql : json format. req.body.sql
  key = "API키";
  data = req.body;
  rfid = data.rfid;
  owner = data.owner;

  if(rfid != "" && owner !=""){
    console.log("data is not null. ok!");
    try{
      const options = {
        uri: "http://apis.data.go.kr/1543061/animalInfoSrvc/animalInfo",
        qs:{
          serviceKey : key, //API 키
          dog_reg_no : rfid, //등록 조회할 입력받은 강아지 RFID
          owner_nm : owner //입력받은 주인 이름
        }
      };
      request(options,async function(err,response,body){
        dat = xmljs.xml2json(body, {compact: true, spaces: 4});
        dat = JSON.parse(dat).response.body.item;
        console.log(dat);
        
        dog_name = dat.dogNm._text; //강아지 이름
        dog_sex = dat.sexNm._text; //성별
        dog_kind = dat.kindNm._text; //종
        dog_neuter = dat.neuterYn._text; //중성화 여부
        dog_org = dat.orgNm._text; //등록기관명
        dog_office = dat.officeTel._text; //기관 전화번호

        //SQL문 DB서버로 전송
        try{
          sql = "insert into animal(rfid, owner, name, sex, kind, neuter, org, office)values('";
          sql += rfid + "','" + owner + "','" + dog_name + "','" + dog_sex + "','";
          sql += dog_kind + "','" + dog_neuter + "','" + dog_org + "','" + dog_office + "')";
          result = await send_query(sql);
          res.send("ok");
        }catch (e){
          console.log("cant't handle the query", e);
        }
      })
    }catch{}
  }
  else{
    res.send("no");
  }
```

## README

1.https://www.animal.go.kr/front/awtis/record/recordConfirmList.do?menuNo=2000000011


