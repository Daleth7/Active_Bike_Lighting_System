/*************************************************************************

        Contains macro switches to choose which test to perform on the
    8-bit timer driver using TC3. Only one test should be performed at
    a time.
    Uncomment the appropriate macro to compile the test.

*************************************************************************/

#ifndef HEADER_W_5312MAC2RO_SW_F_CHOOSE_TEST33
#define HEADER_W_5312MAC2RO_SW_F_CHOOSE_TEST33

#define TIMER8_ONLY_BASIC_FUNCTIONALITY_TEST
//#define TIMER8_ONLY_PWM_TEST
//#define TIMER8_ONLY_SWEEP_DUTY_TEST
//#define TIMER8_ONLY_SWEEP_FREQ_TEST

void setup();
void loop();

#endif