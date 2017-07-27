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
    static bool is_peak[CH]  = {0};
    static ap_uint<1> first_S2[CH] = {0};

    static mua_type buf_2d[T][CH];
//    static ch_type  ch_hash[CH][CH_PG];
    static State state[CH] = {S0};
    static bool start_cnt[CH] = {0};
    static ap_uint<3> cnt[CH] = {0};

// ------------------------------------------------
// input: mua_stream (t, ch, ch_hash, mua_data)
// ------------------------------------------------
    mua_struct  mua;
    ap_uint<32> t;
    ch_type     ch;
    ch_type     ch_nn[CH_PG];
    mua_type    val, _val;
    mua_type    _val_nn[CH_PG];
    mua_type    thr, thr2;

    mua = mua_stream.read();

    t                = mua.data.range(159,128);
    ch     = (ch_type) mua.data.range(107,96);

    val.range(31,1)  = mua.data.range(31,  1);
    val.range(0, 0)  = 0;
    thr.range(31,0)  = mua.data.range(63, 32);
//    ch_nn0 = (ch_type) mua.data.range(71, 64);
//    ch_nn1 = (ch_type) mua.data.range(79, 72);
//    ch_nn2 = (ch_type) mua.data.range(87, 80);
//    ch_nn3 = (ch_type) mua.data.range(95, 88);

    ch_nn[0] = (ch_type) mua.data.range(71, 64);
    ch_nn[1] = (ch_type) mua.data.range(79, 72);
    ch_nn[2] = (ch_type) mua.data.range(87, 80);
    ch_nn[3] = (ch_type) mua.data.range(95, 88);

//    ch_hash[ch][0] = ch_nn[0];
//    ch_hash[ch][1] = ch_nn[1];
//    ch_hash[ch][2] = ch_nn[2];
//    ch_hash[ch][3] = ch_nn[3];

    // The clever trick is when you write `val`, `_val` and `_val_nn` are guaranteed to exist
    bool j,l;
    j = t.range(0,0);
    l = !j;
    // write current time point
    buf_2d[j][ch] = val;
    // read previous time point

//    if(start_cnt[ch])
//    {
//    	CNT_LOOP:
//    	for(int i=0; i<CH_PG; i++)
//    	{
//        	cnt[ch_nn[i]] += 1;
//        	if (cnt[ch] == INACTIVE_LEN)
//        	{
//        		cnt[ch] = 0;
//        		start_cnt[ch_nn[i]] = 0;
//        	}
//    	}
//    }

    if(start_cnt[ch])
    {
		cnt[ch] += 1;
		if (cnt[ch] == INACTIVE_LEN)
		{
			cnt[ch] = 0;
			start_cnt[ch] = 0;
		}
    }

    //    static bool _if_lowest_in_S1[CH] = {0};

    bool if_pivotal = 1;
    Get_Com_Loop:
    for(int i=0; i<CH_PG; i++)
    {
        _val = buf_2d[l][ch];
    	_val_nn[i] = buf_2d[l][ch_nn[i]];
    	if_pivotal = (_val <= Mn[ch_nn[i]]) && (_val <= _val_nn[i]) && !start_cnt[ch_nn[i]] && if_pivotal;
    }

//    cout<<j<<'\t'<<_val<<endl;

//    bool last_channel_in_group = (ch==ch_nn3);
// ------------------------------------------------
// process: find pivital_pos and label the last bit
// ------------------------------------------------

//   if_lowest = Mn[ch] <= Mn[ch_nn0] &&
//               Mn[ch] <= Mn[ch_nn1] &&
//               Mn[ch] <= Mn[ch_nn2] &&
//               Mn[ch] <= Mn[ch_nn3];

//   if_lowest = _val <= buf_2d[!j][ch_nn0] &&
//		       _val <= buf_2d[!j][ch_nn1] &&
//			   _val <= buf_2d[!j][ch_nn2] &&
//			   _val <= buf_2d[!j][ch_nn3];

//	if_lowest = _val <= _val_nn[0] &&
//			    _val <= _val_nn[1] &&
//			    _val <= _val_nn[2] &&
//			    _val <= _val_nn[3];

