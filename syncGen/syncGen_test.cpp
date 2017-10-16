#include <iostream>
#include <hls_stream.h>
#include <math.h>
#include "stdio.h"
#include "syncGen.h"

using namespace std;

int main()
{
	hls::stream<tsec> time_stream;
    tsec time;
    ap_uint<12> sec;
    ap_uint<1>  sync_pulse;

    for(sec=0; sec<400; sec++)
    {
		time.sec = sec;
		time_stream.write(time);
		syncGen(time_stream, &sync_pulse);
		cout << sec <<  ',' << sync_pulse << endl;
    }

    for(sec=0; sec<400; sec++)
    {
		time.sec = sec;
		time_stream.write(time);
		syncGen(time_stream, &sync_pulse);
		cout << sec <<  ',' << sync_pulse << endl;
    }

    return 0;    
}
