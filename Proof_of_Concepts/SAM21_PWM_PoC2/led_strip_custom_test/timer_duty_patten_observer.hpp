#ifndef TIMER_OBSERV_UTIL____23498723498
#define TIMER_OBSERV_UTIL____23498723498

#include <map>
#include <cstdint>

namespace ____TimerDutyPattern_Utils {
    using listener_id_type = std::size_t;

    struct Listener {
        virtual void trigger_overflow_cb(std::uint32_t counter) = 0;
        virtual void trigger_match_cb(std::uint32_t counter) = 0;

        protected:
            listener_id_type m_id;
    };

    class Observer {
        public:
            using listener_type         = Listener;
            // Use map due to id design and for faster insertion and removal
            using listener_list_type    = std::map<listener_id_type, listener_type*>;

            static Observer& singleton(){
                Observer not_so_secret_singleton;
                return not_so_secret_singleton;
            }

            listener_id_type add_listener(listener_type* new_listener){
                static listener_id_type next_id = 0;

                // Search for next available id
                while(m_listeners.count(++next_id) > 0){}

                m_listeners.insert(std::make_pair(next_id, new_listener));

                return next_id;
            }

            void remove_listener(listener_id_type id){
                m_listeners.erase(id);
            }

        protected:
            // Enforce singleton pattern
            Observer() = default;
            Observer(const Observer&) = default;
            Observer& operator=(const Observer&) = default;

        private:
            friend void observer_overflow_callback(std::uint32_t counter);
            friend void observer_match_callback(std::uint32_t counter);

            listener_list_type m_listeners;
    };
}

inline void observer_overflow_callback( std::uint32_t counter){
    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();
    if(obs != 0)    observer = obs;
    if(observer != 0){
        for( auto it = observer.m_listeners.begin();
             it != observer.m_listeners.end();
             ++it
        ){
            it->trigger_overflow_cb(counter);
        }
    }
}

inline void observer_match_callback( std::uint32_t counter){
    ____TimerDutyPattern_Utils::Observer& observer
        = ____TimerDutyPattern_Utils::Observer::singleton();
    if(obs != 0)    observer = obs;
    if(observer != 0){
        for( auto it = observer.m_listeners.begin();
             it != observer.m_listeners.end();
             ++it
        ){
            it->trigger_match_cb(counter);
        }
    }
}

#endif