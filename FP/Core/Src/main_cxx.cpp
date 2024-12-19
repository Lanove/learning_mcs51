/*
 * main_cxx.cpp
 *
 *  Created on: Dec 18, 2024
 *      Author: kohigashi
 */
#include "data_bindings.hpp"
#include "i2c.h"
#include "lcd_controller.h"
#include "main.h"
#include "si5351.h"
#include "tim.h"
#include <Button.h>
#include <LCD.h>
#include <math.h>
#include "adc.h"

Button encoder_button(GPIOA, GPIO_PIN_5);
Si5351 dds(&hi2c1);
Si5315_Channel_Frequency channel_freq;
uint8_t rise_ctr;
uint8_t down_ctr;
uint32_t encoder_counter;

// Global variables
int16_t cev, lev;
int16_t digit_cursor = 0;
uint32_t adc_buffer;
float supply_voltage;


typedef enum {
	ENCODER_IDLE, ENCODER_UP, ENCODER_DOWN
} EncoderState;

EncoderState encoder_state = ENCODER_IDLE;

void check_encoder_direction(TIM_HandleTypeDef *htim) {
	// Read current encoder value
	cev = (int16_t) __HAL_TIM_GET_COUNTER(htim);

	// Compare with the last value
	if (cev > lev) {
		encoder_state = ENCODER_DOWN;
	} else if (cev < lev) {
		encoder_state = ENCODER_UP;
	} else {
		encoder_state = ENCODER_IDLE;
	}

	// Update the last value
	lev = cev;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_5) {
		encoder_button.handleInterrupt();
	}
}

int16_t encoder_value;

void bound_value(uint64_t &value, uint64_t min, uint64_t max) {
	if (value < min) {
		value = min;
	} else if (value > max) {
		value = max;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	// Nilai terkalibrasi dari multimeter
	supply_voltage = float(adc_buffer) * 0.001639047619047619;
}

void main_cxx() {
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(&htim11);
	HAL_ADC_Start_DMA(&hadc1, &adc_buffer, 1);
	HAL_Delay(100);
	channel_freq.ch1 = 8000;
	channel_freq.ch2 = 8000;
	channel_freq.ch3 = 8000;
	lcd.init();
	lcd_controller.begin();

	if (!dds.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0)) {
		while (1)
			;
	}
	dds.set_freq(channel_freq.ch1 * 100, SI5351_CLK0);
	dds.set_freq(channel_freq.ch2 * 100, SI5351_CLK1);
	dds.set_freq(channel_freq.ch3 * 100, SI5351_CLK2);
	while (1) {
		static Si5315_Channel_Frequency last_channel_freq;
		static Button::ButtonState last_button_state;
		check_encoder_direction(&htim3);
		if (encoder_state == ENCODER_UP) {
			if (!lcd_controller.get_arrow_blink()) {
				encoder_counter++;
				lcd_controller.increment_arrow_cursor();
			} else {
				if (lcd_controller.get_arrow_cursor() == 0) {
					int extracted_digit = (channel_freq.ch1
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 9) {
						channel_freq.ch1 -= extracted_digit
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch1 += pow(10,
								lcd_controller.get_digit_col());
					}
				}
				if (lcd_controller.get_arrow_cursor() == 1) {
					int extracted_digit = (channel_freq.ch2
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 9) {
						channel_freq.ch2 -= extracted_digit
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch2 += pow(10,
								lcd_controller.get_digit_col());
					}
				}
				if (lcd_controller.get_arrow_cursor() == 2) {
					int extracted_digit = (channel_freq.ch3
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 9) {
						channel_freq.ch3 -= extracted_digit
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch3 += pow(10,
								lcd_controller.get_digit_col());
					}
				}
				bound_value(channel_freq.ch1, 8000, 999999999);
				bound_value(channel_freq.ch2, 8000, 999999999);
				bound_value(channel_freq.ch3, 8000, 999999999);
			}
		} else if (encoder_state == ENCODER_DOWN) {
			if (!lcd_controller.get_arrow_blink()) {
				encoder_counter--;
				lcd_controller.decrement_arrow_cursor();
			} else {
				if (lcd_controller.get_arrow_cursor() == 0) {

					int extracted_digit = (channel_freq.ch1
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 0) {
						channel_freq.ch1 += 9
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch1 -= pow(10,
								lcd_controller.get_digit_col());
					}
				}
				if (lcd_controller.get_arrow_cursor() == 1) {

					int extracted_digit = (channel_freq.ch2
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 0) {
						channel_freq.ch2 += 9
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch2 -= pow(10,
								lcd_controller.get_digit_col());
					}
				}
				if (lcd_controller.get_arrow_cursor() == 2) {

					int extracted_digit = (channel_freq.ch3
							/ int(pow(10, lcd_controller.get_digit_col())))
							% 10;

					if (extracted_digit == 0) {
						channel_freq.ch3 += 9
								* pow(10, lcd_controller.get_digit_col());
					} else {
						channel_freq.ch3 -= pow(10,
								lcd_controller.get_digit_col());
					}
				}
				bound_value(channel_freq.ch1, 8000, 999999999);
				bound_value(channel_freq.ch2, 8000, 999999999);
				bound_value(channel_freq.ch3, 8000, 999999999);
			}
		}

		Button::ButtonState button_state = encoder_button.getState();
		if (button_state != last_button_state && button_state == Button::HOLD) {
			lcd_controller.set_arrow_blink(!lcd_controller.get_arrow_blink());
			lcd_controller.set_digit_col(0);
			lcd_controller.set_digit_row(lcd_controller.get_arrow_cursor());
		} else if (button_state != last_button_state
				&& button_state == Button::PRESSED) {
			lcd_controller.increment_digit_cursor();
			if (lcd_controller.get_arrow_blink())
				lcd_controller.force_refresh_row(
						lcd_controller.get_digit_row());
		} else if (button_state != last_button_state
				&& button_state == Button::DOUBLE_CLICK) {
			if (lcd_controller.get_arrow_cursor() == 0)
				channel_freq.ch1 = 8000;
			if (lcd_controller.get_arrow_cursor() == 1)
				channel_freq.ch2 = 8000;
			if (lcd_controller.get_arrow_cursor() == 2)
				channel_freq.ch3 = 8000;
		}

		if (last_channel_freq.ch1 != channel_freq.ch1) {
			dds.set_freq(channel_freq.ch1 * 100, SI5351_CLK0);
			last_channel_freq.ch1 = channel_freq.ch1;
		} else if (last_channel_freq.ch2 != channel_freq.ch2) {
			dds.set_freq(channel_freq.ch2 * 100, SI5351_CLK1);
			last_channel_freq.ch2 = channel_freq.ch2;
		} else if (last_channel_freq.ch3 != channel_freq.ch3) {
			dds.set_freq(channel_freq.ch3 * 100, SI5351_CLK2);
			last_channel_freq.ch3 = channel_freq.ch3;
		}

		lcd_controller.service();
		encoder_button.loop();

		last_button_state = button_state;
	}
}
