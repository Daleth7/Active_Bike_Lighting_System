#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"
#include "timer_duty_pattern.hpp"

#include <cstdint>

#define OUT_PIN 13

TimerCount3& timer = TimerCount3::singleton();

float time_seq[9] = { 0, 0.5e6, 1e6, 1.5e6, 2e6, 2.5e6, 3e6, 3.5e6, 3.999e6 };
TimerDutyPattern<float*, TimerCount3> time_pattern( time_seq, time_seq+9,
                                                    &timer,
                                                    OUT_PIN,
                                                    true
                                                    );

void debug_overflow_callback(std::uint32_t counter);
void debug_match_callback(std::uint32_t counter);

C_Listener debug_listener = make_timer_listener( debug_overflow_callback,
                                                 debug_match_callback
                                                 );
float debug_recorded_time_passed = -1.0f;
float debug_recorded_ontime = -1.0f;
bool print_debug_information = false;

void setup(){
    pinMode(OUT_PIN, OUTPUT);
        // Configure generator 2 to output a 8.192 kHz clock
    config_gclk_gen(    0x2,           // Appy changes to generator 2
                        4,             // Divide clock by 4 --> f = 8.192 kHz
                        false,         // Select simple division, i.e. divide by 2
                        osculp32k_clk  // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );

        // Sets a timer at 0.5 Hz
    timer.init( 0x5,        // prescale by 64 --> 128 Hz
                255,        // 8-bit counter period -->  0.50196 Hz
                true,       // Interrupt on overflow
                true,       // Interrupt on match
                50,         // Match value
                0x2,        // Use generic clock generator 2
                8.192e3     // Reference frequency
                );

    timer.add_listener(&debug_listener);

    time_pattern.enable();
    timer.enable();
}

void loop(){
    if(print_debug_information){
        print_debug_information = false;
        SerialUSB.print("Time passed: ");
            SerialUSB.print(debug_recorded_time_passed);
            SerialUSB.print(" s");
        SerialUSB.print(" | ");
        SerialUSB.print("On-time: ");
            SerialUSB.print(debug_recorded_ontime);
            SerialUSB.print(" s");
        SerialUSB.println("");
    }
}

void debug_overflow_callback(std::uint32_t counter){
    debug_recorded_time_passed = counter / timer.frequency();
    print_debug_information = true;
}

void debug_match_callback(std::uint32_t){
    debug_recorded_ontime = timer.duty_cycle() / timer.frequency();
}