/*************************************************************************
    File appended with .h on purpose since Arduino IDE fails to
    see this file when included.
*************************************************************************/

#define TIMER_COUNT_TEMP template <std::uint32_t timer_address, std::uint8_t gclk_out_id, IRQn NVIC_id>
#define TIMER_COUNT_INST TimerCount<timer_address, gclk_out_id, NVIC_id>


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
std::uint32_t TIMER_COUNT_INST::prescaler()const{
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

TIMER_COUNT_TEMP
std::uint8_t TIMER_COUNT_INST::counter_period()const{
    return m_general_timer.raw_TC_pointer()->PER.reg;
}

TIMER_COUNT_TEMP
std::uint8_t TIMER_COUNT_INST::counter_match_value()const{
    return m_general_timer.raw_TC_pointer()->CC[0].reg;
}

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
                        callback_func_type isr_over_cb, callback_func_type isr_match_cb,
                        std::uint8_t generator, float gen_clk_freq
){
    k_gen0_clk_freq = gen_clk_freq;
    ISR_overflow_cb = isr_over_cb;
    ISR_match_cb = isr_match_cb;
	m_general_timer.init(   generator, gclk_out_id,     // Tie clock to Timer TC3
                            (TcCount8*)(timer_address), // Memory address of peripheral settings
                            timer_prescaler, timer_period,
                            NVIC_id,                    // TC3 IRQ ID# in the NVIC
                            interrupt_on_overflow, interrupt_on_match,
                            match_value
                            );
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::enable(){
	m_general_timer.enable();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::disable(){
	m_general_timer.disable();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::configure_settings(std::uint32_t timer_prescaler, std::uint8_t timer_period){
	m_general_timer.configure_settings(timer_prescaler, timer_period);
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::set_prescale(std::uint32_t new_prescale){
	m_general_timer.set_prescale(new_prescale);
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::set_counter_period(std::uint8_t new_period){
	m_general_timer.set_counter_period(new_period);
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::set_duty_cycle(float new_duty_cycle){
    if(new_duty_cycle < 0.0f || new_duty_cycle >= 1.0f) return;
	m_general_timer.set_match_value(this->counter_period()*new_duty_cycle);
}

/*************************************************************************
                            Modifiers end
*************************************************************************/

/*************************************************************************
                    Timer interrupt functions start
*************************************************************************/
TIMER_COUNT_TEMP
void TIMER_COUNT_INST::configure_interrupt( bool interrupt_on_overflow,
                                            bool interrupt_on_match,
                                            std::uint8_t match_value
){
	m_general_timer.configure_interrupt(interrupt_on_overflow, interrupt_on_match, match_value);
}

TIMER_COUNT_TEMP
bool TIMER_COUNT_INST::overflowed() const{
	return m_general_timer.overflowed();
}

TIMER_COUNT_TEMP
bool TIMER_COUNT_INST::matched() const{
	return m_general_timer.matched();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::clear_overflow_interrupt(){
	m_general_timer.clear_overflow_interrupt();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::clear_match_interrupt(){
	m_general_timer.clear_match_interrupt();
}


    // Recommend disabling timer before configuring interrupts
TIMER_COUNT_TEMP
void TIMER_COUNT_INST::enable_overflow_interrupt(){
	m_general_timer.enable_overflow_interrupt();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::disable_overflow_interrupt(){
	m_general_timer.disable_overflow_interrupt();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::enable_match_interrupt(){
	m_general_timer.enable_match_interrupt();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::disable_match_interrupt(){
	m_general_timer.disable_match_interrupt();
}

TIMER_COUNT_TEMP
void TIMER_COUNT_INST::set_match_value(std::uint8_t new_match_value){
	m_general_timer.set_match_value(new_match_value);
}

/*************************************************************************
                    Timer interrupt functions end
*************************************************************************/

/*************************************************************************
                        Timer Constructors start
    - Protect to enforce singleton pattern
*************************************************************************/

TIMER_COUNT_TEMP
TIMER_COUNT_INST::TimerCount()
    : ISR_overflow_cb(0)  // NULL
    , ISR_match_cb(0)     // NULL
    , m_general_timer()
    , k_gen0_clk_freq(-1.0)
{}

/*************************************************************************
                        Timer Constructors end
*************************************************************************/


#undef TIMER_COUNT_TEMP
#undef TIMER_COUNT_INST