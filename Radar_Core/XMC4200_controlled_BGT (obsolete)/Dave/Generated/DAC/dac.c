/**
 * @file dac.c
 * @date 2015-06-20
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 * @cond
 ***********************************************************************************************************************
 * DAC v4.0.6 - Generates Pattern, Noise and Ramp waveform. It can be used for single value and data processing modes.
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
 * 2015-02-20:
 *     - Initial version
 *
 * 2015-06-20:
 *     - Single value mode service request option is given only when synchronize with clock option is selected.
 *
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "dac.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/
DAC_STATUS_t DAC_Ramp_ScaleUp(const DAC_t * const HandlePtr,
uint16_t start_ramp,uint16_t stop_ramp,uint32_t frequency_Hz);
DAC_STATUS_t DAC_Ramp_ScaleDown(const DAC_t * const HandlePtr,
uint16_t start_ramp,uint16_t stop_ramp,uint32_t clk_divider);
void DAC_Ramp_StartStopConfigure(const DAC_t * const HandlePtr,uint16_t start_ramp,uint16_t stop_ramp);
/* This API will configure DAC mode depends upon the user configuration */
XMC_DAC_CH_STATUS_t DAC_ModeConfig(const DAC_t * const HandlePtr);

/**********************************************************************************************************************
* API IMPLEMENTATION
**********************************************************************************************************************/
/*
 * This API will set Ramp frequency by reducing wave points by scale up operation.
*/
DAC_STATUS_t DAC_Ramp_ScaleUp(const DAC_t * const HandlePtr,
uint16_t start_ramp,uint16_t stop_ramp,uint32_t frequency_Hz)
{
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  uint8_t shift_count = 0U;
  uint16_t temp_steps = (uint16_t)((stop_ramp - start_ramp) + 1U);
  uint32_t peripheral_clock_freq_Hz = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
  uint32_t temp_cal_freq = frequency_Hz * temp_steps;
  uint32_t clk_divider;
  XMC_DAC_CH_OUTPUT_SCALE_t scale = XMC_DAC_CH_OUTPUT_SCALE_NONE;

  while((temp_cal_freq > DAC_MAX_FREQUENCY) && (status != DAC_STATUS_API_FAIL))
  {
    if( DAC_RAMP_MIN_STEPS > temp_steps )
    {
      /* Frequency can't be configured */
      status = DAC_STATUS_API_FAIL;
    }
    else
    {
      if(shift_count < DAC_MAX_SCALE_SHIFT )
      {
        /* Decrease scaling factor and re-calculate start and stop ramp */
        stop_ramp = stop_ramp >> 1U;
        start_ramp = start_ramp >> 1U;
        shift_count++;

        /* re-calculate clock divider based on change in steps */
        temp_steps = (uint16_t)((stop_ramp - start_ramp) + 1U);
        if(stop_ramp == start_ramp)
        {
          status = DAC_STATUS_API_FAIL;
        }
        else
        {
          clk_divider = (uint32_t)(peripheral_clock_freq_Hz /
                                  (temp_steps * frequency_Hz));
          if(clk_divider != 0U)
          {
            temp_cal_freq = (uint32_t)(peripheral_clock_freq_Hz / clk_divider);
          }
          else
          {
            temp_cal_freq = peripheral_clock_freq_Hz;
          }
          if(temp_cal_freq < 1U)
          {
            status = DAC_STATUS_API_FAIL;
          }
        }
      }
      else
      {
        /* Frequency Can't be configurable */
        status = DAC_STATUS_API_FAIL;
      }
    }
  }

  if(DAC_STATUS_API_FAIL != status)
  {
    DAC_Ramp_StartStopConfigure(HandlePtr,start_ramp,stop_ramp);
    if(shift_count > 0U)
    {
      /* Scaling required */
      scale = (XMC_DAC_CH_OUTPUT_SCALE_t)((uint32_t)XMC_DAC_CH_OUTPUT_SCALE_MUL_2 +
              (shift_count - (uint32_t)1));
      XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr, HandlePtr->dac_ch,scale);
    }
    status = (DAC_STATUS_t)XMC_DAC_CH_SetFrequency(HandlePtr->dac_module_ptr,
              HandlePtr->dac_ch,temp_cal_freq);
  }
  return status;
}

