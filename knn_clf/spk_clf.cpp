#include "spk_clf.h"

void spk_clf(ap_type   data[data_size],
		     ap32_data vq[vq_depth],
		     ap32_data label[vq_depth],
			 hls::stream<ap32_data> &distance_out,
			 hls::stream<ap32_data> &nnid_out)
{
#pragma HLS INTERFACE mode=ap_fifo port=data register
#pragma HLS INTERFACE mode=axis register_mode=both port=distance_out register
#pragma HLS INTERFACE mode=axis register_mode=both port=nnid_out register

	static ap32_data vq_in;
	static vq_type vq_comp[4][vq_size];
//#pragma HLS ARRAY_PARTITION variable=vq_comp complete dim=1

	static ap32_data label_in;
	static ap32_data labels[vq_size];

	static ap_type data_in[pca_dim];
#pragma HLS ARRAY_PARTITION variable=data_in complete dim=1

	int ch, time; // ch is grp_id at this stage

	int i,j;

	ap_type min_distance;
	static ap_type distance;//[vq_size];
//#pragma HLS ARRAY_PARTITION variable=distance complete dim=1

	int label_out;
	label_out = 0;

	data_in:
	for (i=0; i<data_size; i++)
	{
//#pragma HLS PIPELINE
		if(i==0)
		{
			time = data[i].range(31,0);
		}
		else if(i==1)
		{
			ch   = data[i].range(31,0);
		}
		else if(i>=2)
		{
			if (i<6){
				data_in[i-2] = data[i];
			}
		}
	}


	vq_in_and_label_in:
	for (i=0; i<vq_size; i++)
	{
//#pragma HLS PIPELINE
		// vq_in
		vq_in.range(31, 0) = vq[i+ch*vq_size].range(31, 0);
		vq_comp[0][i].range(7,0) = vq_in.range(7,0);
		vq_comp[1][i].range(7,0) = vq_in.range(15,8);
		vq_comp[2][i].range(7,0) = vq_in.range(23,16);
		vq_comp[3][i].range(7,0) = vq_in.range(31,24);
		// label_in
		label_in.range(31,0)   = label[i+ch*vq_size].range(31, 0);
		labels[i].range(31, 0) = label_in.range(31,0);
		// elicit signal out for debugging
//		distance_out.write(vq_in.range(31,0));
	}


	knn:
	for (i=0; i<vq_size; i++)
	{
#pragma HLS PIPELINE
		if(i==0) min_distance = 1;
		distance = 0;
		vq_type vq_reg;
		ap_type data_reg, sub, sub2;
		accumulate:
		for (j=0; j<pca_dim; j++){
			vq_reg   = vq_comp[j][i];
			data_reg = data_in[j];
			sub      = data_reg - vq_reg;
			sub2     = sub*sub;
			distance += sub2;
		}

		distance_out.write(distance.range(31, 0));

		if (distance < min_distance){
			label_out = labels[i];
			min_distance = distance;
		}
	}

	output:
	nnid_out.write(time);                              // 0       output spike time stamp
	nnid_out.write(ch);                                // 1       output electrode group No
	for (i=0; i<4; i++)
		nnid_out.write(data_in[i].range(31,0));        // 2,3,4,5  output fet0, fet1, fet2, fet3
	nnid_out.write(label_out);                         // 6         output spike-id
}
