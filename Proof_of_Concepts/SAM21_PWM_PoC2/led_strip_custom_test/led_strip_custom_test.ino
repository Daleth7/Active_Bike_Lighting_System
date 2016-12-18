#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"
#include "timer_duty_pattern.hpp"

#include <cstdint>

#define OUT_PIN 13

#define FUNC_NULL (TimerCount3::callback_func_type)(0)

TimerCount3& timer = TimerCount3::singleton();

float time_seq[5] = { 0, 1e6, 2e6, 3e6, 3.999e6 };
TimerDutyPattern<float*, TimerCount3> time_pattern( time_seq, time_seq+5,
                                                    &timer,
                                                    OUT_PIN,
                                                    true
                                                    );

void timer_ovr_cb(std::uint32_t counter);
void timer_mat_cb(std::uint32_t counter);

void setup(){
    SerialUSB.begin(9600);
    pinMode(OUT_PIN, OUTPUT);
        // Configure generator 2 to output a 16.384 kHz clock
    config_gclk_gen(    0x2,        // Appy changes to generator 2
                        1,          // Divide clock by 1 --> f = 32.768 kHz = 32.768 kHz
                        false,      // Select simple division, i.e. divide by 32
                        osculp32k_clk  // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );

        // Sets a timer at 1 Hz
    timer.init( 0x7,                    // prescale by 1024 --> 32 Hz
                128,                    // 8-bit counter period -->  0.25 Hz
                true,                   // Interrupt on overflow
                true,                   // Interrupt on match
                50,                      // Match value
                0x2,                    // Use generic clock generator 2
                32.768e3,               // Reference frequency
                FUNC_NULL, FUNC_NULL    // Set no callbacks. Allow later object to do that
//                timer_ovr_cb, timer_mat_cb    // Set no callbacks. Allow later object to do that
                );

    time_pattern.enable();
    timer.enable();
}

void loop(){
    extern bool overflow_triggered;
    extern float curr_time, freq_used, calc_duty;
    if(overflow_triggered){
        overflow_triggered = false;
        SerialUSB.print("New time: ");
        SerialUSB.print(curr_time);
        SerialUSB.print(" (");
        SerialUSB.print(curr_time/1.0e6);
        SerialUSB.print(") | Duty: ");
        SerialUSB.print(timer.duty_cycle());
        SerialUSB.print(" | Calc. Duty: ");
        SerialUSB.print(calc_duty);
        SerialUSB.print(" | period: ");
        SerialUSB.print(time_pattern.period());
        SerialUSB.print(" | frequency: ");
        SerialUSB.print(freq_used);
        SerialUSB.println("");
    }
}

void timer_ovr_cb(std::uint32_t counter){
    digitalWrite(OUT_PIN, HIGH);
}

void timer_mat_cb(std::uint32_t counter){
    digitalWrite(OUT_PIN, LOW);
}
