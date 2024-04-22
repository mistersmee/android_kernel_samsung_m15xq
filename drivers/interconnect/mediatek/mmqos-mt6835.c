// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 * Author: Ming-Fan Chen <ming-fan.chen@mediatek.com>
 */
#include <dt-bindings/interconnect/mtk,mmqos.h>
#include <dt-bindings/interconnect/mtk,mt6873-emi.h>
#include <dt-bindings/memory/mt6835-larb-port.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include "mmqos-mtk.h"
static const struct mtk_node_desc node_descs_mt6835[] = {
	DEFINE_MNODE(common0,
		SLAVE_COMMON(0), 0, false, 0x0, MMQOS_NO_LINK),
	DEFINE_MNODE(common0_port0,
		MASTER_COMMON_PORT(0, 0), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port1,
		MASTER_COMMON_PORT(0, 1), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port2,
		MASTER_COMMON_PORT(0, 2), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port3,
		MASTER_COMMON_PORT(0, 3), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port4,
		MASTER_COMMON_PORT(0, 4), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port5,
		MASTER_COMMON_PORT(0, 5), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port6,
		MASTER_COMMON_PORT(0, 6), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port7,
		MASTER_COMMON_PORT(0, 7), 0, false, 0x0, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port8,
		MASTER_COMMON_PORT(0, 8), 0, false, 0x0, SLAVE_COMMON(0)),
    /*SMI Common*/
	DEFINE_MNODE(larb0, SLAVE_LARB(0), 0, false, 0x0, MASTER_COMMON_PORT(0, 0)),
	DEFINE_MNODE(larb1, SLAVE_LARB(1), 0, false, 0x0, MASTER_COMMON_PORT(0, 1)),
	DEFINE_MNODE(larb2, SLAVE_LARB(2), 0, false, 0x0, MASTER_COMMON_PORT(0, 2)),
	DEFINE_MNODE(larb9, SLAVE_LARB(9), 0, false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(larb11, SLAVE_LARB(11), 0, false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(larb20, SLAVE_LARB(20), 0, false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(larb7, SLAVE_LARB(7), 0, false, 0x0, MASTER_COMMON_PORT(0, 4)),
	DEFINE_MNODE(larb4, SLAVE_LARB(4), 0, false, 0x0, MASTER_COMMON_PORT(0, 5)),
	DEFINE_MNODE(larb13, SLAVE_LARB(13), 0, false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(larb17, SLAVE_LARB(17), 0, false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(larb14, SLAVE_LARB(14), 0, false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(larb16, SLAVE_LARB(16), 0, false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(larb30, SLAVE_LARB(21), 0, false, 0x1, MASTER_COMMON_PORT(0, 8)), //virt DISP
	/*Larb 0*/
	DEFINE_MNODE(l0_r_disp_postmask0,
		MASTER_LARB_PORT(M4U_PORT_L0_DISP_POSTMASK0), 8, false, 0x1, SLAVE_LARB(0)),
	DEFINE_MNODE(l0_r_disp_ovl0,
		MASTER_LARB_PORT(M4U_PORT_L0_OVL_RDMA0), 8, false, 0x1, SLAVE_LARB(0)),
	DEFINE_MNODE(l0_r_disp_ovl0_hdr,
		MASTER_LARB_PORT(M4U_PORT_L0_OVL_RDMA0_HDR), 7, false, 0x1, SLAVE_LARB(0)),
	DEFINE_MNODE(l0_w_disp_wdma0,
		MASTER_LARB_PORT(M4U_PORT_L0_DISP_WDMA0), 9, true, 0x1, SLAVE_LARB(0)),
	DEFINE_MNODE(l0_r_disp_fake_eng0,
		MASTER_LARB_PORT(M4U_PORT_L0_DISP_FAKE1), 7, false, 0x1, SLAVE_LARB(0)),
	/*Larb 1*/
	DEFINE_MNODE(l1_r_disp_rdma0,
		MASTER_LARB_PORT(M4U_PORT_L1_DISP_RDMA0), 7, false, 0x2,  SLAVE_LARB(1)),
	DEFINE_MNODE(l1_r_disp_ovl1_2l,
		MASTER_LARB_PORT(M4U_PORT_L1_OVL_2L_RDMA0), 8, false, 0x2, SLAVE_LARB(1)),
	DEFINE_MNODE(l1_r_disp_ovl0_2l_hdr,
		MASTER_LARB_PORT(M4U_PORT_L1_OVL_2L_RDMA0_HDR), 7, false, 0x2, SLAVE_LARB(1)),
	DEFINE_MNODE(l1_w_disp_ufbc_wdma0,
		MASTER_LARB_PORT(M4U_PORT_L1_DISP_UFBC_WDMA0), 9, true, 0x2, SLAVE_LARB(1)),
	DEFINE_MNODE(l1_r_disp_fake_eng1,
		MASTER_LARB_PORT(M4U_PORT_L1_DISP_FAKE1), 7, false, 0x2, SLAVE_LARB(1)),
	/*LARB 2*/
	DEFINE_MNODE(l2_r_mdp_rdma0,
		MASTER_LARB_PORT(M4U_PORT_L2_MDP_RDMA0), 7, false, 0x1, SLAVE_LARB(2)),
	DEFINE_MNODE(l2_w_mdp_wrot0,
		MASTER_LARB_PORT(M4U_PORT_L2_MDP_WROT0), 7, true, 0x1, SLAVE_LARB(2)),
	DEFINE_MNODE(l2_w_mdp_wrot2,
		MASTER_LARB_PORT(M4U_PORT_L2_MDP_WROT2), 7, true, 0x1, SLAVE_LARB(2)),
	DEFINE_MNODE(l2_r_mdp_fake_eng0,
		MASTER_LARB_PORT(M4U_PORT_L2_MDP_FAKE0), 7, false, 0x1, SLAVE_LARB(2)),
	/*Larb 4*/
	DEFINE_MNODE(l4_r_hw_vdec_mc_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_MC_EXT), 6, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_w_hw_vdec_pp_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_PP_EXT), 8, true, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_pred_rd_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_PRED_RD_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_pred_wr_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_PRED_WR_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_ppwrap_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_PPWRAP_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_tile_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_TILE_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_vld_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_VLD_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_vld2_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_VLD2_EXT), 7, false, 0x1, SLAVE_LARB(4)),
	DEFINE_MNODE(l4_r_hw_vdec_avc_mv_ext,
		MASTER_LARB_PORT(M4U_PORT_L4_HW_VDEC_AVC_MV_EXT), 6, false, 0x1, SLAVE_LARB(4)),
	/*larb7*/
	DEFINE_MNODE(l7_r_venc_rcpu,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_RCPU), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_w_venc_rec,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_REC), 8, true, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_w_venc_bsdma,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_BSDMA), 8, true, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_w_venc_sv_comv,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_SV_COMV), 6, true, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_venc_rd_comv,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_RD_COMV), 6, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_venc_cur_luma,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_CUR_LUMA), 6, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_venc_cur_chroma,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_CUR_CHROMA), 5, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_venc_ref_luma,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_REF_LUMA), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_venc_ref_chroma,
		MASTER_LARB_PORT(M4U_PORT_L7_VENC_REF_CHROMA), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_jpgenc_y_rdma,
		MASTER_LARB_PORT(M4U_PORT_L7_JPGENC_Y_RDMA), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_jpgenc_c_rdma,
		MASTER_LARB_PORT(M4U_PORT_L7_JPGENC_C_RDMA), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_r_jpgenc_q_table,
		MASTER_LARB_PORT(M4U_PORT_L7_JPGENC_Q_TABLE), 7, false, 0x1, SLAVE_LARB(7)),
	DEFINE_MNODE(l7_w_jpgenc_bsdma,
		MASTER_LARB_PORT(M4U_PORT_L7_JPGENC_BSDMA), 8, true, 0x1, SLAVE_LARB(7)),
	/*LARB 9*/
	DEFINE_MNODE(l9_r_imgi_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_IMGI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_imgbi_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_IMGBI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_dmgi_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_DMGI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_depi_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_UFDI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_lce_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_LCEI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_smti_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_SMTI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_w_smto_d2,
		MASTER_LARB_PORT(M4U_PORT_L9_SMTO_D2), 8, true, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_w_smto_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_SMTO_D1), 8, true, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_w_crzo_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_CRZO_D1), 8, true, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_w_img3o_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_IMG3O_D1), 8, true, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_vipi_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_VIPI_D1), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_r_smti_d5,
		MASTER_LARB_PORT(M4U_PORT_L9_SMTI_D5), 7, false, 0x2, SLAVE_LARB(9)),
	DEFINE_MNODE(l9_w_timgo_d1,
		MASTER_LARB_PORT(M4U_PORT_L9_TIMGO_D1), 8, true, 0x2, SLAVE_LARB(9)),
	/*LARB 13*/
	DEFINE_MNODE(l13_w_camsv_top_1_imgo,
		MASTER_LARB_PORT(M4U_PORT_L13_CAMSV_TOP_1_IMGO), 8, true, 0x2, SLAVE_LARB(13)),
	DEFINE_MNODE(l13_w_camsv_top_2_imgo,
		MASTER_LARB_PORT(M4U_PORT_L13_CAMSV_TOP_2_IMGO), 8, true, 0x2, SLAVE_LARB(13)),
	DEFINE_MNODE(l13_w_camsv_top_3_imgo,
		MASTER_LARB_PORT(M4U_PORT_L13_CAMSV_TOP_3_IMGO), 8, true, 0x2, SLAVE_LARB(13)),
	DEFINE_MNODE(l13_r_fake,
		MASTER_LARB_PORT(M4U_PORT_L13_FAKE), 7, false, 0x2, SLAVE_LARB(13)),
	/*LARB 16*/
	DEFINE_MNODE(l16_w_imgo_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_IMGO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_rrzo_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_RRZO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_cqi_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_CQI_R1_A), 7, false, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_bpci_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_BPCI_R1_A), 7, false, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_yuvo_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_YUVO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_ufdi_r2_a,
		MASTER_LARB_PORT(M4U_PORT_L16_UFDI_R2_A), 7, false, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_rawi_r2_a,
		MASTER_LARB_PORT(M4U_PORT_L16_RAWI_R2_A), 7, false, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_rawi_r3_a,
		MASTER_LARB_PORT(M4U_PORT_L16_RAWI_R3_A), 7, false, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_aao_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_AAO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_afo_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_AFO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_flko_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_FLKO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_lceso_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_LCESO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_crzo_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_CRZO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_ltmso_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_LTMSO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_rsso_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_RSSO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_w_aaho_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_AAHO_R1_A), 8, true, 0x1, SLAVE_LARB(16)),
	DEFINE_MNODE(l16_r_lsci_r1_a,
		MASTER_LARB_PORT(M4U_PORT_L16_LSCI_R1_A), 7, false, 0x1, SLAVE_LARB(16)),
	/*LARB 17*/
	DEFINE_MNODE(l17_w_imgo_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_IMGO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_rrzo_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_RRZO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_cqi_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_CQI_R1_B), 7, false, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_bpci_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_BPCI_R1_B), 7, false, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_yuvo_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_YUVO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_ufdi_r2_b,
		MASTER_LARB_PORT(M4U_PORT_L17_UFDI_R2_B), 7, false, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_rawi_r2_b,
		MASTER_LARB_PORT(M4U_PORT_L17_RAWI_R2_B), 7, false, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_rawi_r3_b,
		MASTER_LARB_PORT(M4U_PORT_L17_RAWI_R3_B), 7, false, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_aao_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_AAO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_afo_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_AFO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_flko_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_FLKO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_lceso_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_LCESO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_crzo_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_CRZO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_ltmso_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_LTMSO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_rsso_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_RSSO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_w_aaho_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_AAHO_R1_B), 8, true, 0x2, SLAVE_LARB(17)),
	DEFINE_MNODE(l17_r_lsci_r1_b,
		MASTER_LARB_PORT(M4U_PORT_L17_LSCI_R1_B), 7, false, 0x2, SLAVE_LARB(17)),
	/*LARB 20*/
	DEFINE_MNODE(l20_r_fdvt_rda,
		MASTER_LARB_PORT(M4U_PORT_L20_FDVT_RDA), 7, false, 0x2, SLAVE_LARB(20)),
	DEFINE_MNODE(l20_r_fdvt_rdb,
		MASTER_LARB_PORT(M4U_PORT_L20_FDVT_RDB), 7, false, 0x2, SLAVE_LARB(20)),
	DEFINE_MNODE(l20_w_fdvt_wra,
		MASTER_LARB_PORT(M4U_PORT_L20_FDVT_WRA), 8, true, 0x2, SLAVE_LARB(20)),
	DEFINE_MNODE(l20_w_fdvt_wrb,
		MASTER_LARB_PORT(M4U_PORT_L20_FDVT_WRB), 8, true, 0x2, SLAVE_LARB(20)),

};
static const char * const comm_muxes_mt6835[] = { "mm" };
static const char * const comm_icc_path_names_mt6835[] = { "icc-bw" };
static const char * const comm_icc_hrt_path_names_mt6835[] = { "icc-hrt-bw" };
static const struct mtk_mmqos_desc mmqos_desc_mt6835 = {
	.nodes = node_descs_mt6835,
	.num_nodes = ARRAY_SIZE(node_descs_mt6835),
	.comm_muxes = comm_muxes_mt6835,
	.comm_icc_path_names = comm_icc_path_names_mt6835,
	.comm_icc_hrt_path_names = comm_icc_hrt_path_names_mt6835,
	.max_ratio = 72,
	.hrt = {
		.hrt_bw = {4360, 0, 0},
		.hrt_total_bw = 17064, /*Todo: Use DRAMC API 4266*2(channel)*2(io width)*/
		.md_speech_bw = {4937, 5523},
		.hrt_ratio = {1000, 880, 900, 1000}, /* MD, CAM, DISP, MML */
		.blocking = true,
		.emi_ratio = 745,
	},
	.hrt_LPDDR4 = {
		.hrt_bw = {4360, 0, 0},
		.hrt_total_bw = 17064, /*Todo: Use DRAMC API 4266*2(channel)*2(io width)*/
		.md_speech_bw = {4937, 5523},
		.hrt_ratio = {1000, 880, 900, 1000}, /* MD, CAM, DISP, MML */
		.blocking = true,
		.emi_ratio = 745,
	},
	.comm_port_channels = {
		{ 0x1, 0x2, 0x1, 0x2, 0x1, 0x1, 0x2, 0x1, 0x3}
	},
	.comm_port_hrt_types = {
		{ HRT_MAX_BWL, HRT_MAX_BWL, HRT_NONE, HRT_NONE, HRT_NONE,
			HRT_NONE, HRT_CAM, HRT_CAM, HRT_DISP },
	},
	.mmqos_state = MMQOS_ENABLE | BWL_MIN_ENABLE | BWL_NO_QOSBOUND,
};
static const struct of_device_id mtk_mmqos_mt6835_of_ids[] = {
	{
		.compatible = "mediatek,mt6835-mmqos",
		.data = &mmqos_desc_mt6835,
	},
	{}
};
MODULE_DEVICE_TABLE(of, mtk_mmqos_mt6835_of_ids);
static struct platform_driver mtk_mmqos_mt6835_driver = {
	.probe = mtk_mmqos_probe,
	.remove = mtk_mmqos_remove,
	.driver = {
		.name = "mtk-mt6835-mmqos",
		.of_match_table = mtk_mmqos_mt6835_of_ids,
	},
};
module_platform_driver(mtk_mmqos_mt6835_driver);
MODULE_LICENSE("GPL v2");

