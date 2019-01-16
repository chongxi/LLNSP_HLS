// Company:  HHMI Janelia
// Author: Chongxi Lai

#include "ref_sub.h"

void ref_sub(hls::stream<mua_struct>  &mua_stream,
             hls::stream<muar_struct> &muar_stream)
{

    static mua_type buf_2d[T][CH];
// ------------------------------------------------
// input: mua_stream (t, ch, ch_hash, mua_data)
// ------------------------------------------------
    mua_struct      mua;
    time_type       t, t_out;
    ch_type         ch, ch_ref;
    ch_hash_type    ch_hash;
    mua_type        val, _val, thr;

    mua = mua_stream.read();

    t.range(31,0)       = mua.data.range(159,128);
    ch_ref.range(11,0)  = mua.data.range(119,108);
    ch.range(11,0)      = mua.data.range(107,96);
    ch_hash.range(31,0) = mua.data.range(95, 64);
    thr.range(31,0)     = mua.data.range(63, 32);
    val.range(31,0)     = mua.data.range(31,  0);

    // The clever trick is when you write `val`, `_val` are guaranteed to exist
    bool j,l;
    j = t.range(0,0);
    l = !j;
    // write current time point
    buf_2d[j][ch] = val;
    // read previous time point and do ref substraction
    if(ch_ref<CH)
        _val = buf_2d[l][ch] - buf_2d[l][ch_ref];
    else
        _val = buf_2d[l][ch];

    t_out = t - 1;
// ------------------------------------------------
// output: muar_stream (_val, thr, ch_hash, ch, ch_ref, t-1)
// ------------------------------------------------
    output:
    {
        if(t>0)
        {
            muar_struct muar;
            muar.data.range(31, 0)   = _val.range(31,0);
            muar.data.range(63, 32)  = thr.range(31,0);
            muar.data.range(95, 64)  = ch_hash.range(31,0);
            muar.data.range(107,96)  = ch.range(11,0);
            muar.data.range(119,108) = ch_ref.range(11,0);
            muar.data.range(159,128) = t_out.range(31,0);
            muar_stream.write(muar);
        }
    }

}
