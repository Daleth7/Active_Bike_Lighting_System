int dutyCycleTest = 50;

void setup() {
  // put your setup code here, to run once:
  interrupts ();
  pinMode(4, OUTPUT); // PWM signal for driving power MOSFET
  pinMode(8, INPUT);
  attachInterrupt(2, checkLeftTurn, CHANGE); // creates an interrupt for SETTING LEFT/RIGHT turn signals
  
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(4, dutyCycleTest);
  SerialUSB.println(dutyCycleTest);
  dutyCycleTest++;
  delay(50);
  if (dutyCycleTest > 250){
    dutyCycleTest = 0;
  }
}

void checkLeftTurn(){
  
  dutyCycleTest++;
  /*
  if ((digitalRead(8)) == 0){ // read digital value of LEFT TURN pin
    leftTurnFlag = 1; // sets LEFT TURN flag to high
  }
  
  if ((digitalRead(8)) == 1){ // read digital value of LEFT TURN pin
    leftTurnFlag = 0; // sets LEFT TURN flag to high
  }
  */
}
