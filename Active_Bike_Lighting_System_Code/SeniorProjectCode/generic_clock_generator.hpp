/*************************************************************************

        Provide a simple interface to configuring the generic clock
    generators. For mor information about these generators, refer to
    pg. 136 [15] of SAMD21 E/G/J datasheet complete.

        To configure a generic clock generator, the following
    information is required:
        1) ID of the generator to configure [0x0 - 0x7]
        2) The division factor to divide the frequency by
        3) Which type of division to apply (see examples below)
        4) The clock source for the generator

        In addition to the above, the following information can
    be used for debugging or power saving purposes:
        1) Whether or not to output the generator output directly
            to a pin. Useful for debugging.
            - Defaulted to false
        2) Whether or not to run the generator even while the chip
            is in standby mode, a.k.a. "sleeping." Useful for
            saving power.
            - Defaulted to false

        Note that the following clock sources are already available
    by default:
        - OSCULP32K [32.768 kHz]: Ultra low power oscillator that
            is always running, even in standby mode. Recommended if
            power saving is required and accuracy and stability of
            the signal are not as important.
        - XOSC32K [32.768 kHz]: External crystal oscillator.
        - OSC8M [8 MHz]: Internal oscillator used to clock the CPU upon
            power up.
        - DFLL48M [48 MHz]: Digital frequency locked loop oscillator
            configured by the Arduino libraries and is used to clock
            the CPU after power up. Generator 0 has also been
            configured by default to use this clock source.

    Example usage:
        // Task: Generate a 4 MHz clock from the DFLL48M clock source.
        config_gclk_gen(    0x2,    // Configure generator 2
                            12,     // Divide clock by 12 --> 48 / 12 = 4 MHz
                            false,  // Simple clock division --> divide by 12
                            dfll48m_clk
                            );

        // Task: Generate a 1.024 kHz clock from the OSCULP32K clock source.
        config_gclk_gen(    0x3,    // Configure generator 3
                            4,      // Divide clock by 32 --> 32.768 / ( 2^(4+1) ) = 1.024 kHz
                            true,   // Binary clock division --> divide by 2^(4+1)
                            osculp32k_clk
                            );

*************************************************************************/

#ifndef GENE_CLO_GENERATOR_UTI9292_
#define GENE_CLO_GENERATOR_UTI9292_

#include <cstdint>

enum gen_clk_src_type : std::uint8_t {
    xosc_clk            = 0x0,  // External clock signal or crystal oscillator
    gclkin_clk          = 0x1,  // External clock from the genererator input pad/pin
    gclkgen1_clk        = 0x2,  // Use output from generator 1
    osculp32k_clk       = 0x3,  // Internal, ultra low power oscillator [32.768 kHz]. Always running even in standby mode.
    osc32k_clk          = 0x4,  // Tunable, internal oscillator [32.768 kHz]
    xosc32kreserved_clk = 0x5,  // Fixed external clock signal or crystal oscillator [32.768 kHz]
    osc8m_clk           = 0x6,  // Internal oscillator. [8 MHz]
    dfll48m_clk         = 0x7,  // Digital Frequency Locked Loop oscillator [48 MHz]
    fdpll96m_clk        = 0x8   // Fractional Digital Phase Locked Loop [48 - 96 MHz]
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