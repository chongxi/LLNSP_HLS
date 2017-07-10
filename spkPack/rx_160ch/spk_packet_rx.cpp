#include "spk_packet_rx.h"

void spk_packet_rx(hls::stream<spk_struct> &pre_in,
				   hls::stream<spk_struct> &post_in,
		           hls::stream<int> &time_stamp,
				   hls::stream<spk_stream> &spk_out_stream)
{
#pragma HLS INTERFACE axis register port=spk_out_stream
#pragma HLS INTERFACE axis register port=post_in
#pragma HLS INTERFACE axis register port=pre_in
#pragma HLS INTERFACE axis register port=time_stamp

	ch_type ch;
	static ap_data spk_A[CH][spklen];
//#pragma HLS ARRAY_PARTITION variable=spk_A cyclic factor=2 dim=0
//#pragma HLS RESOURCE variable=spk_A core=RAM_2P_LUTRAM
	static ap_data spk_B[CH][spklen];
//#pragma HLS ARRAY_PARTITION variable=spk_B cyclic factor=2 dim=0
//#pragma HLS RESOURCE variable=spk_B core=RAM_2P_LUTRAM

	spk_struct pre, post;  // input
	spk_stream spk_info;   // output spk_info
	spk_stream out;        // output

	if(!pre_in.empty()){
		pre = pre_in.read();
		ch  = pre.id;
		if(pre.dest==0)	spk_A[ch][pre.user]  = pre.data;
		else            spk_B[ch][pre.user]  = pre.data;
	}

	if(!post_in.empty()){
		post = post_in.read();
		ch   = post.id;
		if(post.dest==0) spk_A[ch][post.user] = post.data;
		else			 spk_B[ch][post.user] = post.data;

		if(post.last){
			int t = time_stamp.read() - 1; // move to pivotal pos which is previous
			if (t > prelen){
				if(post.dest == 0) {
					// write info (t, ch)
	//				out.id   = ch;
	//				out.user = t;
					out.data.range(31,0 )  = ch;
					out.data.range(63,32)  = t;
					out.data.range(128,64) = 0;
					spk_out_stream.write(out);

					write_loop_spk_A:
					for(int j=0; j<spklen; j++){
					#pragma HLS PIPELINE rewind
						// write wave
	//					out.id   = ch;
	//					out.user = t;
						out.data = spk_A[ch][j];
						spk_out_stream.write(out);

					}
				}

				else {
					// write info (t, ch)
	//				out.id   = ch;
	//				out.user = t;
					out.data.range(31,0 )  = ch;
					out.data.range(63,32)  = t;
					out.data.range(128,64) = 0;
					spk_out_stream.write(out);

					write_loop_spk_B:
					for(int j=0; j<spklen; j++){
					#pragma HLS PIPELINE rewind
						// write wave
	//					out.id   = ch;
	//					out.user = t;
						out.data = spk_B[ch][j];
						spk_out_stream.write(out);
					}
				}
			}
		}
	}
}
