#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"
#include "timer_duty_pattern.hpp"

#include <cstdint>

#define OUT_PIN

TimerCount3& timer = TimerCount3::singleton();

void setup(){
        // Configure generator 2 to output a 16.384 kHz clock
    config_gclk_gen(    0x2,        // Appy changes to generator 2
                        2,          // Divide clock by 1 --> f = 32.768 kHz = 16.384 kHz
                        false,      // Select simple division, i.e. divide by 32
                        osc32k_clk  // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );

        // Sets a timer at 2 Hz
    timer.init( 0x4,    // prescale by 32 --> 512 Hz
                255,    // 8-bit counter period --> 2.007 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                0,      // Match value
                0, 0,   // Set no callbacks. Allow later object to do that
                0x2,    // Use generic clock generator 2
                16.384e3// Reference frequency
                );

    float time_pattern[5] = { 5e5, 2.5e5, 7.5e5, 0, 1e6 };

    auto pattern = make_duty_pattern( time_pattern, time_pattern+5,
                                      &timer,
                                      OUT_PIN,
                                      true
                                      );

    pattern.enable();
    timer.enable();
}

void loop(){
}
