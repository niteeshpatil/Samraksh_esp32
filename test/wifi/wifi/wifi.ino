#include "WiFi.h"
 
const char* ssid = "Ni"; //choose your wireless ssid
const char* password =  "niteesh@"; //put your wireless password here.


void Wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Successfully connected to Access Point");
}

void Get_IPAddress(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WIFI is connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void Wifi_disconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  // Serial.println("Disconnected from WIFI access point");
  // Serial.print("WiFi lost connection. Reason: ");
  // Serial.println(info.disconnected.reason);
  Serial.println("Reconnecting...");
  WiFi.begin(ssid, password);
}

void setup(){
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.onEvent(Wifi_connected,SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(Get_IPAddress, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(Wifi_disconnected, SYSTEM_EVENT_STA_DISCONNECTED); 
  WiFi.begin(ssid, password);
  Serial.println("Waiting for WIFI network...");
}

void loop(){

  if(WiFi.status() == WL_CONNECTED)
  { 
     Serial.println("wifi connected..");
  }
  delay(500);
}
