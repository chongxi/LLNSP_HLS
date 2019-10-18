set C_TypeInfoList {{ 
"rt_feedback" : [[], { "return": [[], "void"]} , [{"ExternC" : 0}], [ {"fet_packet_spk_id": [[], {"array": ["0", [7]]}] }, {"spk_id_out": [[], {"reference": "1"}] }],[],""], 
"1": [ "stream<ap_uint<32> >", {"hls_type": {"stream": [[[[],"2"]],"3"]}}], 
"0": [ "ap32_data", {"typedef": [[[],"2"],""]}], 
"2": [ "ap_uint<32>", {"hls_type": {"ap_uint": [[[[], {"scalar": { "int": 32}}]],""]}}],
"3": ["hls", ""]
}}
set moduleName rt_feedback
set isCombinational 0
set isDatapathOnly 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set C_modelName {rt_feedback}
set C_modelType { void 0 }
set C_modelArgList {
	{ fet_packet_spk_id_V int 32 regular {fifo 0 volatile }  }
	{ spk_id_out_V_V int 32 regular {axi_s 1 volatile  { spk_id_out_V_V data } }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "fet_packet_spk_id_V", "interface" : "fifo", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "fet_packet_spk_id.V","cData": "uint32","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 6,"step" : 1}]}]}]} , 
 	{ "Name" : "spk_id_out_V_V", "interface" : "axis", "bitwidth" : 32, "direction" : "WRITEONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "spk_id_out.V.V","cData": "uint32","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 0,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 12
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst_n sc_in sc_logic 1 reset -1 active_low_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ fet_packet_spk_id_V_dout sc_in sc_lv 32 signal 0 } 
	{ fet_packet_spk_id_V_empty_n sc_in sc_logic 1 signal 0 } 
	{ fet_packet_spk_id_V_read sc_out sc_logic 1 signal 0 } 
	{ spk_id_out_V_V_TDATA sc_out sc_lv 32 signal 1 } 
	{ spk_id_out_V_V_TVALID sc_out sc_logic 1 outvld 1 } 
	{ spk_id_out_V_V_TREADY sc_in sc_logic 1 outacc 1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst_n", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "fet_packet_spk_id_V_dout", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "fet_packet_spk_id_V", "role": "dout" }} , 
 	{ "name": "fet_packet_spk_id_V_empty_n", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "fet_packet_spk_id_V", "role": "empty_n" }} , 
 	{ "name": "fet_packet_spk_id_V_read", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "fet_packet_spk_id_V", "role": "read" }} , 
 	{ "name": "spk_id_out_V_V_TDATA", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "spk_id_out_V_V", "role": "TDATA" }} , 
 	{ "name": "spk_id_out_V_V_TVALID", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "outvld", "bundle":{"name": "spk_id_out_V_V", "role": "TVALID" }} , 
 	{ "name": "spk_id_out_V_V_TREADY", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "outacc", "bundle":{"name": "spk_id_out_V_V", "role": "TREADY" }}  ]}

set RtlHierarchyInfo {[
	{"Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : [], "CDFG" : "rt_feedback", "VariableLatency" : "1", "AlignedPipeline" : "0", "UnalignedPipeline" : "0", "ProcessNetwork" : "0", "Combinational" : "0", "ControlExist" : "1",
		"Port" : [
		{"Name" : "fet_packet_spk_id_V", "Type" : "Fifo", "Direction" : "I", "BlockSignal" : [
			{"Name" : "fet_packet_spk_id_V_blk_n", "Type" : "RtlSignal"}], "SubConnect" : []}, 
		{"Name" : "spk_id_out_V_V", "Type" : "Axis", "Direction" : "O", "BlockSignal" : [
			{"Name" : "spk_id_out_V_V_TDATA_blk_n", "Type" : "RtlSignal"}], "SubConnect" : []}],
		"WaitState" : [],
		"SubBlockPort" : []}]}

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "17", "Max" : "250017"}
	, {"Name" : "Interval", "Min" : "18", "Max" : "250018"}
]}

set Spec2ImplPortList { 
	fet_packet_spk_id_V { ap_fifo {  { fet_packet_spk_id_V_dout fifo_data 0 32 }  { fet_packet_spk_id_V_empty_n fifo_status 0 1 }  { fet_packet_spk_id_V_read fifo_update 1 1 } } }
	spk_id_out_V_V { axis {  { spk_id_out_V_V_TDATA out_data 1 32 }  { spk_id_out_V_V_TVALID out_vld 1 1 }  { spk_id_out_V_V_TREADY out_acc 0 1 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
	fet_packet_spk_id_V { fifo_read 14 has_conditional }
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
