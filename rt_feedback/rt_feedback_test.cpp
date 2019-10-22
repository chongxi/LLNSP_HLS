#include "rt_feedback.h"
#include <iostream>

int main()
{
	int err = 0;
	ap32_data input[3][7] = {{123, 34, 0, 0, 0, 0, 101},
	                         {245, 21, 0, 0, 0, 0, 24},
							 {569, 10, 0, 0, 0, 0, 101}};

	ap_uint<1> output;
	ap_uint<16> target_id;
	target_id = 101;
	rt_feedback(target_id, input[0], &output);
	rt_feedback(target_id, input[1], &output);
	rt_feedback(target_id, input[2], &output);
	return err;
}
