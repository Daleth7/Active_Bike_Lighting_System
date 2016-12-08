/*************************************************************************

                          TimerCount istantiations

        This header contains the instantiations for different timers
    based on a general template. To instantiate a new timer, the
    following parameters are needed:
        1) The address of the timer peripheral                  (pg. 40 [9] SAMD21 E/G/J datasheet complete)
        2) The ID of the timer to tie with the generic clock    (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete)
        3) The IRQ id of the timer in the NVIC                  (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete)

    Remember that each TC peripheral has its own, unique ISR function.
    This needs to be defined to use interrupts with the TC peripheral.

    Example:
        // Task: Instantiate the TC3 peripheral
        //  Memory address:     0x42002C00
        //  Gen. Clk. Sel. ID:  0x1B
        //  NVIC ISR ID:        18
        typedef TimerCounter<0x42002C00, 0x1B, (IRQn)(18)> TimerCount3;

        TimerCount3& my_timer = TimerCount3::singleton();

        ...

        void TC3_Handler(){
            if(my_timer.overflowed()){
                ... Do something after each complete clock cycle
                my_timer.clear_overflow_interrupt();
            }
            if(my_timer.matched()){
                ... Do something after reaching the duty cycle period
                my_timer.clear_match_interrupt();
            }
        }

*************************************************************************/

#ifndef IN_GUARD_TI777MER_INSTANTIATIONS333
#define IN_GUARD_TI777MER_INSTANTIATIONS333

#include "timer8_tc_template.hpp"

typedef TimerCount<(std::uint32_t)(TC3), 0x1B, TC3_IRQn> TimerCount3;   // TC3 timer
typedef TimerCount<(std::uint32_t)(TC4), 0x1C, TC4_IRQn> TimerCount4;   // TC4 timer
typedef TimerCount<(std::uint32_t)(TC5), 0x1C, TC5_IRQn> TimerCount5;   // TC4 timer

#endif