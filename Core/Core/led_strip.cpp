#include "led_strip.hpp"

#include "pin_defines.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, LEDPIN, NEO_RGBW + NEO_KHZ800); // call LED Strip library

void initialize_strip(){
	strip.begin(); //
	strip.show(); // Initialize all pixels to 'off'
}

void displayLEDs() { // LED strip display function
/*
	switch (ledStripMode){
		
		case GREENLEDSTRIP:
*/
			greenLEDs();
/*
			break;
			
		case YELLOWLEDSTRIP:
			//put code here
			break;
			
		case REDLEDSTRIP:
			//put code here
			break;
			
	}
*/
}

void greenLEDs() { // Pulsing Green, 3 LED's are on at a time

	//static int greenCounter = 0;

	//if (greenCounter == 300){
	
		static int led_position = 0;
		static int offCounter = 0; // initializes counter
	
		strip.setPixelColor(led_position, 255, 0, 0, 0);
//		strip.setPixelColor(led_position+1, 255, 0, 0, 0);
		strip.setPixelColor(led_position+2, 255, 0, 0, 0);
	
		if (offCounter == 1){ // turns off LEDs after a cycle of being on
		
			strip.setPixelColor(led_position-1, 0, 0, 0);
			strip.setPixelColor(led_position, 0, 0, 0);
			
			offCounter = -1;
		}	
	
		strip.show();
		//delay (50);
	
		offCounter++;
		led_position++;
		//greenCounter = 0;
		
		if (led_position == 31) { // resets LED strip counter
			led_position = 0;
		}
	//}
	//greenCounter++;
}