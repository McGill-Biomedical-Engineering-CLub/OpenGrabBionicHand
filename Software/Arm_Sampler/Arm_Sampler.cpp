/*
  Authors: Abdullatif Hassan, Theodore Janson
  Source Repository: https://github.com/Abdul099/Bionic-Arm-Controller
  Last Updated:July 17, 2020
*/

#include <Arm_Sampler.h>
#include <Arm_Settings.h>
#include "Arduino.h"
#include "EMGFilters.h"

//constructor
Arm_Sampler::Arm_Sampler()
{
	_open = 1;
	_count = 0;
	_pin = A0;
	hold = 0;
	previousState = 0;
	currentState = 0;
	base = 0;
	peak = 0;
	myBuffer.sum = 0;
	myBuffer.index = 0;
	peakWindow = PEAKWINDOW;
	for (int i = 0; i < buffer_arrayLength; i++) {
   		 myBuffer.window[i] = 0;
    }
}

Arm_Sampler::Arm_Sampler(int pin)
{
	_open = 1;
	_count = 0;
	_pin = pin;
	hold = 0;
	previousState = 0;
	currentState = 0;
	base = 0;
	peak = 0;
	myBuffer.sum = 0;
	myBuffer.index = 0;
	peakWindow = PEAKWINDOW;
	for (int i = 0; i < buffer_arrayLength; i++) {
   		 myBuffer.window[i] = 0;
    }
}


byte Arm_Sampler::evaluateSampleFindPeak(int signal, int threshold)
{
	if(signal > 300){
		return 2;
	}
	if(peak){
		peakWindow--;
		if(peakWindow<=0){
			peakWindow = PEAKWINDOW;//reset back to default value
			peak = 0;
		}
	}
	else{
		if(signal>=threshold){
			hold++;
		}
		else{
			hold = 0;
		}
		if(signal>threshold && hold>=PEAK_HOLD){//look for PEAK_HOLD+1 number of consecutive signals greater than threshold
			hold = 0;
			peak = 1;
			if(_open==1){
				_open = 0;
			}
			else{
				_open = 1;
			}
		}
	}
	return _open;
}

byte Arm_Sampler::evaluateSample2Electrodes(int signal1, int threshold1, int signal2, int threshold2)
{
	byte firstChannel = evaluateSampleFindPeak(signal1, threshold1);
	if(firstChannel==2) return 2; //electrode misplacement error
		if(signal2 > 300){
		return 2;
	}
	if(peak==0){ //if no peak detected in first channel
		if(signal2>threshold2) {
			if(_open == 3 || _open == 10){
				return 10; //neutral response, stay in previous position
			}
			_open = 3; 
			return 3;
		}
		else{// if no peak in both channels just return the previous state
			if(_open == 3 || _open == 10){
				return 10; //neutral response, stay in previous position
			}
			return _open;
		}
	}
	return firstChannel;
}

int Arm_Sampler::read()
{
  	int raw = analogRead(_pin);
  	raw = abs(raw-base);
  	updateBuffer(myBuffer, raw);
  	raw = averageBuffer(myBuffer);
  	raw *=2; 
  	return raw;
}

int Arm_Sampler::simpleSample()//wrapper function for read
{
	int sample = read();
	delay(20);
	Serial.println(sample);
	return sample; 
}

int Arm_Sampler::simpleSample(byte del)
{
	int sample = read();
	delay(del);
	Serial.println(sample);
	return sample; 
}

void Arm_Sampler::checkBelow(int val, byte duration){
	short counter = duration; 
	int i = 0;
	while(counter >0){
		int reading = simpleSample();
		if (reading<=val) counter--; 
		else counter = duration; //reset the counter
		i++;
		if(i%60 == 0) updateBaseline();
	}
	delay(10);
}

void Arm_Sampler::updateBaseline(){
  long avg = 0;
  if(base == 0){
  	for(int i=0; i<100; i++){
   	 avg+= analogRead(_pin);
   	 delay(10);
 	 }
 	avg/=100;
  	base = avg;
	}
  else{
  	avg = 0;
  	for(int i=0; i<100; i++){
    	avg+= simpleSample();
  	}
  avg/=100;
  base = avg;
	}
}

void Arm_Sampler::setBaseline(short value)
{
	base = value;
}

int Arm_Sampler::getBaseline()
{
	return base; 
}


//private methods
void Arm_Sampler::updateBuffer(_buffer& b, int value){                                              
      b.sum -= b.window[b.index];                    
      b.window[b.index] = (value); 
      b.sum += (value);                                         
      b.index = (b.index + 1) % buffer_arrayLength; 
}

/* Get mean value of cycle buffer */
int Arm_Sampler::averageBuffer(_buffer& b){
  return b.sum / buffer_arrayLength;
}
