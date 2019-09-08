#include "bram_xike.h"

void bram_xike(bool _doWrite, int  din,
               bool _doRead,  int* dout, ap_uint<16> _addr,
               bool _scale_read, ap_uint<8 > _scale_addr, int * scale_out,
               bool _shift_read, ap_uint<8 > _shift_addr, int * shift_out,
               bool _pca_read  , ap_uint<16> _pca_addr  , int * pca_out,
               bool _vq_read   , ap_uint<16> _vq_addr   , int * vq_out,
               bool _label_read, ap_uint<16> _label_addr, int * label_out)
{

#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
// host interface
#pragma HLS INTERFACE ap_none port=_doWrite
#pragma HLS INTERFACE ap_none port=_addr
#pragma HLS INTERFACE ap_none port=din
#pragma HLS INTERFACE ap_none port=_doRead
#pragma HLS INTERFACE ap_vld  port=dout
// read
#pragma HLS INTERFACE ap_none port=_scale_read
#pragma HLS INTERFACE ap_none port=_shift_read
#pragma HLS INTERFACE ap_none port=_pca_read
#pragma HLS INTERFACE ap_none port=_vq_read
#pragma HLS INTERFACE ap_none port=_label_read
// addr
#pragma HLS INTERFACE ap_none port=_scale_addr
#pragma HLS INTERFACE ap_none port=_shift_addr
#pragma HLS INTERFACE ap_none port=_pca_addr
#pragma HLS INTERFACE ap_none port=_vq_addr
#pragma HLS INTERFACE ap_none port=_label_addr
// output
#pragma HLS INTERFACE ap_vld port=scale_out
#pragma HLS INTERFACE ap_vld port=shift_out
#pragma HLS INTERFACE ap_vld port=pca_out
#pragma HLS INTERFACE ap_vld port=vq_out
#pragma HLS INTERFACE ap_vld port=label_out

// ------------------ Don't change below ------------------- //
// --------------------------------------------------------- //  

   static int scale[scale_depth];
//#pragma HLS RESOURCE variable=scale core=RAM_T2P_BRAM
//#pragma HLS DEPENDENCE variable=scale intra false

   static int shift[shift_depth];
//#pragma HLS RESOURCE variable=shift core=RAM_T2P_BRAM
//#pragma HLS DEPENDENCE variable=shift intra false

   static int pca[pca_depth];
//#pragma HLS RESOURCE variable=pca core=RAM_T2P_BRAM
//#pragma HLS DEPENDENCE variable=pca intra false

   static int vq[vq_depth];
//#pragma HLS RESOURCE variable=vq core=RAM_T2P_BRAM
//#pragma HLS DEPENDENCE variable=vq intra false

   static int label[label_depth];
//#pragma HLS RESOURCE variable=label core=RAM_T2P_BRAM
//#pragma HLS DEPENDENCE variable=label intra false

   if (_scale_read) *scale_out = scale[_scale_addr];
   if (_shift_read) *shift_out = shift[_shift_addr];
   if (_pca_read  ) *pca_out   = pca[_pca_addr];
   if (_vq_read   ) *vq_out    = vq[_vq_addr];
   if (_label_read) *label_out = label[_label_addr];

   enum mem_selected {mem_scale=0, mem_shift, mem_pca, mem_vq, mem_label};
   mem_selected mem_state;

   mem_state_1_read_write_host_addr:{
       if (_addr < shift_base_addr)
           mem_state = mem_scale;
       else if(_addr>=shift_base_addr && _addr<pca_base_addr)
           mem_state = mem_shift;
       else if(_addr>=pca_base_addr && _addr<vq_base_addr)
           mem_state = mem_pca;
       else if(_addr>=vq_base_addr && _addr<label_base_addr)
           mem_state = mem_vq;
       else if(_addr>=label_base_addr)
           mem_state = mem_label;
   }

   if (_doWrite) {
       switch (mem_state) {
        case mem_scale:
            scale[_addr] = din;
            break;
        case mem_shift:
            shift[_addr-shift_base_addr] = din;
            break;
        case mem_pca:
            pca[_addr-pca_base_addr] = din;
            break;
        case mem_vq:
            vq[_addr-vq_base_addr] = din;
            break;
        case mem_label:
            label[_addr-label_base_addr] = din;
       }
   }

   else if (_doRead) {
       switch (mem_state) {
        case mem_scale:
            *dout = scale[_addr];
            break;
        case mem_shift:
            *dout = shift[_addr-shift_base_addr];
            break;
        case mem_pca:
            *dout = pca[_addr-pca_base_addr];
            break;
        case mem_vq:
            *dout = vq[_addr-vq_base_addr];
            break;
        case mem_label:
            *dout = label[_addr-label_base_addr];
       }
   }
}
