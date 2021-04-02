/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: May 13, 2020

*/
#ifndef Arm_Demo_h
#define Arm_Demo_h

#include <Arduino.h>
#include <Wire.h>
#include <Arm_Settings.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arm_Servo.h>

class Arm_Demo
{
	private:
		void openHand();
		void closeHand();
		void thumbsUp();
		void vSign();
		void fingerByFinger();
		void gangsterSign();
		int _standardDelay;
		int servoPins[NUM_SERVOS];

	public:
		Arm_Demo(); 
		Arm_Demo(int delay);
		void runDemo();
		void specialPositions();

};
#endif