/*************************************************************************

        Contains definitions of the different TC ISRs.

*************************************************************************/

#include "timer8_tc.hpp"

/*************************************************************************
                    Timer Interrupt Service Routine start
*************************************************************************/

void default_timer8_handler_behaviour(Timer8& timer);

void TC3_Handler(){
    default_timer8_handler_behaviour(TimerCount3::singleton());
}

void TC4_Handler(){
    default_timer8_handler_behaviour(TimerCount4::singleton());
}

void TC5_Handler(){
    default_timer8_handler_behaviour(TimerCount5::singleton());
}

static void default_timer8_handler_behaviour(Timer8& timer){
    static std::uint32_t not_so_secret_timer_counter = 0;

    if(timer.overflowed()){
        timer.ISR_overflow_cb(not_so_secret_timer_counter++);
        timer.clear_overflow_interrupt();
    }
    if(timer.matched()){
        timer.ISR_match_cb(not_so_secret_timer_counter);
        timer.clear_match_interrupt();
    }
}

/*************************************************************************
                    Timer Interrupt Service Routine end
*************************************************************************/