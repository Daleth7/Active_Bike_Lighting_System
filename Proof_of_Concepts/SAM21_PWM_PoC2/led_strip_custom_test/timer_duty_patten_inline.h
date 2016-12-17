#define TIMEDUTY_TEMP template <typename Iterator, typename TimerType>
#define TIMEDUTY_INST TimerDutyPattern<Iterator, TimerType>

// Read-only

TIMEDUTY_TEMP
std::size_t TIMEDUTY_INST::pattern_position()const{
    return m_curr - m_beg;
}

// Enable/Disable

TIMEDUTY_TEMP
void TIMEDUTY_INST::enable(){
    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();
    m_id = observer.add_listener(this);

    p_timer->ISR_overflow_cb = observer_overflow_callback;
    p_timer->ISR_match_cb = observer_match_callback;

    pinMode(m_sig_out_pin, OUTPUT);

    m_transmit = true;
}

TIMEDUTY_TEMP
void TIMEDUTY_INST::pause(){
    m_transmit = false;
}

TIMEDUTY_TEMP
void TIMEDUTY_INST::disable()
    this->pause();
    this->reset();

    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();
    observer.remove_listener(m_id);
}

// Modifiers

TIMEDUTY_TEMP
std::size_t TIMEDUTY_INST::seek_position(std::size_t new_pos){
    m_curr = m_beg + new_pos;
    if(m_curr >= m_end){
        m_curr = m_end;
    }
}

TIMEDUTY_TEMP
std::size_t TIMEDUTY_INST::seek_position(std::size_t ref_pos, std::size_t offset){
    m_curr = m_beg + ref_pos + new_pos;
    if(m_curr >= m_end){
        m_curr = m_end;
    }
}

TIMEDUTY_TEMP
void TIMEDUTY_INST::reset(){
    m_curr = m_beg;
}

TIMEDUTY_TEMP
void TIMEDUTY_INST::new_pattern(Iterator new_begin, Iterator new_end){
    m_beg = m_curr = new_begin;
    m_end = new_end;
}

TIMEDUTY_TEMP
void TIMEDUTY_INST::repeat(bool cycle_pattern){
    m_cycle = cycle_pattern;
}

// Constructors and destructor

TIMEDUTY_TEMP
TIMEDUTY_INST::TimerDutyPattern(    Iterator begin, Iterator end,
                                    TimerType* timer_ptr,
                                    std::uint8_t arduino_pin,
                                    bool repeat_pattern
                                    )
    : ____TimerDutyPattern_Utils::Listener()
    , m_beg(begin), m_curr(begin), m_end(end)
    , p_timer(timer_ptr)
    , m_sig_out_pin(arduino_pin)
    , m_cycle(repeat_pattern), m_transmit(false)
{}

TIMEDUTY_TEMP
TIMEDUTY_INST::~TimerDutyPattern(){
    this->disable();
}

// Callback functions for the timer ISR

TIMEDUTY_TEMP
void TIMEDUTY_INST::trigger_overflow_cb() override final{
    digitalWrite(m_sig_out_pin, HIGH);

    if(!m_transmit) return;

    if(m_curr >= m_end){
        if(m_cycle) this->reset();
        else        this->disable();

        return;
    }
    // Set the next duty cycle in the pattern: t us * (1 s/1e6 us) * freq (1/s)
    p_timer->set_duty_cycle((*m_curr / 1.0e6) * p_timer->frequency());
}



TIMEDUTY_TEMP
void TIMEDUTY_INST::trigger_match_cb() override final{
    if(!m_transmit) return;
    digitalWrite(m_sig_out_pin, LOW);
}

TIMEDUTY_TEMP
TIMEDUTY_INST make_duty_pattern(    Iterator begin, Iterator end,
                                    TimerType* timer_ptr,
                                    std::uint8_t arduino_pin,
                                    bool repeat_pattern
){
    return TIMEDUTY_INST(   begin, end,
                            timer_ptr, arduino_pin,
                            repeat_pattern
                            );
}

#undef TIMEDUTY_TEMP
#undef TIMEDUTY_INST