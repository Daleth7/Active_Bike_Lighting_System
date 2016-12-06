// list of includes

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <cstdint>
#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"

#define LEDPIN 6 // this is the dedicated data line for the LED strip
	
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, LEDPIN, NEO_RGBW + NEO_KHZ800); // call LED Strip library

// states for LED strip

#define GREENLEDSTRIP 1
#define YELLOWLEDSTRIP 2
#define REDLEDSTRIP 3

// pin defines

#define OUTERRIGHT	4 	// RIGHT1
#define MIDRIGHT 	5	// RIGHT2
#define INNERRIGHT 	7 	// RIGHT3
#define OUTERLEFT 	12 	// LEFT1
#define MIDLEFT 	11	// LEFT2
#define INNERLEFT 	10 	// LEFT3

#define OUTER_ARROW 	3
#define MIDDLE_ARROW 	2
#define INNER_ARROW 	1

#define BRAKEPIN 3

#define LEFTSW 8
#define RIGHTSW 9


#define RIGHTBRAKE A1
#define LEFTBRAKE A2

// initialization of all global variables

TimerCount3& timer = TimerCount3::singleton();
TimerCount4& pwm = TimerCount4::singleton();

bool leftTurnFlag = false; // initialize turn signal flag variables as 1 = ON
bool rightTurnFlag = false; 
std::uint8_t arrow_pin = OUTER_ARROW; // Keep track of which arrow is turned on (Inner, Middle, Outer)

bool brakeFlag = false; // initializes brake signal flag variables, 1 = ON 

float leftBrakeVoltage = 0; // initialize ADC brake converted voltage value
float rightBrakeVoltage = 0;

int ledStripMode = GREENLEDSTRIP; // initializes variable to be pulsing Green on LED strip

// list of used functions

void configure_pins();
void configure_timers();

void pwm_overflow_cb(std::uint32_t counter);
void pwm_match_cb(std::uint32_t counter);

void timer_overflow_cb(std::uint32_t);
void timer_match_cb(std::uint32_t);

void setup() { // setup code to run once for initalization



	configure_pins();
	configure_timers();
    
    timer.enable();
    pwm.enable();
	
	// setting interrupt
	interrupts (); // initalizes interrupts to be ON explicitly
	
	attachInterrupt(LEFTSW, checkLeftTurn, CHANGE); // creates an interrupt for CHECKING Left turn signal
	attachInterrupt(RIGHTSW, checkRightTurn, CHANGE); // creates an interrupt for CHECKING Right turn signal
	
	// initalize LED strip
	strip.begin(); //
	strip.show(); // Initialize all pixels to 'off'
}

void checkLeftTurn(){ // interrupt call function for checking left turn signal status
	
	if ((digitalRead(LEFTSW)) == 0){ // read digital value of LEFT TURN pin
		leftTurnFlag = true; // sets LEFT TURN flag
		arrow_pin = INNER_ARROW;
	}
	
	if ((digitalRead(LEFTSW)) == 1){ // read digital value of LEFT TURN pin
		leftTurnFlag = false; // clear left turn flag
		digitalWrite(INNERLEFT, LOW);
		digitalWrite(MIDLEFT, LOW);
		digitalWrite(OUTERLEFT, LOW);
	}
	
}

void checkRightTurn() { // interrupt call function for checking right turn signal status

	if ((digitalRead(RIGHTSW)) == 0){ // read digital value of RIGHT TURN pin
		rightTurnFlag = true; // sets RIGHT TURN flag
		arrow_pin = INNER_ARROW;
	}
	
	if ((digitalRead(RIGHTSW)) == 1){ // read digital value of RIGHT TURN pin
		rightTurnFlag = false; // clear right turn flag
		digitalWrite(OUTERRIGHT, LOW);
		digitalWrite(MIDRIGHT, LOW);
		digitalWrite(INNERRIGHT, LOW);
	}


}

