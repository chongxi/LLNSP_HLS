// Company: HHMI Janelia
// Author:  Chongxi Lai

#include "mua_reorder.h"
#include <iostream>

using namespace std;

void mua_reorder(hls::stream<muar_struct> &muar_stream,
                 hls::stream<muao_struct> &muao_stream)
{
    static muar_type buf[CH];
    static int ch_cnt = 0;
    static int ch_No = 0;
    static int ch_cycle = 0;
    static bool output = 0;

// ------------------------------------------------
// input: muar_stream (ch: 0,32,64,96,128...)
// ------------------------------------------------
    muar_struct  muar;
    muao_struct  muao;
    muar_type    val;

    muar = muar_stream.read();
    val.range(31,0) = muar.data.range(31,  0);

    if (ch_cnt % chip_No == 0){
        ch_No = ch_cycle;
        ch_cycle += 1;
        cout << endl;
    }
    else{
    	ch_No  += channel_per_chip;
    }

    cout << ch_No << ' ';

    buf[ch_No] = val;

    ch_cnt += 1;

    if (ch_cnt == CH){
    	output   = 1;
        ch_cnt   = 0;
        ch_No    = 0;
        ch_cycle = 0;
    }
// ------------------------------------------------
// output: muao_stream (ch: 0, 1, 2, 3, 4 ...)
// ------------------------------------------------
//    output:
//    {
        if(output)
        {
        	cout << endl << "start output" << endl;
            output_for:for (int k=0; k<CH; k++) {
                muao.data.range(31, 0) = buf[k].range(31,0);
                muao_stream.write(muao);
            }
            output = 0;
        }
//    }
}
