#include "timer8_tc3.hpp"

#include <cstdint>

#define OUT_PIN 13
#define SLOW_PIN 9
TimerCount3& pwm = TimerCount3::singleton();
volatile std::uint32_t ref_time = 0;
volatile bool slow_state = false;

float duty_tracker = 0.05;
const float duty_step = 0.0025;
bool step_up = true;

void pwm_overflow_cb(std::uint32_t counter);
void pwm_match_cb(std::uint32_t counter);

void setup(){
    pinMode(OUT_PIN, OUTPUT);
    pinMode(SLOW_PIN, OUTPUT);

    digitalWrite(OUT_PIN, LOW);
    digitalWrite(SLOW_PIN, LOW);

        // Sets a PWM at 5 kHz
    pwm.init(   48e6,   // Reference frequency
                0x5,    // prescale by 64
                150,    // 8-bit counter period
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                75,     // Match value
                pwm_overflow_cb, pwm_match_cb
                );
    pwm.enable();
}

void loop(){
    if(duty_tracker >= 0.95)        step_up = false;
    else if(duty_tracker <= 0.05)   step_up = true;

    if(step_up) duty_tracker += duty_step;
    else        duty_tracker -= duty_step;

    pwm.set_duty_cycle(duty_tracker);

    delay(5);
}

void pwm_overflow_cb(std::uint32_t counter){
    digitalWrite(OUT_PIN, HIGH);
    if((counter - ref_time) >= 3125){
        digitalWrite(SLOW_PIN, slow_state = !slow_state);
        ref_time = counter;
    }
}

void pwm_match_cb(std::uint32_t){
    digitalWrite(OUT_PIN, LOW);
}