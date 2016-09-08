/* main.c
 *
 * This application runs on the Sense2Go2 boards that have a silk screen revision
 * of Sense2Go_3.0.  The application runs on an XMC4200 target microcontroller and
 * performs stand alone detection processing for IFI_HG and IFQ_HG inputs from the
 * BGT24TR11.  This application uses Continuous Wave (CW) Doppler Radar and is built
 * with DAVE v4.1.2 using DAVE APPs.
 *
 * A BGT24 detection is signaled by illuminating the yellow LED.
 * A green LED is illuminated at a 1Hz rate, to indicate that the application is running.
 *
 * Support contact: Richard.Nardone@infineon.com
 *
 * Mod Hist:
 *
 * 26-OCT-15 RSN	B01: Beta Release 01 from S2GTWO_MD_SA_T008.
 *
 * */
#include <DAVE.h>   		// Declarations from DAVE Code Generation (includes SFR declaration)
#include "BGT24MTR11.h"     // BGT24 Configuration settings
#include <math.h>

#define FFT_SIZE 512       // Must be 16, 32, 64, 128, 256, 512, 1024, 2048, 4096
#define NUM_SAMPLES FFT_SIZE  // MUST change DMA block size to match FFT_SIZE !!!!!!
#define INVERT_FFT		(0)

#define SAMPLE_FREQ_5KHZ    (200*100)   // Timer setting for Fs =   5,000Hz
#define SAMPLE_FREQ_10KHZ   (100*100)   // Timer setting for Fs =  10,000Hz
#define SAMPLE_FREQ_20KHZ   (50*100)    // Timer setting for Fs =  20,000Hz
#define SAMPLE_FREQ_50KHZ   (20*100)    // Timer setting for Fs =  50,000Hz
#define SAMPLE_FREQ_100KHZ  (10*100)    // Timer setting for Fs = 100,000Hz

#define SAMPLE_FREQ_HZ 10000

#define DOPPLER_EVENT_LVL 50  		// Doppler Event Detection Threshold value was 20

#define LEFT_SPECTRUM   0
#define RIGHT_SPECTRUM  1
#define BOTH_SPECTRUMS  2
#define XCORR_EPSILON   0.1 // Epsilon used for floating comparison. If difference is less
                            //  than this value, the values are considered equal.

void do_rfft_i(uint16_t *pDataBuf) __attribute__((section(".ram_code")));
void do_rfft_q(uint16_t *pDataBuf) __attribute__((section(".ram_code")));
void genHanningData(void);
void checkTargetDetection(void);
uint8_t spectrum_peak(  // Determine which side of the spectrum from a cross-correlation
    float32_t* pSrcA,   //  contains the peak. Assumes lag is in range [-blockSize, blockSize]
    float32_t* pSrcB,   // Can return LEFT_SPECTRUM, RIGHT_SPECTRUM, or BOTH_SPECTRUMS
    uint32_t blockSize
    );
float32_t xcorr_left_max(   // Perform cross correlation but return just
    float32_t* pSrcA,       //  the peak value discovered for the left side.
    float32_t* pSrcB,
    uint32_t blockSize
    );
float32_t xcorr_right_max(  // Perform cross correlation but return just
    float32_t* pSrcA,       //  the peak value discovered for the right side.
    float32_t* pSrcB,
    uint32_t blockSize
    );
void max(float32_t* pSrc, uint32_t blockSize, float32_t* pMaxHolder);

uint8_t  gIFQDataReadyFlag = 0;

uint32_t gQ2Freq = 0;

float32_t gDopFreqIFI = 0;
float32_t gTargetVelocity = 0;

uint16_t gDetectionThreshold = DOPPLER_EVENT_LVL;

// uC/Probe variables
uint8_t gRedLED_On = 0;
BGT_Power_t gTxPowerLvl = TX_MAX;  // BGT24 Transmit Power Level Setting

float32_t gHzPerBin = (float32_t)SAMPLE_FREQ_HZ/(float32_t)FFT_SIZE;

uint16_t ifi_raw_buf0[NUM_SAMPLES*2];

