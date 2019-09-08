#include "ap_int.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define grpNo 40

#define info_dim 2
#define pca_dim 4
#define data_size info_dim+pca_dim
#define vq_size 500
#define vq_depth grpNo*vq_size

typedef ap_fixed<32,16> ap_type;
typedef ap_fixed<8,1> vq_type;
typedef ap_uint<32> ap32_data;
typedef ap_uint<64> spk_info_type;

void spk_clf(ap_type   data[data_size],
		     ap32_data vq[vq_depth],
             ap32_data label[vq_depth],
			 hls::stream<ap32_data> &distance_out,
			 hls::stream<ap32_data> &nnid_out);
