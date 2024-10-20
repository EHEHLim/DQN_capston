#include <Arduino_FreeRTOS.h>

#define STEP_PIN 4
#define DIR_PIN 12
#define ENV_PIN 6
#define MAX_STEPS 6000

int maxSpeedDelay = 2000;
int minSpeedDelay = 250;
int accelerationStep = 5;
int currentSpeedDelay = maxSpeedDelay;
int targetSpeedDelay = 200;
int steps = 0;
bool direction = HIGH;
bool Enable = LOW;


void stepMotor(int speedDelay) {
  digitalWrite(STEP_PIN, HIGH);  // STEP 핀 HIGH
  delayMicroseconds(speedDelay); // 속도에 맞춘 지연 시간
  digitalWrite(STEP_PIN, LOW);   // STEP 핀 LOW
  delayMicroseconds(speedDelay); // 속도에 맞춘 지연 시간
  steps += (direction ? 1 : -1);
}

void accelerate(int targetSpeed){
  if(currentSpeedDelay > targetSpeed) {
    currentSpeedDelay -= accelerationStep;
  }
  if(currentSpeedDelay < targetSpeed) {
    currentSpeedDelay += accelerationStep;
  }

}

 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENV_PIN,OUTPUT);
  pinMode(STEP_PIN,OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
 
  digitalWrite(ENV_PIN, Enable);
  digitalWrite(DIR_PIN,HIGH);

  xTaskCreate(Task1, "Task1", 2048, NULL, 2, NULL);
  xTaskCreate(Task2, "Task2", 2048, NULL, 1, NULL);
}

static void Task1(void* pvParameters){
  while(true) {
    
    Serial.print(currentSpeedDelay);
    Serial.println(steps);
    vTaskDelay(100);
    
  }
}

static void Task2(void* pvParameters){
  while(true) {
    accelerate(targetSpeedDelay);
    stepMotor(currentSpeedDelay);

    if((steps >= 5500 && direction == HIGH) || (steps <= 500 && direction == LOW)){
      targetSpeedDelay = maxSpeedDelay;
    }
    else {
      targetSpeedDelay = minSpeedDelay;
    }

    if((steps >= 5990 && direction == HIGH) || (steps <= 10 && direction == LOW)){
      direction = !direction;
      digitalWrite(DIR_PIN,direction);
    }
  }
}

void loop() {

}