#include "spk_clf.h"
#include <iostream>

using namespace std;


int main()
{
	int i,j;
	int pca_data[122];
	int scale;
	for(i=0;i<32;i++)
		for(j=0;j<57;j++)
			pca_data[i] = i*j;
			scale = i;

	/* code */
//	ap32_data x = 0x00f200ff;
	ap32_data x = 0x00fd06d1;
//	ap_type x = 0b11111111111111100110111000100111;

    ap_type x_t = -117.5606689;
//    x_t.range(31,0)= x.range(31,0);
    pca_type y;
    y.range(7,0) = x.range(7,0);
    pca_type pca_comp[4];
	pca_comp[0].range(7,0) = x.range(7,0);
	pca_comp[1].range(7,0) = x.range(15,8);
	pca_comp[2].range(7,0) = x.range(23,16);
	pca_comp[3].range(7,0) = x.range(31,24);
	for(i=0;i<4;i++)
		std::cout << pca_comp[i] << '\t' << x_t*pca_comp[i] << endl;


//	std::cout << x_t << '\t' << y << endl;
//	ap_type result;
//	result = x_t * y;
//	std::cout << result << endl;
//
//	hls::stream<ap32_data> test_stream;
//	test_stream.write(result.range(31,0));
//	ap_type test;
//	test.range(31,0) = test_stream.read();
//	std::cout << test << endl;


	return 0;
}
