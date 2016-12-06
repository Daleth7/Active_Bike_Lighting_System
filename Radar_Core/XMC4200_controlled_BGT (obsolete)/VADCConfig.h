/*
 * VADCConfig.h
 *
 *  Created on: Sep 1, 2015
 *      Author: nardone
 *
 *  28-SEP	RSN Changed for S2Go2 board.
 *  		P14.4 is IFI_HG assigned to ADC G0_CH4
 *  		P14.3 is IFQ_HG assigned to ADC G1_CH3
 */

#ifndef VADCCONFIG_H_
#define VADCCONFIG_H_

void ADC_G0_ISR(void);
void ADC_G1_ISR(void);

  // Assign IFI_HG  ADC G0 Ch 4
  // Port P14.4 is IFI_HG
  ADC_QUEUE_ENTRY_t ch_entry_G0_0 =
  {
    .queue_position        = 0U,
    .channel_number        = 4,
    .trigger_needed        = true,
    .refill_needed         = true,
    .ch_event_notification = false,
    .rs_event_notification = true,
    .adc_event_callback    = (ADC_G0_ISR)
  };


  // Assign IFQ_HG ADC G1 Ch 3
  // Port P14.3 is IFQ_HG
  ADC_QUEUE_ENTRY_t ch_entry_G1_0 =
  {
    .queue_position        = 0U,
    .channel_number        = 3,
    .trigger_needed        = true,
    .refill_needed         = true,
    .ch_event_notification = false,
    .rs_event_notification = true,
    .adc_event_callback    = NULL
  };

  static XMC_VADC_CHANNEL_CONFIG_t  ch_handle =
  {
    .input_class                = (uint32_t) XMC_VADC_CHANNEL_CONV_GROUP_CLASS1,  // Group ICLASS 1 selected
    .lower_boundary_select      = (uint32_t) XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
    .upper_boundary_select      = (uint32_t) XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
    .event_gen_criteria         = (uint32_t) XMC_VADC_CHANNEL_EVGEN_NEVER, //Channel Event disabled
    .sync_conversion            = (uint32_t) 0,                            // Sync feature disabled
    .alternate_reference        = (uint32_t) XMC_VADC_CHANNEL_REF_INTREF,  // Internal reference selected
    .result_reg_number          = (uint32_t) 1,                            // GxRES[1] selected
    .result_alignment           = (uint32_t) XMC_VADC_RESULT_ALIGN_RIGHT,  // Result alignment - Right Aligned
    .broken_wire_detect_channel = (uint32_t) XMC_VADC_CHANNEL_BWDCH_VAGND, // No Broken wire mode select
    .broken_wire_detect         = (uint32_t) 0,                          // No Broken wire detection
    .bfl                        = (uint32_t) 0,                            //No Boundary flag
    .channel_priority           = (uint32_t) 0,  // Lowest Priority-0 selected; channel can also be converted
                                                 // by background request source
    .alias_channel              = (int8_t) XMC_VADC_CHANNEL_ALIAS_DISABLED // Alias feature not selected
  };

  XMC_VADC_RESULT_CONFIG_t res_handle =
  {
    .data_reduction_control  = (uint8_t)  0,  // No Accumulation
    .post_processing_mode    = (uint32_t) XMC_VADC_DMM_REDUCTION_MODE,
    .wait_for_read_mode      = (uint32_t) 0,  // Result registers are updated as and when the conversion is finished
    .part_of_fifo            = (uint32_t) 0,  // FIFO has been disabled
    .event_gen_enable        = (uint32_t) 0   // result event generation disabled
  };









#endif /* VADCCONFIG_H_ */
