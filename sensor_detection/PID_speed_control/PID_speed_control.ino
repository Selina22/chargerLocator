// Sample code based off PID speed control by Andrew Kramer
// Written by Daniel Jacobs for purposes of ESC204 motor control seminar
//
// Blog explaining approach at http://andrewjkramer.net/pid-motor-control/
//
// Instructions:
// 1. Install PID implementation library in Arduino/Libraries/ from https://github.com/1988kramer/motor_control
// 2. Install TimerOne library from Tools -> Manage Libraries
// 3. Check pin assignments for your rover
// 4. Tune deltaT (microseconds); PID coefficients for your rover
//

#include <TimerOne.h>
#include <PositionControl.h>

// Right side motor + encoder pins
const int InR1 = 48, InR2 = 46, EnR = A6;
const int ENCODER_R1 = 26, ENCODER_R2 = 25;

// Left side motor + encoder pins
const int InL1 = 52, InL2 = 50, EnL = A5;
const int ENCODER_L1 = 24, ENCODER_L2 = 23;

// Tuneable variables **** ADJUST AND TUNE THESE ***
const long deltaT = 20000;
const int encoderTicksPerRev = 816;
const double kP = 100, kI = 0, kD = 20;

// Set up motor, encoder, and speed control objects
// Right side
Motor rhMotor(InR1, InR2, EnR);
Encoder rhEncoder(ENCODER_R1, ENCODER_R2, deltaT, encoderTicksPerRev);
SpeedControl rhSpeedControl(&rhMotor, &rhEncoder);

// Left side
Motor lhMotor(InL1, InL2, EnL);
Encoder lhEncoder(ENCODER_L1, ENCODER_L2, deltaT, encoderTicksPerRev);
SpeedControl lhSpeedControl(&lhMotor, &lhEncoder);


void setup() {
  Timer1.initialize(deltaT); // Set up timer to check speed / adjust pwm every deltaT microseconds
  Timer1.attachInterrupt(adjustPWM);
  attachInterrupt(digitalPinToInterrupt(ENCODER_R1), readRHEncoder, CHANGE); // Set encoder pin interrupts to keep track of rotations
  attachInterrupt(digitalPinToInterrupt(ENCODER_L1), readLHEncoder, CHANGE);
  rhSpeedControl.setGains(kP, kI, kD); // Set PID gains for both motor controllers
  lhSpeedControl.setGains(kP, kI, kD);
}

void loop() {
  if (Serial.available() > 0) {
    char readval = Serial.read(); //constantly ask for user input
    Serial.println(readval);
    if (readval == 'W') {
      rhSpeedControl.setSpeed(720);
      lhSpeedControl.setSpeed(720);
    } else if (readval == 'K') {
      rhSpeedControl.setSpeed(0);
      lhSpeedControl.setSpeed(0);
    }
  }
}

void readRHEncoder()
{
  rhEncoder.updateCount();
}

void readLHEncoder()
{
  lhEncoder.updateCount();
}

void adjustPWM()
{
  rhSpeedControl.adjustPWM();
  lhSpeedControl.adjustPWM();
}
