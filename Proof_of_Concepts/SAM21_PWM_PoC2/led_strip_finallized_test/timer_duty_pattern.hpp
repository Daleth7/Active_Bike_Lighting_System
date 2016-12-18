/*************************************************************************

                          TimerDutyPattern class (C++11)

        Simplified interface to send a specific duty cycle pattern
    through a GPIO. A timer module is used to time the frequency of
    pattern sending. Specifically, the pattern that this class will
    accept is a series of times in microseconds.
        Note that this class does not handle timer configuration, so
    the user must make sure the timer is configured to the desired
    frequency first.
        Also of importance is that this interface does not store the
    pattern to send. This is to allow the user freedom to choose the
    memory management scheme and to avoid forcing any length restriction
    or dynamic memory usage.
        Instead of storing the pattern itself, this class stores
    three iterators that represent: 1) the beginning of the pattern,
    2) the end of the pattern, 3) the current duty cycle in the pattern
    to transmit. Following the same rules of the C++ standard library,
    acceptable iterator types must conform to the RandomAccessIterator
    rules. A raw pointer (e.g. float*) is one example of a random access
    iterator.

        For simplification, the user may use the function
    make_duty_pattern() to create a TimerDutyPattern object.

        Because this class borrows the ISR of a timer module, avoid
    instantiating multiple TimerDutyPattern objects that use the same
    timer.

    Example usage:

*************************************************************************/

#ifndef TIMER_DUTY_PATTERN_EXCCTTTENSS2222
#define TIMER_DUTY_PATTERN_EXCCTTTENSS2222

#include "timer_observer.hpp"

#include <cstdint>
#include <cstddef> // For std::size_t

template <typename Iterator, typename TimerType>
class TimerDutyPattern : protected Timer_Listener {
    public:
        // Read-only
        std::size_t pattern_position()const;
        float frequency()const;
        float period()const;

        // Enable/Disable
        void enable();   // Set up callbacks
        void pause();
        void disable();

        // Modifiers
        void seek_position(std::size_t new_pos);
        void seek_position(std::size_t ref_pos, std::size_t offset);
        void reset(); // Set position back to the beginning
        void new_pattern(Iterator new_begin, Iterator new_end);
        void repeat(bool cycle_pattern);

        // Constructors and destructor
        TimerDutyPattern(   Iterator begin, Iterator end,
                            TimerType* timer_ptr,
                            std::uint8_t arduino_pin,
                            bool repeat_pattern = false
                            );
        TimerDutyPattern(const TimerDutyPattern&)               = default;
        TimerDutyPattern& operator=(const TimerDutyPattern&)    = default;
        TimerDutyPattern(TimerDutyPattern&&)                    = default;
        TimerDutyPattern& operator=(TimerDutyPattern&&)         = default;
        ~TimerDutyPattern();

    protected:
        // Callback functions for the timer ISR
        virtual void trigger_overflow_cb(std::uint32_t counter) override final;
        virtual void trigger_match_cb(std::uint32_t counter) override final;

    private:
        Iterator        m_beg, m_curr, m_end;
        TimerType*      p_timer;
        std::uint8_t    m_sig_out_pin;
        bool            m_cycle;
        bool            m_transmit;
};

template <typename Iterator, typename TimerType>
TimerDutyPattern<Iterator, TimerType>
    make_duty_pattern( Iterator begin, Iterator end,
                       TimerType* timer_ptr,
                       std::uint8_t arduino_pin,
                       bool repeat_pattern = false
                       );

#include "timer_duty_pattern_inline.h"

#endif