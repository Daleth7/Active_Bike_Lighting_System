#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_RGBW + NEO_KHZ800);

void setup() { //setup code to run once for initalization
	//setup output pins for turn signals
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(7, OUTPUT);

	//setup input pins for reading turn signals
	pinMode(8, INPUT); // Left Turn digital read
	pinMode(9, INPUT); // Right Turn digital read
	
	//setup input pins for reading voltage divider for brakes
	pinMode(A0, INPUT); // Brakes analog read
	
	
	//initalize LED strip
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() // put your main code here, to run repeatedly:
{
	checkBrakeSense(); // check if brake is pressed
	checkTurnSignals(); // check if either turn signals are pressed

	displayLEDs(); // function to call for displaying LED's
  
}

void displayLEDs() {
	
	processSignals();
	GreenLEDs();
	YellowLEDs();
	RedLEDs();
	
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
	
	int turnLeftFlag = digitalRead(8); // read digital value (HIGH/LOW) from pin 8 for left turn flag
	int rightTurnFlag = digitalRead(9); // read digital value from pin 9 for right turn flag
}

void processSignals() { //process turn and brake signals
	
	if (turnLeftFlag==0) { // Left Turn button is ON

        digitalWrite(10, HIGH); // LEFT3
        delay (500);
        digitalWrite(10, LOW);
        
        digitalWrite(11, HIGH); // LEFT2
        delay (500);
        digitalWrite(11, LOW);
        
        digitalWrite(12, HIGH); // LEFT1
        delay (500);
        digitalWrite(12, LOW);
		
	}
	
	if (rightTurnFlag==0) { // Right Turn button is ON
	
        digitalWrite(7, HIGH); // RIGHT3
		delay (500);
        digitalWrite(7, LOW); // RIGHT3
		
        digitalWrite(5, HIGH); // RIGHT2
		delay (500);
        digitalWrite(5, LOW); // RIGHT2
		
        digitalWrite(4, HIGH); // RIGHT1
		delay (500);
        digitalWrite(4, LOW); // RIGHT1
		
	}
	
	/*if (brakeVoltage < 2.7){
		
	}*/
}



void RedLEDs() {

  for (int i=0; i<20; i++) {
    
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

void GreenLEDs() {

  for (int i=0; i<5; i++) {

  for (int b=0; b<30; b++) {
    strip.setPixelColor(b, 255, 0, 0, 0);
    strip.setPixelColor(b+1, 255, 0, 0, 0);
    strip.setPixelColor(b+2, 255, 0, 0, 0);
      
    strip.show();
    delay(50);
    strip.setPixelColor(b, 0, 0, 0);
    strip.setPixelColor(b+1, 0, 0, 0);
    strip.setPixelColor(b+2, 0, 0, 0);
    }
}
}
