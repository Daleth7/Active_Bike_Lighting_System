/*
 * sensor2go_library.h
 *
 *  Created on: Apr 13, 2016
 *      Author: Abler
 */

#ifndef RADARSENSE2GO_LIBRARY_H_
#define RADARSENSE2GO_LIBRARY_H_

typedef enum {
	XMC_RADARSENSE2GO_DISABLED = 0,
	XMC_RADARSENSE2GO_ENABLED = 1
} XMC_RADARSENSE2GO_ENABLE_t;

typedef enum {
	XMC_RADARSENSE2GO_OK = 0,
	XMC_RADARSENSE2GO_NOT_INITIALIZED = 1
} XMC_RADARSENSE2GO_ERROR_t;

typedef enum {
	XMC_MOTION_DETECT_APPROACHING = 0,
	XMC_MOTION_DETECT_DEPARTING = 1,
	XMC_NO_MOTION_DETECT = 2
} XMC_RADARSENSE2GO_MOTION_t;

typedef struct {
	uint32_t t_sample_us;
	uint32_t t_cycle_ms;
	uint8_t N_exponent_samples;
} XMC_RADARSENSE2GO_TIMING_t;

typedef struct {
	uint8_t  hold_on_cycles;
	uint32_t trigger_det_level;
	XMC_RADARSENSE2GO_ENABLE_t rootcalc_enable;
} XMC_RADARSENSE2GO_ALG_t;

typedef struct {
	XMC_RADARSENSE2GO_ENABLE_t sleep_deepsleep_enable;
	XMC_RADARSENSE2GO_ENABLE_t mainexec_enable;
	XMC_RADARSENSE2GO_ENABLE_t vadc_clock_gating_enable;
} XMC_RADARSENSE2GO_POWERDOWN_t;

typedef void (*radarsense2go_result_cb_t)(uint32_t *fft_magnitude_array,
                                          uint16_t size_of_array_mag,
										  int16_t *adc_array_I,
										  int16_t *adc_array_Q,
										  uint16_t size_of_array_adc,
										  XMC_RADARSENSE2GO_MOTION_t motion_detected,
										  uint32_t max_frq_mag,
										  uint32_t max_frq_index);
typedef void (*radarsense2go_startacq_cb_t)(void);
typedef void (*radarsense2go_endacq_cb_t)(void);
typedef void (*radarsense2go_trigger_cb_t)(XMC_RADARSENSE2GO_MOTION_t detection_state);


void radarsense2go_init(XMC_RADARSENSE2GO_TIMING_t    init_timing,
						 XMC_RADARSENSE2GO_ALG_t       init_algorithm,
						 XMC_RADARSENSE2GO_POWERDOWN_t init_powerdown,
						TIMER_t *timer);

XMC_RADARSENSE2GO_ERROR_t radarsense2go_start(void);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_stop(void);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_set_detection_threshold(uint32_t threshold);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_exitmain(void);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_regcb_result(radarsense2go_result_cb_t fctptr);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_regcb_startacq(radarsense2go_startacq_cb_t fctptr);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_regcb_endacq(radarsense2go_endacq_cb_t fctptr);
XMC_RADARSENSE2GO_ERROR_t radarsense2go_regcb_trigger(radarsense2go_trigger_cb_t fctptr);

#endif /* RADARSENSE2GO_LIBRARY_H_ */
