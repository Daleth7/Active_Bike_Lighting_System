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

#include "byte_types.h"

/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
void enable_watch_clock(void);
void enable_watch_timer(void);
void disable_watch_timer(void);
   // Call enable_watch_timer() after a call to configure_watch_timer().
void configure_watch_timer(word prescaler, word period);
/*************************************************************************
                    Timer configuration functions end
*************************************************************************/



/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
void configure_watch_interrupt(void);
bool watch_interrupted_overflowed(void);
bool watch_interrupted_match(void);
void clear_watch_interrupt_overflowed(void);
void clear_watch_interrupt_match(void);
/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

#endif