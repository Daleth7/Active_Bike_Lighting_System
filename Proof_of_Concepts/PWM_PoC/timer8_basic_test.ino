/*************************************************************************

        Test if the driver is capable of writing to registers and
    that a square wave is actually generated. Also use this oppurtunity
    to measure the base (generic clock) frequency fed into the timer
    peripheral.

*************************************************************************/

#include "test_switches.hpp"

#ifdef TIMER8_ONLY_BASIC_FUNCTIONALITY_TEST

#include "timer8.hpp"

#define OUT_PIN 13
#define ERR_PIN 12

bool out_pin_state = true;
bool err_pin_state = false;

Timer8 basic_timer( 0x0,    // Select generic clock 0 as the clock for the timer
                    0x1B,   // Tie the generic clock signal to TC3's input
                    TC3,    // Macro representing memory address of peripheral settings
                    0x0,    // Set timer prescaler to 1
                    0x1,    // Set timer counter period to 1
                    18,     // Enable timer interrupts (TC3 IRQ# == 18)
                    true,   // Enable overflow interrupt
                    false,  // Disable match interrupt
                    0       // Match counter value
                    );

void setup(){
    pinMode(OUT_PIN, OUTPUT);
    pinMode(ERR_PIN, OUTPUT);
}

void loop(){
}

void TC3_Handler(){
    if(basic_timer.overflow_interrupt()){
        digitalWrite(OUT_PIN, out_pin_state = !out_pin_state);
        basic_timer.clear_overflow_interrupt();
    }
    if(basic_timer.match_interrupt()){
        digitalWrite(OUT_PIN, err_pin_state = !err_pin_state);  // Should never happen
        basic_timer.clear_match_interrupt();
    }
}

#endif