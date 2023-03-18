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

void setup(){
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);
  
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WIFI network...");
}

void loop() {
  // Send data to ThingSpeak
  if(WiFi.status() == WL_CONNECTED)
  {
      HTTPClient http;
     // String url = "http://api.thingspeak.com/update?api_key=" + String(apiKey) + "&field1=" + String(irData);
     String url = "http://api.thingspeak.com/update?api_key=" + String(apiKey) + "&field1=" + String(1) + "&field2=" + String(2) + "&field3=" + String(3)+ "&field4=" + String(4);
     http.begin(url);
     int httpCode = http.GET();
     Serial.println(httpCode);
     http.end();
  }

  delay(600);
}

