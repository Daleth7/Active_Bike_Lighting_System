#include "timer8.hpp"

const TcCount8* Timer8::raw_TC_pointer()const{
    return p_timer_settings;
}

/*************************************************************************
                    Timer configuration functions start
*************************************************************************/
void Timer8::init(  std::uint8_t generic_clk_id,    // Generic Clock to use for timer (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                    std::uint8_t gen_out_id,        // Timer to recieve generic clock (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                    TcCount8* timer_peripheral,     // Address of timer settings registers (pg. 650 [30.7] SAMD21 E/G/J datasheet complete).
                    std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                    std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                    IRQn timer_irq_id,              // Interrupt Request ID of timer in NVIC (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete).
                    bool interrupt_on_overflow,     // Interrupt every period
                    bool interrupt_on_match,        // Interrupt upon matching specified value
                    std::uint8_t match_value        // Value to interrupt on
){
    p_timer_settings = timer_peripheral;
    m_gen_clk_id = generic_clk_id;
    m_gen_out_id = gen_out_id;
    m_timer_irq_id = timer_irq_id;
    this->configure_generic_clock();
    this->configure_settings(timer_prescaler, timer_period);
    this->configure_interrupt(interrupt_on_overflow, interrupt_on_match, match_value);
}

void Timer8::configure_generic_clock(){
    GCLK->CLKCTRL.reg = (std::uint16_t)(
        GCLK_CLKCTRL_CLKEN
        | (m_gen_clk_id << 8)
        | (m_gen_out_id << 0)
        );
        // Wait until write operation to register has been completed
    while ( GCLK->STATUS.bit.SYNCBUSY == 1 );
}

void Timer8::enable(){
    // Enable timer count peripheral
    p_timer_settings->CTRLA.reg |= TC_CTRLA_ENABLE;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

void Timer8::disable(){
    p_timer_settings->CTRLA.reg &= ~TC_CTRLA_ENABLE;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

void Timer8::configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period){
    this->disable();

    p_timer_settings->CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
    p_timer_settings->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);

    this->set_prescale(timer_prescaler);
    this->set_counter_period(timer_period);
}

void Timer8::set_prescale(std::uint32_t new_prescale){
    p_timer_settings->CTRLA.reg |= new_prescale << 8;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

void Timer8::set_counter_period(std::uint8_t new_period){
    p_timer_settings->PER.reg = new_period;
        // Wait until write operation to register has been completed
    while (p_timer_settings->STATUS.bit.SYNCBUSY == 1);
}

/*************************************************************************
                    Timer configuration functions end
*************************************************************************/



/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/

void Timer8::configure_interrupt(   bool interrupt_on_overflow,
                                    bool interrupt_on_match,
                                    std::uint8_t match_value
){
    // Interrupts
    p_timer_settings->INTENSET.reg = 0;      // disable all interrupts

    if(interrupt_on_overflow)   p_timer_settings->INTENSET.bit.OVF = 1;
    else                        p_timer_settings->INTENCLR.bit.OVF = 1;

    if(interrupt_on_match)      p_timer_settings->INTENSET.bit.MC0 = 1;
    else                        p_timer_settings->INTENCLR.bit.MC0 = 1;

    this->set_match_value(match_value);

    // Enable Interrupt Vector
    NVIC_EnableIRQ(m_timer_irq_id);
}

bool Timer8::overflowed()const{
    return p_timer_settings->INTFLAG.bit.OVF == 1;
}

bool Timer8::matched()const{
    return p_timer_settings->INTFLAG.bit.MC0 == 1;
}

void Timer8::clear_overflow_interrupt(){
    p_timer_settings->INTFLAG.bit.OVF = 1;
}

void Timer8::clear_match_interrupt(){
    p_timer_settings->INTFLAG.bit.MC0 = 1;
}

void Timer8::enable_overflow_interrupt(){
    p_timer_settings->INTENSET.bit.OVF = 1;
}

void Timer8::disable_overflow_interrupt(){
    p_timer_settings->INTENSET.bit.OVF = 0;
}

void Timer8::enable_match_interrupt(){
    p_timer_settings->INTENSET.bit.MC0 = 1;
}

void Timer8::disable_match_interrupt(){
    p_timer_settings->INTENSET.bit.MC0 = 0;
}

void Timer8::set_match_value(std::uint8_t new_match_value){
    p_timer_settings->CC[0].reg = new_match_value;
}

/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/


/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/
Timer8::Timer8()
    : p_timer_settings(0)
    , m_gen_clk_id(0), m_gen_out_id(0)
    , m_timer_irq_id(TCC0_IRQn)
{}
/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/
