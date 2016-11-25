/*************************************************************************

                              TimerCount class

        For more detailed information, refer to the SAMD21 E/G/J
    datasheet complete (pg. 635 [30]).

        Template interface to set up and use an 8-bit timer using the timer
    counter peripheral. The interface follows the singleton pattern
    since there is only one of each TC module.
    
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

        TimerCount3 my_timer;


        Note that, by default, Arduino will set the generic clock
    generators to generate 48 MHz.

    Example usage:

        // Task: Set up a pwm signal at 2.2 kHz with a duty cycle of 80%

        void overflow_cb(std::uint32_t counter);
        void match_cb(std::uint32_t counter);

        typedef TimerCounter<0x42002C00, 0x1B, (IRQn)(18)> TimerCount3;

        ...

        TimerCount3& my_pwm = TimerCount::singleton();
        my_pwm.init(    48e6,   // Reference frequency
                        0x6,    // Set prescaler to 256 --> 187.5 kHz
                        85,     // 8-bit counter period --> 2.206 kHz
                        true,   // Interrupt on overflow
                        true,   // Interrupt on match
                        68,     // Match value --> 80% duty cycle
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

#ifndef TIMER_TC_TEMP_BIT8___LENGTH_DRIV3333_____
#define TIMER_TC_TEMP_BIT8___LENGTH_DRIV3333_____

#include "timer8.hpp"

template <std::uint32_t timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
class TimerCount {
    public:
/*************************************************************************
                            Type-aliases start
*************************************************************************/
        typedef void (*callback_func_type)(std::uint32_t counter);
/*************************************************************************
                            Type-aliases end
*************************************************************************/

/*************************************************************************
                            Singleton access start
    - Parameters will be ignored after first call.
*************************************************************************/
        static TimerCount& singleton();
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
        void init(  std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                    std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                    bool interrupt_on_overflow,     // Interrupt every period
                    bool interrupt_on_match,        // Interrupt upon matching specified value
                    std::uint8_t match_value,       // Value to interrupt on
                    callback_func_type isr_over_cb, // Callback function called by timer ISR upon counter overflow
                    callback_func_type isr_match_cb,// Callback function called by timer ISR upon counter match
                    std::uint8_t generator = 0x0,   // Select which generic clock generator to use. By default, generator 0 is already set up to generate 48 MHz.
                    float gen_clk_freq = 48e6       // Reference frequency to calculate current timer frequency
                    );
        void enable();
        void disable();
           // Call enable() after a call to configure_settings().
        void configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period);
        void set_prescale(std::uint32_t new_prescale);
        void set_counter_period(std::uint8_t new_period);
        void set_duty_cycle(float new_duty_cycle);  // Must be between 0 and 1
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
        callback_func_type  ISR_overflow_cb;
        callback_func_type  ISR_match_cb;
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

        ~TimerCount(){}
    protected:
/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/
        TimerCount();
/*************************************************************************
                        Timer Constructors end
*************************************************************************/

    private:
        Timer8              m_general_timer;

        float               k_gen0_clk_freq;
};

#include "timer8_tc_template_inline.h"

#endif