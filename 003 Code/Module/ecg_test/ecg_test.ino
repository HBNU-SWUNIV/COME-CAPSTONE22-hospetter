void setup() {
  Serial.begin(9600);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
}

void loop() {
  if((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println('!');
  }
  else{
    Serial.println(analogRead(A7));
  }
  delay(10);
}
