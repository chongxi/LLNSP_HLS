#include <ap_int.h>


// parameter for transformation, convert spikes to 3D points //
// Note change spaceNo for more than 32 channels

#define spaceNo  40        // Number of sub-space
#define chNo     4         // channels per space
#define pca_len  4         // 3D features pca dimensions
#define spk_len  19*chNo   // 19 pts * channels
#define vq_len 100

//#define quantization_factor 3

// ------------------ Don't change below ------------------- //
// --------------------------------------------------------- //

#define scale_depth  spaceNo
#define scale_base_addr  0

#define shift_depth  spaceNo*pca_len
#define shift_base_addr  (scale_base_addr+scale_depth)

#define pca_depth  spaceNo*spk_len
#define pca_base_addr  (shift_base_addr+shift_depth)

#define vq_depth  spaceNo*vq_len
#define vq_base_addr  (pca_base_addr+pca_depth)

void bram_xike(bool _doWrite, int  din,
		       bool _doRead,  int* dout, ap_uint<16> _addr,
		       bool _scale_read, ap_uint<8 > _scale_addr, int *scale_out,
		       bool _shift_read, ap_uint<8 > _shift_addr, int *shift_out,
			   bool _pca_read  , ap_uint<16> _pca_addr  , int * pca_out,
			   bool _vq_read   , ap_uint<16> _vq_addr   , int * vq_out);
