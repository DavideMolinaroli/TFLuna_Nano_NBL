#include <Arduino.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <TFLunaI2C.h>
#include <math.h>

// Stepper motor values
#define REV_STEPS 200
#define ANGLE_STEP 1.8

// Stepper motor pins
uint8_t stepPin = 3;
uint8_t dirPin = 4;

// TFLuna data
uint16_t freq = 0X1F4;
uint16_t dist;
uint16_t amp;
uint16_t temp;

// Lidar and stepper motor
TFLunaI2C tfl;
AccelStepper motor = AccelStepper(1,stepPin, dirPin);

// Interrupt variable
uint8_t counts = 0;

// Angle measurements
float angle_deg = 0;
float actual_step_angle = ANGLE_STEP*0.75;

// Interrupt callback to reset the sensor angle
void get_diff() {
	angle_deg = 0;

	if(counts == 0) {
		counts++;
	}
}

// Homing routine
void home_position() {
	motor.setMaxSpeed(1000);

	// step until the first magnet is detected
	motor.setSpeed(700);
	while(!counts){
		motor.runSpeed();
	}
	motor.setCurrentPosition(0);

	// 10 steps equals to 18*0.75 = 13.5 degrees that have to be compensated
	// in the data visualizer
	// compensate for early magnet detection
	//motor.setSpeed(666);
	//while(motor.currentPosition()<=10) {
	//	motor.runSpeed();
	//}
	
	//motor.setCurrentPosition(0);
}

void setup() {
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(2), get_diff, RISING);
	Serial.begin(230400);
	Wire.begin();
	tfl.set_freq(freq);
	tfl.get_freq(freq);

	home_position();
	delay(1000);
	motor.setSpeed(700); // steps/s
}

void loop() {
	// n=pos2;
    // if (n >=spr){n = pos2 % int(spr);}//% operator provides the remainder, so this will provide the remainder after dividing pos2 by the number of steps per revolution
    // angle = 360*(float(n)/spr);
    // motor.runSpeed();
    // pos2=motor.currentPosition();
	// tfl.get_data(dist);
	// Serial.println(String(angle*PI/180)+"p"+String(dist)+"p"+String(n));
	if(motor.runSpeed()) {
		angle_deg += actual_step_angle;
		tfl.get_data(dist);
	}
	//angle_rad = (fmod(motor.currentPosition(),266.67))*ANGLE_STEP*PI/180;
	// Serial.print(tfl.get_data(dist, amp, temp));
	Serial.println(String(dist)+" "+String(angle_deg));
	
	// Serial.print(' ');
	// Serial.print(dist);
	// Serial.print(' ');
	// Serial.println(angle_rad);
}