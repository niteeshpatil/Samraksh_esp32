#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

int timecount=45;
int maxlim=4;
int catofmove[5]={0};

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
float X=0;
float Y=0;
float Z=0;
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("Starting ADXL345 test...");

  if (!accel.begin()) {
    Serial.println("ADXL345 not found");
    while (1);
  }
}
void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  float x=event.acceleration.x;
  float y=event.acceleration.y;
  float z=event.acceleration.z;
  float res=sqrt((X-x)*(X-x)+(Y-y)*(Y-y)+(Z-z)*(Z-z));
  X=x; 
  Y=y;
  Z=z; 
  Serial.println(res);

 
  if(res>=15)
  catofmove[0]+=1;
  if(res<15 && res>=3.5)
  catofmove[1]+=1;
  if(res<3.5 && res>=1)
  catofmove[2]+=1;
  if(res<1 && res>=0.25)
  catofmove[3]+=1;
  if(res<0.25)
  catofmove[4]+=1;
    
  timecount--;

  if(timecount<=0)
  {
    for(int i=0;i<5;i++)
    {
      if(catofmove[i]>=maxlim)
      {
          sendmsg(i); 
          break;
      }
    }
    for(int i=0;i<5;i++)
    { 
       catofmove[i]=0;
    }
   Serial.println("time out"); 
   timecount=45;   
  }  
  delay(100);
}

void sendmsg(int i)
{
  if(i==0)
  Serial.println("Heavy movement");
  if(i==1)
  Serial.println("Doing work");
  if(i==2)
  Serial.println("Moving body parts");
  if(i==3)
  Serial.println("Little movement");
  if(i==4)
  Serial.println("No movement");  
}
