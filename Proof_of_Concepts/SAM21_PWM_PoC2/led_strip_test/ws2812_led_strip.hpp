#ifndef WS2812_MULTICOLOUR_LED_STRIP_DRIVER_H____
#define WS2812_MULTICOLOUR_LED_STRIP_DRIVER_H____

#include "timer8.hpp"
#include "timer_duty_pattern.hpp"

#include <cstdint>
#include <cstddef> // For std::size_t

template <std::size_t LED_Count>
class ws2812_led_strip : public TimerDutyPattern<float*> {
    public:
        using pattern_type = TimerDutyPattern<float*>;
        static constexpr std::size_t pattern_size = LED_Count*4u*8u;

/* Inherited from TimerDutyPattern<Iterator>
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
*/
        void init();
        void set_pixel( std::size_t  pos,
                        std::uint8_t red,
                        std::uint8_t green,
                        std::uint8_t blue,
                        std::uint8_t white
                        );

        ws2812_led_strip( std::uint8_t generator, Timer8& timer,
                          std::uint8_t arduino_pin,
                          bool repeat_pattern = false
                          );
        ~ws2812_led_strip() = default;

    protected:
        // Make noncopyable
        ws2812_led_strip(const ws2812_led_strip&)            = default;
        ws2812_led_strip& operator=(const ws2812_led_strip&) = default;
        ws2812_led_strip(ws2812_led_strip&&)                 = default;
        ws2812_led_strip& operator=(ws2812_led_strip&&)      = default;

/* Inherited from TimerDutyPattern<Iterator>
        // Callback functions for the timer ISR
        virtual void trigger_overflow_cb(std::uint32_t counter) override final;
        virtual void trigger_match_cb(std::uint32_t counter) override final;

        Timer8* p_timer;
        std::uint8_t m_sig_out_pin;
*/
        static float bit_to_ontime(std::uint8_t bit);

        std::uint8_t     m_clk_gen;
        float            m_colour_commands[pattern_size];
};

#include "ws2812_led_strip_inline.h"

#endif