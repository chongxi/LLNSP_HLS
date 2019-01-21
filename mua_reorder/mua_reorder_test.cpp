#include <iostream>
#include <hls_stream.h>
#include <math.h>
#include <fstream>
#include <vector>
#include "stdio.h"
#include "mua_reorder.h"

#define PRINT true

using namespace std;

int main()
{
// ------------------------------------------------
// 1. prepare data ,2. feed input into stream, 3. feed stream into module
// ------------------------------------------------
hls::stream<muar_struct>  muar_stream;
hls::stream<muao_struct>  muao_stream;

muar_struct muar;
muao_struct muao;

int ch;
int result;
int n_round=3;
muar_type received[n_round*CH];


for (int a=0; a<n_round; a++)
	for (int i=0; i<channel_per_chip; i++)
		for (int j=0; j<CH; j=j+channel_per_chip)
			{
				ch = i+j;
				muar.data = ch;
//	            cout << muar.data << endl;
				muar_stream.write(muar);
				mua_reorder(muar_stream, muao_stream);
			}

//	for (int i=0; i<channel_per_chip; i++)
//		for (int j=0; j<CH; j=j+channel_per_chip)
//			{
//				ch = i+j;
//				muar.data = ch;
//	//            cout << muar.data << endl;
//				muar_stream.write(muar);
//				mua_reorder(muar_stream, muao_stream);
//			}

	// ------------------------------------------------
	// 4. get output stream
	// ------------------------------------------------

for (int a=0; a<n_round; a++)
	for(int j=0; j<CH; j++) {
		muao = muao_stream.read();
	//	cout << muao.data << ' ';
		received[j] = muao.data;
	}


for (int a=0; a<n_round; a++)
	for(int j=0; j<CH; j++) {
		if (received[a*j + j] == j){
			cout << received[j] << ' ';
			result += 0;
		}
	}


//if (result==0) return 0;
	return 0;

}
