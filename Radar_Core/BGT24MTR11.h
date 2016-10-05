/*
 * BGT24MTR11.h
 *
 * File includes definitions for SPI Data Bits in the control register of the BGT24.
 * Ref. BGT24MTR11 Data Sheet Rev. 3.1
 *
 *
 *  Created on: Sep 2, 2015
 *      Author: nardone
 */

#ifndef BGT24MTR11_H_
#define BGT24MTR11_H_

typedef enum {TX_OFF, TX_MIN, TX_MID, TX_MAX} BGT_Power_t;

//void BGTConfig(void);
void BGTStartTX(void);
void BGTStopTX(void);
void BGT_LO_Adjust(uint32_t Q2Count);
void BGTSetTxPower(BGT_Power_t PwrLvl);


// Local Oscillator Settings
#define ISM_CNT_MAX  23126     // 24.000GHz/2^^20  Q2 low limit
#define ISM_CNT_MID  23007     // 24.125GHz/2^^20  Q2 mid band
#define ISM_CNT_MIN  22888     // 24.250GHz/2^^20  Q2 high limit
#define ISM_CNT_HYST 5


// SPI control word settings
#define BGT24_GS_MASK     	(uint16_t)0x8000   	// LNA Gain reduction; 1 = reduce LNA gain

#define BGT24_DIS_PA_MASK 	(uint16_t)0x1000  // DISABLE PA - 1 = turn off TX power

#define BGT24_AMUX_MASK   	(uint16_t)0x0F80	// AMUX0, AMUX1, AMUX2 & two Test Bits
#define BGT24_AMUX_POS		(uint16_t)7			// bit 7 of

#define BGT24_DIS_DIV64K_MASK (uint16_t)0x0040  // 1 = Disable 64K divider for Q2

#define BGT24_DIS_DIV16_MASK (uint16_t)0x0020   // 1 = Disable 16 divider for Q2

#define BGT24_PC2_BUF_MASK  (uint16_t)0x0010	// PC2_BUF 1 = High LO buffer output power
#define BGT24_PC1_BUF_MASK 	(uint16_t)0x0008	// PC1_BUF 1 = High TX buffer output power

#define BGT24_PC_PA_MASK  	(uint16_t)0x0007	// PC0_PA, PC1_PA, PC2_PA,  1 = reduce TX power

// AMUX settings
#define BGT24_AMUX_0   ((uint16_t)(0x0000<<BGT24_AMUX_POS))
#define BGT24_AMUX_1   ((uint16_t)(0x0001<<BGT24_AMUX_POS))
#define BGT24_AMUX_2   ((uint16_t)(0x0002<<BGT24_AMUX_POS))
#define BGT24_AMUX_3   ((uint16_t)(0x0003<<BGT24_AMUX_POS))
#define BGT24_AMUX_4   ((uint16_t)(0x0010<<BGT24_AMUX_POS))
#define BGT24_AMUX_5   ((uint16_t)(0x0011<<BGT24_AMUX_POS))
#define BGT24_AMUX_6   ((uint16_t)(0x0012<<BGT24_AMUX_POS))
#define BGT24_AMUX_7   ((uint16_t)(0x0013<<BGT24_AMUX_POS))

// TX PA settings
#define  BGT24_PC_PA_0  (uint16_t)0x0000     // max TX power
#define  BGT24_PC_PA_1  (uint16_t)0x0001     //
#define  BGT24_PC_PA_2  (uint16_t)0x0002     //
#define  BGT24_PC_PA_3  (uint16_t)0x0003     //
#define  BGT24_PC_PA_4  (uint16_t)0x0004     //
#define  BGT24_PC_PA_5  (uint16_t)0x0005     //
#define  BGT24_PC_PA_6  (uint16_t)0x0006     //
#define  BGT24_PC_PA_7  (uint16_t)0x0007     // min TX power


#endif /* BGT24MTR11_H_ */
