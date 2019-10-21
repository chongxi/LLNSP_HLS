//#include <iomanip>
#include "bram_xike.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


int main()
{
	bool _doWrite;
	int _WriteData;
	bool _doRead;
	ap_uint<16> _addr;    // read  addr
	ap_uint<16> _addr_in; // write addr
	int _readData;
	int i,j;
	int errors = 0;

    bool _scale_read; ap_uint<7> _scale_addr; int scale_out;
    bool _shift_read; ap_uint<7> _shift_addr; int shift_out;
    bool _pca_read;  ap_uint<16> _pca_addr; int pca_out;
    bool _vq_read;  ap_uint<16> _vq_addr; int vq_out;
    bool _lb_read;  ap_uint<16> _lb_addr; int lb_out;

    //Write data
	for(i=0; i<256; i++)
	{
		bram_xike(1, 10+i, i,            // write
				  0, &_readData, i,      // read
				  0,0, &scale_out,
				  0,0, &shift_out,
				  0,0, &pca_out,
				  0,0, &vq_out,
				  0,0, &lb_out);
		printf("Write Address = %d\n", i);
		printf("Write Data= %d\n", 10+i);
	}

	//Read data & check
	for(j=0; j<256; j++)
	{
		bram_xike(0, 0, j,              // write
				  1, &_readData, j,     // read
				  1, 0, &scale_out,
				  1, 0, &shift_out,
				  1, 0, &pca_out,
				  1, 0, &vq_out,
				  1, 0, &lb_out);
		printf("Read Address= %d \t Read Data= %d\n", j, _readData);
		printf("scale_out= %d\n", scale_out);
		printf("shift_out= %d\n", shift_out);
		printf("pca_out= %d\n", pca_out);
		printf("vq_out= %d\n", vq_out);
		errors += _readData != (10+j);
	}

	return errors;

}
