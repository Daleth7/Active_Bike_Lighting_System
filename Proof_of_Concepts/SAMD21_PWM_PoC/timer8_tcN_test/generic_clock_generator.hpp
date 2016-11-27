/*************************************************************************

                              Timer8 class

        Provide a simple interface to configuring the generic clock
    generators. For mor information about these generators, refer to
    pg. 136 [15] of SAMD21 E/G/J datasheet complete.

*************************************************************************/

#ifndef GENE_CLO_GENERATOR_UTI9292_
#define GENE_CLO_GENERATOR_UTI9292_

#include <cstdint>

enum gen_clk_src_type : std::uint8_t {
    xosc_clk            = 0x0,
    gclkin_clk          = 0x1,
    gclkgen1_clk        = 0x2,
    osculp32k_clk       = 0x3,
    osc32k_clk          = 0x4,
    xosc32kreserved_clk = 0x5,
    osc8m_clk           = 0x6,
    dfll48m_clk         = 0x7,
    fdpll96m_clk        = 0x8
};

void config_gclk_gen(   std::uint8_t gen_id,                    // ID of generator to configure
                                                                //  (pg. 136 [15.8.4] SAMD21 E/G/J datasheet complete)
                        std::uint16_t gen_div,                  // Factor to divide original frequency by
                                                                //  (pg. 137 [15.8.5] SAMD21 E/G/J datasheet complete)
                                                                //  Note that different generators have different
                                                                //  maximum division factors.
                        bool div_bin,                           // Select if frequency is divided by 2^(gen_div+1) [true]
                                                                //  or simply by gen_div [false]
                        gen_clk_src_type gen_src = dfll48m_clk, // The clock source to use for the generator.
                                                                //  (pg. 134 [15.8.4] SAMD21 E/G/J datasheet complete)
                                                                //  Note that only some clocks are already set up by
                                                                //  Arduino libraries.
                                                                //  E.g. DFLL48M [0x07]
                        bool output_to_pin = false,             // Enable outputing the generator output directly to pin.
                                                                //  See datasheet to find pins associated to generators.
                                                                //  (pg. 31 [7] SAMD21 E/G/J datasheet complete)
                        bool run_in_standby = false             // Allow generator to run even if chip is in standby mode.
                        );

#endif