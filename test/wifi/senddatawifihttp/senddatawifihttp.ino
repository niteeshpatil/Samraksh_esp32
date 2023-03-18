#include "WiFi.h"
#include "HTTPClient.h"
 
const char* ssid = "Niteesh"; //choose your wireless ssid
const char* password =  "niteesh@1234"; //put your wireless password here.
const char* serverUrl = "http://your_server_ip_address/sensor-data";
void setup() {
  
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi..");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
 
void loop() {
  float sensorValue = readSensor(); // read sensor value here
  String postData = String(sensorValue); // convert sensor value to a string
  postData += "&timestamp=" + String(millis()); // append timestamp to string
  
  Serial.println("Posting data to server...");
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(postData);
  if (httpResponseCode > 0) {
    Serial.println("HTTP Response code: " + String(httpResponseCode));
  } else {
    Serial.println("Error posting data to server");
  }
  http.end();
  
  delay(5000); // wait 5 seconds before sending next data
}

float readSensor() {
  // code to read sensor value goes here
  return random(0, 100);
}