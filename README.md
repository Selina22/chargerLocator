# About
This repository is dedicated to the course project I worked on along with two other classmates for ESC204: Praxis III. This  is a project based design course. The project topic that we focused on in the 2019-2020 winter semester is locating the charging port on an electrical car and inserting the charger plug to the port autonomously. 
# Material used
* Arduino Mega
* breadboards
* PCB circuit boards
* LCD display
* NEMA17 Stepper motor
* DC motor
* HC-SR04 ultrasonic sensor
* TCS3200 colour sensor
* microswitches
# Design
We designed a rover that is able to accomplish:
1. run in x and y directions and turning
2. elevating (z direction movement)
3. inserting the charger plug
4. autonomous detection
## base
The front two wheels are DC motors with encoders and the back two wheels are rubber caster wheels. The wheels are calibrated with PID controllers to ensure driving straight. 
## elevation
The elevation mechanism is a pulley system and is driven by NEMA17 stepper motors. 
## insertion
For insertion mechanism, we used gear rack and pinion. 
## sensor detection
We used HC-SR04 ultrasonic sensors for locating the general position of the test rig. We used microswitches to determine if the robot is arrived at the test rig and if it is parallel to the test rig. We used TCS3200 colour sensor to detect the colour changes on the test rig for making the next decision - that is inserting the plug.
