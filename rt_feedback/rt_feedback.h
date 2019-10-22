#include "ap_int.h"
#include "ap_fixed.h"
#include <fstream>
#include <iostream>

#define dim 7
#define pulse_width 25000 //1ms = 250000 * 4ns
typedef ap_uint<32> ap32_data;

//void read_packet(ap32_data fet_packet_spk_id[dim], bool condition);
//void feedback(bool condition, ap_uint<1> stim[pulse_width])

void rt_feedback(ap_uint<16> target_unit_id,
		         ap32_data fet_packet_spk_id[dim],
			     ap_uint<1> *stim);
