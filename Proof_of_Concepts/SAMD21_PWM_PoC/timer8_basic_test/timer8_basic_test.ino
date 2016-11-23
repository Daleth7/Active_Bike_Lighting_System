/*************************************************************************

        Test if the driver is capable of writing to registers and
    that a square wave is actually generated. Also use this oppurtunity
    to measure the base (generic clock) frequency fed into the timer
    peripheral.

    Generated signal frequency was mreasured to be 187 kHz using
    the following settings:
    basic_timer.init(   0x0,
                        0x1B,
                        (TcCount8*)TC3,
                        0x0,
                        0x1,
                        TC3_IRQn,
                        true,
                        false,
                        0
                    );

    Generated signal frequency was mreasured to be 3.7244 kHz using
    the following settings:
    basic_timer.init(   0x0,
                        0x1B,
                        (TcCount8*)TC3,
                        0x5,
                        200,
                        TC3_IRQn,
                        true,
                        false,
                        0
                    );

*************************************************************************/

#include "test_switches.hpp"

#ifdef TIMER8_ONLY_BASIC_FUNCTIONALITY_TEST

#include "timer8.hpp"

#define OUT_PIN 13
#define ERR_PIN 9

bool out_pin_state = true;
bool err_pin_state = false;

volatile Timer8 basic_timer;

void setup(){
    pinMode(OUT_PIN, OUTPUT);
    pinMode(ERR_PIN, OUTPUT);
    digitalWrite(OUT_PIN, LOW);
    digitalWrite(ERR_PIN, LOW);

    basic_timer.init(   0x0,            // Select generic clock 0 as the clock for the timer
                        0x1B,           // Tie the generic clock signal to TC3's input
                        (TcCount8*)TC3, // Macro representing memory address of peripheral settings
                        0x5,            // Set timer prescaler to 1
                        200,            // Set timer counter period to 1
                        TC3_IRQn,       // Enable timer interrupts (TC3 IRQ# == 18)
                        true,           // Enable overflow interrupt
                        true,          // Disable match interrupt
                        0               // Match counter value
                    );
    basic_timer.enable();
}

void loop(){
}

void TC3_Handler(){
    if(basic_timer.overflowed()){
        digitalWrite(OUT_PIN, out_pin_state = !out_pin_state);
        basic_timer.clear_overflow_interrupt();
    }
    if(basic_timer.matched()){
        digitalWrite(ERR_PIN, err_pin_state = !err_pin_state);  // Should never happen
        basic_timer.clear_match_interrupt();
    }
}

#endif