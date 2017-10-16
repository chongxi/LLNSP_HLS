#include "ap_int.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 40

#define ch_span 4
#define spklen 19  // 19 wave pts
#define pca_dim 4

#define scale_size 1
//#define shift_size pca_dim
#define pca_size spklen*ch_span
#define vq_size 100

#define scale_depth CH
#define shift_depth CH*pca_dim
#define pca_depth CH*pca_size
#define vq_depth CH*vq_size

typedef ap_uint<64> spk_info_type;
typedef ap_uint<1> valid_type;
typedef ap_fixed<32,16> ap_type;  // 16 bits float
typedef ap_fixed<8,1> pca_type;
typedef ap_uint<32> ap32_data;
typedef ap_uint<96> ap96_data;

typedef ap_uint<128> ap_data;  // info+data
typedef ap_uint<8> ch_type;

struct spk_stream{
	ap_data     data;
};

void spk_transform(ap_data   spk[spklen+1],
				   ap32_data scale[scale_depth],
				   ap32_data shift[shift_depth],
				   ap32_data pca[pca_depth],
				   hls::stream<ap32_data> &pca_stream,
				   hls::stream<ap32_data> &pca_final);
