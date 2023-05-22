
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>


int timecount=25;//25
int maxlim=4;
int catofmove=0;
String msg="";

int deviceID=1; //deviceId is 1 or 2 based on device
int seldevice=-1; //selected deive is 1 or 2 based on input from app
double limit=7;//changing
int condition=0; //0 unkonown, 1 stable , 2 unstable
int isset=1; //0 not conncected, 1 connected
//isset{0,1} condition{0,1,2}


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

  if(res>=limit)
  catofmove++;

    
  timecount--;

  if(timecount<=0)
  {
      if(catofmove>=maxlim)
      {  
         condition=2; 
      }
  sendmsg();
   catofmove=0;
   timecount=25;   
  }    
    
   delay(50);
}




void sendmsg()
{
    msg =String(deviceID)+String(isset) + String(condition); //"deviceId isset condition" that is "110" or "200" etc
    Serial.println("condition" + condition);
    condition=1;
}




