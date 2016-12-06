/*************************************************************************

            Fixed period timer (clock) for timing measurements

        Set a known, fixed period timer such that when measuring
    current from the battery pack, the discharge capacity can be
    calculated (Coulomb counting). As of right now, uses Timer 3
    and is fixed at 50% duty cycle.

    Example usage:

        configure_watch_timer(TC_CTRLA_PRESCALER_DIV64, 0xFFFF);
        enable_watch_timer();   // Note that after configuration,
                                //  the timer is left disabled.

        ...

        void TC3_Handler(){
            if(watch_interrupted_overflowed()){
                ... Do something after each complete clock cycle
                clear_watch_interrupt_overflowed();
            }
            if(watch_interrupted_match()){
                ... Do something after reaching the duty cycle period
                clear_watch_interrupt_match();
            }
        }

*************************************************************************/

#ifndef WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH
#define WATCH_TIMER_FOR_KEPPING_32888888_CLOCK_SDOUCUNT____HHHHHHHHH

#include <cstdint>
#include "sam.h"

struct Timer8 {
    TcCount8* p_timer_settings;
    std::uint8_t m_gen_clk_id;
    std::uint8_t m_gen_out_id;
    IRQn m_timer_irq_id;

/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
void enable_watch_clock();
void enable_watch_timer();
void disable_watch_timer();
   // Call enable_watch_timer() after a call to configure_watch_timer().
void configure_watch_timer(std::uint16_t prescaler, std::uint8_t period, std::uint8_t match_value);
void set_match_value(std::uint8_t new_match_value);
/*************************************************************************
                    Timer configuration functions end
*************************************************************************/



/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
void configure_watch_interrupt(bool interrupt_on_overflow, bool interrupt_on_match);
bool watch_interrupted_overflowed();
bool watch_interrupted_match();
void clear_watch_interrupt_overflowed();
void clear_watch_interrupt_match();
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/
};

#endif