/*
  Project Name: Bionic Arm Controller ver 1.4.0
  Authors: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
           Theodore Janson
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 15, 2020
  Description: Arm Controller sketch that receives emg input via analog pin (up to 2 channels) and outputs PWM signals for up to 5 servo motors.
*/
#include <Arm_Settings.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arm_Calibration.h>
#include <Adafruit_GFX.h>
#include <Arm_Servo.h>
#include <Arm_Demo.h>
#include <Arm_Sampler.h>
#include <Arm_Screen.h>
#include <Arm_Bluetooth.h>

int amp1;
int amp2;
int thresh;
int thresh2;
byte opened = 0;
short baseline;
short baseline2;
Arm_Calibration Calibrate = Arm_Calibration();
Arm_Servo servo = Arm_Servo();
Arm_Demo demo = Arm_Demo();
Arm_Sampler sampler = Arm_Sampler(emgpin1);
Arm_Sampler sampler2 = Arm_Sampler(emgpin2);
Arm_Screen screen = Arm_Screen();
Arm_Bluetooth bluetooth = Arm_Bluetooth();
void setup() {
  Wire.begin();
  Serial.begin(9600);
  if (NUM_CHANNELS == 1) thresh = Calibrate.CalibrateDry(&baseline);
  else if (NUM_CHANNELS == 2) thresh = Calibrate.CalibrateDry2Electrodes(&baseline, &baseline2);
  servo.setup();
  bluetooth.setup();
  screen.prepare();
  sampler.setBaseline(baseline);
  if(NUM_CHANNELS ==2) sampler2.setBaseline(baseline2);
  
}

void loop() {
   byte special = bluetooth.getSpecialPosition();
   Serial.print("Special: ");
   Serial.println(special);
   //Serial.print("1: ");
   amp1 = sampler.simpleSample();//we start by reading the signal value from the emg sensor --> assign this value to amp
//  Serial.print("2: ");
  if(NUM_CHANNELS  ==2) amp2 = sampler2.simpleSample();
  if (NUM_CHANNELS == 1)  opened = sampler.evaluateSampleFindPeak(amp1, thresh);
  else if (NUM_CHANNELS == 2) opened = sampler.evaluateSample2Electrodes(amp1, thresh, amp2, thresh);
  if (LED_DEBUG_MODE) {
    switch (opened) {
      case 0:
        digitalWrite(3, HIGH);
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        break;
      case 1:
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        digitalWrite(5, LOW);
        break;
      case 2:
        digitalWrite(3, LOW);
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        delay(1000);
        break;
    }
  }
  else {
    switch (opened) {
      case 3: //special move
        screen.printToScreen("");
        servo.PerformSpecialMovement(0);
        delay(OPEN_DELAY);
        break;
      case 2://error in electrode
        screen.printToScreen("Misplaced  Electrode");
        delay(1000);
        break;
      case 1:
        screen.printToScreen("");
        servo.openFinger(thumbServo);
        servo.openFinger(pinkyServo);
        servo.openFinger(indexServo);
        servo.openFinger(middleServo);
        servo.openFinger(ringServo);
        delay(CLOSED_DELAY);
        break;
      case 0:
        screen.printToScreen("");
        servo.closeFinger(thumbServo);
        servo.closeFinger(pinkyServo);
        servo.closeFinger(indexServo);
        servo.closeFinger(ringServo);
        servo.closeFinger(middleServo);
        delay(OPEN_DELAY);
        break;
    }
  }
}
