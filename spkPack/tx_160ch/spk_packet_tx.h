#include "ap_int.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 160
#define T  32
#define N  CH*T
#define spklen 19
#define prelen 7

typedef ap_uint<192> ap_bundle; // {16bits, 16bits, 32bits, 127bits}: {{4'b0, ch_last_bufo}, {4'b0, ch_bufo}, frame_No_bufo, multi_channel_muao}
typedef ap_uint<128> ap_data;   // 32bit * 4 = 128bits for tetrode
typedef ap_uint<8>  ch_type;    // up to 2^8=256 channels

//struct mua_struct{
//	int     user;         // t_in
//	ch_type id;           // channel
//	ch_type dest;         // last channel in ch_group
//	ap_data data;         // tetrode data
//};

struct mua_struct{
	ap_bundle data;         // tetrode data
};

struct spk_struct{
	ap_uint<8>   user;
	ap_uint<16>    id;     // (ch_grpNo, ch)
	bool         last;
	ap_data      data;
	ap_uint<1>   dest;
};

enum txState {IDLE=0, PRE, POST};

void spk_packet_tx(hls::stream<mua_struct> &mua_stream,
				   hls::stream<spk_struct> &out_pre,
				   hls::stream<spk_struct> &out_post,
				   hls::stream<int> &time_stamp);
