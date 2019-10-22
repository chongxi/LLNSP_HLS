#include "rt_feedback.h"

void read_packet(ap32_data fet_packet_spk_id[dim], ap_uint<16> target_unit_id, bool condition[1])
{
#pragma HLS PIPELINE

    int time;
    int fet;
    int unit_id; // first two items in the packet

	read_packet_in:
	for (int i=0; i<dim; i++) {
    	if(i==0)
    		time = fet_packet_spk_id[i].range(31,0);
        if(i>0 && i<dim-1)
            fet  = fet_packet_spk_id[i].range(31,0);
    	if(i==dim-1)
    		unit_id   = fet_packet_spk_id[i].range(31,0);
	}

	fprintf(stdout, "%d\n", unit_id);

	feedback_rule:
	if (unit_id==target_unit_id && target_unit_id!=0)    condition[0]=1;
	else                                                 condition[0]=0;
}

void feedback(bool condition[1], ap_uint<1> *stim)
{
    feedback_out: // 1ms stimulation per targeted spike
    if (condition[0]) {
        for (int i=0; i<pulse_width; i++)
        	*stim = 1;
    }
}


void rt_feedback(ap_uint<16> target_unit_id,
				 ap32_data fet_packet_spk_id[dim],
				 ap_uint<1> *stim)
{
//#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_fifo port=fet_packet_spk_id
#pragma HLS INTERFACE ap_vld port=stim

	bool condition[1];

	read_packet(fet_packet_spk_id, target_unit_id, condition);

	fprintf(stdout, "condition=%d\n\n", condition[0]);

    feedback(condition, stim);

}
