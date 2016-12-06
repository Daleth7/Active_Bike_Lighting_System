
/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2015, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2014-06-16:
 *     - Initial version<br>
 *
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "DAVE.h"

/***********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/*******************************************************************************
 * @brief This function initializes the APPs Init Functions.
 * 
 * @param[in]  None
 *
 * @return  DAVE_STATUS_t <BR>
 ******************************************************************************/
DAVE_STATUS_t DAVE_Init(void)
{
  DAVE_STATUS_t init_status;
  
  init_status = DAVE_STATUS_SUCCESS;

      

/** @Initialization of APPs Init Functions */


  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DIGITAL_IO APP instance LED_RED */
    init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LED_RED);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DIGITAL_IO APP instance LED_YELLOW */
    init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LED_YELLOW);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DIGITAL_IO APP instance LED_GREEN */
    init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LED_GREEN);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of TIMER APP instance ONE_HZ_TIMER */
    init_status = (DAVE_STATUS_t)TIMER_Init(&ONE_HZ_TIMER);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of INTERRUPT APP instance ONE_HZ_INT */
    init_status = (DAVE_STATUS_t)INTERRUPT_Init(&ONE_HZ_INT);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DIGITAL_IO APP instance LDO_BGT_EN */
    init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&LDO_BGT_EN);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DAC APP instance VCOARSE */
    init_status = (DAVE_STATUS_t)DAC_Init(&VCOARSE);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DAC APP instance VFINE */
    init_status = (DAVE_STATUS_t)DAC_Init(&VFINE);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of SPI_MASTER APP instance SPI_BGT */
    init_status = (DAVE_STATUS_t)SPI_MASTER_Init(&SPI_BGT);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of TIMER APP instance FS_ADC */
    init_status = (DAVE_STATUS_t)TIMER_Init(&FS_ADC);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DIGITAL_IO APP instance Q2ONP22 */
    init_status = (DAVE_STATUS_t)DIGITAL_IO_Init(&Q2ONP22);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of COUNTER APP instance Q2_FREQ_P22 */
    init_status = (DAVE_STATUS_t)COUNTER_Init(&Q2_FREQ_P22);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DMA_CH APP instance DMA_CH_IFQ */
    init_status = (DAVE_STATUS_t)DMA_CH_Init(&DMA_CH_IFQ);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of ADC_MEASUREMENT APP instance ADC_IN */
    init_status = (DAVE_STATUS_t)ADC_MEASUREMENT_Init(&ADC_IN);
  }  

  if (init_status == DAVE_STATUS_SUCCESS)
  {
    /**  Initialization of DMA_CH APP instance DMA_CH_IFI */
    init_status = (DAVE_STATUS_t)DMA_CH_Init(&DMA_CH_IFI);
  }  
 

  return init_status;
} /**  End of function DAVE_Init */

