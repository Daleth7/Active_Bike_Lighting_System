/*
 * BGT24MTR11.c
 *
 * Driver for BGT24MTR11 24GHz Radar transceiver.
 *
 *
 *  Created on: Sep 2, 2015
 *      Author: nardone
 */
#include <DAVE.h>
#include "BGT24MTR11.h"

#define BGT_TX_MAX_POWER
//#define BGT_TX_MIN_POWER

//static void delayCycles(uint32_t cyc);

//***********************************************
//  Set the transmitter power on the BGT24MTR11
//
void BGTSetTxPower(BGT_Power_t PwrLvl)
{
	uint16_t SPI_data;

	switch (PwrLvl)
	{
	case TX_OFF:
		SPI_data = BGT24_GS_MASK | BGT24_DIS_PA_MASK | BGT24_PC_PA_7;
		break;

	case TX_MIN:
		SPI_data = BGT24_GS_MASK | BGT24_PC_PA_7;  // TX on with minimum power
		break;

	case TX_MID:
		SPI_data =  BGT24_PC1_BUF_MASK | BGT24_PC_PA_3;  // TX on with medium power
		break;

	case TX_MAX:
		SPI_data =  BGT24_PC1_BUF_MASK | BGT24_PC_PA_0;  // TX on with maximum power
		break;

	default:
		SPI_data = BGT24_PC_PA_7;  // TX on with minimum power

	}  // bottom of switch(PwrLvl)

	SPI_MASTER_Transmit(&SPI_BGT, (uint8_t *)&SPI_data, 1);

}  // end of BGTSetTxPower


#if 0
static void delayCycles(uint32_t cyc)
{
	volatile uint32_t idx;
	volatile int val = 3;

	for(idx=0; idx<cyc; idx++)
		val += idx*3;


}
#endif


//***********************************************
//  Stop the transmitter on the BGT24MTR11
//
void BGTStopTX(void)
{
	uint16_t SPI_data;

	SPI_data = BGT24_DIS_PA_MASK | BGT24_PC_PA_7;

	SPI_MASTER_Transmit(&SPI_BGT, (uint8_t *)&SPI_data, 1);
}  // end of BGTStopTX()


//***********************************************
//  Start the transmitter on the BGT24MTR11
//
void BGTStartTX(void)
{
	uint16_t SPI_data;
#ifdef BGT_TX_MIN_POWER
	SPI_data = BGT24_PC_PA_7;  // TX on with minimum power
#else
	SPI_data = BGT24_PC2_BUF_MASK | BGT24_PC1_BUF_MASK | BGT24_PC_PA_0;  // TX on with maximum power
#endif

	SPI_MASTER_Transmit(&SPI_BGT, (uint8_t *)&SPI_data, 1);
}  // end of BGTStartTX()


//**********************************************
//
//  Set BGT local oscillator to ISM range
//  24.000GHz to 24.250GHz
//  Q2 is the LO/(2^^20)
//
void BGT_LO_Adjust(uint32_t Q2Count)
{
	static int32_t fineDACVal = 0;
	static int32_t coarseDACVal = 0;

    if (Q2Count < ISM_CNT_MIN)   // increase LO
    {
    	coarseDACVal += 10;
    	DAC_SingleValue_SetValue_s16(&VCOARSE,coarseDACVal);
    }
    else if (Q2Count > ISM_CNT_MAX)  // decrease LO
    {
    	coarseDACVal -= 10;
    	DAC_SingleValue_SetValue_s16(&VCOARSE,coarseDACVal);

    }
    else if (Q2Count < (ISM_CNT_MID - ISM_CNT_HYST) )  // increase LO
    {
    	fineDACVal += 1;
    	coarseDACVal += 1;
    	DAC_SingleValue_SetValue_s16(&VCOARSE,coarseDACVal);
    	DAC_SingleValue_SetValue_s16(&VFINE,fineDACVal);
    }
    else if (Q2Count > (ISM_CNT_MID + ISM_CNT_HYST) )  // decrease LO
    {
    	fineDACVal -= 1;
    	coarseDACVal -= 1;
    	DAC_SingleValue_SetValue_s16(&VCOARSE,coarseDACVal);
    	DAC_SingleValue_SetValue_s16(&VFINE,fineDACVal);
    }


}  // end of BGT_LO_Adjust


#if 0
//***********************************************
//
//
void BGTConfig(void)
{
	//SPI_MASTER_STATUS_t myStatus;
	uint16_t SPI_data;

	// SPI_data = 0x0078;
	/* Transmit SPI Data */
	SPI_data = BGT24_DIS_DIV64K_MASK | BGT24_DIS_DIV16_MASK | BGT24_PC1_BUF_MASK  | BGT24_PC_PA_0;
	SPI_MASTER_Transmit(&SPI_BGT, (uint8_t *)&SPI_data, 1);

	//SPI_data = 0x0018;
	SPI_data = BGT24_PC1_BUF_MASK | BGT24_PC_PA_0;
	/* Transmit SPI Data */
	SPI_MASTER_Transmit(&SPI_BGT, (uint8_t *)&SPI_data, 1);

}  // end of BGTConfig()
#endif
