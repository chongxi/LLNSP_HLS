############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
############################################################
open_project rt_feedback
set_top rt_feedback
add_files rt_feedback.cpp
add_files rt_feedback.h
add_files -tb rt_feedback_test.cpp
open_solution "solution1"
set_part {xc7k325tffg900-2} -tool vivado
create_clock -period 4 -name default
#source "./rt_feedback/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -evaluate verilog -format ip_catalog
