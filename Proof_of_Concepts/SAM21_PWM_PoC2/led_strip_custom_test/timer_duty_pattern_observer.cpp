#include "timer_duty_pattern_observer.hpp"

namespace ____TimerDutyPattern_Utils {
    Observer& Observer::singleton(){
        static Observer not_so_secret_singleton;
        return not_so_secret_singleton;
    }

    bool Observer::add_listener(listener_type* new_listener){
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

    void Observer::remove_listener(listener_type* id){
        for(std::uint8_t i = 0; i < k_max_listeners; ++i){
            if(m_listeners[i] == id){
                m_listeners[i] = 0x0;
                return;
            }
        }
    }

    Observer::listener_iterator Observer::list_begin(){
        return m_listeners;
    }

    Observer::listener_iterator Observer::list_end(){
        return m_listeners + k_max_listeners;
    }

    Observer::Observer(){
        for(std::uint8_t i = 0; i < k_max_listeners; ++i){
            m_listeners[i] = 0;
        }
    }
}

void observer_overflow_callback(std::uint32_t counter){
    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();

    for( auto it = observer.list_begin();
         it != observer.list_end();
         ++it
    ){
        if(*it != 0 && it->overflowed())
            (*it)->trigger_overflow_cb(counter);
    }
}

void observer_match_callback( std::uint32_t counter){
    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();

    for( auto it = observer.list_begin();
         it != observer.list_end();
         ++it
    ){
        if(*it != 0 && it->matched())
            (*it)->trigger_match_cb(counter);
    }
}