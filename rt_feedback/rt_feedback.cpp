#include "rt_feedback.h"

void rt_feedback(ap32_data fet_packet_spk_id[dim],
                 hls::stream<ap32_data> &spk_id_out)
{
#pragma HLS INTERFACE ap_fifo port=fet_packet_spk_id
#pragma HLS INTERFACE axis port=spk_id_out

    int time;
    int id; // first two items in the packet

    time = 0;
    id   = -1;

	read_packet_in:
	for (int i=0; i<dim; i++) {
#pragma HLS PIPELINE
		if(i==0)
			time = fet_packet_spk_id[i].range(31,0);
		if(i==6)
			id   = fet_packet_spk_id[i].range(31,0);
	}

    feedback_rule_out: // 1ms stimulation per targeted spike
    if (id == 888) {
        for (int j=0; j<pulse_width; j++)
            spk_id_out.write(id);
    }
}