uint16_t ifq_raw_buf0[NUM_SAMPLES*2];

float32_t ifi_adc_measurements[NUM_SAMPLES*2];  	// ADC data from BGT - IFI_HG
//float32_t ifq_adc_measurements[NUM_SAMPLES];  	// ADC data from BGT - IFQ_HG
float32_t* ifq_adc_measurements = &ifi_adc_measurements[NUM_SAMPLES];  	// ADC data from BGT - IFQ_HG

float32_t i_adc_measurements[NUM_SAMPLES], q_adc_measurements[NUM_SAMPLES];

float32_t hanning_window[FFT_SIZE];

float32_t i_fftResult[FFT_SIZE];      // FFT Result
float32_t q_fftResult[FFT_SIZE];      // FFT Result
float32_t finalResult[FFT_SIZE/2];

float32_t maxValPosSpectrum = 0, maxValNegSpectrum = 0; // Store the maximum values of the spectrums from cross-correlation
float32_t maxVal = 0;
uint32_t  maxBin = 0;

arm_rfft_fast_instance_f32 i_fftStructure;

uint32_t *pDMA_src_IFI;
uint32_t *pDMA_src_IFQ;

uint32_t *pDMA_dst_IFI = (uint32_t *)ifi_raw_buf0;
uint32_t *pDMA_dst_IFQ = (uint32_t *)ifq_raw_buf0;

uint16_t *pifi_raw_buf1 = &ifi_raw_buf0[NUM_SAMPLES];
uint16_t *pifq_raw_buf1 = &ifq_raw_buf0[NUM_SAMPLES];

//*********************************************************
// Set up digital pin to communicate information back to the main controller
//
XMC_GPIO_CONFIG_t PIN1_5_config  =
{
  .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
  .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE,
};

DIGITAL_IO_t PIN1_5 =
{
  .gpio_port = XMC_GPIO_PORT1,
  .gpio_pin = 5U,
  .gpio_config = &PIN1_5_config,
};

//*********************************************************
// Set up digital pin to communicate information back to the main controller
//	OUT1 pin on the demo board
/*
XMC_GPIO_CONFIG_t PIN0_1_config  =
{
  .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
  .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
  .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE,

};

DIGITAL_IO_t PIN0_1 =
{
  .gpio_port = XMC_GPIO_PORT0,
  .gpio_pin = 1U,
  .gpio_config = &PIN0_1_config,
};
*/
//*********************************************************
//
//
int main(void)
{
  DAVE_STATUS_t status;
  uint8_t firstBufDone = 0;

  pDMA_src_IFI = (uint32_t*)&ADC_MEASUREMENT_Channel_A.group_handle->RES[ADC_MEASUREMENT_Channel_A.ch_handle->result_reg_number];
  pDMA_src_IFQ = (uint32_t*)&ADC_MEASUREMENT_Channel_B.group_handle->RES[ADC_MEASUREMENT_Channel_B.ch_handle->result_reg_number];

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  status = DIGITAL_IO_Init(&PIN1_5);
//  status = DIGITAL_IO_Init(&PIN0_1);
  if(status == DAVE_STATUS_FAILURE)
  {
    while(1U)  // loop forever on error
    	;
  }


  DIGITAL_IO_SetOutputHigh(&LDO_BGT_EN);  // Enable power to BGT24

  CCU40->GCTRL |= (0x0001<<CCU4_GCTRL_SUSCFG_Pos);      // stop timers at breakpoint when debugging
  CCU41->GCTRL |= (0x0001<<CCU4_GCTRL_SUSCFG_Pos);

  arm_rfft_fast_init_f32(&i_fftStructure, FFT_SIZE);  // setup FFT

  genHanningData();   						// generate Hanning window data
  DMA_CH_Enable(&DMA_CH_IFI);
  DMA_CH_Enable(&DMA_CH_IFQ);

  TIMER_Stop(&FS_ADC);
  TIMER_SetTimeInterval(&FS_ADC, SAMPLE_FREQ_10KHZ);  	// set the sample frequency
  TIMER_Start(&FS_ADC);        							// start data acquisition
  BGTSetTxPower(gTxPowerLvl);    	// configure BGT24

  while(1U)
  {
	  if (gIFQDataReadyFlag == 0)
		  continue;

	  if (gIFQDataReadyFlag == 1 && firstBufDone == 0)
	  {
		  do_rfft_i(ifi_raw_buf0);   	// Process the ADC data from the BGT24 IFI_HG channel
		  do_rfft_q(ifq_raw_buf0);   	// Process the ADC data from the BGT24 IFQ_HG channel
		  TIMER_Start(&FS_ADC);
		  firstBufDone = 1;
		  checkTargetDetection();
	  }
	  else if (gIFQDataReadyFlag == 2)
	  {
		  do_rfft_i(pifi_raw_buf1);   	// Process the ADC data from the BGT24 IFI_HG channel
		  do_rfft_q(pifq_raw_buf1);   	// Process the ADC data from the BGT24 IFQ_HG channel
		  TIMER_Start(&FS_ADC);

		  gIFQDataReadyFlag = 0;
		  firstBufDone = 0;
		  checkTargetDetection();
	  }

  }  // bottom of while(1) loop
}   // end of main()


