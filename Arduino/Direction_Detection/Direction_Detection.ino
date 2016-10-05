const unsigned
                ifih_pin = 10, ifqh_pin = 11,
                away_led = 8, toward_led = 9
                ;
volatile bool triggered = false;
enum direction_type {   // With respect to radar
    toward = 0, away, still
};

volatile direction_type target_direction = still;

void ifqh_sig_handler();

void setup(){
    pinMode(ifih_pin, INPUT_PULLUP);
    pinMode(ifqh_pin, INPUT_PULLUP);

    pinMode(away_led, OUTPUT);
    pinMode(toward_led, OUTPUT);

    digitalWrite(away_led, LOW);
    digitalWrite(toward_led, LOW);

    attachInterrupt(ifqh_pin, ifqh_sig_handler, CHANGE);
}

void loop(){
    if(!triggered) return;
    triggered = false;
    switch(target_direction){
        case toward:
            digitalWrite(toward_led, HIGH);
            digitalWrite(away_led, LOW);
        case away:
            digitalWrite(toward_led, LOW);
            digitalWrite(away_led, HIGH);
            break;
        case still:
        default:
            digitalWrite(toward_led, LOW);
            digitalWrite(away_led, LOW);
            break;
    }
/*
    if(digitalRead(ifih_pin) == HIGH && digitalRead(ifqh_pin) == HIGH){
        target_direction = still;
    }
*/
}

void ifqh_sig_handler(){
     triggered = true;
    switch(digitalRead(ifqh_pin) | (digitalRead(ifih_pin) << 1)){
        case 0:     // ifqh = falling & ifih = low
            target_direction = toward;
            break;
        case 1:     // ifqh = rising & ifih = low
            target_direction = away;
            break;
        case 2:     // ifqh = falling & ifih = high
            target_direction = away;
            break;
        case 3:     // ifqh = rising & ifih = high
            target_direction = toward;
            break;
        default:
            target_direction = still;
            break;
    }
}