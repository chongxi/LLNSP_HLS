#include "ap_int.h"
#include "ap_utils.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 160  // change to 8 for C simulation
#define T  2

typedef ap_fixed<32,19>  mua_type;
typedef ap_uint<12>      ch_type;       // ch
typedef ap_uint<32>      ch_hash_type;  // (ch_nn3, ch_nn2, ch_nn1, ch_nn0) 
typedef ap_uint<32>      time_type;

// 160 bits are consists of time, ch_ref, ch, mua, thr etc...
struct mua_struct{
  ap_uint<160> data;
};

// 160 bits are consists of time, ch_ref, ch, mua, thr etc...
struct muar_struct{
  ap_uint<160> data;
};

void ref_sub(hls::stream<mua_struct>  &mua_stream,
             hls::stream<muar_struct> &muar_stream);