//*******************************************************************
//  Search IFI & IFQ frequency response for motion detection event
//  and determine target velocity in Mile Per Hour (MPH).
//  Toggle yellow LED on motion detection.
//
void checkTargetDetection(void)
{
	uint16_t idx;

	for (idx = 0; idx< FFT_SIZE/2; idx++)
	{
		finalResult[idx] = (i_fftResult[idx] + q_fftResult[idx])/2.0f;
	}
	/* Calculates maxValue and returns corresponding BIN value */
	arm_max_f32(finalResult, NUM_SAMPLES/2, &maxVal, &maxBin);

	if (maxVal > gDetectionThreshold )  // if the threshold is exceeded, we have a doppler event
	{
		gDopFreqIFI = maxBin*gHzPerBin + gHzPerBin/2.0f;	// compute the frequency of the max bin
		gTargetVelocity = gDopFreqIFI*(float32_t)0.013894f;  // convert freq to velocity in MPH

		DIGITAL_IO_SetOutputHigh(&PIN1_5);

		float32_t temp;
		for(idx = 0; idx < NUM_SAMPLES/2; ++idx){
			temp = i_fftResult[idx];
			i_fftResult[idx] = i_fftResult[NUM_SAMPLES - idx - 1];
			i_fftResult[NUM_SAMPLES - idx - 1] = temp;
		}

		/* Calculates direction and turns on appropriate LED depending on direction */
        switch(spectrum_peak(i_adc_measurements, q_adc_measurements, NUM_SAMPLES)){
            case LEFT_SPECTRUM: // Target is moving toward radar
                DIGITAL_IO_SetOutputHigh(&LED_YELLOW);
                DIGITAL_IO_SetOutputLow(&LED_GREEN);
                break;
            case RIGHT_SPECTRUM: // Target is moving away from radar
                DIGITAL_IO_SetOutputHigh(&LED_YELLOW);
                DIGITAL_IO_SetOutputLow(&LED_GREEN);
                break;
            case BOTH_SPECTRUMS: // Target is still relative to radar
                DIGITAL_IO_SetOutputHigh(&LED_YELLOW);
                DIGITAL_IO_SetOutputHigh(&LED_GREEN);
                break;
        }
/*
        float32_t iq_corrResult[FFT_SIZE*2];	// Cross-correlation result
//        float32_t* iq_corrResult = ifi_adc_measurements;	// Cross-correlation result
//		cross_correlate(i_fftResult, q_fftResult, iq_corrResult, NUM_SAMPLES, NUM_SAMPLES*2);
		cross_correlate(i_adc_measurements, q_adc_measurements, iq_corrResult, NUM_SAMPLES, NUM_SAMPLES*2);
		max(iq_corrResult, NUM_SAMPLES-2, &maxValPosSpectrum);
		max(iq_corrResult+NUM_SAMPLES+2, NUM_SAMPLES-2, &maxValNegSpectrum);

		if(maxValPosSpectrum > maxValNegSpectrum){
			DIGITAL_IO_SetOutputHigh(&LED_YELLOW);
			DIGITAL_IO_SetOutputLow(&LED_GREEN);
		} else {
			DIGITAL_IO_SetOutputHigh(&LED_GREEN);
			DIGITAL_IO_SetOutputLow(&LED_YELLOW);
		}
*/
	}
	else
	{
		gDopFreqIFI = 0;
		gTargetVelocity = 0;
		DIGITAL_IO_SetOutputLow(&PIN1_5);
		DIGITAL_IO_SetOutputHigh(&LED_YELLOW);
		DIGITAL_IO_SetOutputHigh(&LED_GREEN);
	}
}  // end of checkTargetDetection()


