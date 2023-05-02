#include <WiFi.h>
#include <PubSubClient.h>

const char* mqtt_server = "91.121.93.94";
const int mqtt_port = 1883;
const char* ssid = "Ni"; //choose your wireless ssid
const char* password =  "niteesh@"; //put your wireless password here.
const char* stopic="s_mngd123";
const char* rtopic="r_mngd123";
double limit=-1;
int deviceID=1; //deviceId is 1 or 2 based on device
int seldevice=-1; //selected deive is 1 or 2 based on input from app
int condition=0; //0 unkonown, 1 stable , 2 unstable
int isset=0; //0 not conncected, 1 connected
//isset{0,1} condition{0,1,2}

WiFiClient espClient;
PubSubClient client(espClient);

void Wifi_connected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Successfully connected to Access Point");
}

void Get_IPAddress(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WIFI is connected!");
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
      Serial.println(" retrying in 2.5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if(WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected()){
      reconnect();
    }

    if(isset==0)
    {
      Serial.println("set limit");
      client.subscribe(rtopic);
      delay(100);
    }
    else
    {
      client.publish(stopic, "111"); //"deviceId isset condition"
      Serial.println("sending data ...");
      delay(100);      
    }
     client.loop();
  }    
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
  // Serial.print("revmsg: ");
  // Serial.println(convertedPayload);
  // Serial.println(length);
   Serial.println();
  int result = sscanf(convertedPayload,"%d %lf",&seldevice,&limit);
  if (result != 2) {
    Serial.println("Error parsing input string");
  } else {
    Serial.print("limit: ");
    Serial.println(limit);
    Serial.print("seleted device: ");
    Serial.println(seldevice);
    if(limit>0 && seldevice==deviceID)
    isset=1;
    condition=1;
  }
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




