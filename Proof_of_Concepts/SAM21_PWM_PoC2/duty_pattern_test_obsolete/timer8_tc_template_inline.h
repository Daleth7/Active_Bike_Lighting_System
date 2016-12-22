/*************************************************************************
    File appended with .h on purpose since Arduino IDE fails to
    see this file when included.
*************************************************************************/

#define TIMER_COUNT_TEMP template < std::uint32_t t_timer_address,  \
                                    std::uint8_t t_gclk_out_id,     \
                                    std::uint8_t t_NVIC_id          \
                                    >
#define TIMER_COUNT_INST TimerCount<t_timer_address, t_gclk_out_id, t_NVIC_id>


/*************************************************************************
                            Singleton access start
*************************************************************************/

TIMER_COUNT_TEMP
TIMER_COUNT_INST& TIMER_COUNT_INST::singleton(){
    static TIMER_COUNT_INST no_so_hidden_singleton;
    return no_so_hidden_singleton;
}

/*************************************************************************
                            Singleton access end
*************************************************************************/

/*************************************************************************
                            Read-only start
*************************************************************************/

TIMER_COUNT_TEMP
float TIMER_COUNT_INST::reference_frequency()const{
    return k_gen0_clk_freq;
}

TIMER_COUNT_TEMP
float TIMER_COUNT_INST::frequency()const{
    return k_gen0_clk_freq / this->prescaler() / this->counter_period();
}

TIMER_COUNT_TEMP
float TIMER_COUNT_INST::period()const{
    return 1.0f/this->frequency();
}

TIMER_COUNT_TEMP
float TIMER_COUNT_INST::duty_cycle()const{
    return static_cast<float>(this->counter_match_value()) / this->counter_period();
}

/*************************************************************************
                            Read-only end
*************************************************************************/

/*************************************************************************
                            Modifiers start
*************************************************************************/
TIMER_COUNT_TEMP
void TIMER_COUNT_INST::init(
                        std::uint32_t timer_prescaler, std::uint8_t timer_period,
                        bool interrupt_on_overflow, bool interrupt_on_match,
                        std::uint8_t match_value,
                        std::uint8_t generator, float gen_clk_freq,
                        callback_func_type isr_over_cb, callback_func_type isr_match_cb
){
    k_gen0_clk_freq = gen_clk_freq;
	this->child_init(   generator, t_gclk_out_id,     // Tie clock to Timer TC3
                        (TcCount8*)(t_timer_address), // Memory address of peripheral settings
                        timer_prescaler, timer_period,
                        t_NVIC_id,                    // TC3 IRQ ID# in the NVIC
                        interrupt_on_overflow, interrupt_on_match,
                        match_value,
                        isr_over_cb, isr_match_cb
                        );
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::set_duty_cycle(float new_duty_cycle){
    if(new_duty_cycle < 0.0f || new_duty_cycle >= 1.0f) return;
	this->set_match_value(this->counter_period()*new_duty_cycle);
}

/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/

TIMER_COUNT_TEMP
TIMER_COUNT_INST::TimerCount()
    : Timer8()
    , k_gen0_clk_freq(-1.0)
{}

/*************************************************************************
                        Timer Constructors end
*************************************************************************/


#undef TIMER_COUNT_TEMP
#undef TIMER_COUNT_INST