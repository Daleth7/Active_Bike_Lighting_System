#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"

#include <cstdint>

#define DUTY_OUT_PIN    13
#define FREQ_OUT_PIN    12

TimerCount3& duty_sweeper = TimerCount3::singleton();
TimerCount4& frequency_sweeper = TimerCount4::singleton();

bool freq_out_state = false;
bool print_sec = false;
long int seconds_passed = 0;
std::uint32_t cycles_per_second = 0;
bool count_up = true;

void duty_overflow_cb(std::uint32_t counter);
void duty_match_cb(std::uint32_t counter);

void timer_overflow_cb(std::uint32_t);

void setup(){
    SerialUSB.init(9600);

    pinMode(DUTY_OUT_PIN, OUTPUT);
    pinMode(FREQ_OUT_PIN, OUTPUT);

    digitalWrite(DUTY_OUT_PIN, LOW);
    digitalWrite(FREQ_OUT_PIN, LOW);

    // Configure generator 2 to output a 4 MHz clock
    config_gclk_gen(    0x2,    // Appy changes to generator 2
                        12,     // Divide clock by 12 --> f = 48 MHz = 4 MHz
                        false,  // Select simple division, i.e. divide by 12
                        dfll48m // Use Digital Frequency Locked Loop as the generator source (32.768 kHz)
                        );

    // Configure generator 3 to output a 32.768 kHz clock
    config_gclk_gen(    0x3,            // Appy changes to generator 3
                        1,              // Divide clock by 1 --> f = 32.768 kHz = 32.768 kHz
                        false,          // Select simple division, i.e. divide by 1
                        osculp32k_clk   // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );

    // Sets a timer at 4 Hz
    frequency_sweeper.init( 0x0,    // prescale by 1 --> 4 MHz
                            1,      // 8-bit counter period
                            true,   // Interrupt on overflow
                            false,  // Interrupt on match
                            0,      // Match value
                            0x2,    // Use generic clock generator 2
                            4e6,    // Reference frequency
                            timer_overflow_cb
                            );
    cycles_per_second = frequency_sweeper.frequency();

    // Sets a PWM at 131.072 Hz
    duty_sweeper.init(  0x0,        // prescale by 1 --> 32.768 kHz
                        250,        // 8-bit counter period --> 131.072 Hz
                        true,       // Interrupt on overflow
                        true,       // Interrupt on match
                        10,         // Match value
                        0x3,        // Use generic clock generator 3
                        32.768e3,   // Reference frequency
                        duty_overflow_cb, duty_match_cb
                        );

    frequency_sweeper.enable();
    duty_sweeper.enable();
}

void loop(){
    if(print_sec){
        print_sec = false;

        std::uint8_t new_per = frequency_sweeper.counter_period();

        if(new_per == 255)      count_up = false;
        else if(new_per == 0)   count_up = true;

        if(count_up)    ++new_per;
        else            --new_per;

        frequency_sweeper.set_counter_period(new_per);

        Serial.print("Seconds passed: ");
        Serial.println(++seconds_passed);
    }
}

void duty_overflow_cb(std::uint32_t){
    digitalWrite(DUTY_OUT_PIN, HIGH);
}

void duty_match_cb(std::uint32_t){
    digitalWrite(DUTY_OUT_PIN, LOW);
}

void timer_overflow_cb(std::uint32_t counter){
    static ref_time = counter;

    digitalWrite(FREQ_OUT_PIN, freq_out_state = !freq_out_state);
    if((counter - ref_time) == cycles_per_second){
        print_sec = true;
        ref_time = counter;
    }
}
