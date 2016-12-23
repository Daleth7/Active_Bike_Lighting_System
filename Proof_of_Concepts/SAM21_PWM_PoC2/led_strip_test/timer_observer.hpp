#ifndef TIMER_OBSERV_UTIL____23498723498
#define TIMER_OBSERV_UTIL____23498723498

#include <cstdint>

struct Timer_Listener {
    virtual void trigger_overflow_cb(std::uint32_t counter) = 0;
    virtual void trigger_match_cb(std::uint32_t counter) = 0;
};

void ____default_listener_callback_function(std::uint32_t);

struct C_Listener : public Timer_Listener {
    typedef void (*callback_func_type)(std::uint32_t counter);

    void trigger_overflow_cb(std::uint32_t counter){
        this->overflow_cb(counter);
    }

    void trigger_match_cb(std::uint32_t counter){
        this->match_cb(counter);
    }

    callback_func_type overflow_cb;
    callback_func_type match_cb;
};

C_Listener make_timer_listener( C_Listener::callback_func_type overflow_func,
                                C_Listener::callback_func_type match_func = 
                                    ____default_listener_callback_function
                                );

class Timer_Observer {
    public:
        static constexpr std::uint8_t k_max_listeners = 10;

        using listener_type             = Timer_Listener;
        using listener_list_type        = listener_type*[k_max_listeners];
        using listener_iterator         = listener_type**;

        std::uint8_t count_listeners()const;

        bool add_listener(listener_type* new_listener);

        void remove_listener(listener_type* id);
        void remove_all_listeners();

        listener_iterator list_begin();
        listener_iterator list_end();

        void ISR_overflow_cb(std::uint32_t counter);
        void ISR_match_cb(std::uint32_t counter);

    protected:
        // Enforce singleton pattern
        Timer_Observer();
        Timer_Observer(const Timer_Observer&) = default;
        Timer_Observer& operator=(const Timer_Observer&) = default;

    private:
        listener_list_type m_listeners;
};

#endif