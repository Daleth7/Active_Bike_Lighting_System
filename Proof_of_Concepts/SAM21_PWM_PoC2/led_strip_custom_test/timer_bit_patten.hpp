/*************************************************************************

                          TimerBitPattern class

        Simplified interface to send a specific bit pattern through
    a GPIO. A timer module is used to time the frequency of bit sending.
    Note that this class does not handle timer configuration, so the
    user must make sure the timer is configured to the desired
    frequency first.



    To instantiate a new timer, the following parameters are needed:
        1) The address of the timer peripheral                  (pg. 40 [9] SAMD21 E/G/J datasheet complete)
        2) The ID of the timer to tie with the generic clock    (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete)
        3) The IRQ id of the timer in the NVIC                  (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete)



    Example:
        // Task: Instantiate the TC3 peripheral
        //  Memory address:     0x42002C00
        //  Gen. Clk. Sel. ID:  0x1B
        //  NVIC ISR ID:        18
        typedef TimerCounter<0x42002C00, 0x1B, (IRQn)(18)> TimerCount3;

        TimerCount3& my_timer = TimerCount3::singleton();


        Note that, by default, Arduino will set the generic clock
    generator 0 to generate 48 MHz.

    Example usage:

        // Task: Set up a pwm signal at 2.2 kHz with a duty cycle of 80%

        void overflow_cb(std::uint32_t counter);
        void match_cb(std::uint32_t counter);

        typedef TimerCounter<0x42002C00, 0x1B, (IRQn)(18)> TimerCount3;

        ...

        TimerCount3& my_pwm = TimerCount::singleton();
        my_pwm.init(    0x6,    // Set prescaler to 256 --> 187.5 kHz
                        85,     // 8-bit counter period --> 2.206 kHz
                        true,   // Interrupt on overflow
                        true,   // Interrupt on match
                        68,     // Match value --> 80% duty cycle
                        overflow_cb,
                        match_cb,
                        0x0,    // Use generic clock generator 0 (48 MHz)
                        48e6,   // Reference frequency
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

#ifndef TIMER_BIT_PATTERN_EXCCTTTENSS2222
#define TIMER_BIT_PATTERN_EXCCTTTENSS2222

#endif