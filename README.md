# OpenGrabBionicHand
### Date Launched: June 3, 2020
### Date Delivered: September 1, 2020
# Electronic Hardware System
## Circuit Schematic
![image](https://user-images.githubusercontent.com/46327849/116008115-f36a0100-a5e0-11eb-8933-163737559d4a.png)

### Parts Used:
* Arduino Pro Mini: Used to control all other electronics in the device. Receives analog input from the emg sensor and outputs commands to the servo shield and lcd screen via i2c communication. Datasheet: https://components101.com/microcontrollers/arduino-pro-mini
* FT231X: Provides the Arduino with serial communication from a USB port connected to the laptop. Datasheet: https://www.sparkfun.com/products/13263
* Gravity EMG Muscle Sensor: Measures the EMG signal at the patient's muscle and outputs a 10 bit ADC signal (range of 0-1023) to the analog pin A0 of the Arduino UNO. The device consists of 2 modules: The electrode module, containing 3 dry electrodes, is strapped to the user's muscle, and the signal processing module, which performs basic differential signal processing and interfaces with the analog pin. Datasheet: https://wiki.dfrobot.com/Analog_EMG_Sensor_by_OYMotion_SKU_SEN0240
* Servo Controller (PCA 9685): 16-channel servo controller. Receives power input from the external power supply and control input from the Arduino via I2C interface. Can control up to 16 servos using PWM signals (pulse with modulation). 
Datasheet: https://cdn-learn.adafruit.com/downloads/pdf/16-channel-pwm-servo-driver.pdf
* LCD Screen (SSD 1306): 0.96" 126x64 OLED screen. Displays to the patient calibration instructions. Communicates with the Arduino via I2C interface. Datasheet: https://components101.com/oled-display-ssd1306
* Rotational Servos (HS-65MG): 6V servo motor with 3 input pins (VCC, GND, PWM). Controlled by the servo controller. Datasheet: https://www.robotshop.com/en/hs-65mg-metal-gear-feather-micro-servo-motor.html
* Power Supply: 7.4V Rechargable LiPo Battery (2200 mAh). Datasheet: https://abra-electronics.com/batteries-holders/batteries-polymer-lithium-ion/prt-11856-polymer-lithium-ion-battery-2200mah-74v-prt-11856.html
### Soldering:
The modules for this circuit are connected on a 5x7cm soldering board. Both sides of the board are utilized to place the components. 
![image](https://user-images.githubusercontent.com/46327849/87361846-1daf6e00-c53b-11ea-98fe-d23670fc50ca.png)
![image](https://user-images.githubusercontent.com/46327849/87361879-2c962080-c53b-11ea-8ada-aad3bd94d81c.png)
### Current Consumption
Non-Motor Components: (From Datasheets)
|Component |  Idle | Operational | Source of Measurement |  
| ------------- | ------------- | ------------- | ------------- |
|Arduino Pro Mini 5V | 3.1mA | 19.9mA | http://www.home-automation-community.com/arduino-low-power-how-to-run-atmega328p-for-a-year-on-coin-cell-battery/#:~:text=Using%20an%20Arduino%20Pro%20Mini,%2C%20in%20power%2Ddown%20sleep. |
| Gravity EMG Sensor | 0.5mA | 0.5 mA | Measured manually using INA219 current sensor |
| OLED Screen | 1.5mA | 4 mA | Measured manually using INA219 current sensor |
| Servo Controller | 2.2mA | 40mA | Datasheet: https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf |
|**Total**|7.3mA |64.4mA| | 

Adding Motor Consumption

Using an INA 219 current sensor, the current consumed by the motors is measured. This measurement is done on 3 different states of the arm: Idle Open, Idle Closed, and a continous alternation between both states. With a known battery capacity of 2200mAh, the battery life can be calculated.

| Hand State |	Average Motor Current Consumption (mA)	| Average Total Current Consumption (mA)	| Estimated Battery Operating Time (hr) |
| ------------- | ------------- | ------------- | ------------- |
| Idle Open |	41	| 106	| 20.75 |
| Idle Closed	| 181	| 246	| 8.94 |
| Alternating	| 456	| **521**	| **4.22** |

Motor Current Consumption Profile
![image](https://user-images.githubusercontent.com/46327849/116008511-f5cd5a80-a5e2-11eb-8701-b5cf0aeaeab0.png)

* It is important to note that only motor current consumption was measured and consumption by other modules are not taken into consideration for this measurement, as the motors are the main source of current drainage. 
# Software System
## Dependency Graph (Internal Modules Only)
![image](https://user-images.githubusercontent.com/46327849/87354111-03ba5f00-c52c-11ea-9e74-ba72bfa1a638.png)
### Modules involved:
* Bionic_Arm_Controller (main file): Controls the arm (receives EMG input and outputs signals to servo motors) by calling functions from other modules. 
* Arm_Settings: Contains all dynamic parameters that the user can change in the code.
* Arm_Calibration: Runs the calibration for the arm. Collects samples from both resting and active states of the muscle and determines the signal threshold needed to separate between open and closed hand states. 
* Arm_Servo: Handles the direct control of the servos and allows other modules depending on it to call simple methods such as 'open finger' and 'close finger'
* Arm_Demo: Uses functions from the Arm_Servo library to execute a pre-determined set of maneuvers with the arm.
* Arm_Screen: Handles display commands to the OLED screen.
* Arm_Sampler: Collects an EMG sample from the analog pin. Performs all signal analysis. Evaluates samples and outputs an open/close or error command.  
## Errors and Alerts: 
* Electrode Misplacement Error: The dry electrode is not firmly attached to its position which results in readings that are much higher than the normal range. This error is printed to the screen and persists until the electrode position is adjusted to a valid one. 
## Signal Processing:

Unlike that of the Myoware sensor, the signal obtained from the analog reading of the Gravity EMG sensor is raw. The hardware only uses differential denoising to remove the common noise among the electrodes. Thus, the need for software-based signal processing arises.
 
Example of Raw Signal

![raw](https://user-images.githubusercontent.com/46327849/116008361-2365d400-a5e2-11eb-9345-707f2bcfe449.png)

Differentiating a contraction from the resting phase visually in this noisy signal is still possible. However, there must be a systematic way that the software can use to easily distinguish between both phases. Hence, a variety of techniques must be used:
1. Establishing a zero baseline: The raw signal has a resting-phase baseline that ranges between 280 and 350 units of amplitude (1 unit = 5V/1024). During calibration, the average value of this baseline is determined. Upon each subsequent reading, the absolute value of the difference between the raw reading and the baseline is calculated to obtain a 'normalized' signal reading.
2. Moving Average Filter: To obtain more smoothing of the signal and to mitigate the effect of sudden noisy spikes in measurements, a moving average filter of window width 8 is used. The filter was implemented internally (no use of external libraries) and by trial-and-error, the mentioned window width was found to be most convenient in terms of signal quality and memory usage. 
3. Multiplying factor: At the end, the processed signal readings are multiplied by a certain factor to make different readings more distinguishable from each other. A factor of 2 was chosen. It is worth mentioning that squaring the readings was an option that was considered; however, this method was found to require double the amount of memory, as it causes overflows in the 16 bit integers used to store the datapoints. 
4. Sampling Rate: The sampling rate also has an impact on the quality of the output signal. A rate of 50Hz is found to be convenient. 

Note: Frequency domain-based signal processing is not applicable to this iteration, as it requires a significant amount of memory. The Arduino Pro Mini total RAM memory is only 2048 Bytes, of which most are used for calibration and the LCD screen. 

Example of Processed Signal

![final_sig](https://user-images.githubusercontent.com/46327849/116008395-498b7400-a5e2-11eb-8489-1a66b5b90883.png)

As seen in the figure above, clear peaks can be identified in the signal, with each peak corresponding to a muscle contraction.

## Calibration 
The function CalibrateDry is used mainly for this iteration. It is found in the Arm_Calibration.cpp file. 
### Determining the baseline
Before starting calibration, a baseline must be established in order to proceed with sampling. The user is instructed to relax and 100 EMG samples are taken at a rate of 100Hz. The average of those 100 samples is the obtained baseline. This value is then to be verified by the checkBelow function which tests the baseline value and ensures that 30 consecutive readings resting values lie below 50. If that is not the case, then the function responsible for updating the baseline is called again and the new baseline is validated. This process repeats until a valid baseline is obtained and tested. 
### Algorithm for Finding the Threshold Value
The same algorithm used from Iteration 1 (Advanced Calibration) is used to find the threshold value that sets apart the resting state from the contracting state. The only changes done are the following: 
* The control system for this iteration does not require 2 threshold values (high and low). Hence, only 1 threshold value to to be determined by the algorithm. 
* The new signal processing technique is used. All samples are obtained using the simpleSample function, as opposed to the first iteration where samples are directly obtained from AnalogRead. 
### Skip Calibration option
Because the full calibration process takes over 1 minute, the user has the option of skipping most of it. This option limits calibration to only establishing the baseline for each channel and assumes a default threshold of 12. With this option calibration would take around 3 seconds for 1 channel and 6 seconds for 2 channels.
## Sample Analysis and Control
The user is allowed to pre-select the number of EMG channels used to control the arm (1 or 2) by modifying the Arm_Settings.h file.
### 1 Channel Input
The control system used for this iteration consists of a toggle switch that is triggered by values above the threshold. Whenever 2 consecutive values are detected above the threshold value, the switch is activated and the state of the hand is changed. In other words, if the hand is open, the first contraction will cause it to close and the second contraction will cause it to open back again. Both contractions must be separated by a buffer refractory time of 800ms.  
The Arm_Sampler library also contains the tools used to judge whether the arm should be open or closed based on the obtained reading. The function evaluateSample returns an integer of either 0, 1, or 2 which correspond to the following:

0 -->Stay in the previous state

1 -->Change state (open if closed and close if open)

2 --> Electrode Misplacement Error (does not change state)

The following flow chart summarizes the control scheme for single channel input control:


![image](https://user-images.githubusercontent.com/46327849/88839710-6b69de80-d1a9-11ea-9c86-17881c996216.png)

### 2 Channel Input
The arm sampler first processes the incoming signal from each channel separately. Upon obtaining the 2 datapoints, the signal from the first (main) channel is evaluated. If a peak is detected, then the hand will immediately be ordered to the open position if closed, and to the close position if in any other position. This is similar to what occurs in 1-channel input control.
If no peak is detected from the first channel, then the device 'listens' to the processed input from the second channel. If a peak is detected from the second channel, then a 3rd 'special' position is performed by the hand. If no peak is detected from either channel, the hand remains in its previous state.
The function evaluateSample2Electrodes handles control for the 2 channel input option. The function returns the following possible outputs:

0 -->Stay in the previous state

1 -->Change state (open if closed and close if in other position)

2 --> Electrode Misplacement Error (does not change state)

3 --> Perform special movement

The following flow chart summarizes the control scheme for 2 channel input control:


![image](https://user-images.githubusercontent.com/46327849/88865410-ce716a80-d1d5-11ea-95d3-fc7e5b8cd4f4.png)

# Testing
## Preliminary Testing
Testing was done on 3 usage cycles (calibrate-operate-reset). The primary electrode was placed on the biceps muscle whereas the secondary electrode was placed on the Brachio Radialis muscle.
Testing with PLA skin: 
| Test | Round 1 | Round 2| Round 3 |
| ------------- | ------------- | ------------- | ------------- | 
| Resting Hand False Positives |    0   | 0  | 0 |
| Moving Elbow False Positives |   0 |   0|  0|
| Tapping Hand False Positives |    1  |   2 |1  |
| 10 Contractions | 10/10 |  10/10 | 10/10 |
| 10 Special Movement | 10/10 | 10 /10 | 10/10 |
| 10 Contractions with hold | 10/10 | 10/10  | 10/10 |
| Glass Cup Test (small glass)| 10/10|  10/10 | 10/10 |
| Glass Cup Test (large glass)| 0/10|  0/10 | 0/10 |
| Apple Test | 5/10 | 6/10 | 8/10 |
Testing with Rubber Skin
| Test | Round 1 | Round 2| Round 3 |
| ------------- | ------------- | ------------- | ------------- | 
| Resting Hand False Positives |    0   | 0  | 0 |
| Moving Elbow False Positives |   1 |   0|  0|
| Tapping Hand False Positives |    1  |   1 |1  |
| 10 Contractions | 10/10 |  10/10 | 10/10 |
| 10 Special Movement | 10/10 | 10 /10 | 10/10 |
| 10 Contractions with hold | 10/10 | 10/10  | 10/10 |
| Glass Cup Test (small glass)| 10/10|  10/10 | 10/10 |
| Glass Cup Test (large glass)| 9/10|  10/10 | 10/10 |
| Apple Test | 10/10 | 10/10 | 10/10 |
### Test Protocol Details
Prior to each test: Install the Gravity EMG electrode on the Flexor Carpi Radialis Muscle location. Start the hand and calibrate it.
Resting Hand False Positives: Remain at rest for 1 minute. Report the number of movements initiated from false positive readings.
Moving Elbow False Positives: Similar procedure to Resting Hand False Positive Test. This test involves continously rotating the elbow slowly and reporting any false positives that occur within a minute.
Tapping Hand False Positives: Similar procedure to Resting Hand False Positive. This test involves gently tapping the hand at locations ~3cm away from the electrode in a continous manner and reporting false positives within a minute. 
10 Contractions: Perform 10 distinct contractions using the Flexor Carpi Radialis Muscle, once every 10 seconds. Report the number of successful hand movement responses to the muscle contractions. 
10 Special Movements: Before starting the device, add a second electrode on the Biceps Radii Muscle and change the electrode number setting to 2 in the settings.h file. Perform 10 distinct contractions using the Biceps Radii Muscle, once every 10 seconds. Report the number of successful special hand movement responses to the muscle contractions.
10 Contractions with hold: Same protocol as the 10 Contractions test. Leave 30 seconds between 2 successive contractions. 
Glass Cup Tests: Use the hand to pick up an empty glass cup, move it along a distance of 50cm, then lay it down at a specific target point. A glass cup weighing ~200g is used for the small glass and a cup weighing ~500g is used for the large glass. Perform this task 10 times and record the number of successful attempts.
Apple Test: Use the hand to pick up an average size McIntosh Apple, move it along a horizontal distance of 50cm, then place it down on a target location. Perform this task 10 times and record the number of successful attempts.
