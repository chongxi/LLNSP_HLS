#include "ap_int.h"
#include "hls_math.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 40

#define ch_span 4
#define spklen 19  // 19 wave pts
#define pca_dim 4

#define scale_size 1
//#define shift_size pca_dim
#define pca_size spklen*ch_span

#define scale_depth CH
#define shift_depth CH*pca_dim
#define pca_depth CH*pca_size

// type *_type for fixed point arithmetics:
typedef ap_fixed<32,19> io_type;     // 19 bits for the integer part, 13 bits for the fractional part; 13+19 = 32; binpoint: 13
typedef ap_fixed<32,13> param_type;  // 13 bits for the integer part, 19 bits for the fractional part; 13+19 = 32; binpoint: 19
typedef ap_fixed<8,1> pca_type;   // 4 pca dimensions, each one use 7 bits for the fractional part, the first bit is sign bit; binpoint: 7

// type *_data for data transmission at bit level, therefore unsigned is used:
typedef ap_uint<32> ap32_data;    // use uint type to receives and send bits but use ap_fixed<32,19> and ap_fixed<8,1> for internal computing
typedef ap_uint<128> ap128_data;  // info+data

void spk_transform(ap128_data   spk[spklen+1],
				   ap32_data  scale[scale_depth],
				   ap32_data  shift[shift_depth],
				   ap32_data    pca[pca_depth],
				   hls::stream<ap32_data> &pca_final);