//*********************************************************
// DMA IFQ Interrupt Service routine (ISR)
// This ISR executes when a block of data has been transferred
// from the ADC to the DMA buffer (ifi_raw_buf0[] and ifq_raw_buf0.
// the DMA buffers are large enough to handle two blocks of
// data so we can overlap data acquisition with data processing
// operations.
//
void DMA_IFQ_event_handler(XMC_DMA_CH_EVENT_t event)
{
	static uint8_t cnt = 0;

	if (cnt == 0)
	{
		gIFQDataReadyFlag = 1; 	// Data is ready to be processed for first DMA block
		cnt++;
	}
	else
	{			// Second DMA block has been transferred.
		TIMER_Stop(&FS_ADC);	// stop ADC sampling
		cnt = 0;
		DMA_CH_Disable(&DMA_CH_IFI);	// Disable DMA
		DMA_CH_Disable(&DMA_CH_IFQ);

		DMA_CH_Reload(&DMA_CH_IFI);		// Reload DMA addresses
		DMA_CH_Reload(&DMA_CH_IFQ);

		DMA_CH_Enable(&DMA_CH_IFI);		// Restart DMA
		DMA_CH_Enable(&DMA_CH_IFQ);
		gIFQDataReadyFlag = 2;			// second DMA block is ready
	}

} // end of IFQ DMA ISR


//***********************************************
// One Hz Interrupt Service Routine
//
void OneHzISR(void)
{

//	DIGITAL_IO_ToggleOutput(&LED_GREEN);

	gQ2Freq = COUNTER_GetCurrentCount(&Q2_FREQ_P22);  // Read Q2 counter
	COUNTER_ResetCounter(&Q2_FREQ_P22);				  // clear Q2 counter

	BGT_LO_Adjust(gQ2Freq);			// make sure Q2 is in range

}  // end of OneHzISR()


//***************************************************************
//  Convert  IFI_HG sample data to frequency domain
//  Sample frequency determined by TIMER APP
//  FS_ADC.
//  The mean of the time domain data is then calculated and
//  removed from the sample buffer, to pull out the DC content.
//  The Hanning window is then applied to the time domain buffer.
//  This buffer then is converted to frequency domain with the FFT.
//
//
void do_rfft_i(uint16_t * pDataBuf)
{
	uint32_t idx =0;
	float32_t i_mean = 0;

//	float32_t ifi_adc_measurements[NUM_SAMPLES];  	// ADC data from BGT - IFI_HG

	for (idx=0; idx<NUM_SAMPLES; idx++)
		ifi_adc_measurements[idx] = (float32_t)pDataBuf[idx]*3.3f/4095.0f;

	arm_mean_f32(ifi_adc_measurements, NUM_SAMPLES, &i_mean); 						//Finds the mean
	arm_offset_f32(ifi_adc_measurements, -i_mean, i_adc_measurements, NUM_SAMPLES);  	//kills the offset

	arm_mult_f32(i_adc_measurements, hanning_window, i_adc_measurements, NUM_SAMPLES); 	//apply a hanning window

	arm_rfft_fast_f32(&i_fftStructure, i_adc_measurements, i_fftResult, INVERT_FFT); 	//performs fft

	arm_cmplx_mag_f32(i_fftResult, i_fftResult, NUM_SAMPLES);  // convert to real magnitude data

}  // end of do_rfft_i()


