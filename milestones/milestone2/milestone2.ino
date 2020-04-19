int microL_F = A12; //left and right microswitches at the front of the rover
int microR_F = A13;
int microL_S = A14; //left and right microswitches at the side of the rover
int microR_S = A15;

int microRead_frontL = 0;
int microRead_frontR = 0;
int microRead_sideL = 0;
int microRead_sideR = 0;

int ENA = A5;       //define DC motor pins for mobility
int ENB = A6;
int IN1 = 52;
int IN2 = 50;
int IN3 = 48 ;
int IN4 = 46;

// TCS230 or TCS3200 pins wiring to Arduino
#define S0 6
#define S1 7
#define S2 8
#define S3 9
#define sensorOut 10

//success state
int microState = 1;
int colourState = 1;
int state = 1;

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

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

//ultrasonic
int pingTime_front;
int distance_front;
int pingTime_side;
int distance_side;
#define trigPinF 4
#define echoPinF 5
#define trigPinS 3
#define echoPinS 2


void setup() {
  // put your setup code here, to run once:
  pinMode(microL_F, INPUT);   //setting pinMode for microswitches
  pinMode(microR_F, INPUT);
  pinMode(microL_S, INPUT);
  pinMode(microR_S, INPUT);
  pinMode(ENA, OUTPUT);       //setting pinMode for DC motors
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

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

  pinMode(trigPinF, OUTPUT); //ultrasonic
  pinMode(echoPinF, INPUT);
  pinMode(trigPinS, OUTPUT);
  pinMode(echoPinS, INPUT);

  Serial.begin(9600);
}

void loop() {
  while (state == 1) {
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

    /*
       rover is placed at the front-left corner of the initial setup zone
       maximum possible distance in the y-direction is 500 mm
       strange readings from ultrasonic --> test rig is not in the front
    */
    if (distance_front < 0 || distance_front > 2000) { //if test rig is not detected
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, 255);
      delay(1750); //turn right 90 degrees
      stopDC();
      if (distance_side < 0 || distance_side > 2000) { //if test rig is not detected
        analogWrite(ENA, 255);
        analogWrite(ENB, 255);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW); //move forward to find the test rig
        if (distance_side > 0 && distance_side <= 700) { //test rig is detected
          stopDC();
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
          analogWrite(ENA, 255);
          analogWrite(ENB, 0);
          delay(1750); //turn left 90 degrees
          stopDC();
          microswitch(); //execute microswitch
          state = 0;
        }
      }
    } else if (distance_front > 0 && distance_front <= 700) { //test rig is detected
      microswitch(); //execute microswitch
      state = 0;
    }
  }
}

