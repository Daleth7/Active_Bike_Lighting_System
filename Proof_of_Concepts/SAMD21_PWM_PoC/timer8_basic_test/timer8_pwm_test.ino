/*************************************************************************

        Test if the driver will generate a PWM signal at the correct
    frequency and duty cycle.

    Goal: Generate a PWM signal around 77 kHz with a 77% duty cycle.

*************************************************************************/

#include "test_switches.hpp"

#ifdef TIMER8_ONLY_PWM_TEST

#include "timer8.hpp"

#define OUT_PIN 13

Timer8 basic_timer( 0x0,            // Select generic clock 0 as the clock for the timer
                    0x1B,           // Tie the generic clock signal to TC3's input
                    (TcCount8*)TC3, // Macro representing memory address of peripheral settings
                    0x3,            // Set timer prescaler to 8 --> 1 MHz
                    13,             // Set timer counter period to 13 --> 76.923 kHz
                    TC3_IRQn,       // Enable timer interrupts (TC3 IRQ# == 18)
                    true,           // Enable overflow interrupt
                    true,           // Enable match interrupt
                    10              // Match counter value to get 77% duty cycle
                    );

void setup(){
    pinMode(OUT_PIN, OUTPUT);
}

void loop(){
}

void TC3_Handler(){
    if(basic_timer.overflowed()){
        digitalWrite(OUT_PIN, true);
        basic_timer.clear_overflow_interrupt();
    }
    if(basic_timer.matched()){
        digitalWrite(OUT_PIN, false);
        basic_timer.clear_match_interrupt();
    }
}

#endif