/*
 * This API will set Ramp frequency by increasing wave points by scale down operation.
*/
DAC_STATUS_t DAC_Ramp_ScaleDown(const DAC_t * const HandlePtr,
uint16_t start_ramp,uint16_t stop_ramp,uint32_t frequency_Hz)
{
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  uint8_t shift_count = 0U;
  uint16_t temp_steps = (uint16_t)((stop_ramp - start_ramp) + 1U);
  uint32_t peripheral_clock_freq_Hz = XMC_SCU_CLOCK_GetPeripheralClockFrequency();
  uint32_t clk_divider = (uint32_t)(peripheral_clock_freq_Hz / (temp_steps * frequency_Hz));
  uint32_t temp_cal_freq = frequency_Hz * temp_steps;
  XMC_DAC_CH_OUTPUT_SCALE_t scale = XMC_DAC_CH_OUTPUT_SCALE_NONE;

  while((clk_divider > DAC_MAX_CLK_DIV) && (status != DAC_STATUS_API_FAIL))
  {
    if( DAC_RAMP_MAX_STEPS < temp_steps )
    {
      /* Frequency can't be configured */
      status = DAC_STATUS_API_FAIL;
    }
    else
    {
      if(shift_count < DAC_MAX_SCALE_SHIFT )
      {
        /* Increase scaling factor and re-calculate start and stop ramp */
        stop_ramp = (uint16_t)(stop_ramp << 1U);
        start_ramp = (uint16_t)(start_ramp << 1U);
        temp_steps = (uint16_t)((stop_ramp - start_ramp) + 1U);
        if((stop_ramp <= DAC_RAMP_MAX_STEPS) && (start_ramp <= DAC_RAMP_MAX_STEPS) &&
                                                            (stop_ramp != start_ramp))
        {
          shift_count++;
          /* re-calculate clock divider based on change in steps */
          clk_divider = (uint32_t)(peripheral_clock_freq_Hz / (temp_steps * frequency_Hz));
          if(clk_divider == 0U)
          {
            clk_divider = 1U;
          }
          temp_cal_freq = (uint32_t)(peripheral_clock_freq_Hz / clk_divider);
          if(temp_cal_freq < 1U)
          {
            status = DAC_STATUS_API_FAIL;
          }
        }
        else
        {
          status = DAC_STATUS_API_FAIL;
        }
      }
      else
      {
        /* Frequency Can't be configurable */
        status = DAC_STATUS_API_FAIL;
      }
    }
  }
    
  if(DAC_STATUS_API_FAIL != status)
  {
    DAC_Ramp_StartStopConfigure(HandlePtr,start_ramp,stop_ramp);
    if(shift_count > 0U)
    {
      /* Scaling required */
      scale = (XMC_DAC_CH_OUTPUT_SCALE_t)((uint32_t)XMC_DAC_CH_OUTPUT_SCALE_DIV_2 +
                                                        (shift_count - (uint32_t)1));
      XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr, HandlePtr->dac_ch,scale);
    }
    status = (DAC_STATUS_t)XMC_DAC_CH_SetFrequency(HandlePtr->dac_module_ptr,
                                                   HandlePtr->dac_ch,temp_cal_freq);
  }
  return status;
}

/* This API will configure ramp start and stop values */
void DAC_Ramp_StartStopConfigure(const DAC_t * const HandlePtr,uint16_t start_ramp,uint16_t stop_ramp)
{
  uint16_t temp_start_ramp;
  if(HandlePtr->dac_ramp_start_mv > HandlePtr->dac_ramp_stop_mv)
  {
    /* Shift start/stop for negative ramp */
    temp_start_ramp = start_ramp;
    start_ramp = DAC_RAMP_MAX_STEPS - stop_ramp;
    stop_ramp  = DAC_RAMP_MAX_STEPS - temp_start_ramp;
    XMC_DAC_CH_EnableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
  }
  else
  {
    XMC_DAC_CH_DisableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
  }
  XMC_DAC_CH_SetRampStart(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, start_ramp);
  XMC_DAC_CH_SetRampStop(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, stop_ramp);
}

