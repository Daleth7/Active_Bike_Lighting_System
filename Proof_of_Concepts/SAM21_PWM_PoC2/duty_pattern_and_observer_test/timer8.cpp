#include "timer8.hpp"

/*************************************************************************
                            Read-only start
*************************************************************************/

std::uint32_t Timer8::prescaler()const{
    switch((p_timer_settings->CTRLA.reg >> 8) & 0x7){
        case 0x0:   return 1;
        case 0x1:   return 2;
        case 0x2:   return 4;
        case 0x3:   return 8;
        case 0x4:   return 16;
        case 0x5:   return 64;
        case 0x6:   return 256;
        case 0x7:   return 1024;
        default:    return 0x0; // Error
    }
    return 0x0; // Error
}

std::uint8_t Timer8::counter_period()const{
    return p_timer_settings->PER.reg;
}

std::uint8_t Timer8::counter_match_value()const{
    return p_timer_settings->CC[0].reg;
}

float Timer8::reference_frequency()const{
    return k_gen0_clk_freq;
}

float Timer8::frequency()const{
    return k_gen0_clk_freq / this->prescaler() / this->counter_period();
}

float Timer8::period()const{
    return 1.0f/this->frequency();
}

float Timer8::duty_cycle()const{
    return static_cast<float>(this->counter_match_value()) / this->counter_period();
}

/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                    Timer configuration functions start
*************************************************************************/

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
    if(p_timer_settings == 0) return;

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

void Timer8::set_duty_cycle(float new_duty_cycle){
    if(new_duty_cycle < 0.0f || new_duty_cycle >= 1.0f) return;
	this->set_match_value(this->counter_period()*new_duty_cycle);
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
    if(p_timer_settings == 0) return;

    // Interrupts
    p_timer_settings->INTENCLR.reg |= std::uint32_t(0xFFFFFFFF);   // disable all timer interrupts

    if(interrupt_on_overflow)   p_timer_settings->INTENSET.bit.OVF = 1;
    else                        p_timer_settings->INTENCLR.bit.OVF = 1;

    if(interrupt_on_match)      p_timer_settings->INTENSET.bit.MC0 = 1;
    else                        p_timer_settings->INTENCLR.bit.MC0 = 1;

    this->set_match_value(match_value);

    // Enable Interrupt Vector
    NVIC_EnableIRQ((IRQn)m_timer_irq_id);
    //*NVIC->ISER |= 1 << m_timer_irq_id;
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
    , m_timer_irq_id(15)
    , k_gen0_clk_freq(48e6)
{}
/*************************************************************************
                Timer Constructors and destructors start
*************************************************************************/

void Timer8::child_init(    std::uint8_t generic_clk_id,    // Generic Clock to use for timer (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                            std::uint8_t gen_out_id,        // Timer to recieve generic clock (pg. 132 [15.8.3] SAMD21 E/G/J datasheet complete).
                            TcCount8* timer_peripheral,     // Address of timer settings registers (pg. 650 [30.7] SAMD21 E/G/J datasheet complete).
                            std::uint32_t timer_prescaler,  // Desired prescaler of timer frequency (pg. 654 [30.8.1] SAMD21 E/G/J datasheet complete).
                            std::uint8_t timer_period,      // Desired counter period of timer (pg. 672 [30.8.12.1] SAMD21 E/G/J datasheet complete).
                            std::uint8_t timer_irq_id,      // Interrupt Request ID of timer in NVIC (pg. 48 [11.2.2] SAMD21 E/G/J datasheet complete).
                            bool interrupt_on_overflow,     // Interrupt every period
                            bool interrupt_on_match,        // Interrupt upon matching specified value
                            std::uint8_t match_value,       // Value to interrupt on
                            float new_ref_freq              // Reference frequency to calculate current timer frequency
){
    p_timer_settings = timer_peripheral;
    m_gen_clk_id = generic_clk_id;
    m_gen_out_id = gen_out_id;
    m_timer_irq_id = timer_irq_id;
    k_gen0_clk_freq = new_ref_freq;
    this->configure_generic_clock();
    this->configure_settings(timer_prescaler, timer_period);
    this->configure_interrupt(interrupt_on_overflow, interrupt_on_match, match_value);
}