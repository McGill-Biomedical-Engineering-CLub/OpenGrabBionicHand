/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 20, 2020

*/

#include <Wire.h>
#include <Arm_Settings.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arm_Demo.h>
#include <Arm_Servo.h>

extern Arm_Servo servo;

Arm_Demo::Arm_Demo()
{
	_standardDelay = 1500; //delay between movements in demo script
	for (int i=0; i< NUM_SERVOS; i++){ 
		servoPins[i] = i;
	}
}

Arm_Demo::Arm_Demo(int delay)
{
	_standardDelay = delay;
}

void Arm_Demo::runDemo()
{
	openHand();
	delay(_standardDelay);
	closeHand();
	delay(_standardDelay);
	openHand();
	delay(_standardDelay);
	fingerByFinger();
    delay(_standardDelay);
	thumbsUp();
	delay(_standardDelay);
	openHand();
	delay(_standardDelay);
	vSign();
	delay(_standardDelay);
	openHand();
	delay(_standardDelay);
	gangsterSign();
	delay(_standardDelay);
	openHand();
}

void Arm_Demo::specialPositions()
{
	openHand();
	delay(_standardDelay);
	for(int i = 0; i<4; i++){
		servo.PerformSpecialMovement(i);
		delay(_standardDelay);
		openHand();
		delay(_standardDelay);
	}
}

void Arm_Demo::openHand()
{
	for(int i = 0; i<NUM_SERVOS; i++){
		servo.openFinger(servoPins[i]);
	}
}

void Arm_Demo::closeHand()
{
	for(int i = 0; i<NUM_SERVOS; i++){
		servo.closeFinger(servoPins[i]);
	}
}

void Arm_Demo::fingerByFinger()
{
	servo.closeFinger(pinkyServo);
	delay(_standardDelay*0.3);
	servo.closeFinger(ringServo);
	delay(_standardDelay*0.3);
	servo.closeFinger(middleServo);
	delay(_standardDelay*0.3);	
	servo.closeFinger(indexServo);
	delay(_standardDelay*0.3);
	servo.closeFinger(thumbServo);
	delay(_standardDelay*0.3);	
	servo.openFinger(pinkyServo);
	delay(_standardDelay*0.3);
	servo.openFinger(ringServo);
	delay(_standardDelay*0.3);
	servo.openFinger(middleServo);
	delay(_standardDelay*0.3);
	servo.openFinger(indexServo);
	delay(_standardDelay*0.3);
	servo.openFinger(thumbServo);
	delay(_standardDelay*0.3);	
}

void Arm_Demo::thumbsUp()
{
	servo.openFinger(thumbServo);
	servo.closeFinger(indexServo);
	servo.closeFinger(pinkyServo);
	servo.closeFinger(middleServo);
	servo.closeFinger(ringServo);
}

void Arm_Demo::vSign()
{
	servo.openFinger(indexServo);
	servo.openFinger(middleServo);	
	servo.closeFinger(thumbServo);
	servo.closeFinger(pinkyServo);
	servo.closeFinger(ringServo);
}

void Arm_Demo::gangsterSign()
{
	servo.closeFinger(thumbServo);
	servo.openFinger(indexServo);
	servo.closeFinger(middleServo);
	servo.openFinger(pinkyServo);
	servo.openFinger(ringServo);
}