void microswitch() {
  microRead_frontL = analogRead(microL_F);
  microRead_frontR = analogRead(microR_F);
  microRead_sideL = analogRead(microL_S);
  microRead_sideR = analogRead(microR_S);
  Serial.print("Front Left:");
  Serial.print(microRead_frontL);
  Serial.print("   ");
  Serial.print("Front Right:");
  Serial.println(microRead_frontR);
  Serial.print("Side Left:");
  Serial.print(microRead_sideL);
  Serial.print("   ");
  Serial.print("Side Right:");
  Serial.println(microRead_sideR);
  while (microState == 1) {
    char state = Serial.read();
    microRead_frontL = analogRead(microL_F);
    microRead_frontR = analogRead(microR_F);
    microRead_sideL = analogRead(microL_S);
    microRead_sideR = analogRead(microR_S);
    Serial.print("Front Left:");
    Serial.print(microRead_frontL);
    Serial.print("   ");
    Serial.print("Front Right:");
    Serial.println(microRead_frontR);
    Serial.print("Side Left:");
    Serial.print(microRead_sideL);
    Serial.print("   ");
    Serial.print("Side Right:");
    Serial.println(microRead_sideR);
    if (microRead_frontL == 0 && microRead_frontR == 0 && microRead_sideL == 0 && microRead_sideR == 0) {
      analogWrite(ENA, 255);
      analogWrite(ENB, 255);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      Serial.println("Motor forward");
      microRead_frontL = analogRead(microL_F);
      microRead_frontR = analogRead(microR_F);
      microRead_sideL = analogRead(microL_S);
      microRead_sideR = analogRead(microR_S);
      Serial.print("Front Left:");
      Serial.print(microRead_frontL);
      Serial.print("   ");
      Serial.print("Front Right:");
      Serial.println(microRead_frontR);
      Serial.print("Side Left:");
      Serial.print(microRead_sideL);
      Serial.print("   ");
      Serial.print("Side Right:");
      Serial.println(microRead_sideR);
      if (microRead_frontL == 0 && microRead_frontR != 0 && microRead_sideL == 0 && microRead_sideR == 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        analogWrite(ENA, 255);
        analogWrite(ENB, 0);
        Serial.println("Motor right");
        microRead_frontL = analogRead(microL_F);
        microRead_frontR = analogRead(microR_F);
        microRead_sideL = analogRead(microL_S);
        microRead_sideR = analogRead(microR_S);
        Serial.print("Front Left:");
        Serial.print(microRead_frontL);
        Serial.print("   ");
        Serial.print("Front Right:");
        Serial.println(microRead_frontR);
        Serial.print("Side Left:");
        Serial.print(microRead_sideL);
        Serial.print("   ");
        Serial.print("Side Right:");
        Serial.println(microRead_sideR);
        if (microRead_frontL != 0) {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          analogWrite(ENA, 0);
          analogWrite(ENB, 255);
          Serial.println("Motor left");
          if (microRead_sideL != 0 && microRead_sideR != 0) {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            Serial.println("Success");
            //side parallel state is achieved, and will execute colour sensor algorithm
            colourSensor();
            microState = 0;
          }
        }
      } else if (microRead_frontL != 0 && microRead_frontR == 0 && microRead_sideL == 0 && microRead_sideR == 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        analogWrite(ENA, 0);
        analogWrite(ENB, 255);
      }
    } else if (microRead_frontL > 30) { //">30" is equivalent to "!=0"
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, 255);
      Serial.println("Motor left");
      delay(1750);
      microRead_frontL = analogRead(microL_F);
      microRead_frontR = analogRead(microR_F);
      microRead_sideL = analogRead(microL_S);
      microRead_sideR = analogRead(microR_S);
      Serial.print("Front Left:");
      Serial.print(microRead_frontL);
      Serial.print("   ");
      Serial.print("Front Right:");
      Serial.println(microRead_frontR);
      Serial.print("Side Left:");
      Serial.print(microRead_sideL);
      Serial.print("   ");
      Serial.print("Side Right:");
      Serial.println(microRead_sideR);
      if (microRead_sideL > 30 && microRead_sideR > 30) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        Serial.println("STOP1");
        //side parallel state is achieved, and will execute colour sensor algorithm
        colourSensor();
        microState = 0;
      }
    } else if (microRead_frontR > 30) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, 255);
    } else if (microRead_sideR > 30 && microRead_sideL == 0) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, 255);
      delay(500);
    } else if (microRead_sideL > 30 && microRead_sideR == 0) {
      analogWrite(ENA, 255);
      analogWrite(ENB, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else if (microRead_sideL > 30 && microRead_sideR > 30) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      Serial.println("STOP2");
      //side parallel state is achieved, and will execute colour sensor algorithm
      colourSensor();
      microState = 0;
    }
  }
}


void turnDCfront() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnDCback() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnDCleft() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 0);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnDCright() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnDCstop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void colourSensor() {

  /* for the current stage, assumed always moving from left to right
     a black tape is placed on the left hand side of the charger port
     when white is detected, continue moving right
     once the black tape is detected, stop
     move forward for a distance (tested), and stop right in front of the port
     insert the plug
  */
  while (colourState == 1) {
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
        colourState = 0;
        Serial.println("Black");
      }
    }
    else if (redFrequency > 90) {
      stopDC();
      insert();
      colourState = 0;
      Serial.println("Black");
    }
    Serial.print("colourState = ");
    Serial.println(colourState);
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
  /*
     if motor can stop immediately after the colour sensor has detected the black tape
     the rover still needs to move forward a bit to get to the port
     this would be a fixed and constant distance since the distance to the center of the tape would be always the same
  */
  startDC();
  delay(50);
  stopDC();
  turnStepper();
  stopStepper();
}
