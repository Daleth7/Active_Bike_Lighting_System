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
    generators to generate 48 MHz.

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

#include "timer_observer.hpp"

#include <cstdint>      // Contains known bit length types
#include "sam.h"    // Contains register memory mapping for SAMD21
                    // Should include samd21g18a.h

class Timer8 : public Timer_Observer {
    public:
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

        /* Inherited from Timer_Observer
        std::uint8_t count_listeners()const;
        */
/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
        virtual void init( std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                           std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                           bool interrupt_on_overflow,     // Interrupt every period
                           bool interrupt_on_match,        // Interrupt upon matching specified value
                           std::uint8_t match_value,       // Value to interrupt on
                           std::uint8_t generator = 0x0,   // Select which generic clock generator to use. By default, generator 0 is already set up to generate 48 MHz.
                           float gen_clk_freq = 48e6       // Reference frequency to calculate current timer frequency
                           ) = 0;

        void configure_generic_clock();
        void enable();
        void disable();
           // Call enable() after a call to configure_settings().
        void configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period);
        void set_prescale(std::uint32_t new_prescale);
        void set_counter_period(std::uint8_t new_period);
        void set_duty_cycle(float new_duty_cycle);  // Must be between 0 and 1
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

        /* Inherited from Timer_Observer
        bool add_listener(listener_type* new_listener);

        void remove_listener(listener_type* id);
        void remove_all_listeners();

        listener_iterator list_begin();
        listener_iterator list_end();

        void ISR_overflow_cb(std::uint32_t counter);
        void ISR_match_cb(std::uint32_t counter);
        */
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/
            // After construction, configure these in order:
            //  1) generic clock
            //  2) timer settings
            //  3) interrupt settings
        Timer8();
        ~Timer8(){}
/*************************************************************************
                Timer Constructors and destructors end
*************************************************************************/

    protected:
        Timer8(const Timer8&) = default; // Force the class to be non-copyable
        Timer8& operator=(const Timer8&) = default;

        // Init function for child classes to load their own default settings
        void child_init(    std::uint8_t generic_clk_id,    // Generic Clock to use for timer (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                            std::uint8_t gen_out_id,        // Timer to recieve generic clock (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                            TcCount8* timer_peripheral,     // Address of timer settings registers (pg. 650 [30.7] SAMD21 E/G/J datasheet complete).
                            std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                            std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                            std::uint8_t timer_irq_id,      // Interrupt Request ID of timer in NVIC (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete).
                            bool interrupt_on_overflow,     // Interrupt every period
                            bool interrupt_on_match,        // Interrupt upon matching specified value
                            std::uint8_t match_value = 0,   // Value to interrupt on
                            float new_ref_freq = 48e6       // Reference frequency to calculate current timer frequency
                            );

    private:
        TcCount8*       p_timer_settings;
        std::uint8_t    m_gen_clk_id;
        std::uint8_t    m_gen_out_id;
        std::uint8_t    m_timer_irq_id;
        float k_gen0_clk_freq;
};

#endif