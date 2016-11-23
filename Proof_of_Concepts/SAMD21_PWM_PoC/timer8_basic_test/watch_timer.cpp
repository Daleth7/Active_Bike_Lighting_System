#include "watch_timer.h"

#include "sam.h"

volatile TcCount8* watch_timer = 0x0;

void enable_watch_clock(void){
    GCLK->CLKCTRL.reg = (word)(
        GCLK_CLKCTRL_CLKEN
        | GCLK_CLKCTRL_GEN_GCLK0
        | GCLK_CLKCTRL_ID_TCC2_TC3
        );
    while ( GCLK->STATUS.bit.SYNCBUSY == 1 );
}

void enable_watch_timer(void){
    // Enable watch_timer
    watch_timer->CTRLA.reg |= TC_CTRLA_ENABLE;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);
}

void disable_watch_timer(void){
    watch_timer->CTRLA.reg &= ~TC_CTRLA_ENABLE;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);
}

void configure_watch_interrupt(void){
    // Interrupts 
    watch_timer->INTENSET.reg = 0;      // disable all interrupts
    watch_timer->INTENSET.bit.OVF = 1;  // enable overflow interrupt
    watch_timer->INTENSET.bit.MC0 = 1;  // enable compare match to CC0 interrupt

    // Enable InterruptVector
    NVIC_EnableIRQ(TC3_IRQn);
}

void configure_watch_timer(word prescaler, word period){
    enable_watch_clock();

    watch_timer = (TcCount8*) TC3;

    disable_watch_timer();

    watch_timer->CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);
    watch_timer->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);

    watch_timer->CTRLA.reg |= prescaler;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);

    watch_timer->PER.reg = period;
    while (watch_timer->STATUS.bit.SYNCBUSY == 1);

    // Interrupts 
    configure_watch_interrupt();
}

bool watch_interrupted_overflowed(void){
    return watch_timer->INTFLAG.bit.OVF == 1;
}

bool watch_interrupted_match(void){
    return watch_timer->INTFLAG.bit.MC0 == 1;
}

void clear_watch_interrupt_overflowed(void){
    watch_timer->INTFLAG.bit.OVF = 1;
}

void clear_watch_interrupt_match(void){
    watch_timer->INTFLAG.bit.MC0 = 1;
}
