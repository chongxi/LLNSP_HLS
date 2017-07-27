#include "ap_int.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 160
#define spklen 19
#define prelen 7
#define spkNo  2

typedef ap_uint<128> ap_data;  // tetrode 32*4 = 128
typedef ap_uint<12> ch_type;

struct spk_struct{
	ap_uint<5>  user;       // pos
	bool        last;       // last
	ch_type     id;
	ap_data     data;  // data
	ap_uint<1>  dest;
};

struct spk_stream{
//	ch_type     id;
//	int         user;
	ap_data     data;
//	ap_uint<16>  dest;
};

enum rxState {IDLE=0, PRE, POST};

void spk_packet_rx(hls::stream<spk_struct> &pre_in,
		 	 	   hls::stream<spk_struct> &post_in,
		           hls::stream<int> &time_stamp,
				   hls::stream<spk_stream> &spk_out_stream);
