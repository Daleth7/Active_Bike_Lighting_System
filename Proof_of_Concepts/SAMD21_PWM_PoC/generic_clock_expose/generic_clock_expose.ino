/*************************************************************************

        Output the generic clock signal being used for the timer driver
    (generic clock 0) for characterization.

*************************************************************************/



#include "sam.h"

#define GCLK_OUT_PIN 11  // D11 == PA16, PA16 can be used to output GCLK_GEN[2]
                        //  (pg. 32 [7.1] SAMD21 E/G/J datasheet complete

void setup(){
        // Multiplex PORT IO to output generated generic clock
    pinMode(GCLK_OUT_PIN, OUTPUT);
    PortGroup* port = ((Port*)(PORT))->Group + 0;                   // Get address for Port A
    volatile PORT_PINCFG_Type* gclk_pin_cfg = port->PINCFG + 16;    // Get config address for PA16;
    volatile PORT_PMUX_Type* gclk_pin_mux = port->PMUX + 8;         // Get mux address for PA16 (n = 16 / 2)

    gclk_pin_cfg->bit.PMUXEN = 1;   // Enable multiplexing
    gclk_pin_mux->bit.PMUXE = 0x7;  // Select peripheral function H (GCLK_GEN[2] output)

        // Configure generic clock generator 2 to output to IO
    GCLK->GENDIV.reg =    (0x0 << 8)    // Set signal division to 1
                        | (0x2 << 0)    // Apply division to generator 2
                        ;   // pg. 137
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // Wait for synchronization
    GCLK->GENCTRL.reg =   (1 << 21)     // Keep generating clock even if no peripheral needs it
                        | (0 << 20)     // Ensures clock is not divided
                        | (1 << 19)     // Enable outputting clock to IO
                        | (1 << 17)     // Force 50% duty cycle
                        | (1 << 16)     // Enable the clock generator
                        | (0x7 << 8)    // Select the 48 MHz DFLL as the clock source for the generator
                        | (0x2 << 0)    // Apply these configurations to generator 2
                        ;   // pg. 133
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // Wait for synchronization
    GCLK->CLKCTRL.reg =   (1 << 14) // Enable generic clock
                        | (2 << 8)  // Use generic clock generator 2
                        | (0x1B)    // Tie clock to TC3 peripheral
                        ;
    while(GCLK->STATUS.bit.SYNCBUSY == 1); // Wait for synchronization
}

void loop(){}