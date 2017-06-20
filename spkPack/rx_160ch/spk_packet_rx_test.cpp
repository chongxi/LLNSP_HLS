#include "spk_packet_rx.h"
#include <iostream>

using namespace std;


int main()
{
	int t, ch1, ch2, i;
	hls::stream<spk_struct> pre_stream, post_stream;
	hls::stream<spk_stream> spk_out_stream;
	hls::stream<int> time_stamp;

	spk_struct spk_pre_1, spk_post_1;
	spk_struct spk_pre_2, spk_post_2;


	// input   //////////////////////////////////////////////////////////////////////////////////
	ch1=29; ch2 = 16;
	for(i=0;i<8;i++){
		// spk_1
		spk_pre_1.data  = i;
		spk_pre_1.user  = i;
		spk_pre_1.last  = 0;
		spk_pre_1.id    = ch1;
		pre_stream.write(spk_pre_1);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);
		spk_post_1.data = i+prelen;
		spk_post_1.user = i+prelen;
		spk_post_1.last = 0;
		spk_post_1.id   = ch1;
		post_stream.write(spk_post_1);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);

		// spk_2
		spk_pre_2.data  = i;
		spk_pre_2.user  = i;
		spk_pre_2.last  = 0;
		spk_pre_2.id    = ch2;
		pre_stream.write(spk_pre_2);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);
		spk_post_2.data = i+prelen;
		spk_post_2.user = i+prelen;
		spk_post_2.last = 0;
		spk_post_2.id    = ch2;
		post_stream.write(spk_post_2);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);
	}

	for(i=16;i<spklen;i++){
		// spk_1
		spk_post_1.data = i;
		spk_post_1.user = i;
		spk_post_1.last = 0;
		spk_post_1.id   = ch1;
		if(i==spklen-1) {
			spk_post_1.last = 1;
			time_stamp.write(70);
		}
		post_stream.write(spk_post_1);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);
		spk_post_1.last = 0;

		// spk_2
		spk_post_2.data = i;
		spk_post_2.user = i;
		spk_post_2.last = 0;
		spk_post_2.id   = ch2;
		if(i==spklen-1) {
			spk_post_2.last = 1;
			time_stamp.write(78);
		}
		post_stream.write(spk_post_2);
		spk_packet_rx(pre_stream, post_stream, time_stamp, spk_out_stream);
		spk_post_2.last = 0;
	}


	// output  /////////////////////////////////////////////////////////////////////////////////
	int ch, frameNo, n;
	ap_data data;
	spk_stream result;
	while(!spk_out_stream.empty()){
		result = spk_out_stream.read();
		ch = result.id;
		frameNo = result.user;
		n = result.dest;
		data = result.data;
		cout << n << '\t' << frameNo << '\t' << ch << '\t' << data << endl;
	}

	return 0;
}
