; ModuleID = '/disk0/Work/xike_hls_module/rt_feedback/rt_feedback/solution1/.autopilot/db/a.o.2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@rt_feedback_str = internal unnamed_addr constant [12 x i8] c"rt_feedback\00"
@llvm_global_ctors_1 = appending global [1 x void ()*] [void ()* @_GLOBAL__I_a]
@llvm_global_ctors_0 = appending global [1 x i32] [i32 65535]
@p_str3 = private unnamed_addr constant [15 x i8] c"read_packet_in\00", align 1
@p_str2 = private unnamed_addr constant [5 x i8] c"axis\00", align 1
@p_str1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@p_str = private unnamed_addr constant [8 x i8] c"ap_fifo\00", align 1

define void @rt_feedback(i32* %fet_packet_spk_id_V, i32* %spk_id_out_V_V) {
  %val_assign = alloca i32
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %fet_packet_spk_id_V), !map !39
  call void (...)* @_ssdm_op_SpecBitsMap(i32* %spk_id_out_V_V), !map !45
  call void (...)* @_ssdm_op_SpecTopModule([12 x i8]* @rt_feedback_str) nounwind
  call void (...)* @_ssdm_op_SpecInterface(i32* %fet_packet_spk_id_V, [8 x i8]* @p_str, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str1)
  call void (...)* @_ssdm_op_SpecInterface(i32* %spk_id_out_V_V, [5 x i8]* @p_str2, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str1, [1 x i8]* @p_str1, [1 x i8]* @p_str1, i32 0, i32 0, i32 0, i32 0, [1 x i8]* @p_str1) nounwind
  store i32 -1, i32* %val_assign
  br label %1

; <label>:1                                       ; preds = %._crit_edge24, %0
  %i = phi i3 [ 0, %0 ], [ %i_1, %._crit_edge24 ]
  %exitcond1 = icmp eq i3 %i, -1
  %i_1 = add i3 %i, 1
  br i1 %exitcond1, label %5, label %2

; <label>:2                                       ; preds = %1
  %empty = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 7, i64 7, i64 7)
  call void (...)* @_ssdm_op_SpecLoopName([15 x i8]* @p_str3) nounwind
  %tmp_3 = call i32 (...)* @_ssdm_op_SpecRegionBegin([15 x i8]* @p_str3)
  call void (...)* @_ssdm_op_SpecPipeline(i32 -1, i32 1, i32 1, i32 0, [1 x i8]* @p_str1) nounwind
  %tmp_1 = icmp eq i3 %i, 0
  br i1 %tmp_1, label %3, label %._crit_edge

; <label>:3                                       ; preds = %2
  %p_Val2_s = call i32 @_ssdm_op_Read.ap_fifo.volatile.i32P(i32* %fet_packet_spk_id_V)
  br label %._crit_edge

._crit_edge:                                      ; preds = %3, %2
  %tmp_2 = icmp eq i3 %i, -2
  br i1 %tmp_2, label %4, label %._crit_edge24

; <label>:4                                       ; preds = %._crit_edge
  %p_Val2_2 = call i32 @_ssdm_op_Read.ap_fifo.volatile.i32P(i32* %fet_packet_spk_id_V)
  store i32 %p_Val2_2, i32* %val_assign
  br label %._crit_edge24

._crit_edge24:                                    ; preds = %4, %._crit_edge
  %empty_3 = call i32 (...)* @_ssdm_op_SpecRegionEnd([15 x i8]* @p_str3, i32 %tmp_3)
  br label %1

; <label>:5                                       ; preds = %1
  %val_assign_load = load i32* %val_assign
  %tmp = icmp eq i32 %val_assign_load, 888
  br i1 %tmp, label %.preheader, label %.loopexit

.preheader:                                       ; preds = %5, %6
  %j = phi i18 [ %j_1, %6 ], [ 0, %5 ]
  %exitcond = icmp eq i18 %j, -12144
  %empty_4 = call i32 (...)* @_ssdm_op_SpecLoopTripCount(i64 250000, i64 250000, i64 250000)
  %j_1 = add i18 %j, 1
  br i1 %exitcond, label %.loopexit, label %6

; <label>:6                                       ; preds = %.preheader
  call void @_ssdm_op_Write.axis.volatile.i32P(i32* %spk_id_out_V_V, i32 888)
  br label %.preheader

.loopexit:                                        ; preds = %.preheader, %5
  ret void
}

declare void @llvm.dbg.value(metadata, i64, metadata) nounwind readnone

declare void @llvm.dbg.declare(metadata, metadata) nounwind readnone

define weak void @_ssdm_op_Write.axis.volatile.i32P(i32*, i32) {
entry:
  store i32 %1, i32* %0
  ret void
}

