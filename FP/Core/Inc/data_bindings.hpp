/*
 * data_bindings.hpp
 *
 *  Created on: Dec 18, 2024
 *      Author: kohigashi
 */

#ifndef INC_DATA_BINDINGS_HPP_
#define INC_DATA_BINDINGS_HPP_
#include <stdint.h>

struct Si5315_Channel_Frequency{
	uint64_t ch1;
	uint64_t ch2;
	uint64_t ch3;
};

extern struct Si5315_Channel_Frequency channel_freq;
extern float supply_voltage;

#endif /* INC_DATA_BINDINGS_HPP_ */
