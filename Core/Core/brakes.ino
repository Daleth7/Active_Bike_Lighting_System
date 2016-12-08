#include "brakes.hpp"

#include "pin_defines.h"
#include "timer8_tc.hpp"

static TimerCount4& brake_pwm = TimerCount4::singleton();

float leftBrakeVoltage = 0; // initialize ADC brake converted voltage value
float rightBrakeVoltage = 0;

void checkBrakeSignals() {
	
	// read the input on analog pin 1:
	int leftSensorValue = analogRead(RIGHTBRAKE); // reads analog voltage on pin A1 for Left Brakes
	int rightSensorValue = analogRead(LEFTBRAKE); // reads analog voltage on pin A2 for Right Brakes
	
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
	leftBrakeVoltage = leftSensorValue * (3.3 / 1023.0); // Conversion of int ADC value to voltage, using 3.3V reference limit
	rightBrakeVoltage = rightSensorValue * (3.3 / 1023.0); // 

	// Raising brake flag if EITHER left OR right brake is engaged
	// if value read by ADC after conversion is less than 2.7, raise brake flag
	static bool braked = false;	// Make sure register is not constantly being written to.
								//	If so, the duty cycle will fluctuate.
	if((leftBrakeVoltage < 2.7) || (rightBrakeVoltage < 2.7)){
		brake_pwm.set_duty_cycle(0.2);	// Turn brighter
		braked = true;
	} else if(braked){
		braked = false;
		brake_pwm.set_duty_cycle(0.075);	// Turn dimmer
	}
}