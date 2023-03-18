#include "WiFi.h"

const char* ssid = "Niteesh"; //choose your wireless ssid
const char* password =  "niteesh@1234"; //put your wireless password here.
 

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //show ip address when connected on serial monitor.

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected");

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><head><title>wifi http</title></head><body><h1>Hello Avengers!</h1></body></html>");

    delay(10);
    client.stop();
    Serial.println("Client disconnected");
  }
}
