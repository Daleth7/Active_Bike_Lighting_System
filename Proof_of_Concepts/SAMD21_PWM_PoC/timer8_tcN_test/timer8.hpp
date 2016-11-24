/*************************************************************************

                              Timer8 class

        For more detailed information, refer to the SAMD21 E/G/J
    datasheet complete (pg. 635 [30]).

        Basic interface to set up and use an 8-bit timer. Each instance
    is responsible only for storing configuration information about the
    timer used (e.g. TC3 or TC2) and to allow simpler reconfiguration of
    properties such as timer duty cycle or interrupt enable. Remember
    that each timer interrupt has its own respective ISR. For example,
    interrupts for TC3 will be handled by TC3_Handler(). These handlers
    have to be defined by the user.
        Note that, by default, Arduino will set the generic clock
    generators to generate 47.8 MHz (Need to verify).

    Example usage:

        //  Task: Set up PWM at 2 kHz with 80% duty cycle. Use the TC3 peripheral.
        Timer8 my_pwm;
        my_pwm.init(    0x0,    // Select generic clock 0 as the clock for the timer
                        0x1B,   // Tie the generic clock signal to TC3's input
                        TC3,    // Macro representing memory address of peripheral settings
                        0x6,    // Set timer prescaler to 256 --> f = 186 kHz
                        93,     // Set timer counter period --> f = 2 kHz
                        18,     // Enable timer interrupts (TC3 IRQ# == 18)
                        true,   // Enable overflow interrupt
                        true,   // Enable match interrupt
                        74      // Interrupt every time counter reaches this value
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

#ifndef WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH
#define WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH

#include <cstdint>      // Contains known bit length types
#include "sam.h"    // Contains register memory mapping for SAMD21
                    // Should include samd21g18a.h

class Timer8 {
    public:
        const TcCount8* raw_TC_pointer()const;
/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
        void init(  std::uint8_t generic_clk_id,    // Generic Clock to use for timer (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                    std::uint8_t gen_out_id,        // Timer to recieve generic clock (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                    TcCount8* timer_peripheral,     // Address of timer settings registers (pg. 650 [30.7] SAMD21 E/G/J datasheet complete).
                    std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                    std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                    IRQn timer_irq_id,              // Interrupt Request ID of timer in NVIC (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete).
                    bool interrupt_on_overflow,     // Interrupt every period
                    bool interrupt_on_match,        // Interrupt upon matching specified value
                    std::uint8_t match_value        // Value to interrupt on
                    );
        void configure_generic_clock();
        void enable();
        void disable();
           // Call enable() after a call to configure_settings().
        void configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period);
        void set_prescale(std::uint32_t new_prescale);
        void set_counter_period(std::uint8_t new_period);
/*************************************************************************
                    Timer configuration functions end
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

/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/
            // Upon construction, the instance will configure:
            //  1) generic clock
            //  2) timer settings
            //  3) interrupt settings
        Timer8();
        ~Timer8(){}
/*************************************************************************
                Timer Constructors and destructors end
*************************************************************************/

    protected:
        Timer8(const Timer8&){} // Force the class to be non-copyable
        Timer8& operator=(const Timer8&){return *this;}

        TcCount8*       p_timer_settings;
        std::uint8_t    m_gen_clk_id;
        std::uint8_t    m_gen_out_id;
        IRQn            m_timer_irq_id;
};

#endif