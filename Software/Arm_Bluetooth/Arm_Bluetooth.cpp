/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 10, 2020

*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Arm_Settings.h>
#include <Arm_Bluetooth.h>

SoftwareSerial Blue(2,3);

Arm_Bluetooth::Arm_Bluetooth(){
	state = SPECIAL_POSITION;
}

void Arm_Bluetooth::Arm_Bluetooth::setup(){
	Blue.begin(19200);
}

byte Arm_Bluetooth::getSpecialPosition(){
	if (Blue.available() > 0) {
    	byte receivedData = Blue.read();
    	if (receivedData == 48) {
    		state = 0;
    	}
    	else if (receivedData == 49) {
    		state = 1;
    	}
    	else if (receivedData == 50) {
    		state = 2;
    	}
    	else if (receivedData == 51) {
    		state = 3;
    	}
    }
    return state;

}