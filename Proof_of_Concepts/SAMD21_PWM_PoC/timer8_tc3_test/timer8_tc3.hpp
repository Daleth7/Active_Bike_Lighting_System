/*************************************************************************

                              TimerCount3 class

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
        TimerCount3& my_pwm = TimerCount3::singleton();
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

#ifndef TIMER_TC3_BIT8___LENGTH_DRIV3333_____
#define TIMER_TC3_BIT8___LENGTH_DRIV3333_____

#include "timer8.hpp"

class TimerCount3 {
    public:
        typedef void (*callback_func_type)(std::uint32_t counter);
/*************************************************************************
                            Singleton access start
    - Parameters will be ignored after first call.
*************************************************************************/
        static TimerCount3& singleton();
/*************************************************************************
                            Singleton access end
*************************************************************************/

/*************************************************************************
                            Read-only start
*************************************************************************/
        std::uint32_t prescaler()const;
        std::uint8_t counter_period()const;
        std::uint8_t counter_match_value()const;

        float reference_frequency()const;   // Hz
        float frequency()const;             // Hz
        float period()const;                // ms
        float duty_cycle()const;            // %
/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                            Modifiers start
*************************************************************************/
        void init(  float generic_clock_frequency,  // Reference frequency to calculate current timer frequency
                    std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                    std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                    bool interrupt_on_overflow,     // Interrupt every period
                    bool interrupt_on_match,        // Interrupt upon matching specified value
                    std::uint8_t match_value,       // Value to interrupt on
                    callback_func_type isr_over_cb, // Callback function called by timer ISR upon counter overflow
                    callback_func_type isr_match_cb // Callback function called by timer ISR upon counter match
                    );
        void enable();
        void disable();
           // Call enable() after a call to configure_settings().
        void configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period);
        void set_prescale(std::uint32_t new_prescale);
        void set_counter_period(std::uint8_t new_period);
/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
        void configure_interrupt(   bool interrupt_on_overflow,
                                    bool interrupt_on_match,
                                    std::uint8_t match_value);
        bool overflowed()const;
        bool matched()const;
        void clear_overflow_interrupt();
        void clear_match_interrupt();

            // Recommend disabling timer before configuring interrupts
        void enable_overflow_interrupt();
        void disable_overflow_interrupt();
        void enable_match_interrupt();
        void disable_match_interrupt();
        void set_match_value(std::uint8_t new_match_value);
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

        ~TimerCount3(){}
    protected:
/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/
        TimerCount3();
/*************************************************************************
                        Timer Constructors end
*************************************************************************/

/*************************************************************************
                    Timer Interrupt Service Routine start
*************************************************************************/
        friend void TC3_Handler();
/*************************************************************************
                    Timer Interrupt Service Routine end
*************************************************************************/

    private:
        Timer8              m_general_timer;

        float               k_gen0_clk_freq;
        callback_func_type  m_ISR_overflow_cb;
        callback_func_type  m_ISR_match_cb;
};

#endif