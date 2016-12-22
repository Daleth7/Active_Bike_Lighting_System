#include "generic_clock_generator.hpp"

#include "sam.h"

void config_gclk_gen(   std::uint8_t gen_id,
                        std::uint16_t gen_div, bool div_bin,
                        gen_clk_src_type gen_src,
                        bool output_to_pin, bool run_in_standby
){
    gen_id &= 0xF; // Limit to four bits

    GCLK->GENCTRL.reg =   (gen_id << 0) // Make sure generator is disabled
                        & ~(1 << 16)    //  while configurations are changed.
                        ;
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // Wait for synchronization
    GCLK->GENDIV.reg =    (gen_div << 8)
                        | (gen_id << 0)
                        ;   // pg. 137
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // Wait for synchronization
    GCLK->GENCTRL.reg =   (run_in_standby << 21)
                        | (div_bin << 20)
                        | (output_to_pin << 19)
                        | (1 << 17)     // Force 50% duty cycle
                        | (1 << 16)     // Enable the clock generator
                        | (((std::uint8_t)gen_src) << 8)
                        | (gen_id << 0)
                        ;   // pg. 133
}
