int pingTime_front;
int distance_front;
int pingTime_side;
int distance_side;
#define trigPinF 5
#define echoPinF 4
#define trigPinS 7
#define echoPinS 8

int ENA = A5;       //define DC motor pins for mobility
int ENB = A6;
int IN1 = 52;
int IN2 = 50;
int IN3 = 48;
int IN4 = 46;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, INPUT);
  pinMode(trigPinS, OUTPUT);
  pinMode(echoPinS, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPinF, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinF, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF, LOW);
  pingTime_front = pulseIn(echoPinF, HIGH);
  distance_front = pingTime_front * 0.34 / 2;
  digitalWrite(trigPinS, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPinS, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinS, LOW);
  pingTime_side = pulseIn(echoPinS, HIGH);
  distance_side = pingTime_front * 0.34 / 2;

  if (distance_front > 100) { //100mm
    analogWrite(ENA, 255);
    analogWrite(ENB, 255);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    //start microswitch algorithm
    if (distance_side > 10) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      //check side parallel state again
    }
  }
}
