/*************************************************************************

        Test if the driver is capable of writing to registers and
    that a square wave is actually generated. Also use this oppurtunity
    to measure the base (generic clock) frequency fed into the timer
    peripheral.

*************************************************************************/

#include "watch_timer.h"

#include <cstdint>

#define OUT_PIN 13

Timer8 pwm {
    .p_timer_settings = (TcCount8*) TC3,
    .m_gen_clk_id = 0x0,
    .m_gen_out_id = 0x1B,
    .m_timer_irq_id = TC3_IRQn
};

std::uint8_t duty_counter = 77;
bool step_up = true;

void setup(){
    pinMode(OUT_PIN, OUTPUT);

    pwm.enable_watch_clock();
    pwm.configure_watch_timer(TC_CTRLA_PRESCALER_DIV8, 100, 77);
    pwm.configure_watch_interrupt(true, true);

        // Do any enabling after all setup work is done
    pwm.enable_watch_timer();
}

void loop(){
    if(duty_counter == 80){
        step_up = false;
    } else if(duty_counter == 15){
        step_up = true;
    }
    if(step_up) ++duty_counter;
    else        --duty_counter;
    pwm.set_match_value(duty_counter);
    delay(10);
}

void TC3_Handler(){
    if(pwm.watch_interrupted_overflowed()){
        digitalWrite(OUT_PIN, HIGH);
        pwm.clear_watch_interrupt_overflowed();
    }
    if(pwm.watch_interrupted_match()){
        digitalWrite(OUT_PIN, LOW);
        pwm.clear_watch_interrupt_match();
    }
}