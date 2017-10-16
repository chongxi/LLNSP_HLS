#include "spk_packet_tx.h"
#include "ap_int.h"
#include "ap_utils.h"


void spk_packet_tx(hls::stream<mua_struct> &mua_stream,
				   hls::stream<spk_struct> &out_pre,
				   hls::stream<spk_struct> &out_post,
				   hls::stream<int> &time_stamp)
{
#pragma HLS INTERFACE ap_ctrl_none  port=return
#pragma HLS INTERFACE ap_fifo port=mua_stream
#pragma HLS INTERFACE axis register port=out_pre
#pragma HLS INTERFACE axis register port=out_post
#pragma HLS INTERFACE axis register port=time_stamp

//#pragma HLS PIPELINE


   static  ap_data buf_2d[T][CH];
   #pragma HLS DEPENDENCE variable=buf_2d false
//   #pragma HLS ARRAY_PARTITION variable=buf_2d complete dim=1
   #pragma HLS RESOURCE variable=buf_2d core=RAM_S2P_BRAM

   static txState state_A[CH]  = {IDLE};
   static txState state_B[CH]  = {IDLE};
//   #pragma HLS ARRAY_PARTITION variable=state_A complete dim=1
   static ap_uint<5> cnt_A[CH] = {0};
   static ap_uint<5> cnt_B[CH] = {0};
//   #pragma HLS ARRAY_PARTITION variable=cnt_A complete dim=1

   ap_uint<5> t;
   ch_type ch_w, ch_last_in_group, ch_groupNo;  // support up to 256 channels
   ap_uint<1> is_peak;
   ap_data pre, post;
   ap_data tetrode_data;

   static ap_uint<1> _busy_A[CH] = {0};
   static ap_uint<1> _busy_B[CH] = {0};
//   *busy = _busy_A;

   mua_struct mua;
   spk_struct spk_pre, spk_post;

   int frameNo;
   mua = mua_stream.read();
   ch_groupNo = mua.data.range(183,176);
   ch_last_in_group = mua.data.range(175,168);
   ch_w = mua.data.range(167,160);
   frameNo = mua.data.range(159,128);
   tetrode_data = mua.data.range(127,0);

   t = frameNo % T;
   buf_2d[t][ch_w] = tetrode_data;

   pre  = buf_2d[(ap_uint<5>)(t-prelen)][ch_w];
   post = buf_2d[(ap_uint<5>)(t       )][ch_w];
   // for tetrode:
   is_peak = (post[0] | post[32] | post[64] | post[96]) & (ch_w==ch_last_in_group);

   spk_pre.data = pre;   spk_post.data = post;
   spk_pre.last = 0;     spk_post.last = 0;

//////////////////////////////////////////////////
////// output state and cnt for FSM debugging ////
//	   *_state = state_A[ch_w];               ////
//	   *_cnt   = cnt_A[ch_w];                 ////
//////////////////////////////////////////////////

   FSM_Bank_A:
   switch(state_A[ch_w]){
	   case IDLE:
		   if(is_peak){
			    spk_pre.dest = 0;
			   spk_post.dest = 0;
			    spk_pre.user = 0;
			   spk_post.user = prelen;
			    spk_pre.id = (ch_groupNo, ch_w);
			   spk_post.id = (ch_groupNo, ch_w);
			    spk_pre.last = 0;
			   spk_post.last = 0;
			    out_pre.write(spk_pre);             // write 0   (start point)
			   out_post.write(spk_post);            // write 8   (turning point)
			   state_A[ch_w] = PRE;
			   cnt_A[ch_w] += 1;
		   }
		   else {
			   state_A[ch_w] = IDLE;
			   cnt_A[ch_w]   = 0;
			   _busy_A[ch_w]   = 0;
		   }
		   break;

	   case PRE:
		   if(is_peak)  {
			   _busy_A[ch_w] = 1;
		   }

		   if(cnt_A[ch_w]<prelen-1){
			    spk_pre.dest = 0;
			   spk_post.dest = 0;
			    spk_pre.user = cnt_A[ch_w];         // write 1-6
			   spk_post.user = cnt_A[ch_w]+prelen;  // write 9-14
			    spk_pre.id = (ch_groupNo, ch_w);
			   spk_post.id = (ch_groupNo, ch_w);
			    spk_pre.last = 0;
			   spk_post.last = 0;
			    out_pre.write(spk_pre);
			   out_post.write(spk_post);
			   cnt_A[ch_w] += 1;
			   state_A[ch_w] = PRE;
		   }
		   else if(cnt_A[ch_w]==prelen-1){
			    spk_pre.dest = 0;
			   spk_post.dest = 0;
			    spk_pre.user = cnt_A[ch_w];
			   spk_post.user = cnt_A[ch_w]+prelen;
			    spk_pre.id = (ch_groupNo, ch_w);
			   spk_post.id = (ch_groupNo, ch_w);
			    spk_pre.last = 1;                   //             (pre last)
			   spk_post.last = 0;
			    out_pre.write(spk_pre);             // write 7     (7 is pre last)
			   out_post.write(spk_post);            // write 15
			   cnt_A[ch_w] += 1;
			   state_A[ch_w] = POST;
		   }
		   else{
			   state_A[ch_w] = IDLE;
			   cnt_A[ch_w] = 0;
		   }
		   break;

	   case POST:
		   if(is_peak)  {
			   _busy_A[ch_w] = 1;
		   }

		   if(cnt_A[ch_w]<spklen-prelen-1){
			   spk_post.dest = 0;
			   spk_post.user = cnt_A[ch_w]+prelen;
			   spk_post.id = (ch_groupNo, ch_w);
			   spk_post.last = 0;
			   out_post.write(spk_post);           // write 16-17
			   cnt_A[ch_w] += 1;
			   state_A[ch_w] = POST;
		   }
		   else if(cnt_A[ch_w]==spklen-prelen-1){
			   spk_post.dest = 0;
			   spk_post.user = cnt_A[ch_w]+prelen;
			   spk_post.id = (ch_groupNo, ch_w);
			   spk_post.last = 1;
			   time_stamp.write(frameNo-(spklen-prelen-1));
			   out_post.write(spk_post);           // write 18   (post last)
			   state_A[ch_w] = IDLE;
			   cnt_A[ch_w] = 0;
		   }
		   else{
			   state_A[ch_w] = IDLE;
			   cnt_A[ch_w] = 0;
		   }
		   break;
   }



   FSM_Bank_B:
   switch(state_B[ch_w]){
	   case IDLE:
		   if(is_peak && _busy_A[ch_w]){
			    spk_pre.dest = 1;
			   spk_post.dest = 1;
			    spk_pre.user = 0;
			   spk_post.user = prelen;
			    spk_pre.id = ch_w;
			   spk_post.id = ch_w;
			    spk_pre.last = 0;
			   spk_post.last = 0;
			    out_pre.write(spk_pre);             // write 0   (start point)
			   out_post.write(spk_post);            // write 8   (turning point)
			   state_B[ch_w] = PRE;
			   cnt_B[ch_w] += 1;
		   }
		   else {
			   state_B[ch_w] = IDLE;
			   cnt_B[ch_w]   = 0;
			   _busy_B[ch_w]   = 0;
		   }
		   break;

	   case PRE:
		   if(is_peak)  {
			   _busy_B[ch_w] = 1;
		   }

		   if(cnt_B[ch_w]<prelen-1){
			    spk_pre.dest = 1;
			   spk_post.dest = 1;
			    spk_pre.user = cnt_B[ch_w];         // write 1-6
			   spk_post.user = cnt_B[ch_w]+prelen;  // write 9-14
			    spk_pre.id = ch_w;
			   spk_post.id = ch_w;
			    spk_pre.last = 0;
			   spk_post.last = 0;
			    out_pre.write(spk_pre);
			   out_post.write(spk_post);
			   cnt_B[ch_w] += 1;
			   state_B[ch_w] = PRE;
		   }
		   else if(cnt_B[ch_w]==prelen-1){
			    spk_pre.dest = 1;
			   spk_post.dest = 1;
			    spk_pre.user = cnt_B[ch_w];
			   spk_post.user = cnt_B[ch_w]+prelen;
			    spk_pre.id = ch_w;
			   spk_post.id = ch_w;
			    spk_pre.last = 1;                   //             (pre last)
			   spk_post.last = 0;
			    out_pre.write(spk_pre);             // write 7     (7 is pre last)
			   out_post.write(spk_post);            // write 15
			   cnt_B[ch_w] += 1;
			   state_B[ch_w] = POST;
		   }
		   else{
			   state_B[ch_w] = IDLE;
			   cnt_B[ch_w] = 0;
		   }
		   break;

	   case POST:
		   if(is_peak)  {
			   _busy_A[ch_w] = 1;
		   }

		   if(cnt_B[ch_w]<spklen-prelen-1){
			   spk_post.dest = 1;
			   spk_post.user = cnt_B[ch_w]+prelen;
			   spk_post.id = ch_w;
			   spk_post.last = 0;
			   out_post.write(spk_post);           // write 16-17
			   cnt_B[ch_w] += 1;
			   state_B[ch_w] = POST;
		   }
		   else if(cnt_B[ch_w]==spklen-prelen-1){
			   spk_post.dest = 1;
			   spk_post.user = cnt_B[ch_w]+prelen;
			   spk_post.id = ch_w;
			   spk_post.last = 1;
			   time_stamp.write(frameNo-(spklen-prelen-1));
			   out_post.write(spk_post);           // write 18   (post last)
			   state_B[ch_w] = IDLE;
			   cnt_B[ch_w] = 0;
		   }
		   else{
			   state_B[ch_w] = IDLE;
			   cnt_B[ch_w] = 0;
		   }
		   break;
   }

}
