#include "interrupt_callbacks.hpp"

#include "pin_defines.h"



#define OUTER_ARROW 	3
#define MIDDLE_ARROW 	2
#define INNER_ARROW 	1

bool leftTurnFlag = false; // initialize turn signal flag variables as 1 = ON
bool rightTurnFlag = false; 
std::uint8_t arrow_pin = INNER_ARROW; // Keep track of which arrow is turned on (Inner, Middle, Outer)

bool led_strip_flag = false;

void checkLeftTurn(){ // interrupt call function for checking left turn signal status
	
	if ((digitalRead(LEFTSW)) == 0){ // read digital value of LEFT TURN pin
		leftTurnFlag = true; // sets LEFT TURN flag
		arrow_pin = INNER_ARROW;
	}
	
	if ((digitalRead(LEFTSW)) == 1){ // read digital value of LEFT TURN pin
		leftTurnFlag = false; // clear left turn flag
	}
	
}

void checkRightTurn() { // interrupt call function for checking right turn signal status

	if ((digitalRead(RIGHTSW)) == 0){ // read digital value of RIGHT TURN pin
		rightTurnFlag = true; // sets RIGHT TURN flag
		arrow_pin = INNER_ARROW;
	}
	
	if ((digitalRead(RIGHTSW)) == 1){ // read digital value of RIGHT TURN pin
		rightTurnFlag = false; // clear right turn flag
	}


}

void pwm_overflow_cb(std::uint32_t){
    digitalWrite(BRAKEPIN, HIGH);
}

void pwm_match_cb(std::uint32_t){
    digitalWrite(BRAKEPIN, LOW);
}

void timer_overflow_cb(std::uint32_t){ // operating off 4 Hz timer/counter setting
	//emergency case
	static bool emergency_toggle_state = true;

    const bool
            // Check if both switches are on
            both_sw_pressed = (rightTurnFlag && leftTurnFlag),

            // Turn on arrows on both sides if both switches are on
            both_turn_on    = emergency_toggle_state & both_sw_pressed,

            // If in cycling state, determine which arrow should be on
            outer_right_cyc = rightTurnFlag & (arrow_pin == OUTER_ARROW),
            outer_left_cyc  = leftTurnFlag  & (arrow_pin == OUTER_ARROW),
            mid_right_cyc   = rightTurnFlag & (arrow_pin == MIDDLE_ARROW),
            mid_left_cyc    = leftTurnFlag  & (arrow_pin == MIDDLE_ARROW),
            inner_right_cyc = rightTurnFlag & (arrow_pin == INNER_ARROW),
            inner_left_cyc  = leftTurnFlag  & (arrow_pin == INNER_ARROW),

            // Calculate final turn on state based on which switches
            //  are pressed and which arrow is on
            outer_right_on  = both_turn_on | (!both_sw_pressed & outer_right_cyc),
            outer_left_on   = both_turn_on | (!both_sw_pressed & outer_left_cyc),
            mid_right_on    = both_turn_on | (!both_sw_pressed & mid_right_cyc),
            mid_left_on     = both_turn_on | (!both_sw_pressed & mid_left_cyc),
            inner_right_on  = both_turn_on | (!both_sw_pressed & inner_right_cyc),
            inner_left_on   = both_turn_on | (!both_sw_pressed & inner_left_cyc)
            ;

    // Turn on the appropriate arrows
    digitalWrite(OUTERRIGHT,    outer_right_on);
    digitalWrite(OUTERLEFT,     outer_left_on);
    digitalWrite(MIDRIGHT,      mid_right_on);
    digitalWrite(MIDLEFT,       mid_left_on);
    digitalWrite(INNERRIGHT,    inner_right_on);
    digitalWrite(INNERLEFT,     inner_left_on);

    // cycle through the arrows (from inner to outer)
    ++arrow_pin;
    if(arrow_pin > OUTER_ARROW)
    {
        arrow_pin = INNER_ARROW;
    }

    emergency_toggle_state = !emergency_toggle_state;

	
}

void timer_match_cb(std::uint32_t){
}

void striptimer_overflow_cb(std::uint32_t){
	//greenLEDs();
    led_strip_flag = true;
}

void striptimer_match_cb(std::uint32_t){}