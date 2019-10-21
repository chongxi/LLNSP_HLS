#include <ap_int.h>


#define grpNo     40        // Number of sub-space
#define chNo      4         // channels per space
#define pca_len   4         // 3D features pca dimensions
#define spk_len   19*chNo   // 19 pts * channels
#define vq_len    500
#define label_len 500

// ------------------ Don't change below ------------------- //
// --------------------------------------------------------- //

#define scale_depth  grpNo
#define scale_base_addr  0

#define shift_depth  grpNo*pca_len
#define shift_base_addr  (scale_base_addr+scale_depth)

#define pca_depth  grpNo*spk_len
#define pca_base_addr  (shift_base_addr+shift_depth)

#define vq_depth  grpNo*vq_len
#define vq_base_addr  (pca_base_addr+pca_depth)

#define label_depth  grpNo*label_len
#define label_base_addr  (vq_base_addr+label_depth)

void bram_xike(bool _doWrite, int  din,  ap_uint<16> _addr_in,
               bool _doRead,  int* dout, ap_uint<16> _addr,
               bool _scale_read, ap_uint<8 > _scale_addr, int * scale_out,
               bool _shift_read, ap_uint<8 > _shift_addr, int * shift_out,
               bool _pca_read  , ap_uint<16> _pca_addr  , int * pca_out,
               bool _vq_read   , ap_uint<16> _vq_addr   , int * vq_out,
               bool _label_read, ap_uint<16> _label_addr, int * label_out);
