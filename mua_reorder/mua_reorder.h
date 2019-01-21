#include "ap_int.h"
#include "ap_utils.h"
#include "ap_fixed.h"
#include <hls_stream.h>


#define channel_per_chip  32
#define chip_No 5
#define CH channel_per_chip * chip_No  // change to 8 for C simulation

typedef ap_fixed<32,19>  muar_type;
typedef ap_fixed<32,19>  muao_type;

struct muar_struct{
  muar_type data;
};

struct muao_struct{
  muao_type data;
};

void mua_reorder(hls::stream<muar_struct> &muar_stream,
                 hls::stream<muao_struct> &muao_stream);
