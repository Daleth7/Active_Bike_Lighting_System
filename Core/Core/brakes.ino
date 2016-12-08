#include "brakes.hpp"

#include "pin_defines.h"
#include "timer8_tc.hpp"

static BrakePWM& brakepwm = BrakePWM::singleton();

void checkBrakeSignals() {
	
	// read the input on analog pins 1 and 2:
    float leftBrakeVoltage = analogRead(RIGHTBRAKE) * (3.3 / 1023.0); // reads analog voltage on pin A1 for Left Brakes
    float rightBrakeVoltage = analogRead(LEFTBRAKE) * (3.3 / 1023.0); // reads analog voltage on pin A2 for Right Brakes

	// Raising brake flag if EITHER left OR right brake is engaged
	// if value read by ADC after conversion is less than 2.7, raise brake flag
	static bool braked = false;	// Make sure register is not constantly being written to.
								//	If so, the duty cycle will fluctuate.
	if( ((leftBrakeVoltage < 2.7) || (rightBrakeVoltage < 2.7)) && !braked ){
		brakepwm.set_duty_cycle(0.2);	// Turn brighter
		braked = true;
	} else if(braked){
		braked = false;
		brakepwm.set_duty_cycle(0.075);	// Turn dimmer
	}
}