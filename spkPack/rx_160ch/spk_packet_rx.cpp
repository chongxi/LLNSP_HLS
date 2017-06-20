#include "spk_packet_rx.h"

void spk_packet_rx(hls::stream<spk_struct> &pre_in, hls::stream<spk_struct> &post_in,
		           hls::stream<int> &time_stamp,
				   hls::stream<spk_stream> &spk_out_stream)
{
#pragma HLS INTERFACE axis register port=spk_out_stream
#pragma HLS INTERFACE axis register port=post_in
#pragma HLS INTERFACE axis register port=pre_in
#pragma HLS INTERFACE axis register port=time_stamp

	ch_type ch;
	static ap_data spk[CH][spklen];
#pragma HLS RESOURCE variable=spk core=RAM_S2P_BRAM

	spk_struct pre, post;  // input
	spk_stream spk_info;   // output spk_info
	spk_stream out;        // output

	if(!pre_in.empty()){
		pre = pre_in.read();
		ch  = pre.id;
		spk[ch][pre.user]  = pre.data;
	}

	if(!post_in.empty()){
		post = post_in.read();
		ch   = post.id;
		spk[ch][post.user] = post.data;
		if(post.last){
			int frameNo = time_stamp.read();
			write_loop:
			for(int j=0; j<=spklen; j++){
			#pragma HLS PIPELINE rewind
				if(j==0){
					out.id   = ch;
					out.user = frameNo;
					out.dest = j;
					out.data.range(31,0 ) = ch;
					out.data.range(63,32) = frameNo;
					spk_out_stream.write(out);
				}
				else {
					out.id   = ch;
					out.user = frameNo;
					out.dest = j;
					out.data = spk[ch][j-1];
					spk_out_stream.write(out);
				}
			}
		}
	}
}
