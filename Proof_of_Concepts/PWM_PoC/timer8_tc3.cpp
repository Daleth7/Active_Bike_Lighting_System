#include "timer8_tc3.hpp"

/*************************************************************************
                            Singleton access start
*************************************************************************/

TimerCount3& TimerCount3::singleton(
    std::uint32_t prescaler, std::uint8_t period,
    bool interrupt_on_overflow, bool interrupt_on_match,
    std::uint8_t match_value, float gen0_clk_frequency
){
    static TimerCount3 no_so_hidden_singleton(  prescaler, period,
                                                interrupt_on_overflow,
                                                interrupt_on_match,
                                                match_value, gen0_clk_frequency
                                                );
                                                
    return no_so_hidden_singleton;
}

/*************************************************************************
                            Singleton access end
*************************************************************************/

/*************************************************************************
                            Read-only start
*************************************************************************/

std::uint32_t TimerCount3::prescaler()const{
    return m_pscale;
}

std::uint8_t TimerCount3::counter_period()const{
    return m_per;
}

std::uint8_t TimerCount3::counter_match_value()const{
    return m_match;
}

float TimerCount3::reference_frequency()const{
    return gen0_clk_frequency;
}

float TimerCount3::frequency()const{
    return gen0_clk_frequency / m_pscale / m_per;
}

float TimerCount3::period()const{
    return 1.0f/this->frequency();
}

float TimerCount3::duty_cycle()const{
    return static_cast<float>(m_match) / m_per;
}


bool TimerCount3::overflowed()const{
    return overflow_interrupt();
}

bool TimerCount3::matched()const{
    return match_interrupt();
}

/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                            Modifiers start
*************************************************************************/

void TimerCount3::enable(){
    Timer8::enable_timer();
}

void TimerCount3::disable(){
    Timer8::disable_timer();
}

void TimerCount3::prescale(std::uint32_t new_prescale){
    Timer8::configure_settings(new_prescale, m_per);
}

void TimerCount3::counter_period(std::uint8_t new_period){
    Timer8::configure_settings(m_pscale, new_period);
}

void TimerCount3::counter_match_value(std::uint8_t new_counter_match_value){
    /// Not implemented yet
}

void TimerCount3::set_overflow_interrupt(bool enable_interrupt){
    if(enable_interrupt)    Timer8::enable_overflow_interrupt();
    else                    Timer8::disable_overflow_interrupt();
}

void TimerCount3::set_match_interrupt(bool enable_interrupt){
    if(enable_interrupt)    Timer8::enable_match_interrupt();
    else                    Timer8::disable_match_interrupt();
}

/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/

TimerCount3::TimerCount3(   std::uint32_t new_prescaler,
                            std::uint8_t new_period,
                            bool interrupt_on_overflow,
                            bool interrupt_on_match,
                            std::uint8_t match_value,
                            float gen0_clk_frequency
)
    : Timer8(   0x0,    // Generic Clock 0
                0x1B,   // Tie clock to Timer TC3
                TC3,    // Memory address of peripheral settings
                new_prescaler, new_period,
                18,     // TC3 IRQ ID# in the NVIC
                interrupt_on_overflow, interrupt_on_match,
                match_value
                )
    , k_gen0_clk_freq(gen0_clk_frequency)
    , m_pscale(new_prescaler)
    , m_per(new_period)
    , m_match(match_value)
{}

/*************************************************************************
                        Timer Constructors end
*************************************************************************/