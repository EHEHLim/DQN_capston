#include <Wire.h>
float Degree = 0;
float Rev = 0;
int RPM = 0;
int steps = 0;
int speed = 0;;
int targetSpeed =200;
int dir = 0;

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(38400);
  Wire.begin(1);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  /**
  if(Serial1.available()){
    String inString = Serial1.readStringUntil('\n');
    int index1 = inString.indexOf(',');
    int index2 = inString.indexOf(',',index1+1);
    int index3 = inString.indexOf(',',index2+1);
    int index4 = inString.length();
 
    float Degree = (inString.substring(index1+1,index2).toFloat());
    while(Degree > 360){
      Degree = Degree - 360;
    }
    float Rev = inString.substring(index2+1,index3).toFloat();
    int RPM = inString.substring(index3+1, index4).toFloat();
 
    Serial.print("Degree:");
    Serial.print(Degree,1);
    Serial.print(",");
    Serial.print("Rev:");
    Serial.print(Rev,3);
    Serial.print(",");
    Serial.print("RPM:");
    Serial.print(RPM);
    Serial.print('\n');
    delay(50);
  }
  **/
  Serial.print(speed);
  Serial.print(" ");
  Serial.println(steps);
}

void receiveEvent(int howMany){
  while(Wire.available()>1){
    speed = Wire.read();
  }
  steps = Wire.read();
}

void requestEvent() {
  Wire.write(targetSpeed);
  Wire.write(dir);
}