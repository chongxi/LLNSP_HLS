#include "spk_transform.h"

void spk_transform(ap_data   spk[spklen+1],
				   ap32_data scale[scale_depth],
				   ap32_data shift[shift_depth],
				   ap32_data pca[pca_depth],
				   hls::stream<ap32_data> &pca_stream,
				   hls::stream<ap32_data> &pca_final)
{
#pragma HLS INTERFACE ap_fifo port=spk
#pragma HLS INTERFACE axis port=pca_stream
#pragma HLS INTERFACE axis port=pca_final

	static ap_type  spk_comp[spklen*ch_span];   // concatenated spike waveform
#pragma HLS ARRAY_PARTITION variable=spk_comp complete dim=1

	static ap_type scale_in;
	
	static ap_type shift_in[pca_dim];
#pragma HLS ARRAY_PARTITION variable=shift_in complete dim=1
	
	static ap32_data pca_in;
	static pca_type pca_comp[4][pca_size];  // 4*8bits = 32 bits
#pragma HLS ARRAY_PARTITION variable=pca_comp complete dim=1
	
	static ap_type  data[pca_dim];
#pragma HLS ARRAY_PARTITION variable=data complete dim=1

	int ch, time;
	int i,j,k;

	init:
	for(i=0;i<pca_dim;i++)
	{
#pragma HLS UNROLL
		data[i]=0;
	}

	spk_in:
	for(k=0; k<spklen+1; k++)
	{
#pragma HLS PIPELINE
		if (k==0)
		{
			ch   = spk[k].range(31, 0);
			time = spk[k].range(63,32);
//			pca_stream.write(ch);
//			pca_stream.write(time);
		}
		else
		{
			spk_comp[           k - 1].range(31,0) = spk[k].range(127,96);
			spk_comp[spklen   + k - 1].range(31,0) = spk[k].range( 95,64);
			spk_comp[spklen*2 + k - 1].range(31,0) = spk[k].range( 63,32);
			spk_comp[spklen*3 + k - 1].range(31,0) = spk[k].range( 31, 0);
//			pca_stream.write(spk_comp[           k - 1].range(31, 0));
//			pca_stream.write(spk_comp[spklen   + k - 1].range(31, 0));
//			pca_stream.write(spk_comp[spklen*2 + k - 1].range(31, 0));
//			pca_stream.write(spk_comp[spklen*3 + k - 1].range(31, 0));
		}
	}

	pca_in:
	for (i=0; i<pca_size; i++)
	{
#pragma HLS PIPELINE
		pca_in.range(31, 0)       = pca[i+ch*pca_size].range(31, 0);
		pca_comp[0][i].range(7,0) = pca_in.range(7,0);
		pca_comp[1][i].range(7,0) = pca_in.range(15,8);
		pca_comp[2][i].range(7,0) = pca_in.range(23,16);
		pca_comp[3][i].range(7,0) = pca_in.range(31,24);

		data[0] += pca_comp[0][i] * spk_comp[i];
		data[1] += pca_comp[1][i] * spk_comp[i];
		data[2] += pca_comp[2][i] * spk_comp[i];
		data[3] += pca_comp[3][i] * spk_comp[i];
		pca_stream.write(pca_in.range(31, 0));
	}

	shift_scale_in:
	for (i=0; i<pca_dim; i++)
	{
#pragma HLS PIPELINE
		if(i==0) {
			scale_in.range(31, 0) = scale[ch].range(31, 0);
			shift_in[i].range(31, 0) = shift[i+ch*pca_dim].range(31, 0);
			pca_stream.write(shift_in[i].range(31, 0));
		}
		else {
			shift_in[i].range(31, 0) = shift[i+ch*pca_dim].range(31, 0);
			pca_stream.write(shift_in[i].range(31, 0));
		}
	}

	pca_stream.write(scale_in.range(31,0));

	transform:
	for(i=0; i<pca_dim; i++){
#pragma HLS UNROLL
		data[i] += shift_in[i];
		data[i] /= scale_in;
	}

	if (ch < CH && time > 2000)
	{
		output_info:
		{
			pca_final.write(time);
			pca_final.write(ch);
		}

		output_data:
		for(i=0; i<pca_dim; i++){
	#pragma HLS PIPELINE
			pca_final.write(data[i].range(31, 0));
		}
	}

}
