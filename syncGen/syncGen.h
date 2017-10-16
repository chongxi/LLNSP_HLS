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
//     |---|                  |---|       |---|
//     |   |                  |   |       |   |
// ----|   |------------------|   |-------|   |----------
//
//
//
//

#include "ap_int.h"
#include "ap_utils.h"
#include <hls_stream.h>

#define intervals 42
#define eoftime   2048
#define pulse_width 25000000 //100ms = 25000000 * 4ns

struct tsec{
      ap_uint<12> sec;
};

void syncGen(hls::stream<tsec> &time_stream,
	         ap_uint<1>        *sync_pulse);
