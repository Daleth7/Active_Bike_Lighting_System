#ifndef LED_STRIP_DRIVER_HEADER_DDSSSS
#define LED_STRIP_DRIVER_HEADER_DDSSSS

#include <cstdint>

void initialize_strip();

void cycle_strip_red();
void cycle_strip_yellow();
void cycle_strip_green();
void cycle_strip_cyan();
void cycle_strip_blue();
void cycle_strip_magenta();
void cycle_strip_white();
void cycle_strip_leds(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t white);

#endif