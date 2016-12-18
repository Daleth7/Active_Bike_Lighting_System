#ifndef TIMER_OBSERV_UTIL____23498723498
#define TIMER_OBSERV_UTIL____23498723498

#include <cstdint>

namespace ____TimerDutyPattern_Utils {
    struct Listener {
        virtual void trigger_overflow_cb(std::uint32_t counter) = 0;
        virtual void trigger_match_cb(std::uint32_t counter) = 0;
    };

    class Observer {
        public:
            static constexpr std::uint8_t k_max_listeners = 10;

            using listener_type             = Listener;
            using listener_list_type        = listener_type*[k_max_listeners];
            using listener_iterator         = listener_type**;

            static Observer& singleton();

            bool add_listener(listener_type* new_listener);

            void remove_listener(listener_type* id);

            listener_iterator list_begin();
            listener_iterator list_end();

        protected:
            // Enforce singleton pattern
            Observer();
            Observer(const Observer&) = default;
            Observer& operator=(const Observer&) = default;

        private:
            listener_list_type m_listeners;
    };
}

void observer_overflow_callback(std::uint32_t counter);

void observer_match_callback( std::uint32_t counter);

#endif