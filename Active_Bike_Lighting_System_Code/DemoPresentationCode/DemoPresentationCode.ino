#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
	
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_RGBW + NEO_KHZ800); // call LED Strip library

#define GreenLEDstrip 1
#define YellowLEDstrip 2
#define RedLEDstrip 3


int leftTurnFlag = 0; // initialize turn signal flag variables as 1 = ON
int rightTurnFlag = 0; 

volatile static int turnTimeL = 0; // initalizes counter for arrow display. ie. turns on 3 for 10 cycles, 2 for 10 cycles, 1 for 10 cycles
volatile static int turnTimeR = 0; // counter for RIGHT display

int LEDstripmode = GreenLEDstrip; // initializes variable to be pulsing Green on LED strip

void setup() { // setup code to run once for initalization
	// setup output pins for turn signals

	pinMode(4, OUTPUT); // RIGHT 1 OUTERMOSt
	pinMode(5, OUTPUT); // RIGHT 2
	pinMode(7, OUTPUT); // RIGHT 3 (CLOSEST TO MIDDLE)
	pinMode(10, OUTPUT); // LEFT 3 (CLOSEST TO MIDDLE)
	pinMode(11, OUTPUT); // LEFT 2
	pinMode(12, OUTPUT); // LEFT 1 OUTERMOST
	
	digitalWrite(4, LOW); // initializes all turn signal pins OFF
	digitalWrite(5, LOW); 
	digitalWrite(7, LOW); 
	digitalWrite(10, LOW);
	digitalWrite(11, LOW); 
	digitalWrite(12, LOW); 
	
	// setup input pins for reading turn signals
	pinMode(8, INPUT); // Left Turn digital read
	pinMode(9, INPUT); // Right Turn digital read
	
	// setup input pins for reading voltage divider for brakes
	pinMode(A1, INPUT); // Brakes analog read

	// setting interrupt
	interrupts (); // initalizes interrupts to be ON explicitly
	
	
//////////////////////not sure if Arduino Interrupt functions are working correctly
	
	//attachInterrupt(8, checkTurnSignals, FALLING); // creates an interrupt for SETTING LEFT/RIGHT turn signals
	//attachInterrupt(8, clearTurnSignals, RISING); // creates interrupt for CLEARING turn signal flags
	
	//attachInterrupt(9, checkTurnSignals, FALLING); // creates an interrupt for SETTING LEFT/RIGHT turn signals
	//attachInterrupt(9, clearTurnSignals, RISING); // creates interrupt for CLEARING turn signal flags
	
	// initalize LED strip
	strip.begin(); //
	strip.show(); // Initialize all pixels to 'off'
}

void loop() // put your main code here, to run repeatedly:
{

	processSignals(); // processing turn signals & brake signals, and displays appropriately
	
	checkTurnSignals(); // have opted to call check/clear signals in main loop instead of interrupts at this time
	clearTurnSignals(); // operates extremely fast to not be an issue
	
	displayLEDs(); // function to call for displaying LED's
  
}

void processSignals() { // process turn and brake signals
	

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
	
	if (rightTurnFlag == 1){ // Right Turn button is ON
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
	
	/*if (brakeVoltage < 2.7){
		
	}*/
	
}



void displayLEDs() { // LED strip display function
	
	switch (LEDstripmode){
		
		case GreenLEDstrip:
			
			GreenLEDs();
			break;
			
		case YellowLEDstrip:
			//put code here
			break;
			
		case RedLEDstrip:
			//put code here
			break;
			
	}

	//GreenLEDs(); // LED strip should always be on, green will be default color
	//YellowLEDs();
	//RedLEDs();
	
}
void checkBrakeSense() {
	
	// read the input on analog pin 0:
	int sensorValue = analogRead(A0);
	
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.3V):
	float brakeVoltage = sensorValue * (3.3 / 1023.0);

	//debugging purposes, print out the value you read:
	SerialUSB.println(brakeVoltage);
	
}

void checkTurnSignals() {
	
	// read digital input from turn signal I/O's powered by 3.3 (HIGH), grounded to 0 when switch is on
	
	if ((digitalRead(8)) == 0){ // read digital value of LEFT TURN pin
		leftTurnFlag = 1; // sets LEFT TURN flag to high
	}
	
	if ((digitalRead(9)) == 0){ // read digital value of RIGHT TURN pin
		rightTurnFlag = 1; // sets RIGHT TURN flag to high
	}
	
}

void clearTurnSignals() {
	
	// clear turn signal flags when turn signals are switched to OFF
	
	if (digitalRead(8) == 1){ // read digital value of LEFT TURN pin
		leftTurnFlag = 0; // sets LEFT TURN flag to low
		turnTimeL = 0;
		digitalWrite(10, LOW);
		digitalWrite(11, LOW);
		digitalWrite(12, LOW);
	}
	
	if (digitalRead(9) == 1){ // read digital value of RIGHT TURN pin
		rightTurnFlag = 0; // sets RIGHT TURN flag to low
		turnTimeR = 0;
		digitalWrite(4, LOW);
		digitalWrite(5, LOW);
		digitalWrite(7, LOW);
	}

}


void GreenLEDs() { // Pulsing Green, 3 LED's are on at a time

	static int greenCounter = 0;

	if (greenCounter == 3000){
	
		static int b = 0;
		static int offCounter = 0; // initializes counter
	
		strip.setPixelColor(b, 255, 0, 0, 0);
		strip.setPixelColor(b+1, 255, 0, 0, 0);
		strip.setPixelColor(b+2, 255, 0, 0, 0);
	
		if (offCounter == 1){ // turns off LEDs after a cycle of being on
		
			strip.setPixelColor(b-1, 0, 0, 0);
			//strip.setPixelColor(b, 0, 0, 0); testing with 1 clear or 2 cleared
		
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


