#include "timer_observer.hpp"

C_Listener make_timer_listener( C_Listener::callback_func_type overflow_func,
                                C_Listener::callback_func_type match_func
){
    return C_Listener   {
                            .overflow_cb(overflow_func),
                            .match_cb(match_func)
                        }
}

std::uint8_t Timer_Observer::count_listeners()const{
    std::uint8_t toreturn = 0;

    for(std::uint8_t i = 0; i < k_max_listeners; ++i){
        toreturn += (m_listeners[i] != 0);
    }

    return toreturn;
}

bool Timer_Observer::add_listener(listener_type* new_listener){
    // Check if the listener is already in the list
    for(std::uint8_t i = 0; i < k_max_listeners; ++i){
        if(m_listeners[i] == new_listener) return false;
    }

    // Find available slot
    for(std::uint8_t i = 0; i < k_max_listeners; ++i){
        if(m_listeners[i] == 0){
            m_listeners[i] = new_listener;
            return true;
        }
    }
    return false;
}

void Timer_Observer::remove_listener(listener_type* id){
    for(std::uint8_t i = 0; i < k_max_listeners; ++i){
        if(m_listeners[i] == id){
            m_listeners[i] = 0x0;
            return;
        }
    }
}

void Timer_Observer::remove_all_listeners(){
    for(std::uint8_t i = 0; i < k_max_listeners; ++i){
        m_listeners[i] = 0;
    }
}

Timer_Observer::listener_iterator Timer_Observer::list_begin(){
    return m_listeners;
}

Timer_Observer::listener_iterator Timer_Observer::list_end(){
    return m_listeners + k_max_listeners;
}

void Timer_Observer::ISR_overflow_cb(std::uint32_t counter){
    for( auto it = this->list_begin();
         it != this->list_end();
         ++it
    ){
        if(*it != 0) (*it)->trigger_overflow_cb(counter);
    }
}

void Timer_Observer::ISR_match_cb(std::uint32_t counter){
    for( auto it = this->list_begin();
         it != this->list_end();
         ++it
    ){
        if(*it != 0) (*it)->trigger_match_cb(counter);
    }
}

Timer_Observer::Timer_Observer(){
    this->remove_all_listeners();
}