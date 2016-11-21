/*************************************************************************

        Test if the driver will generate a PWM signal at the correct
     duty cycles.

*************************************************************************/

#include "test_switches.hpp"

#ifdef TIMER8_ONLY_SWEEP_DUTY_TEST

#include "timer8.hpp"

#define OUT_PIN 13

Timer8 basic_timer( 0x0,            // Select generic clock 0 as the clock for the timer
                    0x1B,           // Tie the generic clock signal to TC3's input
                    (TcCount8*)TC3, // Macro representing memory address of peripheral settings
                    0x5,            // Set timer prescaler to 64
                    200,            // Set timer counter period to multiple of 100 for easier visualization
                    TC3_IRQn,       // Enable timer interrupts (TC3 IRQ# == 18)
                    true,           // Enable overflow interrupt
                    true,           // Enable match interrupt
                    100             // Start at 50% duty cycle
                    );

void setup(){
    pinMode(OUT_PIN, OUTPUT);
}

void loop(){
    static std::uint8_t match_value_counter = 100;
    if(match_value_counter >= 200)  match_value_counter = 0;
    basic_timer.set_match_value(++match_value_counter);

    delay(250);
}

void TC3_Handler(){
    if(basic_timer.overflow_interrupt()){
        digitalWrite(OUT_PIN, true);
        basic_timer.clear_overflow_interrupt();
    }
    if(basic_timer.match_interrupt()){
        digitalWrite(OUT_PIN, false);
        basic_timer.clear_match_interrupt();
    }
}

#endif