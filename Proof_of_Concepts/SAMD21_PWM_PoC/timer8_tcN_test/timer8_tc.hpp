/*************************************************************************

                              TimerCount class

        For more detailed information, refer to the SAMD21 E/G/J
    datasheet complete (pg. 635 [30]).

        Basic interface to set up and use an 8-bit timer using the timer
    counter three peripheral. The interface follows the singleton pattern
    since there is only one TC3 module possible.
        Note that, by default, Arduino will set the generic clock
    generators to generate 48 MHz (Need to verify).

    Example usage:

        void overflow_cb(std::uint32_t counter);
        void match_cb(std::uint32_t counter);

        // Task: Set up a pwm signal at 2.2 kHz with a duty cycle of 80%
        TimerCount& my_pwm = TimerCount::singleton();
        my_pwm.init(    48e6,   // Reference frequency
                        0x5,    // Set prescaler to 64
                        100,    // 8-bit counter period
                        true,   // Interrupt on overflow
                        false,  // Interrupt on match
                        80,     // Match value
                        overflow_cb,
                        match_cb
                        );
        my_pwm.enable();   // After configuration, the timer is still disabled.

        ...

        void TC3_Handler(){
            if(my_pwm.overflowed()){
                ... Do something after each complete clock cycle
                my_pwm.clear_overflow_interrupt();
            }
            if(my_pwm.matched()){
                ... Do something after reaching the duty cycle period
                my_pwm.clear_match_interrupt();
            }
        }

*************************************************************************/

#ifndef IN_GUARD_TI777MER_INSTANTIATIONS333
#define IN_GUARD_TI777MER_INSTANTIATIONS333

#include "timer8_tc_template.hpp"

typedef TimerCount<(TcCount*)(TC3), 0x1B, TC3_IRQn> TimerCount3;

#endif