var express = require('express');
var request = require('request');
var xmljs = require("xml-js");
var router = express.Router();
var fs = require('fs');
const mysql = require('mysql');
const dbconfig = require('../config/database.js');
const connection = mysql.createConnection(dbconfig);
connection.connect();

//res.sendStatus(200);

router.get('/', function(req, res, next) {
  res.send("ok");
});

router.post('/', function(req, res, next) {
  res.send("ok");
});

//recent data(5 limit)
router.get("/api/recent_data", async function(req, res, next) {
  sql = "SELECT * FROM animal inner join sensor on animal.rfid = sensor.rfid order by sensor.date desc limit 3";
  data = await send_query(sql);
  res.send(data);
});

router.get("/api/recent_animal_data", async function(req, res, next) {
  sql = "SELECT animal.rfid, owner, name, sex, kind, neuter, max(date) as date, sensor.breath, sensor.temp FROM animal inner join sensor on animal.rfid = sensor.rfid group by animal.rfid";
  data = await send_query(sql);
  res.send(data);
});

//api for getting sensordata
router.post('/api/get_sensor_data', function(req, res) {
  //reqbody : Ardino json
  jsondata = req.body;
  
  //Saving json data in path and Log to dastabase server
  try{
    save_sensor_data(jsondata);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }

  res.send("ok");
});

router.post('/api/get_temp_breath', async function(req, res) {
  //reqbody : Ardino json
  jsondata = req.body;
  type = jsondata.type;
  rfid = jsondata.rfid;

  sql = 'SELECT '+ type +' FROM `sensor` WHERE rfid="' + rfid + '" order by date desc';
  
  //Saving json data in path and Log to dastabase server
  try{
    data = await send_query(sql);
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
});

router.post('/api/get_rfid_info', async function(req, res) {
  console.log(req.body);
  jsondata = req.body;
  rfid = jsondata.rfid;
  console.log(rfid);
  
  //Saving json data in path and Log to dastabase server
  try{
    sql = "select * from animal where rfid='" + rfid + "' limit 1"
    data = await send_query(sql);
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
});

router.post('/api/search', async function(req, res) {
  console.log(req.body);
  jsondata = req.body;
  data = jsondata.data;
  console.log(data);
  
  //Saving json data in path and Log to dastabase server
  try{
    sql = 'select * from sensor NATURAL JOIN animal where owner="' + data + '" or name="';
    sql += data+ '" or rfid="' + data+ '" order by date DESC  limit 100;'
    data = await send_query(sql);
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
});

router.get('/api/recent_rfid', async function(req, res) {
  //Saving json data in path and Log to dastabase server
  try{
    sql = "select * from sensor order by date desc limit 1";
    data = await send_query(sql);
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
});

router.get('/api/animal_status', async function(req, res) {
  //Saving json data in path and Log to dastabase server
  try{
    sql = 'select (select count(*) from sensor NATURAL JOIN animal where temp>="38.9" limit 1000)temp1,'
    sql += '(select count(*) from sensor NATURAL JOIN animal where temp<"38.9" limit 1000)temp2,'
    sql += '(select count(*) from sensor NATURAL JOIN animal where breath<="29" limit 1000)breath1,'
    sql += '(select count(*) from sensor NATURAL JOIN animal where breath>"29" limit 1000)breath2';
    data = await send_query(sql);
    res.send(data);
  }catch (e){
    console.log("cant't save data. error : ", e);
  }
});


router.post('/api/query', async function(req, res) {
  //sql : json format. req.body.sql
  sql = req.body.sql;

  //send sql to db server
  try{
    result = await send_query(sql);
  }catch (e){
    console.log("cant't handle the query", e);
  }
  res.send(result);
  console.log("result = ", result);
});

router.post('/api/set_owner', async function(req, res) {
  //sql : json format. req.body.sql
  key = "User API KEY";
  data = req.body;
  rfid = data.rfid;
  owner = data.owner;

  if(rfid != "" && owner !=""){
    console.log("data is not null. ok!");
    try{
      const options = {
        uri: "http://apis.data.go.kr/1543061/animalInfoSrvc/animalInfo",
        qs:{
          serviceKey : key,
          dog_reg_no : rfid,
          owner_nm : owner
        }
      };
      request(options,async function(err,response,body){
        dat = xmljs.xml2json(body, {compact: true, spaces: 4});
        dat = JSON.parse(dat).response.body.item;
        console.log(dat);
        
        dog_name = dat.dogNm._text;
        dog_sex = dat.sexNm._text;
        dog_kind = dat.kindNm._text;
        dog_neuter = dat.neuterYn._text;
        dog_org = dat.orgNm._text;
        dog_office = dat.officeTel._text;

        //send sql to db server
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
});

//Sending the query function
function send_query(sql){
  return new Promise((resolve, reject) => {
    try{
      connection.query(sql, (error, datas, fields) => {
        if (error) {
          reject(error);
          console.log("sorry, i can't send query");
        }
        else{
          resolve(datas);
        } 
      });
    }catch{}
  })
};

//Making date string function
function get_date_string(){
  data = new Date();
  year = data.getFullYear();
  month = (data.getMonth() + 1).toString().padStart(2, "0");
  date = data.getDate();
  hours = data.getHours();
  if (parseInt(hours) < 10){
    hours = "0"+ hours
  }
  minutes = data.getMinutes();
  if (parseInt(minutes) < 10){
    minutes = "0"+ minutes
  }
  seconds = data.getSeconds().toString().padStart(2, '0');
  //millis = data.getMilliseconds();

  //date_string YY-MM-DD_hh:mm:ss:milis
  date_string = year + "-" + month + "-" + date + "_" 
  + hours + ":" + minutes + ":" + seconds; //+ ":" + millis;

  return date_string;
}

async function save_sensor_data(data){
  try{

    date = get_date_string();
    //console.log("received data : ", JSON.stringify(data));
    string = JSON.stringify(data).slice(2, -5).replace(/\\/g, "");
    json = JSON.parse(string)
    console.log("json : ", json)
    
    temp = json.temp;
    rfid = json.rfid;
    breath = json.breath;

    console.log(breath, temp, rfid)
    
    if (breath != 0 || rfid != ""){
      sql = "insert into sensor(date, breath, temp, rfid)values('";
      sql += date + "','" + breath + "','" + temp + "','" + rfid + "')";
      await send_query(sql);
    }
    else{
      console.log("check the values are right (temp, rfid, breath");
    }
  }catch (e){
    console.log("sorry, i can't read sensor data!");
    console.log(e)
  }
}

module.exports = router;

