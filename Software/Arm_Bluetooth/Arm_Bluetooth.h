/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 10, 2020

*/

#ifndef Arm_Bluetooth_h
#define Arm_Bluetooth_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Arm_Settings.h>


class Arm_Bluetooth
{
	private: 
		byte state;
	public:
		Arm_Bluetooth();
		void setup();
		byte getSpecialPosition();
};

#endif