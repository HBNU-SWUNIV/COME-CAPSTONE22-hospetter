long lastMillis = 0;
long loops = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  long currentMillis = millis();
  loops++;
  
  /* By doing complex math, reading sensors, using the "delay" function,
  *  etc you will increase the time required to finish the loop,
  *  which will decrease the number of loops per second.
  */

  if(currentMillis - lastMillis > 1000){
    Serial.print("Loops last second:");
    Serial.println(loops);
    
    lastMillis = currentMillis;
    loops = 0;
  }
}
