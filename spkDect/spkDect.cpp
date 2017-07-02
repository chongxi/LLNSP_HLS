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

#include "spkDect.h"
//#include <iostream>
//using namespace std;

void spk_dect(hls::stream<mua_struct>  &mua_stream,
              hls::stream<muap_struct> &muap_stream)
{

    static mua_type Mn[CH] = {0};
    static ap_uint<1> is_peak[CH]  = {0};
    static ap_uint<1> first_S2[CH] = {0};

    static mua_type buf_2d[T][CH];
    static State state[CH] = {S0};

// ------------------------------------------------
// input: mua_stream (t, ch, ch_hash, mua_data)
// ------------------------------------------------
    mua_struct  mua;
    mua_type    val, thr;
    ap_uint<32> t;
    ch_type     ch, ch_nn0, ch_nn1, ch_nn2, ch_nn3;
    bool if_lowest;

    mua = mua_stream.read();
    val.range(31,0)  = mua.data.range(31,  0);
    thr.range(31,0)  = mua.data.range(63, 32);
    ch_nn0 = (ch_type) mua.data.range(71, 64);
    ch_nn1 = (ch_type) mua.data.range(79, 72);
    ch_nn2 = (ch_type) mua.data.range(87, 80);
    ch_nn3 = (ch_type) mua.data.range(95, 88);
    ch     = (ch_type) mua.data.range(107,96);
    t                = mua.data.range(159,128);
//    t                = mua.user             ;
//    ch     = (ch_type) mua.id               ;
//    ch_nn0 = (ch_type) mua.dest.range( 7, 0);
//    ch_nn1 = (ch_type) mua.dest.range(15, 8);
//    ch_nn2 = (ch_type) mua.dest.range(23,16);
//    ch_nn3 = (ch_type) mua.dest.range(31,24);

// ------------------------------------------------
// process: find pivital_pos and label the last bit
// ------------------------------------------------

   if_lowest = Mn[ch]<=Mn[ch_nn0] &&
               Mn[ch]<=Mn[ch_nn1] &&
               Mn[ch]<=Mn[ch_nn2] &&
               Mn[ch]<=Mn[ch_nn3];

   FSM_SPK_DECT:{
       switch(state[ch]){
           case S0:
               if(val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if(val<thr){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;

           case S1:
               if(val<thr && val<Mn[ch]){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if(val<thr && val>=Mn[ch]){
                   state[ch] = S2;
                   is_peak[ch] = 0;
                   first_S2[ch] = 1;
               }
               else if(val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;
           case S2:
               if(val<thr && val<Mn[ch]){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if (val<thr && val>=Mn[ch]){
                   state[ch] = S2;
                   if(first_S2[ch]==1 && if_lowest) is_peak[ch] = 1;
                   else is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if (val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;
       }
   }

   if(val<thr && val<Mn[ch])  Mn[ch] = val;
   else if(val>=thr)          Mn[ch] = 0;

// ------------------------------------------------
// output: muap_stream (t, ch, ch_hash, muap_data)
// ------------------------------------------------
    muap_struct muap;
    muap.id   = ch;
    muap.user = t;
    muap.data = val;
    muap.data.range(0,0) = is_peak[ch];
//    muap.data = Mn[ch];
//    muap.data.range(31,0) = (val.range(31,1), is_peak[ch]);
    muap.dest = (ch_nn3, ch_nn2, ch_nn1, ch_nn0);
    muap_stream.write(muap);

}
