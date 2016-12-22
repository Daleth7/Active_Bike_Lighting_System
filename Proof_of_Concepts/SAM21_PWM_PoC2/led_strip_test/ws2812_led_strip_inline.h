#ifndef WS2812_MULTICOLOUR_LED_STRIP_DRIVER_H____
#define WS2812_MULTICOLOUR_LED_STRIP_DRIVER_H____

#define STRIP_TEMP template <std::size_t LED_Count>
#define STRIP_INST ws2812_led_strip<LED_Count>

#include <cstring>

STRIP_TEMP
static constexpr STRIP_INST::pattern_size;

STRIP_TEMP
void STRIP_INST::init(){
    pinMode(m_sig_out_pin, OUTPUT);
        // Configure generator 2 to output a 48 MHz clock
    config_gclk_gen( m_clk_gen,   // Appy changes to generator m_clk_gen
                     1,           // Divide clock by 1 --> f = 48 MHz kHz
                     false,       // Select simple division, i.e. divide by 1
                     dfll48m_clk  // Use Digital Frequency Locked Loop clock (48 MHz)
                     );

    // Sets a timer at 800 kHz
    
    p_timer->init( 0x1,       // prescale by 1 --> 48 Mhz Hz
                   60,        // 8-bit counter period -->  800 kHz (1.667% precision)
                   true,      // Interrupt on overflow
                   true,      // Interrupt on match
                   0,         // Match value
                   m_clk_gen, // Use generic clock generator 2
                   48e6       // Reference frequency
                   );

    p_timer->add_listener(&debug_listener);
}

STRIP_TEMP
void STRIP_INST::set_pixel( std::size_t  pos,
                            std::uint8_t red,
                            std::uint8_t green,
                            std::uint8_t blue,
                            std::uint8_t white
){
    /******************* *******************************/
}

STRIP_TEMP
STRIP_INST::ws2812_led_strip( std::uint8_t generator, Timer8& timer,
                              std::uint8_t arduino_pin,
                              bool repeat_pattern
)
    : pattern_type( m_colour_commands, m_colour_commands + pattern_size,
                    &timer,
                    arduino_pin,
                    repeat_pattern
                    )
    , m_clk_gen(generator)
{
    std::memset(m_colour_commands, 0x0, sizeof(float)*pattern_size);
}