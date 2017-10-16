//////////////////////////////////////////////////////////////////////////////////
// Company:  HHMI Janelia
// Engineer: Chongxi Lai
// 
// Create Date: 06/28/2017 12:18:22 PM
// Design Name: spkDect
// Module Name: spkDect
// Project Name: Xike
// Target Devices: KC705
// Tool Versions: Vivado HLS 2016.1
// Description: HLS version of spike detector for 160 channels
//              mua -> spkDect -> muap
//
// Dependencies: 
// 
// Revision: 
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
// 
//    s0                               s0
//      s0                    s0    s0    s0
//        s0               s0    s0          s0          s0
//   --------------------------------------------------------- threshold
//          s1          s2                      s1    s2
//                    s2                           s1    
//            s1    s2                                      
//                s2
//              s1
// 
//////////////////////////////////////////////////////////////////////////////////

#include "ap_int.h"
#include "ap_utils.h"
#include "ap_fixed.h"
#include <hls_stream.h>

#define CH 160  // change to 8 for C simulation
#define T  2
#define N  CH*T
#define CH_PG 4
#define INACTIVE_LEN 4 // time length that disable any peak once you get one

#define chs_per_chip    32
#define number_of_chips 5

typedef ap_fixed<32,19> mua_type;
typedef ap_uint<64>     data_type;     // (31,0): mua   (63,32): thr
typedef ap_uint<8>      ch_type;       // ch
typedef ap_uint<32>     ch_hash_type;  // (ch_nn3, ch_nn2, ch_nn1, ch_nn0) 

//struct mua_struct{
//  ap_uint<32>     user;     // t:                                 | 32 bits
//  ch_type         id;       // ch:                                | 8  bits
//  data_type       data;     // (thr, mua):              | 64 bits
//  ch_hash_type    dest;     // (ch_nn3, ch_nn2, ch_nn1, ch_nn0)   | 32 bits
//};

struct mua_struct{
  ap_uint<160>    data;
};

struct muap_struct{
  ap_uint<32>     user;     // t                  | 32 bits
  ch_type         id;       // ch                 | 8  bits
  mua_type        data;     // muap                 | 32 bits
  ch_hash_type    dest;   // (ch_nn3, ch_nn2, ch_nn1, ch_nn0)   | 32 bits
};

enum State {S0=0, S1, S2};

void spk_dect(hls::stream<mua_struct>  &mua_stream,
              hls::stream<muap_struct> &muap_stream);
