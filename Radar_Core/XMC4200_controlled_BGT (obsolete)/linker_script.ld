/**
 * @file XMC4200x256.ld
 * @date 2015-07-07
 *
 * @cond
 *********************************************************************************************************************
 * Linker file for the GNU C Compiler v1.5
 * Supported devices: XMC4200-F64x256
 *                    XMC4200-Q48x256
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-07-07:
 *     - Product splitting
 *     - Copyright notice update
 *      
 * @endcond 
 *
 */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
	FLASH_1_cached(RX) : ORIGIN = 0x08000000, LENGTH = 0x40000
	FLASH_1_uncached(RX) : ORIGIN = 0x0C000000, LENGTH = 0x40000
	PSRAM_1(!RX) : ORIGIN = 0x1FFFC000, LENGTH = 0x4000
	DSRAM_1_system(!RX) : ORIGIN = 0x20000000, LENGTH = 0x6000
	SRAM_combined(!RX) : ORIGIN = 0x1FFFC000, LENGTH = 0xA000
}

stack_size = 2048;

SECTIONS
{
	/* TEXT section */

	.text : AT(ORIGIN(FLASH_1_uncached))
	{
		sText = .;
		KEEP(*(.reset));
		*(.text .text.* .gnu.linkonce.t.*);

		/* ARM <->THUMB interworking */
		*(.glue*)
		*(.v4*)
		*(.vfp11_veneer)

		/* C++ Support */
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		KEEP(*(.fini))
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;

		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

		/* Exception handling support */
		__extab_start = .;
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		. = ALIGN(4);
		__extab_end = ABSOLUTE(.);
	} > FLASH_1_cached

	/* Exception handling, exidx needs a dedicated section */
	.ARM.exidx ABSOLUTE(__extab_end): AT(__extab_end | 0x04000000)
	{
		__exidx_start = .;
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		. = ALIGN(4);
		__exidx_end = ABSOLUTE(.);
	} > FLASH_1_cached

	/* CONST data section */
	.rodata ABSOLUTE(__exidx_end): AT(__exidx_end | 0x04000000)
	{
		*(.rodata .rodata.*)
		*(.gnu.linkonce.r*)
	} > FLASH_1_cached

	. = ALIGN(16);
	
	/* End of RO-DATA and start of LOAD region for DATA */
	__ram_load = . | 0x04000000;

    /* DSRAM layout (Lowest to highest)*/
	Stack (NOLOAD) : 
	{
		. = . + stack_size;
		__initial_sp = .;
	} > SRAM_combined

	/* functions with __attribute__((section(".ram_code"))) */
    .ram_code : AT(__ram_load)
	{
		__ram_code_start = .;
		*(.ram_code)  	
        . = ALIGN(4);
		__ram_code_end = .;
	} > SRAM_combined
	__ram_code_load = LOADADDR (.ram_code);
	__ram_code_size = __ram_code_end - __ram_code_start;

	/* Standard DATA and user defined DATA/BSS/CONST sections */
	.data : AT(LOADADDR(.ram_code)  + SIZEOF(.ram_code))
	{
		__data_start = .;
		* (.data);
		* (.data*);
		*(*.data);
		*(.gnu.linkonce.d*)
        . = ALIGN(4);
        __data_end = .;
	} > SRAM_combined
    __data_load = LOADADDR (.data);
	__data_size = __data_end - __data_start;
        
	/* BSS section */
	.bss (NOLOAD) : 
	{
		__bss_start = .;
		* (.bss);
		* (.bss*);
		* (COMMON);
		*(.gnu.linkonce.b*)
        . = ALIGN(4);
        __bss_end = .;
	} > SRAM_combined
	__bss_size = __bss_end - __bss_start;

    /* Shift location counter, so that USB_RAM is located in DSRAM1 */    
    __shift_loc =  (__bss_end >= ORIGIN(DSRAM_1_system)) ? 0 : (ORIGIN(DSRAM_1_system) - __bss_end);

    USB_RAM  (__bss_end + __shift_loc) (NOLOAD) :
    {
        USB_RAM_start = .;
        *(USB_RAM)
        . = ALIGN(4);
        USB_RAM_end = .;
		. = ALIGN(8);
		Heap_Bank1_Start = .;
    } > SRAM_combined
	USB_RAM_size = USB_RAM_end - USB_RAM_start;

    /* .no_init section */
    .no_init 0x20005FC0 (NOLOAD) : 
    {
        Heap_Bank1_End = .;
        * (.no_init);
    } > SRAM_combined
        
	/* Heap - Bank1*/
	Heap_Bank1_Size  = Heap_Bank1_End - Heap_Bank1_Start;

	/DISCARD/ :
	{
		*(.comment)
	}

	.stab		0 (NOLOAD) : { *(.stab) }
	.stabstr	0 (NOLOAD) : { *(.stabstr) }

	/* DWARF 1 */
	.debug				0 : { *(.debug) }
	.line				0 : { *(.line) }

	/* GNU DWARF 1 extensions */
	.debug_srcinfo		0 : { *(.debug_srcinfo) }
	.debug_sfnames		0 : { *(.debug_sfnames) }

	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges		0 : { *(.debug_aranges) }
	.debug_pubnames		0 : { *(.debug_pubnames) }
	.debug_pubtypes		0 : { *(.debug_pubtypes) }

	/* DWARF 2 */
	.debug_info			0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev		0 : { *(.debug_abbrev) }
	.debug_line			0 : { *(.debug_line) }
	.debug_frame		0 : { *(.debug_frame) }
	.debug_str			0 : { *(.debug_str) }
	.debug_loc			0 : { *(.debug_loc) }
	.debug_macinfo		0 : { *(.debug_macinfo) }

	/* DWARF 2.1 */
	.debug_ranges		0 : { *(.debug_ranges) }

	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames	0 : { *(.debug_weaknames) }
	.debug_funcnames	0 : { *(.debug_funcnames) }
	.debug_typenames	0 : { *(.debug_typenames) }
	.debug_varnames		0 : { *(.debug_varnames) }

	/* Build attributes */
	.build_attributes	0 : { *(.ARM.attributes) }
}
