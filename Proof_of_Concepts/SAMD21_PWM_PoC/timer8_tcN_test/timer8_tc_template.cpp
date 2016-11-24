#include "timer8_tc3.hpp"

/*************************************************************************
                            Singleton access start
*************************************************************************/

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
TimerCount<timer_address, gclk_out_id, NVIC_id>& TimerCount<timer_address, gclk_out_id, NVIC_id>::singleton(){
    static TimerCount<timer_address, gclk_out_id, NVIC_id> no_so_hidden_singleton;
    return no_so_hidden_singleton;
}

/*************************************************************************
                            Singleton access end
*************************************************************************/

/*************************************************************************
                            Read-only start
*************************************************************************/

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
std::uint32_t TimerCount<timer_address, gclk_out_id, NVIC_id>::prescaler()const{
    switch((m_general_timer.raw_TC_pointer()->CTRLA.reg >> 8) & 0x7){
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

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
std::uint8_t TimerCount<timer_address, gclk_out_id, NVIC_id>::counter_period()const{
    return m_general_timer.raw_TC_pointer()->PER.reg;
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
std::uint8_t TimerCount<timer_address, gclk_out_id, NVIC_id>::counter_match_value()const{
    return m_general_timer.raw_TC_pointer()->CC[0].reg;
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
float TimerCount<timer_address, gclk_out_id, NVIC_id>::reference_frequency()const{
    return k_gen0_clk_freq;
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
float TimerCount<timer_address, gclk_out_id, NVIC_id>::frequency()const{
    return k_gen0_clk_freq / this->prescaler() / this->counter_period();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
float TimerCount<timer_address, gclk_out_id, NVIC_id>::period()const{
    return 1.0f/this->frequency();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
float TimerCount<timer_address, gclk_out_id, NVIC_id>::duty_cycle()const{
    return static_cast<float>(this->counter_match_value()) / this->counter_period();
}

/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                            Modifiers start
*************************************************************************/
template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::init( float generic_clock_frequency,
                        std::uint32_t timer_prescaler,
                        std::uint8_t timer_period,
                        bool interrupt_on_overflow,
                        bool interrupt_on_match,
                        std::uint8_t match_value,
                        callback_func_type isr_over_cb,
                        callback_func_type isr_match_cb 
){
    k_gen0_clk_freq = generic_clock_frequency;
    ISR_overflow_cb = isr_over_cb;
    ISR_match_cb = isr_match_cb;
	m_general_timer.init(   0x0,                        // Generic Clock 0
                            gclk_out_id,                // Tie clock to Timer TC3
                            timer_address,              // Memory address of peripheral settings
                            timer_prescaler, timer_period,
                            NVIC_id,                    // TC3 IRQ ID# in the NVIC
                            interrupt_on_overflow, interrupt_on_match,
                            match_value
                            );
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::enable(){
	m_general_timer.enable();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::disable(){
	m_general_timer.disable();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period){
	m_general_timer.configure_settings(timer_prescaler, timer_period);
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::set_prescale(std::uint32_t new_prescale){
	m_general_timer.set_prescale(new_prescale);
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::set_counter_period(std::uint8_t new_period){
	m_general_timer.set_counter_period(new_period);
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::set_duty_cycle(float new_duty_cycle){
    if(new_duty_cycle < 0.0f || new_duty_cycle >= 1.0f) return;
	m_general_timer.set_match_value(this->counter_period()*new_duty_cycle);
}

/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::configure_interrupt(  bool interrupt_on_overflow,
                                        bool interrupt_on_match,
                                        std::uint8_t match_value
){
	m_general_timer.configure_interrupt(interrupt_on_overflow, interrupt_on_match, match_value);
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
bool TimerCount<timer_address, gclk_out_id, NVIC_id>::overflowed() const{
	return m_general_timer.overflowed();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
bool TimerCount<timer_address, gclk_out_id, NVIC_id>::matched() const{
	return m_general_timer.matched();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::clear_overflow_interrupt(){
	m_general_timer.clear_overflow_interrupt();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::clear_match_interrupt(){
	m_general_timer.clear_match_interrupt();
}


    // Recommend disabling timer before configuring interrupts
template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::enable_overflow_interrupt(){
	m_general_timer.enable_overflow_interrupt();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::disable_overflow_interrupt(){
	m_general_timer.disable_overflow_interrupt();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::enable_match_interrupt(){
	m_general_timer.enable_match_interrupt();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::disable_match_interrupt(){
	m_general_timer.disable_match_interrupt();
}

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
void TimerCount<timer_address, gclk_out_id, NVIC_id>::set_match_value(std::uint8_t new_match_value){
	m_general_timer.set_match_value(new_match_value);
}

/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/

template <TcCount8* timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
TimerCount<timer_address, gclk_out_id, NVIC_id>::TimerCount<timer_address, gclk_out_id, NVIC_id>()
    : m_general_timer()
    , k_gen0_clk_freq(-1.0)
    , ISR_overflow_cb(0)  // NULL
    , ISR_match_cb(0)     // NULL
{}

/*************************************************************************
                        Timer Constructors end
*************************************************************************/