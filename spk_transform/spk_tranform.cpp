#include "spk_transform.h"

void spk_transform(ap128_data   spk[spklen+1],
				   ap32_data  scale[scale_depth],
				   ap32_data  shift[shift_depth],
				   ap32_data    pca[pca_depth],
				   hls::stream<ap32_data> &pca_final)
{
#pragma HLS BIND_STORAGE variable=pca type=ram_t2p impl=bram
#pragma HLS BIND_STORAGE variable=shift type=ram_t2p impl=bram
#pragma HLS BIND_STORAGE variable=scale type=ram_t2p impl=bram
#pragma HLS INTERFACE mode=ap_memory port=pca
#pragma HLS INTERFACE mode=ap_memory port=shift
#pragma HLS INTERFACE mode=ap_memory port=scale
// Array arguments are implemented by default as an ap_memory interface.
// This is a standard block RAM interface with data, address, chip-enable, and write-enable ports.

#pragma HLS INTERFACE ap_fifo port=spk
//#pragma HLS INTERFACE axis port=pca_stream
#pragma HLS INTERFACE mode=axis register_mode=both port=pca_final register

	static io_type spk_comp[spklen*ch_span];   // concatenated spike waveform (first 19pts is the spike waveform from 1st channel, second 19pts is the spike waveform from 2nd channel)
	io_type  spk_val;
	io_type  spk_abs;
	io_type  spk_sum;
    io_type  spk_min;                    // lower_bound of spikes from 4 channels (scalar)
	io_type  spk_max;                    // upper_bound of spikes from 4 channels (scalar)
	io_type  spk_range;                  // upper_bound - lower_bound (scalar)
	io_type  mean_spk_range;             // mean(upper_bound - lower_bound)  (scalar)
	io_type  mean_spk_total;             // mean(abs(spk))
	io_type  spk_energy;                 // defined by mean_spk_range/mean_spk_total
    bool     spurious_spk;               // mean_spk_range < 0.01

    mean_spk_range = 0;
    mean_spk_total = 0;
    spk_energy     = 0;

#pragma HLS ARRAY_PARTITION variable=spk_comp complete dim=1

	static param_type scale_in;
	
	static param_type shift_in[pca_dim];
#pragma HLS ARRAY_PARTITION variable=shift_in complete dim=1
	
	static ap32_data pca_in;
	static pca_type pca_comp[4][pca_size];  // 4*8bits = 32 bits
#pragma HLS ARRAY_PARTITION variable=pca_comp complete dim=1
	
	static io_type  data[pca_dim];
	io_type data_out;
//#pragma HLS ARRAY_PARTITION variable=data complete dim=1

	int ch, time;
	int i,j,k;

	init:
	for(i=0;i<pca_dim;i++)
	{
//#pragma HLS UNROLL
		data[i]=0;
	}

	spk_in:      // unpack 128 bits spk input
	for(k=0; k<spklen+1; k++)
	{
//#pragma HLS PIPELINE
		if (k==0)
		{
			ch   = spk[k].range(31, 0);
			time = spk[k].range(63,32);
		}
		else if (k>0)
		{
			spk_comp[           k - 1].range(31,0) = spk[k].range(127,96);
			spk_comp[spklen   + k - 1].range(31,0) = spk[k].range( 95,64);
			spk_comp[spklen*2 + k - 1].range(31,0) = spk[k].range( 63,32);
			spk_comp[spklen*3 + k - 1].range(31,0) = spk[k].range( 31, 0);
		}
	}

	pca_in_matmul:
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

//		pca_final.write(pca_in.range(31,0));

//		pca_final.write(pca_comp[0][i]);
//		pca_final.write(pca_comp[1][i]);
//		pca_final.write(pca_comp[2][i]);
//		pca_final.write(pca_comp[3][i]);

//		pca_final.write(spk_comp[i].range(31,0));

//		pca_final.write(data[0].range(31,0));
//		pca_final.write(data[1].range(31,0));
//		pca_final.write(data[2].range(31,0));
//		pca_final.write(data[3].range(31,0));
//		pca_stream.write(pca_in.range(31, 0));
	}

	scale_in.range(31, 0) = scale[ch].range(31, 0);

	shift_in:
	for (i=0; i<pca_dim; i++)
		shift_in[i].range(31, 0) = shift[i+ch*pca_dim].range(31, 0);

	shift:
	for(i=0; i<pca_dim; i++)
		data[i] += shift_in[i];

	scale:
	for(i=0; i<pca_dim; i++)
		data[i] *= scale_in;


	spk_energy:  // decide whether this spike is spurious based on its energy, lower energy is harder to triangulate; too low energy can be spurious
	for(i=0; i<spklen; i++)
	{
//#pragma HLS PIPELINE off
		spk_min = spk_comp[i];  // don't initialize as 0 (critical), as spk_val might never <= 0, then spk_range is calculated using 0
		spk_max = spk_comp[i];  // don't initialize as 0 (critical), as spk_val might never >  0, then spk_range is calculated using 0
		spk_range = 0;
		spk_sum   = 0;
		for(j=0; j<ch_span; j++)
		{
			spk_val = spk_comp[i+j*spklen];
			spk_abs = fabs(spk_val);      // take absolute value to accumulate
			spk_sum = spk_sum + spk_abs;
			if(spk_val <= spk_min)
				spk_min = spk_val;
			if(spk_val >  spk_max)
				spk_max = spk_val;
		}
		spk_range = spk_max - spk_min;    // upper_bound - lower_bound for ith points in spike waveforms across 4 channels
		mean_spk_range = mean_spk_range + spk_range;    // accumulated spk_range for each sample
		mean_spk_total = mean_spk_total + spk_sum;      // accumulated spk_total for each sample
	}

	mean_spk_total = mean_spk_total/ch_span;
	spk_energy = mean_spk_range/mean_spk_total;    // the range of multi-channel spike waveforms divided by absolute average of spike waveforms

	output_info:
	pca_final.write(time);
	pca_final.write(ch);

	output_fet:
	for(i=0; i<pca_dim; i++){
		pca_final.write(data[i].range(31, 0));
	}

	output_mean_spk_range:
	pca_final.write(spk_energy.range(31, 0));

}
