#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Ni"; 
const char* password =  "niteesh@"; 
const char* mqtt_server = "91.121.93.94";
const int mqtt_port = 1883;

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

StaticJsonDocument<128> jsonDoc;
char jsonBuffer[128];

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
  Serial.print("res: ");
  Serial.println(res);
  Serial.print("X: ");
  Serial.print(x);
  Serial.print("  Y: ");
  Serial.print(y);
  Serial.print("  Z: ");
  Serial.print(z);
  Serial.println(" m/s^2");
  
  if(WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected()){
      reconnect();
    }
     unsigned long timenow= millis();

     jsonDoc["x"] = x;
     jsonDoc["y"] = y;
     jsonDoc["z"] = z;
     jsonDoc["res"] = res;
     jsonDoc["time"] = timenow;
      
     serializeJson(jsonDoc,jsonBuffer);
     
     client.publish("test_topic",jsonBuffer);
     Serial.println("can pubish");
     client.loop();
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



