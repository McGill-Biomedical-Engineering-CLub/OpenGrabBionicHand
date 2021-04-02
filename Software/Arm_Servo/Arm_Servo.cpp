/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 8, 2020

*/

#include <Arm_Servo.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <Arm_Settings.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();//default i2c address: 0x40

Arm_Servo::Arm_Servo()
{
	_currentIndex = 0;
}

void Arm_Servo::setup(){
	pwm.begin();
	pwm.setPWMFreq(PWM_frequency);
}

void Arm_Servo::closeFinger(int pin)
{
	//moveServo(pin, -10);
	moveServo2(pin, 0);
}

void Arm_Servo::openFinger(int pin)
{
	//moveServo(pin, 180);
	moveServo2(pin,1);
}
//add a close hand function

void Arm_Servo::moveServo(int pin, int pos) //might become a public method later on
{
	if(HAND_TYPE == 0){
		if(pin == thumbServo) pwm.setPWM(pin, 0, SERVOMAX-pos+THUMB_MOD);
		else if(pin ==ringServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + RING_MOD);
		else if(pin == middleServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + MIDDLE_MOD);
		else if(pin == indexServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + RING_MOD);
		else pwm.setPWM(pin, 0, pos + SERVOMIN);
	}
	else{
		if(pin == thumbServo) pwm.setPWM(pin, 0, SERVOMAX-pos+THUMB_MOD);
		else if(pin ==ringServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + RING_MOD);
		else if(pin == middleServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + MIDDLE_MOD);
		else if(pin == indexServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + RING_MOD);
		else if(pin == pinkyServo) pwm.setPWM(pin, 0, pos+ SERVOMIN + PINKY_MOD);
		else pwm.setPWM(pin, 0, pos + SERVOMIN);
	}
}

void Arm_Servo::moveServo2(int pin, bool open) //might become a public method later on
{
	if(!open){//CLOSING POSITION
		if(HAND_TYPE == 0){//brunel hand
			if(pin == thumbServo) pwm.setPWM(pin, 0, THUMB_MAX);
			else if(pin ==ringServo) pwm.setPWM(pin, 0, RING_MIN);
			else if(pin == middleServo) pwm.setPWM(pin, 0, MIDDLE_MIN);
			else if(pin == indexServo) pwm.setPWM(pin, 0, INDEX_MIN);
			else pwm.setPWM(pin, 0, SERVOMIN);
		}
		else{
			if(pin == thumbServo) pwm.setPWM(pin, 0, THUMB_MIN);
			else if(pin ==ringServo) pwm.setPWM(pin, 0, RING_MIN);
			else if(pin == middleServo) pwm.setPWM(pin, 0, MIDDLE_MIN);
			else if(pin == indexServo) pwm.setPWM(pin, 0, INDEX_MIN);
			else if(pin == pinkyServo) pwm.setPWM(pin, 0, PINKY_MIN);
			else pwm.setPWM(pin, 0, SERVOMIN);
		}
	}
	else{
		if(HAND_TYPE == 0){//brunel
			if(pin == thumbServo) pwm.setPWM(pin, 0, THUMB_MIN);
			else if(pin ==ringServo) pwm.setPWM(pin, 0, RING_MAX);
			else if(pin == middleServo) pwm.setPWM(pin, 0, MIDDLE_MAX);
			else if(pin == indexServo) pwm.setPWM(pin, 0, INDEX_MAX);
			else pwm.setPWM(pin, 0, SERVOMAX);
		}
		else{
			if(pin == thumbServo) pwm.setPWM(pin, 0, THUMB_MAX);
			else if(pin ==ringServo) pwm.setPWM(pin, 0, RING_MAX);
			else if(pin == middleServo) pwm.setPWM(pin, 0, MIDDLE_MAX);
			else if(pin == indexServo) pwm.setPWM(pin, 0, INDEX_MAX);
			else if(pin == pinkyServo) pwm.setPWM(pin, 0, PINKY_MAX);
			else pwm.setPWM(pin, 0, SERVOMAX);
		}
	}
}
void Arm_Servo::PerformSpecialMovement(byte POSITION) //only for mbec arm
{
	if(POSITION == 0){ //V-Sign
		openFinger(indexServo);
		openFinger(middleServo);	
		closeFinger(thumbServo);
		closeFinger(pinkyServo);
		closeFinger(ringServo);
	}
	else if(POSITION==1){  // 2 finger fine grip
		pwm.setPWM(middleServo, 0, MIDDLE_MIN);
		pwm.setPWM(thumbServo, 0, THUMB_MIN);
		delay(1000);
		pwm.setPWM(thumbServo, 0, 250);
		delay(500);
		pwm.setPWM(middleServo, 0, 300);
		delay(500);
		pwm.setPWM(thumbServo, 0, THUMB_MAX);
		pwm.setPWM(middleServo,0,MIDDLE_MAX);
		delay(1000);
	}
	else if(POSITION==2){  // slow fine grip
  		for (int i = 0; i <= 20; i++) {
    		pwm.setPWM(middleServo, 0, MIDDLE_MAX + (MIDDLE_MIN - MIDDLE_MAX)*i / 10);//min is greater than max for both fingers
    		pwm.setPWM(thumbServo, 0, THUMB_MAX+ (THUMB_MIN -THUMB_MAX)*i / 10);
    		delay(100);
			}
	}
	else if(POSITION==3){  // OK sign
		closeFinger(indexServo);
		openFinger(middleServo);	
		closeFinger(thumbServo);
		openFinger(pinkyServo);
		openFinger(ringServo);
	}
	else if(POSITION==4){  // Thumbs-Up
		closeFinger(indexServo);
		openFinger(middleServo);	
		closeFinger(thumbServo);
		openFinger(pinkyServo);
		openFinger(ringServo);
	}
}

void Arm_Servo::OpenHand()
{
	if(HAND_TYPE == 0){
		openFinger(indexServo);
		openFinger(middleServo);	
		openFinger(thumbServo);
		openFinger(ringServo);
	}
	else if(HAND_TYPE == 1){
		openFinger(indexServo);
		openFinger(middleServo);	
		openFinger(thumbServo);
		openFinger(pinkyServo);
		openFinger(ringServo);
	}
}

void Arm_Servo::CloseHand()
{
	if(HAND_TYPE ==0){
		closeFinger(indexServo);
		closeFinger(middleServo);	
		closeFinger(thumbServo);
		closeFinger(ringServo);
	}
	else if(HAND_TYPE == 1){
		closeFinger(indexServo);
		closeFinger(middleServo);	
		closeFinger(thumbServo);
		closeFinger(pinkyServo);
		closeFinger(ringServo);
	}
}
