const int sampleWindow = 47; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

int data[16000];
int i = 0;

void setup() 
{
 
   Serial.begin(9600);

}


void loop() 
{
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
        sample = analogRead(0);
        data[i] = sample;
        i = i+1;
        if (sample < 1024)  // toss out spurious readings
        {
           if (sample > signalMax)
           {
              signalMax = sample;  // save just the max levels
           }
           else if (sample < signalMin)
           {
              signalMin = sample;  // save just the min levels
           }
        }

        if(i == 15999){
          Serial.println(data[0], data[15999]);
          i = 0;
        }
     }
     peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
     double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  
    if (volts > 0.03)
     {
     digitalWrite (7,1);
     }
  
    else
    {
      digitalWrite (7,0);
    }
  
     
     Serial.println(volts);

  
  }
