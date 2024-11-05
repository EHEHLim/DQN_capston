#define DIR_PIN 6
#define STEP_PIN 7
#define ENV_PIN 5
#define MAX_STEPS 6000

int maxSpeedDelay = 2000;
int minSpeedDelay = 300;
int accelerationStep = 5;
int currentSpeedDelay = maxSpeedDelay;
int targetSpeedDelay = 300;
int steps = 0;
bool direction = HIGH;
bool Enable = LOW;
bool isReseting = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENV_PIN,OUTPUT);
  pinMode(STEP_PIN,OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
 
  digitalWrite(ENV_PIN, Enable);
  digitalWrite(DIR_PIN,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(isReseting == false){
    accelerate(targetSpeedDelay);
    stepMotor(currentSpeedDelay);

    if((steps >= 5700 && direction == HIGH) || (steps <= 300 && direction == LOW)){
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
