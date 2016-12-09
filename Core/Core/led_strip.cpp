#include "led_strip.hpp"

#include "pin_defines.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



#define GREENLEDSTRIP 1
#define YELLOWLEDSTRIP 2
#define REDLEDSTRIP 3


const std::uint8_t colour_brightness = 128;
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
    cycle_strip_leds(colour_brightness, colour_brightness/2, 0, 0);
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

void cycle_strip_fire() { // Pulsing fire, 3 LED's are on at a time
    cycle_strip_leds(   128, 0, 0, 0,   // red
                        128, 15, 0, 0,  // orange
                        128, 47, 0, 0   // yellow
                        );
}

void cycle_strip_leds(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t white){
    cycle_strip_leds(   red, green, blue, white,
                        red, green, blue, white,
                        red, green, blue, white
                        );
}

void cycle_strip_leds(  std::uint8_t red1, std::uint8_t green1, std::uint8_t blue1, std::uint8_t white1,
                        std::uint8_t red2, std::uint8_t green2, std::uint8_t blue2, std::uint8_t white2,
                        std::uint8_t red3, std::uint8_t green3, std::uint8_t blue3, std::uint8_t white3
                        ){
    static int led_position = 1;

    int led_pos_prev = (led_position-1),
        led_pos_curr =  led_position   ,
        led_pos_next = (led_position+1)
        ;

        // Turn on led at current position and one position on either side
    strip.setPixelColor( led_pos_prev, red1, green1, blue1, white1 );
    strip.setPixelColor( led_pos_curr, red2, green2, blue2, white2 );
    strip.setPixelColor( led_pos_next, red3, green3, blue3, white3 );

    if (led_pos_prev > 0){ // turns off LED two positions back
        strip.setPixelColor( led_pos_prev-1, 0, 0, 0 );
    } else {
        strip.setPixelColor( strip_led_count-1, 0, 0, 0 );
        strip.setPixelColor( strip_led_count-2, 0, 0, 0 );
    }

    strip.show();
    led_position++;
    led_position %= strip_led_count;
}