void loop() // put your main code here, to run repeatedly:
{

	// polling for brake status
	checkBrakeSignals();
	
//	checkLeftTurn();
//	checkRightTurn();
	//processSignals(); // processing turn signals & brake signals, and displays appropriately
	displayLEDs(); // function to call for displaying LED's
  
}

void pwm_overflow_cb(std::uint32_t){
//	if(brakeFlag)
//	{
		digitalWrite(BRAKEPIN, HIGH);
//	}
}

void pwm_match_cb(std::uint32_t){
    digitalWrite(BRAKEPIN, LOW);
}

void timer_overflow_cb(std::uint32_t){ // operating off 4 Hz timer/counter setting
	//emergency case
	static int emergency_arrows = 1;
	if (rightTurnFlag && leftTurnFlag) {
		
		digitalWrite(OUTERRIGHT, emergency_arrows);
		digitalWrite(OUTERLEFT, emergency_arrows);
		digitalWrite(MIDRIGHT, emergency_arrows);
		digitalWrite(MIDLEFT, emergency_arrows);
		digitalWrite(INNERRIGHT, emergency_arrows);
		digitalWrite(INNERLEFT, emergency_arrows);	
		
		emergency_arrows = !emergency_arrows;
	}
	else {
		
		digitalWrite(MIDRIGHT, LOW);
		digitalWrite(MIDLEFT, LOW);
		digitalWrite(INNERRIGHT, LOW);
		digitalWrite(INNERLEFT, LOW);
		digitalWrite(OUTERRIGHT, LOW);
		digitalWrite(OUTERLEFT, LOW);
		
		switch(arrow_pin){
			case OUTER_ARROW:
				digitalWrite(OUTERRIGHT, rightTurnFlag);
				digitalWrite(OUTERLEFT, leftTurnFlag);
				break;
			case MIDDLE_ARROW:
				digitalWrite(MIDRIGHT, rightTurnFlag);
				digitalWrite(MIDLEFT, leftTurnFlag);
				break;
			case INNER_ARROW:
				digitalWrite(INNERRIGHT, rightTurnFlag);
				digitalWrite(INNERLEFT, leftTurnFlag);
				break;
			default: break;	// Should never happen
		}
		// cycle through the arrows (from inner to outer)
		++arrow_pin;
		
		if(arrow_pin > OUTER_ARROW)
		{
			arrow_pin = INNER_ARROW;
		}
	}

	
}

void timer_match_cb(std::uint32_t){
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
}

/*
void processSignals() { // process turn and brake signals
	
	static int turnTimeL = 0; // initalizes counter for arrow display. ie. turns on 3 for 10 cycles, 2 for 10 cycles, 1 for 10 cycles
	static int turnTimeR = 0; // counter for RIGHT display
	
	if (leftTurnFlag == 1) { // Left Turn button is ON
	
		if (turnTimeL < 20000){ // turns on arrow closest to middle (arrow 1)
			
			digitalWrite(10, HIGH); // LEFT3
			
		}
		if (turnTimeL == 20000) { // after 10 cycles, go to arrow 2
		
			digitalWrite (10, LOW); // turns LEFT3 OFF
			digitalWrite (11, HIGH); // turns LEFT2 ON
			
		}
		
		if (turnTimeL == 40000) { // after 10 cycles, go to arrow 3
		
			digitalWrite (11, LOW); // turns LEFT2 OFF
			digitalWrite (12, HIGH); // turns LEFT1 ON
			
		}
		
		if (turnTimeL == 60000){ // reset & turn off arrow 3
		
			turnTimeL = 0; // resets counter for arrow display
			digitalWrite (12, LOW); // turns LEFT1 OFF
			
		}
        
		turnTimeL++; 
	}
	
	else {
		
		clearLeftTurn(&turnTimeL);
		
	}
	
	if (rightTurnFlag == 1) { // Right Turn button is ON
		if (turnTimeR < 20000){ // turns on arrow closest to middle (arrow 1)
			
			digitalWrite(7, HIGH); // LEFT3
			
		}
		
		if (turnTimeR == 20000) { // after 10 cycles, go to arrow 2
		
			digitalWrite (7, LOW); // turns LEFT3 OFF
			digitalWrite (5, HIGH); // turns LEFT2 ON
			
		}
		
		if (turnTimeR == 40000) { // after 10 cycles, go to arrow 3
		
			digitalWrite (5, LOW); // turns LEFT2 OFF
			digitalWrite (4, HIGH); // turns LEFT1 ON
			
		}
		
		if (turnTimeR == 60000){ // reset & turn off arrow 3
		
			turnTimeR = 0; // resets counter for arrow display
			digitalWrite (4, LOW); // turns LEFT1 OFF
			
		}
        
		turnTimeR++; 
	}
	
	if (brakeFlag == 1) { // Brake Flag is ON, either LEFT or RIGHT is engaged
	
		digitalWrite(13, HIGH); // SEND 100% duty cycle value to brakes, FULL BRIGHTNESS
		
	}
}
*/

