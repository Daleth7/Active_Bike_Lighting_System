#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"

#include <cstdint>

#define RED1_PIN    4
#define YELLOW1_PIN 3
#define GREEN1_PIN  2
#define RED2_PIN    7
#define YELLOW2_PIN 6
#define GREEN2_PIN  5

#define WHITE_PIN   9

#define OUT_PIN 13

TimerCount3& timer = TimerCount3::singleton();
TimerCount4& pwm = TimerCount4::singleton();
volatile std::uint32_t ref_time = 0;
volatile bool out_state = false;

float duty_tracker = 0.01;
const float duty_step = 0.0025;
bool step_up = true;

void pwm_overflow_cb(std::uint32_t counter);
void pwm_match_cb(std::uint32_t counter);

void timer_overflow_cb(std::uint32_t);
void timer_match_cb(std::uint32_t);

void setup(){
    pinMode(RED1_PIN, OUTPUT);
    pinMode(YELLOW1_PIN, OUTPUT);
    pinMode(GREEN1_PIN, OUTPUT);
    pinMode(RED2_PIN, OUTPUT);
    pinMode(YELLOW2_PIN, OUTPUT);
    pinMode(GREEN2_PIN, OUTPUT);
    pinMode(WHITE_PIN, OUTPUT);

    pinMode(OUT_PIN, OUTPUT);

    digitalWrite(RED1_PIN, LOW);
    digitalWrite(YELLOW1_PIN, LOW);
    digitalWrite(GREEN1_PIN, LOW);
    digitalWrite(RED2_PIN, HIGH);
    digitalWrite(YELLOW2_PIN, LOW);
    digitalWrite(GREEN2_PIN, LOW);
    digitalWrite(WHITE_PIN, LOW);

    digitalWrite(OUT_PIN, LOW);

        // Configure generator 2 to output a 4 MHz clock
    config_gclk_gen(    0x2,       // Appy changes to generator 2
                        250,        // Divide clock by 250 --> f = 8e6 / 250 = 32 kHz
                        false,      // Select simple division, i.e. divide by 250
                        osc8m_clk  // Use OSC8M as the generator source (8 MHz)
                        );

        // Sets a timer at 1 Hz
    timer.init( 0x6,    // prescale by 4
                125,    // 8-bit counter period
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                10,     // Match value
                timer_overflow_cb, timer_match_cb,
                0x2,    // Use generic clock generator 2
                32e3    // Reference frequency
                );
        // Sets a PWM at 160 Hz
    pwm.init(   0x0,    // prescale by 1
                200,    // 8-bit counter period
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                10,     // Match value
                pwm_overflow_cb, pwm_match_cb,
                0x2,    // Use generic clock generator 2
                32e3    // Reference frequency
                );

    timer.enable();
    pwm.enable();
}

void loop(){
    if(duty_tracker >= 0.9925)        step_up = false;
    else if(duty_tracker <= 0.0075)   step_up = true;

    if(step_up) duty_tracker += duty_step;
    else        duty_tracker -= duty_step;

    pwm.set_duty_cycle(duty_tracker);

    delay(5);
}

void pwm_overflow_cb(std::uint32_t counter){
    digitalWrite(WHITE_PIN, HIGH);
}

void pwm_match_cb(std::uint32_t){
    digitalWrite(WHITE_PIN, LOW);
}

void timer_overflow_cb(std::uint32_t){
    static std::uint8_t led_pin = RED2_PIN;

    digitalWrite(led_pin, LOW);
    digitalWrite(--led_pin, HIGH);

    if(led_pin < GREEN1_PIN){
        led_pin = RED2_PIN;
        digitalWrite(RED2_PIN, HIGH);
    }

    digitalWrite(OUT_PIN, out_state = !out_state);
}

void timer_match_cb(std::uint32_t){
}