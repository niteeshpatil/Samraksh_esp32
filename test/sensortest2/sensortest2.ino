#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

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

  delay(500);
}