/*************************************************************************

        Test if the driver will generate a PWM signal at the correct
     frequencies.

     FAILED

*************************************************************************/

#include "test_switches.hpp"

#ifdef TIMER8_ONLY_SWEEP_FREQ_TEST

#include "timer8.hpp"

#define OUT_PIN 13
#define ERR_PIN 9

bool out_pin_state = true;
bool err_pin_state = false;

Timer8 basic_timer;

void setup(){
    pinMode(OUT_PIN, OUTPUT);
    pinMode(ERR_PIN, OUTPUT);

    basic_timer.init(   0x0,            // Select generic clock 0 as the clock for the timer
                        0x1B,           // Tie the generic clock signal to TC3's input
                        (TcCount8*)TC3, // Macro representing memory address of peripheral settings
                        0x2,            // Set timer prescaler to 1
                        1,              // Start at highest frequency and slow down
                        TC3_IRQn,       // Enable timer interrupts (TC3 IRQ# == 18)
                        true,           // Enable overflow interrupt
                        true,          // Disable match interrupt
                        100             // Match counter value
                        );
    basic_timer.enable();
}

void loop(){
    static std::uint8_t period_counter = 1;
    basic_timer.set_counter_period(++period_counter);

    delay(10);
}

void TC3_Handler(){
    if(basic_timer.overflowed()){
        digitalWrite(OUT_PIN, out_pin_state = !out_pin_state);
        basic_timer.clear_overflow_interrupt();
    }
    if(basic_timer.matched()){
        digitalWrite(OUT_PIN, err_pin_state = !err_pin_state);  // Should never happen
        basic_timer.clear_match_interrupt();
    }
}

#endif