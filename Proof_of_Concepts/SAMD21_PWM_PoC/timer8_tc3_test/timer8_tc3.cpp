#include "timer8_tc3.hpp"

/*************************************************************************
                            Singleton access start
*************************************************************************/

TimerCount3& TimerCount3::singleton(){
    static TimerCount3 no_so_hidden_singleton;
    return no_so_hidden_singleton;
}

/*************************************************************************
                            Singleton access end
*************************************************************************/

/*************************************************************************
                            Read-only start
*************************************************************************/

std::uint32_t TimerCount3::prescaler()const{
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

std::uint8_t TimerCount3::counter_period()const{
    return m_general_timer.raw_TC_pointer()->PER.reg;
}

std::uint8_t TimerCount3::counter_match_value()const{
    return m_general_timer.raw_TC_pointer()->CC[0].reg;
}

float TimerCount3::reference_frequency()const{
    return k_gen0_clk_freq;
}

float TimerCount3::frequency()const{
    return k_gen0_clk_freq / this->prescaler() / this->counter_period();
}

float TimerCount3::period()const{
    return 1.0f/this->frequency();
}

float TimerCount3::duty_cycle()const{
    return static_cast<float>(this->counter_match_value()) / this->counter_period();
}

/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                            Modifiers start
*************************************************************************/
void TimerCount3::init( float generic_clock_frequency,
                        std::uint32_t timer_prescaler,
                        std::uint8_t timer_period,
                        bool interrupt_on_overflow,
                        bool interrupt_on_match,
                        std::uint8_t match_value,
                        callback_func_type isr_over_cb,
                        callback_func_type isr_match_cb 
){
    k_gen0_clk_freq = generic_clock_frequency;
    m_ISR_overflow_cb = isr_over_cb;
    m_ISR_match_cb = isr_match_cb;
	m_general_timer.init(   0x0,                        // Generic Clock 0
                            0x1B,                       // Tie clock to Timer TC3
                            (TcCount8*)TC3,             // Memory address of peripheral settings
                            timer_prescaler, timer_period,
                            TC3_IRQn,                   // TC3 IRQ ID# in the NVIC
                            interrupt_on_overflow, interrupt_on_match,
                            match_value
                            );
}

void TimerCount3::enable(){
	m_general_timer.enable();
}

void TimerCount3::disable(){
	m_general_timer.disable();
}

void TimerCount3::configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period){
	m_general_timer.configure_settings(timer_prescaler, timer_period);
}

void TimerCount3::set_prescale(std::uint32_t new_prescale){
	m_general_timer.set_prescale(new_prescale);
}

void TimerCount3::set_counter_period(std::uint8_t new_period){
	m_general_timer.set_counter_period(new_period);
}

/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
void TimerCount3::configure_interrupt(  bool interrupt_on_overflow,
                                        bool interrupt_on_match,
                                        std::uint8_t match_value
){
	m_general_timer.configure_interrupt(interrupt_on_overflow, interrupt_on_match, match_value);
}

bool TimerCount3::overflowed() const{
	return m_general_timer.overflowed();
}

bool TimerCount3::matched() const{
	return m_general_timer.matched();
}

void TimerCount3::clear_overflow_interrupt(){
	m_general_timer.clear_overflow_interrupt();
}

void TimerCount3::clear_match_interrupt(){
	m_general_timer.clear_match_interrupt();
}


    // Recommend disabling timer before configuring interrupts
void TimerCount3::enable_overflow_interrupt(){
	m_general_timer.enable_overflow_interrupt();
}

void TimerCount3::disable_overflow_interrupt(){
	m_general_timer.disable_overflow_interrupt();
}

void TimerCount3::enable_match_interrupt(){
	m_general_timer.enable_match_interrupt();
}

void TimerCount3::disable_match_interrupt(){
	m_general_timer.disable_match_interrupt();
}

void TimerCount3::set_match_value(std::uint8_t new_match_value){
	m_general_timer.set_match_value(new_match_value);
}

/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/

TimerCount3::TimerCount3()
    : m_general_timer()
    , k_gen0_clk_freq(-1.0)
    , m_ISR_overflow_cb(0)  // NULL
    , m_ISR_match_cb(0)     // NULL
{}

/*************************************************************************
                        Timer Constructors end
*************************************************************************/

/*************************************************************************
                    Timer Interrupt Service Routine start
*************************************************************************/

void TC3_Handler(){
    static std::uint32_t not_so_secret_timer_counter = 0;
    TimerCount3& tc3_timer = TimerCount3::singleton();

    if(tc3_timer.overflowed()){
        tc3_timer.m_ISR_overflow_cb(not_so_secret_timer_counter++);
        tc3_timer.clear_overflow_interrupt();
    }
    if(tc3_timer.matched()){
        tc3_timer.m_ISR_match_cb(not_so_secret_timer_counter);
        tc3_timer.clear_match_interrupt();
    }
}

/*************************************************************************
                    Timer Interrupt Service Routine end
*************************************************************************/