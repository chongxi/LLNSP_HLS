#include <iostream>
#include <hls_stream.h>
#include <math.h>
#include <fstream>
#include <vector>
#include "stdio.h"
#include "ref_sub.h"

#define PRINT true

using namespace std;

int main()
{
// ------------------------------------------------
// 1. read binary file to memory
// ------------------------------------------------

    streampos npts = 320;        // 8 channels * 40 time points
    vector<float> memblock(npts);
    ifstream file("/disk0/Work/xike_hls_module/spkDect/spkDect_test_spk.bin", ios::binary);
    file.read(reinterpret_cast<char*>(&memblock[0]), npts*sizeof(float));

// ------------------------------------------------
// 2. feed input stream
// ------------------------------------------------
    hls::stream<mua_struct>  mua_stream;
    hls::stream<muap_struct> muap_stream;

    mua_struct  mua;
    mua_type    val_in, thr;
    ch_type     ch, ch_nn0, ch_nn1, ch_nn2, ch_nn3;

    int i,t,k;
    k = 0;
    for(t=0;t<41;t++)
    for(i=0;i<8;i++){
        thr = -50;
        val_in = (mua_type)memblock[k++];
        //    cout << val_in << '\t' << thr << endl;
        ch = i;
        ch_nn0 = ch/4*4+0;
        ch_nn1 = ch/4*4+1;
        ch_nn2 = ch/4*4+2;
        ch_nn3 = ch/4*4+3;

        mua.data.range(63,0)    = (thr.range(31,0), val_in.range(31,0));
        mua.data.range(71, 64)  = ch_nn0;
        mua.data.range(79, 72)  = ch_nn1;
        mua.data.range(87, 80)  = ch_nn2;
        mua.data.range(95, 88)  = ch_nn3;
        mua.data.range(107,96)  = i;   // ch
        mua.data.range(159,128) = t;   // t

        //      mua.id   = ch;
        //      mua.dest = (ch_nn3, ch_nn2, ch_nn1, ch_nn0);
        mua_stream.write(mua);
        // ------------------------------------------------
        // 3. execute function
        // ------------------------------------------------
        spk_dect(mua_stream, muap_stream);
    }

// ------------------------------------------------
// 4. get output stream
// ------------------------------------------------

    vector<int32_t> vout;
    ap_int<32> temp;

    muap_struct muap;
    mua_type    val_out;
    ap_uint<32> _t;
    ch_type     _ch, _ch_nn0, _ch_nn1, _ch_nn2, _ch_nn3;

    int j;
    for(t=0;t<40;t++){
        cout << "---output--- t=" << t << "--------" << endl;
        for(j=0;j<8;j++){
            muap = muap_stream.read();
            val_out = muap.data;
            _t      = muap.user;
            _ch     = (ch_type) muap.id;
            _ch_nn0 = (ch_type) muap.dest.range( 7, 0);
            _ch_nn1 = (ch_type) muap.dest.range(15, 8);
            _ch_nn2 = (ch_type) muap.dest.range(23,16);
            _ch_nn3 = (ch_type) muap.dest.range(31,24);

        if (PRINT==true) {
            cout << "(t,ch)=" << _t << ',' <<_ch << endl;
            cout << "value="  << val_out << endl;
            cout << "ch_hash=" << '(' << _ch_nn0 << ','
                                << _ch_nn1 << ','
                        << _ch_nn2 << ','
                        << _ch_nn3 << ')' << endl;
            }
            temp.range(31,0) = val_out.range(31,0);
            vout.push_back((int32_t)temp);
        }
    }

// ------------------------------------------------
// 5. write binary file to disk for compare
// ------------------------------------------------
    ofstream fout("/disk0/Work/xike_hls_module/spkDect/spkDect_test_compare.bin", ios::binary);
    fout.write(reinterpret_cast<char*>(&vout[0]), npts*sizeof(int32_t));

// ------------------------------------------------
// 6. return simulation results
// ------------------------------------------------
    return 0;
}