//   thr2 = thr; // *(mua_type)(0.7)
//    if(last_channel_in_group)
//    {
//    	find_lowest_loop:
// 		for(int i=0; i<CH_PG; i++)
// 		{
// 		   if_lowest[ch] = Mn[ch_hash[ch][i]] <= Mn[ch_hash[ch][0]] &&
// 						   Mn[ch_hash[ch][i]] <= Mn[ch_hash[ch][1]] &&
// 						   Mn[ch_hash[ch][i]] <= Mn[ch_hash[ch][2]] &&
// 						   Mn[ch_hash[ch][i]] <= Mn[ch_hash[ch][3]];
//
// //		   if(first_S2[ch_hash[ch][i]]==1 && if_lowest[i])
// //			   {
// //				   is_peak[ch_hash[ch][i]] = 1;
// //			   }
// //		   else is_peak[ch_hash[ch][i]] = 0;
// 		}
//    }


   FSM_SPK_DECT:{
       switch(state[ch]){
           case S0:
//        	   _if_lowest_in_S1[ch] = 0;

               if(_val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if(_val<thr){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;

           case S1:
               if(_val<thr && _val>val){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
//                   _if_lowest_in_S1[ch] = if_pivotal;
               }
               else if(_val<thr && _val<=val){
                   state[ch] = S2;
                   is_peak[ch] = 0;
                   first_S2[ch] = 1;
                   if(if_pivotal) //&& _if_lowest_in_S1[ch]
                	   {
                	   	   _val.range(0,0) = 1;
                	   	   is_peak[ch] = 1;
//                	   	   _if_lowest_in_S1[ch] = 0;
                	   	   start_cnt[ch] = 1;
                	   }
                   else
                	   {
                	   	   is_peak[ch] = 0;
                	   }
               }
               else if(_val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;

           case S2:
//        	   _if_lowest_in_S1[ch] = 0;

               if(_val<thr && _val>val){
                   state[ch] = S1;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if (_val<thr && _val<=val){
                   state[ch] = S2;
//                   if(first_S2[ch]==1 && if_lowest[ch])
//                	   {
//                	   	   is_peak[ch] = 1;
//                	   }
//                   else is_peak[ch] = 0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               else if (_val>=thr){
                   state[ch] = S0;
                   is_peak[ch] = 0;
                   first_S2[ch] = 0;
               }
               break;
       }
   }


   if(val<thr && val<Mn[ch])        Mn[ch] = val;
   else if(val>=thr)                Mn[ch] = 0;


//		for(int i=0; i<CH_PG; i++)
//		{
//
//		}


// ------------------------------------------------
// output: muap_stream (t, ch, ch_hash, muap_data)
// ------------------------------------------------
    output:
	{
		if(t>0)
		{
			muap_struct muap;
			muap.id   = ch;
			muap.user = t - 1;
			muap.data = _val;
//			muap.data = state[ch]*-10;
//			muap.data = Mn[ch];
//			muap.data = -10 * if_pivotal;
//			muap.data = thr*is_peak[ch];
			muap.dest = (ch_nn[3], ch_nn[2], ch_nn[1], ch_nn[0]);
			muap_stream.write(muap);
		}
	}


//    if(t>=1){
//    	if(ch==CH-1)
//    	{
//			OUTPUT_TETRODE:
//			for (int i=0; i<CH; i++)
//			{
//				muap.id   = i;
//				muap.user = t;
//				muap.data = buf_2d[!j][i];
//				muap.data.range(0,0) = is_peak[i];
//				muap.dest = (ch_hash[i][3], ch_hash[i][2], ch_hash[i][1], ch_hash[i][0]);
//				muap_stream.write(muap);
//			}
//    	}

//    	if(last_channel_in_group){
//    		OUTPUT_TETRODE:
//    		for (int i=0; i<CH_PG; i++)
//    		{
//				muap.id   = ch_nn[i];
//				muap.user = t;
////				muap.data = _val;
//				muap.data = buf_2d[!j][ch_nn[i]];
//				muap.data.range(0,0) = is_peak[ch_nn[i]];
//			//    muap.data = Mn[ch];
//			//    muap.data.range(31,0) = (val.range(31,1), is_peak[ch]);
//				muap.dest = (ch_nn3, ch_nn2, ch_nn1, ch_nn0);
//				muap_stream.write(muap);
//    		}
//    	}
//    }
}
