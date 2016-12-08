// list of includes

#include <cstdint>
#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"
#include "interrupt_callbacks.hpp"
#include "led_strip.hpp"
#include "brakes.hpp"

#include "pin_defines.h"

// initialization of all global variables

static TimerCount3& timer = TimerCount3::singleton();
static TimerCount4& pwm = TimerCount4::singleton();
static TimerCount5& striptimer = TimerCount5::singleton();

// list of used functions

void configure_pins();
void configure_timers();

void pwm_overflow_cb(std::uint32_t counter);
void pwm_match_cb(std::uint32_t counter);

void timer_overflow_cb(std::uint32_t);
void timer_match_cb(std::uint32_t);

void striptimer_overflow_cb(std::uint32_t);
void striptimer_match_cb(std::uint32_t);

void setup() { // setup code to run once for initalization



	configure_pins();

	configure_timers();
	configure_interrupts();

	NVIC_SetPriority(TC5_IRQn, 4);	// set TC6 ISR to lowest priority
	NVIC_SetPriority(TC4_IRQn, 3);	// set TC6 ISR to lowest priority
	NVIC_SetPriority(TC3_IRQn, 2);	// set TC6 ISR to lowest priority
	
	// initalize LED strip
	initialize_strip();
}

void loop() // put your main code here, to run repeatedly:
{

	// polling for brake status
	checkBrakeSignals();
	
	//processSignals(); // processing turn signals & brake signals, and displays appropriately
//	displayLEDs(); // function to call for displaying LED's
  
}

void configure_pins(){
	
	// setup output pins for turn signals
	pinMode(OUTERRIGHT, OUTPUT); // RIGHT 1 OUTERMOST
	pinMode(MIDRIGHT, OUTPUT); // RIGHT 2
	pinMode(INNERRIGHT, OUTPUT); // RIGHT 3 (CLOSEST TO MIDDLE)
	pinMode(INNERLEFT, OUTPUT); // LEFT 3 (CLOSEST TO MIDDLE)
	pinMode(MIDLEFT, OUTPUT); // LEFT 2
	pinMode(OUTERLEFT, OUTPUT); // LEFT 1 OUTERMOST
	
	// initializes all turn signal pins OFF
	digitalWrite(OUTERRIGHT, LOW);
	digitalWrite(MIDRIGHT, LOW);
	digitalWrite(INNERRIGHT, LOW);
	digitalWrite(INNERLEFT, LOW);
	digitalWrite(MIDLEFT, LOW);
	digitalWrite(OUTERLEFT, LOW);
	
	// setup input pins for reading turn signals
	pinMode(LEFTSW, INPUT); // Left Turn digital read
	pinMode(RIGHTSW, INPUT); // Right Turn digital read
	
	// setup input pins for reading voltage divider for brakes
	pinMode(RIGHTBRAKE, INPUT); // Left Brakes Analog read
	pinMode(LEFTBRAKE, INPUT); // Right Brakes Analog read

	// setup Output pin for driving LED MATRIX (RED: BRAKE/LIGHTS)
	pinMode(BRAKEPIN, OUTPUT); // PWM signal for driving power MOSFET
	digitalWrite(BRAKEPIN, LOW); // initialize as OFF
}

void configure_timers(){
	// Configure generator 2 to output a 4 MHz clock
    config_gclk_gen(    0x2,            // Appy changes to generator 2
                        1,              // Divide clock by 1 --> f = 32.768 kHz = 32.768 kHz
                        false,          // Select simple division, i.e. divide by 1
                        osculp32k_clk   // Use Ultra low power OSC as the generator source (32.768 kHz)
                        );
	
    // Sets a timer at 4 Hz
    timer.init( 0x7,    // prescale by 1024 --> 32 Hz
                8,    // 8-bit counter period --> 4 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                4,     // Match value
                timer_overflow_cb, timer_match_cb,
                0x2,    // Use generic clock generator 2
                32.768e3// Reference frequency
                );
    // Sets a PWM at 131.072 Hz
    pwm.init(   0x0,    // prescale by 1 --> 32.768 kHz
                250,    // 8-bit counter period --> 131.072 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                10,     // Match value
                pwm_overflow_cb, pwm_match_cb,
                0x2,    // Use generic clock generator 2
                32.768e3// Reference frequency
                );
	pwm.set_duty_cycle(0.075);
	
    striptimer.init(   
				0x6,    // prescale by 256 --> 128 Hz
                8,    // 8-bit counter period -->  16 Hz
                true,   // Interrupt on overflow
                true,   // Interrupt on match
                0,     // Match value
                striptimer_overflow_cb, striptimer_match_cb,
                0x2,    // Use generic clock generator 2
                32.768e3// Reference frequency
                );
	
    timer.enable();
    pwm.enable();
	striptimer.enable();
}