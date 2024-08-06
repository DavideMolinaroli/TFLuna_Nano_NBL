#include <Arduino.h>
#include <Wire.h>
#include <TFLunaI2C.h>
#include <math.h>

// Stepper motor values
#define REV_STEPS 200
#define ANGLE_STEP 1.8

// Stepper motor pins
uint8_t stepPin = 3;
uint8_t dirPin = 4;

// TFLuna data
uint16_t freq = 0XFA;
uint16_t dist;
uint16_t amp;
uint16_t temp;

// Lidar
TFLunaI2C tfl;

// Interrupt variable
uint8_t counts = 0;

// Angle measurements
float angle_deg = 0;
float actual_step_angle = ANGLE_STEP*0.75;

unsigned long cur_millis;
unsigned long prev_millis = 0;
unsigned long millis_between_steps = 4.5;

bool perform_step(){
	if(cur_millis - prev_millis >= millis_between_steps){
		prev_millis = cur_millis;
		digitalWrite(stepPin,HIGH);
		digitalWrite(stepPin,LOW);

		return true;
	}

	return false;
}

// Interrupt callback, called when hall sensor is triggered
void magnet_detected() {
	if(counts == 0) {
		counts++;
	}
}

// Homing routine
void home_position() {
	while(!counts) {
		cur_millis = millis();
		perform_step();
	}

	// Compensate for early detection due to magnet position
	uint8_t remaining = 150;

	while(remaining != 0) {
		cur_millis = millis();
		if(perform_step()) {
			remaining--;
		}
	}

	angle_deg = 0;
}

void setup() {
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(2), magnet_detected, RISING);
	Serial.begin(230400);
	Wire.begin();
	tfl.set_freq(freq);
	tfl.get_freq(freq);

	cur_millis = millis();
	home_position();
	delay(1000);
}

void loop() {
	cur_millis = millis();
	
	if(perform_step()) {
		angle_deg += actual_step_angle;
		
		if(angle_deg > 360 && angle_deg - actual_step_angle < 360) {
			angle_deg = angle_deg - 360;
		}

		tfl.get_data(dist);
		Serial.println(String(dist)+" "+String(angle_deg));
	}
	
}