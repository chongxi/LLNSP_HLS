#include "hls_design_meta.h"
const Port_Property HLS_Design_Meta::port_props[]={
	Port_Property("ap_clk", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_rst_n", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_start", 1, hls_in, -1, "", "", 1),
	Port_Property("ap_done", 1, hls_out, -1, "", "", 1),
	Port_Property("ap_idle", 1, hls_out, -1, "", "", 1),
	Port_Property("ap_ready", 1, hls_out, -1, "", "", 1),
	Port_Property("fet_packet_spk_id_V_dout", 32, hls_in, 0, "ap_fifo", "fifo_data", 14),
	Port_Property("fet_packet_spk_id_V_empty_n", 1, hls_in, 0, "ap_fifo", "fifo_status", 14),
	Port_Property("fet_packet_spk_id_V_read", 1, hls_out, 0, "ap_fifo", "fifo_update", 14),
	Port_Property("spk_id_out_V_V_TDATA", 32, hls_out, 1, "axis", "out_data", 1),
	Port_Property("spk_id_out_V_V_TVALID", 1, hls_out, 1, "axis", "out_vld", 1),
	Port_Property("spk_id_out_V_V_TREADY", 1, hls_in, 1, "axis", "out_acc", 1),
};
const char* HLS_Design_Meta::dut_name = "rt_feedback";
