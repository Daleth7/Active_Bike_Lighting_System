#include "timer8_tc.hpp"
#include "generic_clock_generator.hpp"
#include "ws2812_led_strip.hpp"

#include <cstdint>

#define OUT_PIN 13

TimerCount3& timer = TimerCount3::singleton();
ws2812_led_strip<7> led_strip(0x3, timer, OUT_PIN, true);

void debug_overflow_callback(std::uint32_t counter);
void debug_match_callback(std::uint32_t counter);

C_Listener debug_listener = make_timer_listener( debug_overflow_callback,
                                                 debug_match_callback
                                                 );

std::uint32_t debug_counter_copy = 0;
float debug_recorded_time_passed = -1.0f;
float debug_recorded_ontime = -1.0f;
bool print_debug_information = false;
std::uint32_t ref_time = 0;

void setup(){
    SerialUSB.begin(9600);

    led_strip.init();

    timer.add_listener(&debug_listener);

//    led_strip.set_pixel(0, 0, 0, 0, 0);

    led_strip.enable();
    timer.enable();
}

void loop(){
    if(print_debug_information){
        print_debug_information = false;
        SerialUSB.print("Counter: ");
            SerialUSB.print(debug_counter_copy);
        SerialUSB.print(" | ");
        SerialUSB.print("Time passed: ");
            SerialUSB.print(debug_recorded_time_passed);
            SerialUSB.print(" s");
        SerialUSB.print(" | ");
        SerialUSB.print("On-time: ");
            SerialUSB.print(debug_recorded_ontime);
            SerialUSB.print(" s");
        SerialUSB.print(" | ");
        SerialUSB.print("Frequency: ");
            SerialUSB.print(timer.frequency());
            SerialUSB.print(" Hz");
        SerialUSB.println("");
    }
}

void debug_overflow_callback(std::uint32_t counter){
    if((counter - ref_time) >= timer.frequency()){
        debug_counter_copy = counter;
        debug_recorded_time_passed = counter / timer.frequency();
        print_debug_information = true;
        ref_time = counter;
    }
}

void debug_match_callback(std::uint32_t){
    debug_recorded_ontime = timer.duty_cycle() / timer.frequency();
}