#include "ap_int.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define dim 7
#define pulse_width 250000 //1ms = 250000 * 4ns
typedef ap_uint<32> ap32_data;


void rt_feedback(ap32_data fet_packet_spk_id[dim],
			     hls::stream<ap32_data> &spk_id_out);
