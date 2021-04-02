/*
  Author: Abdullatif Hassan <abdullatif.hassan@mail.mcgill.ca>
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated: August 24, 2020
*/

#ifndef Arm_Settings_h
#define Arm_Settings_h

//#define BRUNEL_ARM
#define MBEC_ARM

#define SKIP_CALIBRATION 1
#define LED_DEBUG_MODE 0
#define SPECIAL_POSITION 2

//emg channel and servo settings
#define NUM_CHANNELS 2 //1 or 2
#define NUM_SERVOS 5//max 5
#define emgpin1 A0
#define emgpin2 A3
#define indexServo 3//pin on servo relay module
#define thumbServo 0
#define middleServo 1 
#define ringServo 2
#define pinkyServo 4


#ifdef BRUNEL_ARM
#define HAND_TYPE 0
#define PWM_frequency 60
#define SERVOMIN 230 
#define SERVOMAX 410 
#define THUMB_MOD 25 //20
#define INDEX_MOD 20  //20
#define MIDDLE_MOD 20 //25
#define RING_MOD 50 //30
#define PINKY_MOD 0 //not used in this case but must be defined nevertheless
#define THUMB_MIN 300
#define INDEX_MIN 230
#define MIDDLE_MIN 230
#define RING_MIN 230
#define PINKY_MIN 230
#define THUMB_MAX 410
#define INDEX_MAX 430
#define MIDDLE_MAX 410
#define RING_MAX 475
#define PINKY_MAX 410
#define THUMB_MID 300
#define MIDDLE_MID 300
#endif

#ifdef MBEC_ARM
#define HAND_TYPE 1
#define PWM_frequency 50
#define SERVOMIN 610
#define SERVOMAX 2360
#define THUMB_MOD 0 
#define INDEX_MOD 0
#define MIDDLE_MOD 0
#define RING_MOD 0
#define PINKY_MOD 0
//closed
#define THUMB_MIN 320
#define INDEX_MIN 170//170
#define MIDDLE_MIN 400//400
#define RING_MIN 180//440
#define PINKY_MIN 430//155
//open
#define THUMB_MAX 170
#define INDEX_MAX 450//455
#define MIDDLE_MAX 133//133
#define RING_MAX 440//180
#define PINKY_MAX 210//440
//for special positions
#define THUMB_MID 300
#define MIDDLE_MID 300
#endif

//delay intervals
#define OPEN_DELAY 20
#define CLOSED_DELAY 20

//lcd screen settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 13

//Calibration Settings
#define samples 200
#define NUM_CONTRACTIONS 10 // number of contractions needed for advanced calibration mode
#define SIZE_TRAININGDATA 30 //max 40
#define TRAINING_DELAY 20 //sampling period for trainingData

//sampler settings
#define THRESHOLD_TOLERENCE 2 //number of successive highs required to initiate a 'close hand' order
#define THRESHOLD_TOLERENCE2 2 //number of successive lows required to initiate an 'open hand' order
#define HOLD 2
#define buffer_arrayLength 8
#define PEAKWINDOW 10//20
#define PEAK_HOLD 0

#endif