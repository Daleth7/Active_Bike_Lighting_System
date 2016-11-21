/*************************************************************************

                              Timer8 class

        Basic interface to set up and use an 8-bit timer. Each instance
    is responsible only for storing configuration information about the
    timer used (e.g. TC3 or TC2) and to allow simpler reconfiguration of
    properties such as timer duty cycle or interrupt enable. Remember
    that each timer interrupt has its own respective ISR. For example,
    interrupts for TC3 will be handled by TC3_Handler(). These handlers
    have to be defined by the user.

    Example usage:

        configure_settings(TC_CTRLA_PRESCALER_DIV64, 0xFFFF);
        enable();   // Note that after configuration,
                                //  the timer is left disabled.

        ...

        void TC3_Handler(){
            if(overflowed()){
                ... Do something after each complete clock cycle
                clear_overflow_interrupt();
            }
            if(matched()){
                ... Do something after reaching the duty cycle period
                clear_match_interrupt();
            }
        }

*************************************************************************/

#ifndef WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH
#define WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH

#include <cstdint>      // Contains known bit length types
#include "samd21g18a.h" // Contains register memory mapping for SAMD21

class Timer8 {
    public:
/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
        void configure_generic_clock();
        void enable();
        void disable();
           // Call enable() after a call to configure_settings().
        void configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period);
/*************************************************************************
                    Timer configuration functions end
*************************************************************************/

/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
        void configure_interrupt(  bool interrupt_on_overflow,
                                            bool interrupt_on_match,
                                            std::uint8_t match_value);
        bool overflowed();
        bool matched();
        void clear_overflow_interrupt();
        void clear_match_interrupt();
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/
        Timer8( std::uint8_t generic_clk_id,    // Generic Clock to use for timer (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                std::uint8_t gen_out_id,        // Timer to recieve generic clock (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                TcCount8* timer_peripheral,     // Address of timer settings registers (pg. 650 [30.7] SAMD21 E/G/J datasheet complete).
                std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                IRQn timer_irq_id,              // Interrupt Request ID of timer in NVIC (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete).
                bool interrupt_on_overflow,     // Interrupt every period
                bool interrupt_on_match,        // Interrupt upon matching specified value
                std::uint8_t match_value        // Value to interrupt on
                );
        ~Timer8(){}
/*************************************************************************
                Timer Constructors and destructors end
*************************************************************************/

    private:
        Timer8(const Timer8&){} // Force the class to be non-copyable
        Timer8& operator=(const Timer8&){}

        TcCount8* p_timer_settings;
        std::uint8_t m_gen_clk_id;
        std::uint8_t m_gen_out_id;
        IRQn m_timer_irq_id;
};

#endif