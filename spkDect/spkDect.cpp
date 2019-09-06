// Company:  HHMI Janelia

#include "spkDect.h"
//#include <iostream>
//using namespace std;

void spk_dect(hls::stream<mua_struct>  &mua_stream,
              hls::stream<muap_struct> &muap_stream)
{

    static mua_type Mn[CH] = {0};
    static bool is_peak[CH]  = {0};
    static ap_uint<1> first_S2[CH] = {0};
    static int syn_large_amp[T][number_of_chips] = {0};

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
    ch_type     ch, ch_ref;
    ch_type     ch_nn[CH_PG];
    mua_type    val, _val;
    mua_type    _val_nn[CH_PG];
    mua_type    thr, thr2;

    mua = mua_stream.read();

    t                = mua.data.range(159,128);
    ch_ref = (ch_type) mua.data.range(119,108);
    ch     = (ch_type) mua.data.range(107,96);

    ch_nn[3] = (ch_type) mua.data.range(95, 88);
    ch_nn[2] = (ch_type) mua.data.range(87, 80);
    ch_nn[1] = (ch_type) mua.data.range(79, 72);
    ch_nn[0] = (ch_type) mua.data.range(71, 64);

    thr.range(31,0)  = mua.data.range(63, 32);
    val.range(31,1)  = mua.data.range(31,  1);
    val.range(0, 0)  = 0;

    // The clever trick is when you write `val`, `_val` and `_val_nn` are guaranteed to exist
    bool j,l;
    j = t.range(0,0);
    l = !j;
    // write current time point
    buf_2d[j][ch] = val;
    // read previous time point
    _val = buf_2d[l][ch];

    if(start_cnt[ch])
    {
  		cnt[ch] += 1;
  		if (cnt[ch] == INACTIVE_LEN)
  		{
  			cnt[ch] = 0;
  			start_cnt[ch] = 0;
  		}
    }

//    int chipNo = (int)ch/chs_per_chip;

    //    static bool _if_lowest_in_S1[CH] = {0};
//    if (val<thr)
//    {
//      syn_large_amp[j][chipNo] += 1;
//    }

    bool if_pivotal = 1;
    Get_Com_Loop:
    for(int i=0; i<CH_PG; i++)
    {
        _val_nn[i] = buf_2d[l][ch_nn[i]];
        // if one of the condition fail, if_pivotal becomes 0
        if_pivotal =
//        		         (_val>=(mua_type)(-10000.0))
//                    && (syn_large_amp[l][chipNo] < 30)
                    (_val <= Mn[ch_nn[i]])
                    && (_val <= _val_nn[i])
                    && !start_cnt[ch_nn[i]]
                    && if_pivotal;
    }

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

	if (ch==CH-1)
	{
		reset_syn_large_amp_loop:
		for(int i=0; i<number_of_chips; i++)
			syn_large_amp[l][i] = 0;
	}

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
}