/* This API will configure DAC mode depends upon the user configuration */
XMC_DAC_CH_STATUS_t DAC_ModeConfig(const DAC_t * const HandlePtr)
{
  XMC_DAC_CH_STATUS_t api_status = XMC_DAC_CH_STATUS_OK;
  /* Initialize DAC configuration created by the user */
  if ( XMC_DAC_CH_MODE_PATTERN == HandlePtr->dac_mode )
  {
    api_status = XMC_DAC_CH_StartPatternMode(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,
    HandlePtr->dac_pat_table_ptr,HandlePtr->dac_sign_output,
    HandlePtr->dac_input_trigger,HandlePtr->dac_frequency);
  }
  else if ( XMC_DAC_CH_MODE_SINGLE == HandlePtr->dac_mode )
  {
    api_status = XMC_DAC_CH_StartSingleValueMode(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
    /*Initialized data register with zero */
    XMC_DAC_CH_Write(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,0U);
  }
  else if ( XMC_DAC_CH_MODE_DATA ==  HandlePtr->dac_mode )
  {
    api_status = XMC_DAC_CH_StartDataMode(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,
    HandlePtr->dac_input_trigger,HandlePtr->dac_frequency);
    if(1U == HandlePtr->dac_service_request)
    {
      /* Enable Service Request */
      XMC_DAC_CH_EnableEvent(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
    }

    /*Initialized data register with zero */
    XMC_DAC_CH_Write(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,0U);
  }
  else if ( XMC_DAC_CH_MODE_RAMP == HandlePtr->dac_mode )
  {
    api_status = XMC_DAC_CH_StartRampMode(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,
    HandlePtr->dac_ramp_start,HandlePtr->dac_ramp_stop,
    HandlePtr->dac_input_trigger,HandlePtr->dac_frequency);

    XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,
                       (XMC_DAC_CH_OUTPUT_SCALE_t)HandlePtr->dac_config_ptr->output_scale);
  }
  else if ( XMC_DAC_CH_MODE_NOISE == HandlePtr->dac_mode )
  {
    api_status = XMC_DAC_CH_StartNoiseMode(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,
    HandlePtr->dac_input_trigger,HandlePtr->dac_frequency);
  }
  else
  {
    /* DAC Mode Idle */
    api_status = XMC_DAC_CH_STATUS_OK;
  }
  return api_status;
}

/* App API to retrieve the App version information */
DAVE_APP_VERSION_t DAC_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = DAC_MAJOR_VERSION;
  version.minor = DAC_MINOR_VERSION;
  version.patch = DAC_PATCH_VERSION;

  return version;
}

/**
    * This function initializes DAC
    * peripheral as per the user configurations
*/

DAC_STATUS_t DAC_Init(const DAC_t * const HandlePtr)
{
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  XMC_DAC_CH_STATUS_t api_status;

  /*Assert variables.*/
  XMC_ASSERT(" DAC_Init: Passing argument DAC HandlePtr of type DAC_t is Null",
             (HandlePtr != (DAC_t *)NULL));

  /* Enable DAC module */
  XMC_DAC_CH_Init(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,HandlePtr->dac_config_ptr);

  /* Configure DAC with user configured mode */
  api_status = DAC_ModeConfig(HandlePtr);

  /* Check if start after initialization is enabled */
  if(HandlePtr->dac_start_after_init == 0U)
  {
    XMC_DAC_CH_SetMode(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, XMC_DAC_CH_MODE_IDLE);
  }

  if(1U == HandlePtr->dac_coupled)
  {
    /* Enable Simultaneous Data Mode */
    XMC_DAC_EnableSimultaneousDataMode(HandlePtr->dac_module_ptr);
  }

  if(api_status != XMC_DAC_CH_STATUS_OK)
  {
    status = DAC_STATUS_FAILURE;
  }

  return status;
}

/*
  * This API will set the amplitude in Pattern Mode.
*/
void DAC_Pattern_SetAmplitude(const DAC_t * const HandlePtr,DAC_VSS_t Amplitude)
{
  XMC_DAC_CH_OUTPUT_SCALE_t scale;

  if(Amplitude > DAC_VSS_0034_mV)
  {
    /* If amplitude greater or equal to 34mV then scale Up */
    scale = (XMC_DAC_CH_OUTPUT_SCALE_t)((uint32_t)XMC_DAC_CH_OUTPUT_SCALE_MUL_2 +
                               ((uint32_t)Amplitude - (uint32_t)DAC_VSS_0069_mV));
  }
  else if(Amplitude < DAC_VSS_0034_mV)
  {
    /* If amplitude less than 34mV then scale down */
    scale = (XMC_DAC_CH_OUTPUT_SCALE_t)((uint32_t)XMC_DAC_CH_OUTPUT_SCALE_DIV_2 +
                              ((uint32_t)DAC_VSS_0017_mV - (uint32_t)Amplitude));
  }
  else
  {
    /* No scaling required */
    scale = XMC_DAC_CH_OUTPUT_SCALE_NONE;
  }

  XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr,HandlePtr->dac_ch, scale);
}

