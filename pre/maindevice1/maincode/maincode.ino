
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "Redmi"; 
const char* password =  "manognash@19"; 
const char* mqtt_server = "91.121.93.94";
const int mqtt_port = 1883;
const char* stopic="s_mngd123";
const char* rtopic="r_mngd123";

int timecount=25;//25
int maxlim=4;
int catofmove=0;
String msg="";

int mobile_id=-1; //idetify the mobile from which limit is set
int deviceID=2; //deviceId is 1 or 2 based on device
int seldevice=-1; //selected deive is 1 or 2 based on input from app
double limit=-1;
int condition=0; //0 unkonown, 1 stable , 2 unstable
int isset=0; //0 not conncected, 1 connected
//isset{0,1} condition{0,1,2}

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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}



void loop() {

  if(isset==0)
  {
    if(WiFi.status() == WL_CONNECTED)
    {
      if (!client.connected()){
      reconnect();
       }
      Serial.println("set limit");
      client.subscribe(rtopic);
      delay(50);
      msg = String(isset) + String(condition);
      client.publish(stopic,msg.c_str());
      Serial.println(msg);
      delay(50);
    }

  }
  else
  {  
  sensors_event_t event;
  accel.getEvent(&event);
  float x=event.acceleration.x;
  float y=event.acceleration.y;
  float z=event.acceleration.z;
  float res=sqrt((X-x)*(X-x)+(Y-y)*(Y-y)+(Z-z)*(Z-z));
  X=x; 
  Y=y;
  Z=z;  
  // Serial.println(res);

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
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  char* curmsg=reinterpret_cast<char*>(payload);
  char convertedPayload[length];  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    convertedPayload[i]=curmsg[i];
  }
   Serial.println();
   //message from app should be in from of "device limit mobile_id" that is "1 3.5 4586" or "2 5.0 4586" 
  int result = sscanf(convertedPayload,"%d %lf %d",&seldevice,&limit,&mobile_id);
  if (result != 3) {
    Serial.println("Error parsing input string");
  } else {
    Serial.print("seleted device: ");
    Serial.println(seldevice);
    Serial.print("limit: ");
    Serial.println(limit);
    Serial.print("mobile_id: ");
    Serial.println(mobile_id);
    if(limit>0 && seldevice==deviceID)
    {
      isset=1;
      condition=1;  
      //sending ack
      sendmsg(); 
      client.unsubscribe(rtopic);
    }
  }
  
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");      
      delay(5000);
    }
  }
}

void sendmsg()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    msg =String(deviceID)+String(isset) + String(condition)+String(mobile_id); //"deviceId isset condition mobile_id" that is "1104456" or "2004456" etc
    if (!client.connected()){
      reconnect();
    }
     client.publish(stopic,msg.c_str());
     Serial.println(condition);
     condition=1;
  }
}




