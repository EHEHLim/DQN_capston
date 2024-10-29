#include <Arduino_FreeRTOS.h>
#include <Stepper.h>

#define DIR_PIN 11
#define STEP_PIN 12
#define ENV_PIN 13
#define MAX_STEPS 6000

#define M1 22
#define M2 24
#define M3 26

#define underMotorOneCycle 60
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

Stepper stepper(underMotorOneCycle, IN1, IN2, IN3, IN4);

int maxSpeedDelay = 2000;
int minSpeedDelay = 100;
int accelerationStep = 5;
int currentSpeedDelay = maxSpeedDelay;
int targetSpeedDelay = 100;
int steps = 0;
bool direction = HIGH;
bool Enable = LOW;
bool isReseting = false;

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

  pinMode(M1,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(M3,OUTPUT);
  digitalWrite(M1,HIGH);
  digitalWrite(M2,LOW);
  digitalWrite(M3,LOW);
}

static void Task1(void* pvParameters){
  while(true) {
      if(isReseting == false){
      Serial.print(currentSpeedDelay);
      Serial.println(steps);
      vTaskDelay(100);
    } 
  }
}

static void Task2(void* pvParameters){
  while(true) {
    if(isReseting == false){
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
    else {
      stepper.step(underMotorOneCycle);
      vTaskDelay(50);
    }
  }
}

void loop() {
  
}