define weak void @_ssdm_op_SpecTopModule(...) {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecRegionEnd(...) {
entry:
  ret i32 0
}

define weak i32 @_ssdm_op_SpecRegionBegin(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecPipeline(...) nounwind {
entry:
  ret void
}

define weak i32 @_ssdm_op_SpecLoopTripCount(...) {
entry:
  ret i32 0
}

define weak void @_ssdm_op_SpecLoopName(...) nounwind {
entry:
  ret void
}

define weak void @_ssdm_op_SpecInterface(...) nounwind {
entry:
  ret void
}

define weak void @_ssdm_op_SpecBitsMap(...) {
entry:
  ret void
}

define weak i32 @_ssdm_op_Read.ap_fifo.volatile.i32P(i32*) {
entry:
  %empty = call i32 @_autotb_FifoRead_i32(i32* %0)
  ret i32 %empty
}

declare i32 @_autotb_FifoRead_i32(i32*)

declare void @_GLOBAL__I_a() nounwind section ".text.startup"

!opencl.kernels = !{!0, !7, !7, !13, !13, !19, !13, !13, !22, !26, !26}
!hls.encrypted.func = !{}
!llvm.map.gv = !{!32}

!0 = metadata !{null, metadata !1, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!1 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 0}
!2 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none"}
!3 = metadata !{metadata !"kernel_arg_type", metadata !"ap32_data*", metadata !"hls::stream<ap32_data> &"}
!4 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !""}
!5 = metadata !{metadata !"kernel_arg_name", metadata !"fet_packet_spk_id", metadata !"spk_id_out"}
!6 = metadata !{metadata !"reqd_work_group_size", i32 1, i32 1, i32 1}
!7 = metadata !{null, metadata !8, metadata !9, metadata !10, metadata !11, metadata !12, metadata !6}
!8 = metadata !{metadata !"kernel_arg_addr_space", i32 0}
!9 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none"}
!10 = metadata !{metadata !"kernel_arg_type", metadata !"int"}
!11 = metadata !{metadata !"kernel_arg_type_qual", metadata !""}
!12 = metadata !{metadata !"kernel_arg_name", metadata !"val"}
!13 = metadata !{null, metadata !14, metadata !15, metadata !16, metadata !17, metadata !18, metadata !6}
!14 = metadata !{metadata !"kernel_arg_addr_space"}
!15 = metadata !{metadata !"kernel_arg_access_qual"}
!16 = metadata !{metadata !"kernel_arg_type"}
!17 = metadata !{metadata !"kernel_arg_type_qual"}
!18 = metadata !{metadata !"kernel_arg_name"}
!19 = metadata !{null, metadata !8, metadata !9, metadata !20, metadata !11, metadata !21, metadata !6}
!20 = metadata !{metadata !"kernel_arg_type", metadata !"const struct ap_uint<32> &"}
!21 = metadata !{metadata !"kernel_arg_name", metadata !"din"}
!22 = metadata !{null, metadata !23, metadata !2, metadata !24, metadata !4, metadata !25, metadata !6}
!23 = metadata !{metadata !"kernel_arg_addr_space", i32 0, i32 0}
!24 = metadata !{metadata !"kernel_arg_type", metadata !"int", metadata !"int"}
!25 = metadata !{metadata !"kernel_arg_name", metadata !"Hi", metadata !"Lo"}
!26 = metadata !{null, metadata !27, metadata !28, metadata !29, metadata !30, metadata !31, metadata !6}
!27 = metadata !{metadata !"kernel_arg_addr_space", i32 1, i32 0, i32 0}
!28 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none", metadata !"none", metadata !"none"}
!29 = metadata !{metadata !"kernel_arg_type", metadata !"ap_int_base<32, false>*", metadata !"int", metadata !"int"}
!30 = metadata !{metadata !"kernel_arg_type_qual", metadata !"", metadata !"", metadata !""}
!31 = metadata !{metadata !"kernel_arg_name", metadata !"bv", metadata !"h", metadata !"l"}
!32 = metadata !{metadata !33, [1 x i32]* @llvm_global_ctors_0}
!33 = metadata !{metadata !34}
!34 = metadata !{i32 0, i32 31, metadata !35}
!35 = metadata !{metadata !36}
!36 = metadata !{metadata !"llvm.global_ctors.0", metadata !37, metadata !"", i32 0, i32 31}
!37 = metadata !{metadata !38}
!38 = metadata !{i32 0, i32 0, i32 1}
!39 = metadata !{metadata !40}
!40 = metadata !{i32 0, i32 31, metadata !41}
!41 = metadata !{metadata !42}
!42 = metadata !{metadata !"fet_packet_spk_id.V", metadata !43, metadata !"uint32", i32 0, i32 31}
!43 = metadata !{metadata !44}
!44 = metadata !{i32 0, i32 6, i32 1}
!45 = metadata !{metadata !46}
!46 = metadata !{i32 0, i32 31, metadata !47}
!47 = metadata !{metadata !48}
!48 = metadata !{metadata !"spk_id_out.V.V", metadata !37, metadata !"uint32", i32 0, i32 31}
