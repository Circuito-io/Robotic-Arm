// Include Libraries
#include "Arduino.h"
#include "Joystick.h"
#include "Servo.h"


// Pin Definitions
#define base_PIN_SIG  2
#define arm_PIN_SIG 3
#define gripper_PIN_SIG 4
#define JOYSTICK_PIN_VRX  A3
#define JOYSTICK_PIN_VRY  A4
#define JOYSTICK_PIN_SW 5



const int baseMinPosition   = 20;
const int baseMaxPosition = 90; 
const int baseStartPosition = 55; 


const int armMinPosition   = 20;
const int armMaxPosition = 60;
const int armStartPosition = 60; 


const int gripperMinPosition   = 0;
const int gripperMaxPosition = 90; 
const int gripperStartPosition = 90; 

int joystickX = baseStartPosition;
int joystickY = armStartPosition;

int joystickX_Cal = 0;
int joystickY_Cal = 0;

bool gripperState = 0;

Servo base;
Servo arm;
Servo gripper;
Joystick joystick(JOYSTICK_PIN_VRX,JOYSTICK_PIN_VRY,JOYSTICK_PIN_SW);


/* This code sets up the essentials for your circuit to work. It runs first every time your circuit is powered with electricity. */
void setup() {
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    Serial.println("start");
    
    base.attach(base_PIN_SIG);
    base.write(baseStartPosition);
    arm.attach(arm_PIN_SIG);
    arm.write(armStartPosition);
    gripper.attach(gripper_PIN_SIG);
    gripper.write(gripperStartPosition);

    // Read Joystick X,Y axis and press
    joystickX_Cal =  joystick.getX();
    joystickY_Cal =  joystick.getY();

    
}

/* This code is the main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop. */
void loop() {
    // Read Joystick X,Y axis and press
    joystickX +=  (joystick.getX() - joystickX_Cal)/50;
    joystickY +=  (joystick.getY() - joystickY_Cal)/20;
    bool joystickSW =  joystick.getSW();

    joystickX = constrain(joystickX, 0, 1023);
    joystickY = constrain(joystickY, 0, 1023);
   
    int basePos = map(joystickX,0,1023,baseMinPosition, baseMaxPosition);
    int armPos = map(joystickY,0,1023,armMaxPosition, armMinPosition);

    
    Serial.print(basePos);
    Serial.print('\t');
    Serial.print(armPos);
    Serial.print('\t');
    Serial.println(joystickSW);
    
    base.write(basePos);  
    arm.write(armPos);  

    // On each joystick press, toggle the gripper's state
    if (joystickSW)
    {
    if(gripperState == 0)
      gripper.write(gripperMaxPosition);  
    else
      gripper.write(gripperMinPosition);  
    // Update gripper state
    gripperState = !gripperState;  
    }

    delay(20);
    
}
