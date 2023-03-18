#include <WiFi.h>
#include <PubSubClient.h>

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

void setup() {
  Serial.begin(115200);

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
  if(WiFi.status() == WL_CONNECTED)
  {
    if (!client.connected()){
      reconnect();
    }
     publishMessage();
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

void publishMessage() {
  client.publish("test_topic", "Hello from ESP32");
  Serial.println("can pubish");
}


