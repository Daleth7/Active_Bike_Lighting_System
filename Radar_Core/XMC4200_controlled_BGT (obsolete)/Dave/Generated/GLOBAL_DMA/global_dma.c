
/**
 * @file global_dma.c
 * @date 2015-06-20
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * GLOBAL_DMA v4.0.4 - Initializes the DMA module, sets channel priorities and reserves DMA interrupt node
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
 *     - Initial version <br>
 *
 * 2015-04-22:
 *     - GLOBAL_DMA_IsInitialized() is now a static function <br>
 *
 * @endcond
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

#include "global_dma.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * API IMPLEMENTATION
 **********************************************************************************************************************/

/* Retrieve the version of the GLOBAL_DMA APP */
DAVE_APP_VERSION_t GLOBAL_DMA_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = GLOBAL_DMA_MAJOR_VERSION;
  version.minor = GLOBAL_DMA_MINOR_VERSION;
  version.patch = GLOBAL_DMA_PATCH_VERSION;

  return version;
}

/* GLOBAL_DMA initialization function */
GLOBAL_DMA_STATUS_t GLOBAL_DMA_Init(GLOBAL_DMA_t *const obj)
{
  XMC_ASSERT("DMA_GLOBAL_Init: NULL DMA_GLOBAL_t object", (obj != NULL));

  if (obj->initialized == false)
  {
    /* Enable DMA module */
    XMC_DMA_Init(obj->dma);

    /* Initialize CPU_CTRL */
    CPU_CTRL_XMC4_Init(CPU_CTRL_HANDLE);

    /* Enable DMA event handling */
    NVIC_SetPriority(obj->irq_node, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                                                        obj->config->priority,
                                                        obj->config->sub_priority));
    NVIC_EnableIRQ(obj->irq_node);
    obj->initialized = true;
  }

  return GLOBAL_DMA_STATUS_SUCCESS;
}
