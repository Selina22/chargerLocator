// TCS230 or TCS3200 pins wiring to Arduino
#define S0 6
#define S1 7
#define S2 8
#define S3 9
#define sensorOut 10

//success state
int state = 1;

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int ENA = A5;       //define DC motor pins for mobility
int ENB = A6;
int IN1 = 52;
int IN2 = 50;
int IN3 = 48;
int IN4 = 46;

int holdKey = 0;
int holdStepper = 0;

int stepPin1 = 30;   //define stepper motor pin for insertion mechanism
int stepPin2 = 32;
int stepPin3 = 34;
int stepPin4 = 36;
int dirPin1 = A0;
int dirPin2 = A1;

int DIRPIN1 = A8;
int DIRPIN2 = A9;
int STEPPIN1 = 31;
int STEPPIN2 = 33;
int STEPPIN3 = 35;
int STEPPIN4 = 37;


void setup() {
  Serial.begin(9600);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  pinMode(ENA, OUTPUT);     //setting pinMode for DC motors for mobility
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(stepPin1, OUTPUT); //setting pinMode for stepper motors for insertion mechanism
  pinMode(stepPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  pinMode(DIRPIN1, OUTPUT);
  pinMode(DIRPIN2, OUTPUT);
  pinMode(STEPPIN1, OUTPUT);
  pinMode(STEPPIN2, OUTPUT);
  pinMode(STEPPIN3, OUTPUT);
  pinMode(STEPPIN4, OUTPUT);

}

void loop() {

  /* for the current stage, assumed always moving from left to right
     a black tape is placed on the left hand side of the charger port
     when white is detected, continue moving right
     once the black tape is detected, stop
     move forward for a distance (tested), and stop right in front of the port
     insert the plug
  */
  while (state == 1) {
    //red pixel
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redFrequency = pulseIn(sensorOut, LOW);
    Serial.println(redFrequency);

    //green pixel
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    greenFrequency = pulseIn(sensorOut, LOW);

    //blue pixel
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blueFrequency = pulseIn(sensorOut, LOW);
    if (redFrequency < 90) { //this value is found through sensor testing
      Serial.println("White");
      startDC();
      if (redFrequency > 90) {
        stopDC();
        insert();
        state = 0;
        Serial.println("Black");
      }
    }
    else if (redFrequency > 90) {
      stopDC();
      insert();
      state = 0;
      Serial.println("Black");
    }
    Serial.print("state = ");
    Serial.println(state);
  }
}

void startDC() {
  analogWrite(ENA, 130);
  analogWrite(ENB, 130);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopDC() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopStepper() {  //this function will turn off the stepper motor
  digitalWrite(stepPin4, LOW);
  digitalWrite(stepPin3, LOW);
  digitalWrite(stepPin2, LOW);
  digitalWrite(stepPin1, LOW);
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  digitalWrite(DIRPIN1, LOW);
  digitalWrite(DIRPIN2, LOW);
  digitalWrite(STEPPIN1, LOW);
  digitalWrite(STEPPIN2, LOW);
  digitalWrite(STEPPIN3, LOW);
  digitalWrite(STEPPIN4, LOW);

  holdStepper = 1;
}

void turnStepper() {          //this function will turn the stepper motor
  digitalWrite(stepPin4, LOW);
  digitalWrite(stepPin3, LOW);
  digitalWrite(stepPin2, LOW);
  digitalWrite(stepPin1, LOW);
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  digitalWrite(DIRPIN1, LOW);
  digitalWrite(DIRPIN2, LOW);
  digitalWrite(STEPPIN1, LOW);
  digitalWrite(STEPPIN2, LOW);
  digitalWrite(STEPPIN3, LOW);
  digitalWrite(STEPPIN4, LOW);

  holdStepper = 1;

  for (int x = 0; x < 50; x++) {  //with step angle = 1.8 degree, the stepper will turn 90 degree
    digitalWrite(stepPin4, LOW);
    digitalWrite(stepPin1, HIGH);
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, LOW);

    digitalWrite(STEPPIN4, LOW);
    digitalWrite(STEPPIN1, HIGH);
    digitalWrite(DIRPIN1, HIGH);
    digitalWrite(DIRPIN2, LOW);

    delayMicroseconds(2000);
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin3, HIGH);
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, HIGH);

    digitalWrite(STEPPIN1, LOW);
    digitalWrite(STEPPIN3, HIGH);
    digitalWrite(DIRPIN1, LOW);
    digitalWrite(DIRPIN2, HIGH);

    delayMicroseconds(2000);
    digitalWrite(stepPin3, LOW);
    digitalWrite(stepPin2, HIGH);
    digitalWrite(dirPin1, HIGH);
    digitalWrite(dirPin2, LOW);

    digitalWrite(STEPPIN3, LOW);
    digitalWrite(STEPPIN2, HIGH);
    digitalWrite(DIRPIN1, HIGH);
    digitalWrite(DIRPIN2, LOW);

    delayMicroseconds(2000);
    digitalWrite(stepPin2, LOW);
    digitalWrite(stepPin4, HIGH);
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, HIGH);

    digitalWrite(STEPPIN2, LOW);
    digitalWrite(STEPPIN4, HIGH);
    digitalWrite(DIRPIN1, LOW);
    digitalWrite(DIRPIN2, HIGH);

    delayMicroseconds(2000);
  }
  delay(1000); // One second delay
}

void insert() {
  turnStepper();
  stopStepper();
}
