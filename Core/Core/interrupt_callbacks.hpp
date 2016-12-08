#ifndef INTERRUPT_CALLBACK_HEADER____
#define INTERRUPT_CALLBACK_HEADER____

#include <cstdint>	// For std::uint32_t

void checkLeftTurn();
void checkRightTurn();

void pwm_overflow_cb(std::uint32_t);
void pwm_match_cb(std::uint32_t);
void timer_overflow_cb(std::uint32_t);
void timer_match_cb(std::uint32_t);
void striptimer_overflow_cb(std::uint32_t);
void striptimer_match_cb(std::uint32_t);

#endif