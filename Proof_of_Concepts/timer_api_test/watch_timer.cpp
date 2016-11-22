#include "watch_timer.h"

#include "sam.h"

void Timer8::enable_watch_clock(){
    GCLK->CLKCTRL.reg = (std::uint16_t)(
        GCLK_CLKCTRL_CLKEN
        | (m_gen_clk_id << 8)
        | (m_gen_out_id << 0)
        );
    while ( GCLK->STATUS.bit.SYNCBUSY == 1 );
}

void Timer8::enable_watch_timer(){
    // Enable watch_timer
    p_timer_settings->CTRLA.reg |= TC_CTRLA_ENABLE;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

void Timer8::disable_watch_timer(){
    p_timer_settings->CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

void Timer8::configure_watch_interrupt(bool interrupt_on_overflow, bool interrupt_on_match){
    // Interrupts 
    p_timer_settings->INTENSET.reg = 0;      // disable all interrupts
    p_timer_settings->INTENSET.bit.OVF = interrupt_on_overflow;  // enable overflow interrupt
    p_timer_settings->INTENSET.bit.MC0 = interrupt_on_match;  // enable compare match to CC0 interrupt

    // Enable InterruptVector
    NVIC_EnableIRQ(m_timer_irq_id);
}

void Timer8::configure_watch_timer(std::uint16_t prescaler, std::uint8_t period, std::uint8_t match_value){

    disable_watch_timer();

    p_timer_settings->CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
    p_timer_settings->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);

    p_timer_settings->CTRLA.reg |= prescaler;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);

    p_timer_settings->PER.reg = period;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);

    p_timer_settings->CC[0].reg = match_value;
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

bool Timer8::watch_interrupted_overflowed(){
    return p_timer_settings->INTFLAG.bit.OVF == 1;
}

bool Timer8::watch_interrupted_match(){
    return p_timer_settings->INTFLAG.bit.MC0 == 1;
}

void Timer8::clear_watch_interrupt_overflowed(){
    p_timer_settings->INTFLAG.bit.OVF = 1;
}

void Timer8::clear_watch_interrupt_match(){
    p_timer_settings->INTFLAG.bit.MC0 = 1;
}

void Timer8::set_match_value(std::uint8_t new_match_value){
    p_timer_settings->CC[0].reg = new_match_value;
}
