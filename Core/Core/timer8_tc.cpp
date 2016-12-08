/*************************************************************************

        Contains definitions of the different TC ISRs.

*************************************************************************/

#include "timer8_tc.hpp"

/*************************************************************************
                    Timer Interrupt Service Routine start
*************************************************************************/

void TC3_Handler(){
    static std::uint32_t not_so_secret_timer_counter = 0;
    TimerCount3& tc3_timer = TimerCount3::singleton();

    if(tc3_timer.overflowed()){
        tc3_timer.ISR_overflow_cb(not_so_secret_timer_counter++);
        tc3_timer.clear_overflow_interrupt();
    }
    if(tc3_timer.matched()){
        tc3_timer.ISR_match_cb(not_so_secret_timer_counter);
        tc3_timer.clear_match_interrupt();
    }
}

void TC4_Handler(){
    static std::uint32_t not_so_secret_timer_counter = 0;
    TimerCount4& tc4_timer = TimerCount4::singleton();

    if(tc4_timer.overflowed()){
        tc4_timer.ISR_overflow_cb(not_so_secret_timer_counter++);
        tc4_timer.clear_overflow_interrupt();
    }
    if(tc4_timer.matched()){
        tc4_timer.ISR_match_cb(not_so_secret_timer_counter);
        tc4_timer.clear_match_interrupt();
    }
}

void TC5_Handler(){
    static std::uint32_t not_so_secret_timer_counter = 0;
    TimerCount5& tc5_timer = TimerCount5::singleton();

    if(tc5_timer.overflowed()){
        tc5_timer.ISR_overflow_cb(not_so_secret_timer_counter++);
        tc5_timer.clear_overflow_interrupt();
    }
    if(tc5_timer.matched()){
        tc5_timer.ISR_match_cb(not_so_secret_timer_counter);
        tc5_timer.clear_match_interrupt();
    }
}

/*************************************************************************
                    Timer Interrupt Service Routine end
*************************************************************************/