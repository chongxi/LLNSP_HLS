#include "rt_feedback.h"
#include <iostream>

void rt_feedback(ap32_data fet_packet_spk_id[dim],
			     hls::stream<ap32_data> &spk_id_out);
