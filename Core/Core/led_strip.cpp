#include "led_strip.hpp"

#include "pin_defines.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



#define GREENLEDSTRIP 1
#define YELLOWLEDSTRIP 2
#define REDLEDSTRIP 3


const std::uint8_t colour_brightness = 63;
const std::uint8_t strip_led_count = 30;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(strip_led_count, LEDPIN, NEO_GRBW + NEO_KHZ800); // call LED Strip library

void initialize_strip(){
	strip.begin(); //
	strip.show(); // Initialize all pixels to 'off'
}

void cycle_strip_red() { // Pulsing red, 3 LED's are on at a time
    cycle_strip_leds(colour_brightness, 0, 0, 0);
}

void cycle_strip_yellow() { // Pulsing yellow, 3 LED's are on at a time
    cycle_strip_leds(colour_brightness, colour_brightness, 0, 0);
}

void cycle_strip_green() { // Pulsing Green, 3 LED's are on at a time
    cycle_strip_leds(0, colour_brightness, 0, 0);
}

void cycle_strip_cyan() { // Pulsing cyan, 3 LED's are on at a time
    cycle_strip_leds(0, colour_brightness, colour_brightness, 0);
}

void cycle_strip_blue() { // Pulsing blue, 3 LED's are on at a time
    cycle_strip_leds(0, 0, colour_brightness, 0);
}

void cycle_strip_magenta() { // Pulsing magenta, 3 LED's are on at a time
    cycle_strip_leds(colour_brightness, 0, colour_brightness, 0);
}

void cycle_strip_white() { // Pulsing white, 3 LED's are on at a time
    cycle_strip_leds(0, 0, 0, colour_brightness);
}

void cycle_strip_leds(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t white){
    static int led_position = 1;

    int led_pos_prev = (led_position-1)%strip_led_count,
        led_pos_curr =  led_position   %strip_led_count,
        led_pos_next = (led_position+1)%strip_led_count
        ;

        // Turn on led at current position and one position on either side
    strip.setPixelColor( led_pos_prev, red, green, blue, white );
    strip.setPixelColor( led_pos_curr, red, green, blue, white );
    strip.setPixelColor( led_pos_next, red, green, blue, white );

    if (led_pos_prev > 0){ // turns off LED two positions back
        strip.setPixelColor( led_pos_prev-1, 0, 0, 0 );
    }

    strip.show();
    led_position++;

    if (led_position > 15000) { // resets LED strip counter
        led_position = 1;
        strip.setPixelColor( strip_led_count, 0, 0, 0 );
    }
}