/*
  * This API will set offset in Pattern Mode.
*/
DAC_STATUS_t DAC_Pattern_SetOffset(const DAC_t * const HandlePtr,int16_t Dc_offset_mv)
{
  DAC_STATUS_t status;
  uint8_t u_offset;
  int16_t i_offset;
  uint16_t amplitude_in_mV;
  uint32_t scale;

  scale = (uint32_t)XMC_DAC_CH_GetOutputScale(HandlePtr->dac_module_ptr, HandlePtr->dac_ch);
  if(scale & DAC_DAC0CFG1_MULDIV_Msk)
  {
    amplitude_in_mV = (uint16_t)((uint16_t)DAC_VSS_34_MV << (scale & DAC_DAC0CFG1_SCALE_Msk));
  }
  else
  {
    amplitude_in_mV = (uint16_t)((uint16_t)DAC_VSS_34_MV >> (scale & DAC_DAC0CFG1_SCALE_Msk));
  }

  i_offset = (Dc_offset_mv - (int16_t)DAC_OFFSET_SIGN_mV);
  i_offset = (int16_t)((i_offset * DAC_PATTERN_AMPLITUDE)/ (int16_t)amplitude_in_mV);

  if((i_offset > DAC_OFFSET_MAX_SIGN) || (i_offset < DAC_OFFSET_MIN_SIGN))
  {
    /* Offset not configurable */
    status = DAC_STATUS_API_FAIL;
  }
  else
  {
    if(HandlePtr->dac_config_ptr->output_negation)
    {
      /* compensation of inversion*/
      i_offset = -i_offset;
    }
    
    /* Convert int32_t to uint8_t */
    if(i_offset < 0)
    {
      i_offset = (0x100 + i_offset);
      u_offset = (uint8_t)(i_offset);
    }
    else
    {
      u_offset = (uint8_t)((uint8_t)(i_offset) & (uint8_t)0x0FF);
    }

    XMC_DAC_CH_SetOutputOffset(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,u_offset);
    status = DAC_STATUS_SUCCESS;
  }
  return status;
}

