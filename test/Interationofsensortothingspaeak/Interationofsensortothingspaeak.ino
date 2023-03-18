#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "HTTPClient.h"
#include "WiFi.h"

const char* ssid = "Ni"; 
const char* password =  "niteesh@"; 
const char* apiKey = "X4ICSBAHDXSMG5U7";
const long channelId = 2046777;


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
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + String(apiKey) + "&field1=" + String(x)+ "&field2=" + String(y)+ "&field3=" + String(z)+ "&field4=" + String(res);
    http.begin(url);
    int httpCode = http.GET();
    Serial.println(httpCode);
    http.end();
  }
  delay(600); 
  
}