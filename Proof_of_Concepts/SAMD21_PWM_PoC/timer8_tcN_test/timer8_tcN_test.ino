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

bool brake_harder = false;

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
    config_gclk_gen(    0x2,            // Appy changes to generator 2
                        1,              // Divide clock by 1 --> f = 32.768 kHz = 32.768 kHz
                        false,          // Select simple division, i.e. divide by 1
                        osculp32k_clk   // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );

        // Sets a timer at 4 Hz
    timer.init( 0x7,    // prescale by 1024 --> 32 Hz
                8,    // 8-bit counter period --> 4 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                4,     // Match value
                timer_overflow_cb, timer_match_cb,
                0x2,    // Use generic clock generator 2
                32.768e3// Reference frequency
                );
        // Sets a PWM at 131.072 Hz
    pwm.init(   0x0,    // prescale by 1 --> 32.768 kHz
                250,    // 8-bit counter period --> 131.072 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                10,     // Match value
                pwm_overflow_cb, pwm_match_cb,
                0x2,    // Use generic clock generator 2
                32.768e3// Reference frequency
                );

    timer.enable();
    pwm.enable();
}

void loop(){
    if(brake_harder)    pwm.set_duty_cycle(0.2);
    else                pwm.set_duty_cycle(0.075);
    brake_harder = !brake_harder;

    delay(500);
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
    digitalWrite(OUT_PIN, out_state = !out_state);
}