/*
  * This API will set data value in Single Value Signed Mode.
*/
DAC_STATUS_t DAC_SingleValue_SetValue_s16(const DAC_t * const HandlePtr,int16_t Data)
{
  DAC_STATUS_t status;
  int16_t temp_data = 0;
  if((DAC_DATA_VALUE_MAX_SIGN >= Data) && (DAC_DATA_VALUE_MIN_SIGN < Data))
  {
    if(Data < 0)
    {
      temp_data = (0x1000 + Data);
    }
    else
    {
      temp_data = Data;
    }

    /* Update Corresponding CH Data register */
    XMC_DAC_CH_Write(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,(uint16_t)temp_data);
    status = DAC_STATUS_SUCCESS;
  }
  else
  {
    /* Data Values are out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
  * This API will set data value in Single Value Signed Mode when couple mode is selected.
*/
DAC_STATUS_t DAC_SingleValue_SetValue_s16_Coupled(const DAC_t * const HandlePtr,int16_t Data0, int16_t Data1)
{
  DAC_STATUS_t status;
  int16_t temp_data0,temp_data1 = 0;
  if((DAC_DATA_VALUE_MAX_SIGN >= Data0) && (DAC_DATA_VALUE_MAX_SIGN >= Data1) &&
     (DAC_DATA_VALUE_MIN_SIGN < Data0) && (DAC_DATA_VALUE_MIN_SIGN < Data1))
  {
    if(Data0 < 0)
    {
      temp_data0 = (0x1000 + Data0);
    }
    else
    {
      temp_data0 = Data0;
    }

    if(Data1 < 0)
    {
      temp_data1 = (0x1000 + Data1);
    }
    else
    {
      temp_data1 = Data1;
    }
    XMC_DAC_SimultaneousWrite(HandlePtr->dac_module_ptr,(uint16_t)temp_data0,(uint16_t)temp_data1);
    status = DAC_STATUS_SUCCESS;
  }
  else
  {
    /* Data Values are out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
  * This API will set data value in Single Value in Unsigned Mode.
*/
DAC_STATUS_t DAC_SingleValue_SetValue_u16(const DAC_t * const HandlePtr,uint16_t Data)
{
  DAC_STATUS_t status;
  if(DAC_DATA_VALUE_MAX_USIGN >= Data)
  {
    /* Update Corresponding CH Data register */
    XMC_DAC_CH_Write(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,Data);
    status = DAC_STATUS_SUCCESS;
  }
  else
  {
    /* Data Values are out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
  * This API will set data value in Single Value in Unsigned Mode when couple mode is selected.
*/
DAC_STATUS_t DAC_SingleValue_SetValue_u16_Coupled(const DAC_t * const HandlePtr,uint16_t Data0, uint16_t Data1)
{
  DAC_STATUS_t status;
  if((DAC_DATA_VALUE_MAX_USIGN >= Data0) && (DAC_DATA_VALUE_MAX_USIGN >= Data1))
  {
    /* Coupled Mode Enabled */
    XMC_DAC_SimultaneousWrite(HandlePtr->dac_module_ptr,Data0,Data1);
    status = DAC_STATUS_SUCCESS;
  }
  else
  {
    /* Data Values are out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
  * This API will set Ramp Start in Ramp Mode.
*/
DAC_STATUS_t DAC_Ramp_SetStart(DAC_t * const HandlePtr,uint16_t ramp_start_mv)
{
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  uint16_t start_ramp,stop_ramp,temp;

  if((DAC_OUT_MAX_MV >= ramp_start_mv) && (DAC_OUT_MIN_MV <= ramp_start_mv))
  {
    #if (UC_SERIES != XMC44)
      if(ramp_start_mv >= HandlePtr->dac_ramp_stop_mv)
      {
        /* Negative Ramp is not possible for other than XMC44 device */
        status = DAC_STATUS_API_FAIL;
      }
    if( status == DAC_STATUS_SUCCESS )
    {
    #endif
      if(ramp_start_mv > HandlePtr->dac_ramp_stop_mv)
      {
        start_ramp = (uint16_t)(((HandlePtr->dac_ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
        stop_ramp = (uint16_t)(((ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      }
      else
      {
        start_ramp = (uint16_t)(((ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
        stop_ramp = (uint16_t)(((HandlePtr->dac_ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      }

      HandlePtr->dac_ramp_start_mv = ramp_start_mv;
      if(HandlePtr->dac_ramp_start_mv > HandlePtr->dac_ramp_stop_mv)
      {
        /* Shift start/stop for negative ramp */
        temp = start_ramp;
        start_ramp = DAC_RAMP_MAX_STEPS - stop_ramp;
        stop_ramp  = DAC_RAMP_MAX_STEPS - temp;
        XMC_DAC_CH_SetRampStop(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, stop_ramp);
        XMC_DAC_CH_SetRampStart(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, start_ramp);
        XMC_DAC_CH_EnableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
      }
      else
      {
        XMC_DAC_CH_DisableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
        XMC_DAC_CH_SetRampStop(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, stop_ramp);
        XMC_DAC_CH_SetRampStart(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, start_ramp);
      }
    #if(UC_SERIES != XMC44)
    }
    #endif
  }
  else
  {
    /* Ramp start Value is out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
  * This API will set Ramp Stop in Ramp Mode.
*/
DAC_STATUS_t DAC_Ramp_SetStop(DAC_t * const HandlePtr,uint16_t ramp_stop_mv)
{
  uint16_t start_ramp,stop_ramp,temp;
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;

  if((DAC_OUT_MAX_MV >= ramp_stop_mv)&&(DAC_OUT_MIN_MV <= ramp_stop_mv))
  {
    #if (UC_SERIES != XMC44)
      if(ramp_stop_mv <= HandlePtr->dac_ramp_start_mv)
      {
        /* Negative Ramp is not possible for other than XMC44 device */
        status = DAC_STATUS_API_FAIL;
      }
    if( status == DAC_STATUS_SUCCESS )
    {
    #endif
      if(HandlePtr->dac_ramp_start_mv > ramp_stop_mv)
      {
        stop_ramp = (uint16_t)(((HandlePtr->dac_ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
        start_ramp = (uint16_t)(((ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      }
      else
      {
        stop_ramp = (uint16_t)(((ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
        start_ramp = (uint16_t)(((HandlePtr->dac_ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      }

      HandlePtr->dac_ramp_stop_mv = ramp_stop_mv;
      if(HandlePtr->dac_ramp_start_mv > HandlePtr->dac_ramp_stop_mv)
      {
        /* Shift start/stop for negative ramp */
        temp = start_ramp;
        start_ramp = DAC_RAMP_MAX_STEPS - stop_ramp;
        stop_ramp  = DAC_RAMP_MAX_STEPS - temp;
        XMC_DAC_CH_SetRampStart(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, start_ramp);
        XMC_DAC_CH_SetRampStop(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, stop_ramp);
        XMC_DAC_CH_EnableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
      }
      else
      {
        XMC_DAC_CH_DisableOutputNegation(HandlePtr->dac_module_ptr,HandlePtr->dac_ch);
        XMC_DAC_CH_SetRampStart(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, start_ramp);
        XMC_DAC_CH_SetRampStop(HandlePtr->dac_module_ptr, HandlePtr->dac_ch, stop_ramp);
      }
    #if(UC_SERIES != XMC44)
    }
    #endif
  }
  else
  {
    /* Ramp start Value is out of range */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}

/*
 * This API will set Ramp frequency in Ramp Mode.
*/
DAC_STATUS_t DAC_Ramp_SetFrequency(const DAC_t * const HandlePtr,uint32_t Frequency_Hz)
{
  uint16_t start_ramp;
  uint16_t stop_ramp;
  uint32_t clk_divider = 0U;
  uint16_t temp_steps;
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  uint32_t temp_freq;

  if((HandlePtr->dac_input_trigger == XMC_DAC_CH_TRIGGER_INTERNAL) && (Frequency_Hz != 0U))
  {
    if(HandlePtr->dac_ramp_start_mv > HandlePtr->dac_ramp_stop_mv)
    {
      start_ramp = (uint16_t)(((HandlePtr->dac_ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      stop_ramp  = (uint16_t)(((HandlePtr->dac_ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
    }
    else
    {
      start_ramp = (uint16_t)(((HandlePtr->dac_ramp_start_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
      stop_ramp  = (uint16_t)(((HandlePtr->dac_ramp_stop_mv - DAC_OUT_MIN_MV) * DAC_RAMP_MAX_STEPS) / DAC_VSS_MAX);
    }

    temp_steps = (uint16_t)((stop_ramp - start_ramp) + 1U);

    if(stop_ramp == start_ramp)
    {
      status = DAC_STATUS_API_FAIL;
    }
    else
    {
      clk_divider = (uint32_t)(XMC_SCU_CLOCK_GetPeripheralClockFrequency() / (temp_steps * Frequency_Hz));
      if(clk_divider == 0U)
      {
        clk_divider = 1U;
      }
      temp_freq = (uint32_t)(XMC_SCU_CLOCK_GetPeripheralClockFrequency()/clk_divider);

      if(clk_divider <= DAC_MAX_CLK_DIV)
      {
        if( temp_freq > DAC_MAX_FREQUENCY)
        {
          /* Up-scaling is required to configure required frequency */
          status = DAC_Ramp_ScaleUp(HandlePtr,start_ramp,stop_ramp,Frequency_Hz);
        }
        else
        {
          /* No need of scaling */
          XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,XMC_DAC_CH_OUTPUT_SCALE_NONE);
          status = (DAC_STATUS_t)XMC_DAC_CH_SetFrequency(HandlePtr->dac_module_ptr,HandlePtr->dac_ch,temp_freq);
        }
      }
      else
      {
        /* Down-scaling is required to configure required frequency */
        status = DAC_Ramp_ScaleDown(HandlePtr,start_ramp,stop_ramp,Frequency_Hz);
      }
    }
  }
  else
  {
    status = DAC_STATUS_API_FAIL;
  }
  return status;
 }

/*
  * This API will set the amplitude in Noise Mode.
*/
DAC_STATUS_t DAC_Noise_SetAmplitude(const DAC_t * const HandlePtr,DAC_VSS_t Amplitude)
{
  DAC_STATUS_t status = DAC_STATUS_SUCCESS;
  XMC_DAC_CH_OUTPUT_SCALE_t scale;

  /* Check if Amplitude is configurable */
  if(DAC_VSS_0017_mV <= Amplitude)
  {
    if(Amplitude == DAC_VSS_2200_mV)
    {
      /* Scale not required */
      scale = XMC_DAC_CH_OUTPUT_SCALE_NONE;
    }
    else
    {
      scale = (XMC_DAC_CH_OUTPUT_SCALE_t)((uint32_t)XMC_DAC_CH_OUTPUT_SCALE_DIV_128 +
                                   ((uint32_t)DAC_VSS_0017_mV - (uint32_t)Amplitude));
    }
    XMC_DAC_CH_SetOutputScale(HandlePtr->dac_module_ptr,HandlePtr->dac_ch, scale);
  }
  else
  {
    /* Amplitude can't be configurable */
    status = DAC_STATUS_API_FAIL;
  }
  return status;
}
/**
  * @}
*/
/**
  * @endcond
*/
/*CODE_BLOCK_END*/