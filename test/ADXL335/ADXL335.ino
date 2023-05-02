const int xPin = 15;  // X-axis input pin
const int yPin = 2;  // Y-axis input pin
const int zPin = 4;  // Z-axis input pin

float X=0;
float Y=0;
float Z=0;


const float Sensitivity = 0.001;  // X-axis sensitivity factor


void setup() {
  Serial.begin(115200);
}

void loop() {
  float x = analogRead(xPin) * Sensitivity;
  float y = analogRead(yPin) * Sensitivity;
  float z= analogRead(zPin) * Sensitivity;
  float res=sqrt((X-x)*(X-x)+(Y-y)*(Y-y)+(Z-z)*(Z-z));
  X=x; 
  Y=y;
  Z=z;  

  // Serial.print("res: ");
  Serial.println(res);
  // Serial.print("X: ");
  // Serial.print(x);
  // Serial.print(", Y: ");
  // Serial.print(y);
  // Serial.print(", Z: ");
  // Serial.println(z);

  delay(50);
}




