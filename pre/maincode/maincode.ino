
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "Ni"; 
const char* password =  "niteesh@"; 
const char* mqtt_server = "91.121.93.94";
const int mqtt_port = 1883;

int timecount=25;
int maxlim=4;
int catofmove[5]={0};
String msg="";

WiFiClient espClient;
PubSubClient client(espClient);

void Wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Wifi Reconnecting...");
  WiFi.begin(ssid, password);
}

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

  WiFi.disconnect(true);
  delay(1000);
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WIFI network..."); 


  // Connect to MQTT broker
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32_Client")) {
      Serial.println("connected");
      // client.subscribe("test_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
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
   timecount=25;   
  }    
    
   delay(50);
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println((char*)message);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Client")) {
      Serial.println("connected");
      // client.subscribe("test_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");      
      delay(5000);
    }
  }
}

void sendmsg(int i)
{
  if(i==0)
  msg="Heavy movement";
  if(i==1)
  msg="Doing work";
  if(i==2)
  msg="Moving body parts";
  if(i==3)
  msg="Little movement";
  if(i==4)
  msg="No movement";  
  
  if(WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected()){
      reconnect();
    }
     client.publish("test_topic",msg.c_str());
     Serial.println(msg);
     client.loop();
  }
}