void displayLEDs() { // LED strip display function
	
	switch (ledStripMode){
		
		case GREENLEDSTRIP:
			
			greenLEDs();
			break;
			
		case YELLOWLEDSTRIP:
			//put code here
			break;
			
		case REDLEDSTRIP:
			//put code here
			break;
			
	}

}


void checkBrakeSignals() {
	
	// read the input on analog pin 1:
	int leftSensorValue = analogRead(RIGHTBRAKE); // reads analog voltage on pin A1 for Left Brakes
	int rightSensorValue = analogRead(LEFTBRAKE); // reads analog voltage on pin A2 for Right Brakes
	
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
	leftBrakeVoltage = leftSensorValue * (3.3 / 1023.0); // Conversion of int ADC value to voltage, using 3.3V reference limit
	rightBrakeVoltage = rightSensorValue * (3.3 / 1023.0); // 

	// Raising brake flag if EITHER left OR right brake is engaged
	// if value read by ADC after conversion is less than 2.7, raise brake flag
	static bool braked = false;	// Make sure register is not constantly being written to.
								//	If so, the duty cycle will fluctuate.
	if((leftBrakeVoltage < 2.7) || (rightBrakeVoltage < 2.7)){
		pwm.set_duty_cycle(0.2);	// Turn brighter
		braked = true;
	} else if(braked){
		braked = false;
		pwm.set_duty_cycle(0.075);	// Turn dimmer
	}
}

void greenLEDs() { // Pulsing Green, 3 LED's are on at a time

	static int greenCounter = 0;

	if (greenCounter == 3000){
	
		static int b = 0;
		static int offCounter = 0; // initializes counter
	
		strip.setPixelColor(b, 255, 0, 0, 0);
		strip.setPixelColor(b+1, 255, 0, 0, 0);
		strip.setPixelColor(b+2, 255, 0, 0, 0);
	
		if (offCounter == 1){ // turns off LEDs after a cycle of being on
		
			strip.setPixelColor(b-1, 0, 0, 0);
		
			offCounter = -1;
		}	
	
		strip.show();
		//delay (50);
	
		offCounter++;
		b++;
		greenCounter = 0;
		
		if (b == 31) { // resets LED strip counter
			b = 0;
		}
	}
	greenCounter++;
}

void RedLEDs() {

  for (int i=0; i<5; i++) {
    
  for (int j=0; j<30; j++) {
    strip.setPixelColor(j, 0, 255, 0, 0);
    strip.show(); 
  }

  delay(50);
  for (int k=0; k<30; k++) {
    strip.setPixelColor(k, 0, 0, 0, 0);
    strip.show();
    }
    delay (50);
  }
}

void YellowLEDs() {

  for (int i=0; i<10; i++) {
    
    for (int c=0; c<30; c++) {
  strip.setPixelColor(c, 255, 255, 0, 0);
  strip.show(); 
  }

  delay(200);
  for (int d=0; d<30; d++) {
    strip.setPixelColor(d, 0, 0, 0, 0);
    strip.show();
    }
  delay (200);
}
}


