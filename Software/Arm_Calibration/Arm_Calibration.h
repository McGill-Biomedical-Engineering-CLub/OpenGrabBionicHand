/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: July 17, 2020

*/

#ifndef Arm_Calibration_h

#define Arm_Calibration_h

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arm_Settings.h>
#include <Arm_Screen.h>
#include <Arm_Sampler.h>

class Arm_Calibration
{
	private: 
		int _amplitude;
		uint8_t _emg_pin;
		uint8_t _tempAvgMin;
		long int _averageMin;
		int _peak;
		void printToLaptop(int val);

	public:
		Arm_Calibration();
		Arm_Calibration(int pin);
		int Calibrate();
		int CalibrateAdvanced(int* steadyclose);
		int CalibrateDry(short* baseline);
		int CalibrateDry2Electrodes(short* baseline, short* baseline2);
};

#endif

