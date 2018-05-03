#include "spk_clf.h"

void spk_clf(ap_type   data[data_size],
		     ap32_data vq[vq_depth],
			 hls::stream<ap32_data> &distance_out,
			 hls::stream<ap32_data> &nnid_out)
{
#pragma HLS INTERFACE ap_fifo port=data
#pragma HLS INTERFACE axis port=distance_out
#pragma HLS INTERFACE axis port=nnid_out

	static ap32_data vq_in;
	static vq_type vq_comp[4][vq_size];
#pragma HLS ARRAY_PARTITION variable=vq_comp complete dim=1

	static ap_type data_in[pca_dim];
#pragma HLS ARRAY_PARTITION variable=data_in complete dim=1

	int ch, time;

	int i,j,k;

	ap_type min_distance;
	static ap_type distance[vq_size];
#pragma HLS ARRAY_PARTITION variable=distance complete dim=1

	int nnid;


	data_in:
	for (i=0; i<data_size; i++)
	{
#pragma HLS PIPELINE
		if(i==0)
		{
			time = data[i].range(31,0);
			nnid_out.write(time);
		}
		if(i==1)
		{
			ch   = data[i].range(31,0);
			nnid_out.write(ch);
		}
		if(i>=2)
		{
			data_in[i-2] = data[i];
			nnid_out.write(data_in[i-2].range(31,0));
		}
	}


	vq_in:
	for (i=0; i<vq_size; i++)
	{
#pragma HLS PIPELINE
		vq_in.range(31, 0) = vq[i+ch*vq_size].range(31, 0);
		vq_comp[0][i].range(7,0) = vq_in.range(7,0);
		vq_comp[1][i].range(7,0) = vq_in.range(15,8);
		vq_comp[2][i].range(7,0) = vq_in.range(23,16);
		vq_comp[3][i].range(7,0) = vq_in.range(31,24);
		distance_out.write(vq_in.range(31,0));
	}


	distance_out:
	for (i=0; i<vq_size; i++)
	{
#pragma HLS PIPELINE
		if(i==0) min_distance = 1;
		distance[i] = 0;
		vq_type vq_reg;
		ap_type data_reg, sub, sub2;
		accumulate:
		for (j=0; j<pca_dim; j++){
			vq_reg   = vq_comp[j][i];
			data_reg = data_in[j];
			sub      = data_reg - vq_reg;
			sub2     = sub*sub;
			distance[i] += sub2;
		}

		distance_out.write(distance[i].range(31, 0));

		if (distance[i] < min_distance){
			nnid = i;
			min_distance = distance[i];
		}
	}
	nnid_out.write(nnid);

}
