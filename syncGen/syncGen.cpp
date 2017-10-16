//////////////////////////////////////////////////////////////////////////////////
// Company:  HHMI Janelia
// Engineer: Chongxi Lai
//
// Create Date: 09/20/2017
// Design Name: syncGen
// Module Name: syncGen
// Project Name: Xike
// Target Devices: KC705
// Tool Versions: Vivado HLS 2016.1
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// This module generate irregular interval of 100ms sync pulse to behaviour box
//
//     |---|                  |---|       |---|
//     |   |                  |   |       |   |
// ----|   |------------------|   |-------|   |----------
//


#include "syncGen.h"
#include <iostream>
using namespace std;

void syncGen(hls::stream<tsec> &time_stream,
             ap_uint<1>        *sync_pulse)
{
	/*
	array([   0,    0,    3,   64,  105,  181,  266,  284,  382,  469,  531,
	        545,  551,  614,  712,  726,  810,  830,  846,  893,  983, 1024,
	       1113, 1196, 1214, 1242, 1257, 1285, 1379, 1477, 1537, 1567, 1634,
	       1697, 1718, 1744, 1749, 1811, 1862, 1917, 1995, 2048])
	*/

    static int time_to_sync[intervals] = { 0,    1,    3,   64,  105,  181,  266,  284,  382,  469,  531,
	                                     545,  551,  614,  712,  726,  810,  830,  846,  893,  983, 1024,
	                                    1113, 1196, 1214, 1242, 1257, 1285, 1379, 1477, 1537, 1567, 1634,
	                                    1697, 1718, 1744, 1749, 1811, 1862, 1917, 1995, eoftime-1};
    static int i = 0;

// ------------------------------------------
// input time
    tsec time;
    ap_uint<12> sec;
    time = time_stream.read();
    sec  = time.sec % eoftime;

    loop_find_idx:
    for (int k=0; k<intervals; k++) {
    	if (sec==time_to_sync[k])
    		i=k;
    }

    if (sec==time_to_sync[i]) {
//		i += 1;
		syncGen_loop:
		for(int j=0; j<pulse_width; j++)
		{
			*sync_pulse = 1;
		}
//		if(i==8) i=0;
    }
    else *sync_pulse = 0;

}