//***************************************************************
//  Convert  IFQ_HG sample data to frequency domain
//  Sample frequency determined by TIMER APP
//  FS_ADC.
//  The mean of the time domain data is then calculated and
//  removed from the sample buffer, to pull out the DC content.
//  The Hanning window is then applied to the time domain buffer.
//  This buffer then is converted to frequency domain with the FFT.
//
//
void do_rfft_q(uint16_t * pDataBuf)
{
	uint32_t idx =0;
	float32_t i_mean = 0;   // mean

//	float32_t ifq_adc_measurements[NUM_SAMPLES];	// ADC data from BGT - IFQ_HG

	for (idx=0; idx<NUM_SAMPLES; idx++)
		ifq_adc_measurements[idx] = (float32_t)pDataBuf[idx]*3.3f/4095.0f;

	arm_mean_f32(ifq_adc_measurements, NUM_SAMPLES, &i_mean); //Finds the mean
	arm_offset_f32(ifq_adc_measurements, -i_mean, q_adc_measurements, NUM_SAMPLES);  //kills the offset

	arm_mult_f32(q_adc_measurements, hanning_window, q_adc_measurements, NUM_SAMPLES); //apply a hanning window

	arm_rfft_fast_f32(&i_fftStructure, q_adc_measurements, q_fftResult, INVERT_FFT); //performs fft

	arm_cmplx_mag_f32(q_fftResult, q_fftResult, NUM_SAMPLES);  // convert to real magnitude data

}  // end of do_rfft_q()


//***********************************************
//   Generate Hanning Window data for FFT
//   Signal Processing.  Hanning window helps
//   smooth the edges of the sample window.
//
void genHanningData(void)
{
	uint16_t idx;
	float32_t  piVal = (float)22.0/(float)7.0;

	for (idx=0; idx<FFT_SIZE; idx++)
	{
		hanning_window[idx] = (1.0f - arm_cos_f32(2.0*piVal*idx/FFT_SIZE))/2.0f;
	}

}  // end of genHanningData()


//***************************************************************
//  Perform the cross correlation of the target signal and the
//	sampled signal. Return which side of the spectrum the peak
//  value was found. This is to save memory by avoiding having
//  to store all the calculations.
//
uint8_t spectrum_peak(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize
    )
{
    float32_t
            left_peak = xcorr_left_max(pSrcA, pSrcB, blockSize),
            right_peak = xcorr_right_max(pSrcA, pSrcB, blockSize)
            ;
    if(fabs(left_peak - right_peak) < XCORR_EPSILON)
    {
        return BOTH_SPECTRUMS;
    }
    return left_peak > right_peak;
}

float32_t xcorr_left_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize
    )
{
    uint32_t i = 0, j = 0;
    float32_t toreturn = 0.0;
    float32_t calc_hold = 0.0;

        // Slide srcA left-to-right starting from no overlap
    for(i=0; i < blockSize; ++i){
        calc_hold = 0;
        for(j=0; j+i < blockSize; ++j){
            calc_hold += pSrcA[j+i] * pSrcB[j];
        }
        if(i == 0 || toreturn < calc_hold)
        {
            toreturn = calc_hold;
        }
    }
    return toreturn;
}

float32_t xcorr_right_max(
    float32_t* pSrcA,
    float32_t* pSrcB,
    uint32_t blockSize
    )
{
    uint32_t i = 0, j = 0;
    float32_t toreturn = 0.0;
    float32_t calc_hold = 0.0;

        // Slide srcA left-to-right starting from complete overlap
    for(i=0; i < blockSize; ++i){
        calc_hold = 0;
        for(j=0; j+i < blockSize; ++j){
            calc_hold += pSrcA[j] * pSrcB[j + i];
        }
        if(i == 0 || toreturn < calc_hold)
        {
            toreturn = calc_hold;
        }
    }
    return toreturn;
}

void max(float32_t* pSrc, uint32_t blockSize, float32_t* pMaxHolder){
	*pMaxHolder = pSrc[0];
	uint32_t idx = 1;
	for(; idx < blockSize; ++idx){
		if(*pMaxHolder < pSrc[idx]){
			*pMaxHolder = pSrc[idx];
		}
	}
}

