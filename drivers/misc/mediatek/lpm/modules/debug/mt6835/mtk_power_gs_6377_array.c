// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */


/* This file is generated by GenLP_setting.pl v1.5.7 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

const unsigned int AP_PMIC_REG_MT6377_gs_suspend_32kless_data[] = {
	/*  Address     Mask        Golden Setting Value */
	0x1A, 0xA, 0xA,/* TOP_CON */
	0x1D, 0x1F, 0x1F,/* TEST_CON0 */
	0x25, 0x1, 0x1,/* SMT_CON0 */
	0x10F, 0x10, 0x10,/* TOP_CKPDN_CON1 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON2 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON1 */
	0x11B, 0xC, 0x0,/* TOP_CKSEL_CON2 */
	0x128, 0x55, 0x0,/* TOP_CLK_CON0 */
	0x129, 0x5, 0x0,/* TOP_CLK_CON1 */
	0x141, 0x1, 0x0,/* TOP_DCXO_CKEN_SW */
	0x142, 0x1, 0x0,/* TOP_SPMI_CON0 */
	0x143, 0x1, 0x0,/* TOP_SPMI_CON1 */
	0x243, 0x8, 0x0,/* TOP_VRCTL_VR1_EN */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0_L */
	0x51A, 0x1F, 0x10,/* SCK_TOP_CKHWEN_CON0_L */
	0x54C, 0xFF, 0x21,/* XO_BUF_CTL0_L */
	0x54D, 0x3F, 0x0,/* XO_BUF_CTL0_H */
	0x54E, 0xFF, 0xC0,/* XO_BUF_CTL1_L */
	0x54F, 0x3F, 0x3,/* XO_BUF_CTL1_H */
	0x550, 0xFF, 0x0,/* XO_BUF_CTL2_L */
	0x551, 0x3F, 0x8,/* XO_BUF_CTL2_H */
	0x552, 0xFF, 0x40,/* XO_BUF_CTL3_L */
	0x553, 0x3F, 0x0,/* XO_BUF_CTL3_H */
	0x554, 0xFF, 0xC0,/* XO_BUF_CTL4_L */
	0x555, 0x3F, 0x3,/* XO_BUF_CTL4_H */
	0x788, 0xDB, 0x49,/* DCXO_CW00 */
	0x78B, 0x6, 0x4,/* DCXO_CW00_H */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x79C, 0x4, 0x0,/* DCXO_CW08 */
	0x7A1, 0x36, 0x12,/* DCXO_CW09_H */
	0x7A8, 0x80, 0x0,/* DCXO_CW12 */
	0x7A9, 0x10, 0x10,/* DCXO_CW12_H */
	0xC0B, 0x5, 0x1,/* BM_TOP_CKPDN_CON0 */
	0xC0E, 0xFD, 0xE4,/* BM_TOP_CKPDN_CON1 */
	0xC17, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC21, 0x1, 0x1,/* BM_TOP_RST_CON2 */
	0xD07, 0x3, 0x3,/* FGADC_CON0 */
	0xD08, 0x1, 0x1,/* FGADC_CON1 */
	0xD0D, 0x7F, 0x78,/* FGADC_CON6 */
	0xD37, 0xFF, 0x1F,/* FGADC_SON_CON0_L */
	0xD38, 0xFF, 0x0,/* FGADC_SON_CON0_H */
	0xD39, 0xFF, 0x14,/* FGADC_SON_CON1_L */
	0xD3A, 0x3, 0x0,/* FGADC_SON_CON1_H */
	0xD3B, 0xFF, 0xFF,/* FGADC_SON_CON2_L */
	0xD3C, 0x3, 0x0,/* FGADC_SON_CON2_H */
	0xD3D, 0xFF, 0x1F,/* FGADC_SOFF_CON0_L */
	0xD3E, 0xFF, 0x0,/* FGADC_SOFF_CON0_H */
	0xD3F, 0xFF, 0x14,/* FGADC_SOFF_CON1_L */
	0xD40, 0x3, 0x0,/* FGADC_SOFF_CON1_H */
	0xD41, 0xFF, 0xFF,/* FGADC_SOFF_CON2_L */
	0xD42, 0x3, 0x0,/* FGADC_SOFF_CON2_H */
	0xF8C, 0xFF, 0x15,/* HK_TOP_CLK_CON0 */
	0xF8D, 0x3, 0x1,/* HK_TOP_CLK_CON1 */
	0x1188, 0xE0, 0x0,/* AUXADC_CON0 */
	0x1189, 0x3F, 0x14,/* AUXADC_CON1 */
	0x1199, 0x70, 0x60,/* AUXADC_AVG_CON6 */
	0x1208, 0x1, 0x0,/* AUXADC_IMP0 */
	0x120C, 0x2, 0x0,/* AUXADC_IMP4 */
	0x120D, 0x1, 0x0,/* AUXADC_LBAT0 */
	0x1218, 0x2, 0x0,/* AUXADC_LBAT11 */
	0x1219, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x121A, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x121B, 0x2, 0x2,/* AUXADC_LBAT2_2 */
	0x121E, 0x2, 0x2,/* AUXADC_LBAT2_5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT2_11 */
	0x1225, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x1227, 0x3, 0x2,/* AUXADC_BAT_TEMP_2 */
	0x1228, 0x2, 0x2,/* AUXADC_BAT_TEMP_3 */
	0x122B, 0x2, 0x2,/* AUXADC_BAT_TEMP_6 */
	0x1231, 0x2, 0x0,/* AUXADC_BAT_TEMP_12 */
	0x1232, 0x1, 0x0,/* AUXADC_THR0 */
	0x123D, 0x2, 0x0,/* AUXADC_THR11 */
	0x1247, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x1259, 0x2, 0x0,/* AUXADC_NAG_18 */
	0x140B, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x140E, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1488, 0x10, 0x0,/* BUCK_VBUCK1_SLP_CON */
	0x148F, 0x80, 0x80,/* BUCK_VBUCK1_OP_EN_2 */
	0x1496, 0x3, 0x0,/* BUCK_VBUCK1_CON0 */
	0x1508, 0x10, 0x0,/* BUCK_VBUCK2_SLP_CON */
	0x150F, 0x80, 0x80,/* BUCK_VBUCK2_OP_EN_2 */
	0x1516, 0x3, 0x0,/* BUCK_VBUCK2_CON0 */
	0x1588, 0x10, 0x0,/* BUCK_VBUCK3_SLP_CON */
	0x158F, 0x81, 0x81,/* BUCK_VBUCK3_OP_EN_2 */
	0x1592, 0x1, 0x1,/* BUCK_VBUCK3_OP_CFG_2 */
	0x1595, 0x1, 0x1,/* BUCK_VBUCK3_OP_MODE_2 */
	0x1596, 0x3, 0x1,/* BUCK_VBUCK3_CON0 */
	0x1608, 0x10, 0x0,/* BUCK_VBUCK4_SLP_CON */
	0x160F, 0x80, 0x80,/* BUCK_VBUCK4_OP_EN_2 */
	0x1616, 0x3, 0x0,/* BUCK_VBUCK4_CON0 */
	0x1688, 0x10, 0x0,/* BUCK_VBUCK5_SLP_CON */
	0x168D, 0x1, 0x1,/* BUCK_VBUCK5_OP_EN_0 */
	0x168F, 0x80, 0x80,/* BUCK_VBUCK5_OP_EN_2 */
	0x1690, 0x1, 0x1,/* BUCK_VBUCK5_OP_CFG_0 */
	0x1693, 0x1, 0x1,/* BUCK_VBUCK5_OP_MODE_0 */
	0x1696, 0x3, 0x1,/* BUCK_VBUCK5_CON0 */
	0x1708, 0x10, 0x0,/* BUCK_VBUCK6_SLP_CON */
	0x170F, 0x81, 0x81,/* BUCK_VBUCK6_OP_EN_2 */
	0x1712, 0x1, 0x1,/* BUCK_VBUCK6_OP_CFG_2 */
	0x1715, 0x1, 0x1,/* BUCK_VBUCK6_OP_MODE_2 */
	0x1716, 0x3, 0x1,/* BUCK_VBUCK6_CON0 */
	0x1788, 0x10, 0x0,/* BUCK_VS1_SLP_CON */
	0x178D, 0x1, 0x1,/* BUCK_VS1_OP_EN_0 */
	0x178E, 0x2, 0x2,/* BUCK_VS1_OP_EN_1 */
	0x178F, 0x80, 0x80,/* BUCK_VS1_OP_EN_2 */
	0x1790, 0x1, 0x1,/* BUCK_VS1_OP_CFG_0 */
	0x1791, 0x2, 0x0,/* BUCK_VS1_OP_CFG_1 */
	0x1793, 0x1, 0x1,/* BUCK_VS1_OP_MODE_0 */
	0x1794, 0x2, 0x0,/* BUCK_VS1_OP_MODE_1 */
	0x1796, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1808, 0x10, 0x0,/* BUCK_VS2_SLP_CON */
	0x180D, 0x1, 0x1,/* BUCK_VS2_OP_EN_0 */
	0x180E, 0x2, 0x2,/* BUCK_VS2_OP_EN_1 */
	0x180F, 0x80, 0x80,/* BUCK_VS2_OP_EN_2 */
	0x1810, 0x1, 0x1,/* BUCK_VS2_OP_CFG_0 */
	0x1811, 0x2, 0x0,/* BUCK_VS2_OP_CFG_1 */
	0x1813, 0x1, 0x1,/* BUCK_VS2_OP_MODE_0 */
	0x1814, 0x2, 0x0,/* BUCK_VS2_OP_MODE_1 */
	0x1816, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1B0D, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B0E, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B3E, 0x2, 0x2,/* LDO_VRTC28_CON */
	0x1B87, 0x3, 0x1,/* LDO_VA12_CON0 */
	0x1B88, 0x8, 0x0,/* LDO_VA12_CON1 */
	0x1B89, 0x80, 0x0,/* LDO_VA12_CON2 */
	0x1B8C, 0x1, 0x1,/* LDO_VA12_OP_EN0 */
	0x1B8D, 0x2, 0x2,/* LDO_VA12_OP_EN1 */
	0x1B8E, 0x80, 0x80,/* LDO_VA12_OP_EN2 */
	0x1B8F, 0x1, 0x1,/* LDO_VA12_OP_CFG0 */
	0x1B90, 0x2, 0x0,/* LDO_VA12_OP_CFG1 */
	0x1B92, 0x1, 0x1,/* LDO_VA12_OP_MODE0 */
	0x1B93, 0x2, 0x0,/* LDO_VA12_OP_MODE1 */
	0x1B95, 0x3, 0x0,/* LDO_VAUD18_CON0 */
	0x1B96, 0x8, 0x0,/* LDO_VAUD18_CON1 */
	0x1B97, 0x80, 0x0,/* LDO_VAUD18_CON2 */
	0x1B9C, 0x80, 0x80,/* LDO_VAUD18_OP_EN2 */
	0x1BA3, 0x3, 0x0,/* LDO_VAUD28_CON0 */
	0x1BA4, 0x8, 0x0,/* LDO_VAUD28_CON1 */
	0x1BA5, 0x80, 0x0,/* LDO_VAUD28_CON2 */
	0x1BAA, 0x80, 0x80,/* LDO_VAUD28_OP_EN2 */
	0x1BB1, 0x3, 0x1,/* LDO_VAUX18_CON0 */
	0x1BB2, 0x8, 0x0,/* LDO_VAUX18_CON1 */
	0x1BB3, 0x80, 0x0,/* LDO_VAUX18_CON2 */
	0x1BB6, 0x1, 0x1,/* LDO_VAUX18_OP_EN0 */
	0x1BB8, 0x80, 0x80,/* LDO_VAUX18_OP_EN2 */
	0x1BB9, 0x1, 0x1,/* LDO_VAUX18_OP_CFG0 */
	0x1BBC, 0x1, 0x1,/* LDO_VAUX18_OP_MODE0 */
	0x1BBF, 0x3, 0x1,/* LDO_VBIF28_CON0 */
	0x1BC0, 0x8, 0x0,/* LDO_VBIF28_CON1 */
	0x1BC1, 0x80, 0x0,/* LDO_VBIF28_CON2 */
	0x1BC6, 0x81, 0x81,/* LDO_VBIF28_OP_EN2 */
	0x1BC9, 0x1, 0x0,/* LDO_VBIF28_OP_CFG2 */
	0x1BCC, 0x1, 0x1,/* LDO_VBIF28_OP_MODE2 */
	0x1BCD, 0x3, 0x0,/* LDO_VCN33_1_CON0 */
	0x1BCE, 0x8, 0x0,/* LDO_VCN33_1_CON1 */
	0x1BCF, 0x80, 0x0,/* LDO_VCN33_1_CON2 */
	0x1BD4, 0x80, 0x80,/* LDO_VCN33_1_OP_EN2 */
	0x1C07, 0x3, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C08, 0x8, 0x0,/* LDO_VCN33_2_CON1 */
	0x1C09, 0x80, 0x0,/* LDO_VCN33_2_CON2 */
	0x1C0E, 0x80, 0x80,/* LDO_VCN33_2_OP_EN2 */
	0x1C15, 0x3, 0x0,/* LDO_VCN18_CON0 */
	0x1C16, 0x8, 0x0,/* LDO_VCN18_CON1 */
	0x1C17, 0x80, 0x0,/* LDO_VCN18_CON2 */
	0x1C1C, 0x80, 0x80,/* LDO_VCN18_OP_EN2 */
	0x1C23, 0x3, 0x0,/* LDO_VRFCK_CON0 */
	0x1C24, 0x8, 0x0,/* LDO_VRFCK_CON1 */
	0x1C25, 0x80, 0x0,/* LDO_VRFCK_CON2 */
	0x1C2A, 0xC0, 0xC0,/* LDO_VRFCK_OP_EN2 */
	0x1C2D, 0x40, 0x0,/* LDO_VRFCK_OP_CFG2 */
	0x1C30, 0x40, 0x0,/* LDO_VRFCK_OP_MODE2 */
	0x1C31, 0x3, 0x0,/* LDO_VBBCK_CON0 */
	0x1C32, 0x8, 0x0,/* LDO_VBBCK_CON1 */
	0x1C33, 0x80, 0x0,/* LDO_VBBCK_CON2 */
	0x1C38, 0xC0, 0xC0,/* LDO_VBBCK_OP_EN2 */
	0x1C3B, 0x40, 0x0,/* LDO_VBBCK_OP_CFG2 */
	0x1C3E, 0x40, 0x0,/* LDO_VBBCK_OP_MODE2 */
	0x1C3F, 0x3, 0x1,/* LDO_VXO22_CON0 */
	0x1C40, 0x8, 0x0,/* LDO_VXO22_CON1 */
	0x1C41, 0x80, 0x0,/* LDO_VXO22_CON2 */
	0x1C46, 0x81, 0x81,/* LDO_VXO22_OP_EN2 */
	0x1C49, 0x1, 0x1,/* LDO_VXO22_OP_CFG2 */
	0x1C4C, 0x1, 0x1,/* LDO_VXO22_OP_MODE2 */
	0x1C4D, 0x3, 0x1,/* LDO_VM18_CON0 */
	0x1C4E, 0x8, 0x0,/* LDO_VM18_CON1 */
	0x1C4F, 0x80, 0x0,/* LDO_VM18_CON2 */
	0x1C52, 0x1, 0x1,/* LDO_VM18_OP_EN0 */
	0x1C54, 0x80, 0x80,/* LDO_VM18_OP_EN2 */
	0x1C55, 0x1, 0x1,/* LDO_VM18_OP_CFG0 */
	0x1C58, 0x1, 0x1,/* LDO_VM18_OP_MODE0 */
	0x1C87, 0x3, 0x1,/* LDO_VMDDR_CON0 */
	0x1C88, 0x8, 0x0,/* LDO_VMDDR_CON1 */
	0x1C89, 0x80, 0x0,/* LDO_VMDDR_CON2 */
	0x1C8C, 0x1, 0x1,/* LDO_VMDDR_OP_EN0 */
	0x1C8E, 0x80, 0x80,/* LDO_VMDDR_OP_EN2 */
	0x1C8F, 0x1, 0x1,/* LDO_VMDDR_OP_CFG0 */
	0x1C92, 0x1, 0x1,/* LDO_VMDDR_OP_MODE0 */
	0x1C95, 0x3, 0x0,/* LDO_VMDDQ_CON0 */
	0x1C96, 0x8, 0x0,/* LDO_VMDDQ_CON1 */
	0x1C97, 0x80, 0x0,/* LDO_VMDDQ_CON2 */
	0x1C9C, 0x80, 0x80,/* LDO_VMDDQ_OP_EN2 */
	0x1CA3, 0x3, 0x0,/* LDO_VEFUSE_CON0 */
	0x1CA4, 0x8, 0x0,/* LDO_VEFUSE_CON1 */
	0x1CA5, 0x80, 0x0,/* LDO_VEFUSE_CON2 */
	0x1CAA, 0x80, 0x80,/* LDO_VEFUSE_OP_EN2 */
	0x1CB1, 0x3, 0x0,/* LDO_VEMC_CON0 */
	0x1CB2, 0x8, 0x0,/* LDO_VEMC_CON1 */
	0x1CB3, 0x80, 0x0,/* LDO_VEMC_CON2 */
	0x1CB8, 0x80, 0x80,/* LDO_VEMC_OP_EN2 */
	0x1CBF, 0x3, 0x1,/* LDO_VUFS_CON0 */
	0x1CC0, 0x8, 0x0,/* LDO_VUFS_CON1 */
	0x1CC1, 0x80, 0x0,/* LDO_VUFS_CON2 */
	0x1CC4, 0x1, 0x1,/* LDO_VUFS_OP_EN0 */
	0x1CC6, 0x80, 0x80,/* LDO_VUFS_OP_EN2 */
	0x1CC7, 0x1, 0x1,/* LDO_VUFS_OP_CFG0 */
	0x1CCA, 0x1, 0x1,/* LDO_VUFS_OP_MODE0 */
	0x1CCD, 0x3, 0x1,/* LDO_VIO18_CON0 */
	0x1CCE, 0x8, 0x0,/* LDO_VIO18_CON1 */
	0x1CCF, 0x80, 0x0,/* LDO_VIO18_CON2 */
	0x1CD2, 0x1, 0x1,/* LDO_VIO18_OP_EN0 */
	0x1CD4, 0x80, 0x80,/* LDO_VIO18_OP_EN2 */
	0x1CD5, 0x1, 0x1,/* LDO_VIO18_OP_CFG0 */
	0x1CD8, 0x1, 0x1,/* LDO_VIO18_OP_MODE0 */
	0x1D07, 0x3, 0x1,/* LDO_VRF18_CON0 */
	0x1D08, 0x8, 0x0,/* LDO_VRF18_CON1 */
	0x1D09, 0x80, 0x0,/* LDO_VRF18_CON2 */
	0x1D0E, 0x82, 0x82,/* LDO_VRF18_OP_EN2 */
	0x1D11, 0x2, 0x0,/* LDO_VRF18_OP_CFG2 */
	0x1D14, 0x2, 0x2,/* LDO_VRF18_OP_MODE2 */
	0x1D15, 0x3, 0x1,/* LDO_VRF12_CON0 */
	0x1D16, 0x8, 0x0,/* LDO_VRF12_CON1 */
	0x1D17, 0x80, 0x0,/* LDO_VRF12_CON2 */
	0x1D1C, 0x82, 0x82,/* LDO_VRF12_OP_EN2 */
	0x1D1F, 0x2, 0x0,/* LDO_VRF12_OP_CFG2 */
	0x1D22, 0x2, 0x2,/* LDO_VRF12_OP_MODE2 */
	0x1D23, 0x3, 0x1,/* LDO_VRF09_CON0 */
	0x1D24, 0x8, 0x0,/* LDO_VRF09_CON1 */
	0x1D25, 0x80, 0x0,/* LDO_VRF09_CON2 */
	0x1D2A, 0x82, 0x82,/* LDO_VRF09_OP_EN2 */
	0x1D2D, 0x2, 0x0,/* LDO_VRF09_OP_CFG2 */
	0x1D30, 0x2, 0x2,/* LDO_VRF09_OP_MODE2 */
	0x1D31, 0x3, 0x1,/* LDO_VRFVA12_CON0 */
	0x1D32, 0x8, 0x0,/* LDO_VRFVA12_CON1 */
	0x1D33, 0x80, 0x0,/* LDO_VRFVA12_CON2 */
	0x1D36, 0x1, 0x1,/* LDO_VRFVA12_OP_EN0 */
	0x1D38, 0x80, 0x80,/* LDO_VRFVA12_OP_EN2 */
	0x1D39, 0x1, 0x1,/* LDO_VRFVA12_OP_CFG0 */
	0x1D3C, 0x1, 0x1,/* LDO_VRFVA12_OP_MODE0 */
	0x1D3F, 0x3, 0x1,/* LDO_VRFIO18_CON0 */
	0x1D40, 0x8, 0x0,/* LDO_VRFIO18_CON1 */
	0x1D41, 0x80, 0x0,/* LDO_VRFIO18_CON2 */
	0x1D44, 0x1, 0x1,/* LDO_VRFIO18_OP_EN0 */
	0x1D46, 0x80, 0x80,/* LDO_VRFIO18_OP_EN2 */
	0x1D47, 0x1, 0x1,/* LDO_VRFIO18_OP_CFG0 */
	0x1D4A, 0x1, 0x1,/* LDO_VRFIO18_OP_MODE0 */
	0x1D4D, 0x3, 0x0,/* LDO_VMCH_CON0 */
	0x1D4E, 0x8, 0x0,/* LDO_VMCH_CON1 */
	0x1D4F, 0x80, 0x0,/* LDO_VMCH_CON2 */
	0x1D54, 0x80, 0x80,/* LDO_VMCH_OP_EN2 */
	0x1D87, 0x3, 0x0,/* LDO_VMC_CON0 */
	0x1D88, 0x8, 0x0,/* LDO_VMC_CON1 */
	0x1D89, 0x80, 0x0,/* LDO_VMC_CON2 */
	0x1D8E, 0x80, 0x80,/* LDO_VMC_OP_EN2 */
	0x1D95, 0x3, 0x0,/* LDO_VSIM1_CON0 */
	0x1D96, 0x8, 0x0,/* LDO_VSIM1_CON1 */
	0x1D97, 0x80, 0x0,/* LDO_VSIM1_CON2 */
	0x1D9C, 0x80, 0x80,/* LDO_VSIM1_OP_EN2 */
	0x1DA4, 0x3, 0x0,/* LDO_VSIM2_CON0 */
	0x1DA5, 0x8, 0x0,/* LDO_VSIM2_CON1 */
	0x1DA6, 0x80, 0x0,/* LDO_VSIM2_CON2 */
	0x1DAB, 0x80, 0x80,/* LDO_VSIM2_OP_EN2 */
	0x1DB3, 0x3, 0x1,/* LDO_VUSB_CON0 */
	0x1DB4, 0x8, 0x0,/* LDO_VUSB_CON1 */
	0x1DB5, 0x80, 0x0,/* LDO_VUSB_CON2 */
	0x1DB8, 0x1, 0x1,/* LDO_VUSB_OP_EN0 */
	0x1DBA, 0x80, 0x80,/* LDO_VUSB_OP_EN2 */
	0x1DBB, 0x1, 0x1,/* LDO_VUSB_OP_CFG0 */
	0x1DBE, 0x1, 0x1,/* LDO_VUSB_OP_MODE0 */
	0x1DC1, 0x3, 0x0,/* LDO_VIBR_CON0 */
	0x1DC2, 0x8, 0x0,/* LDO_VIBR_CON1 */
	0x1DC3, 0x80, 0x0,/* LDO_VIBR_CON2 */
	0x1DC8, 0x80, 0x80,/* LDO_VIBR_OP_EN2 */
	0x1DCF, 0x3, 0x1,/* LDO_VIO28_CON0 */
	0x1DD0, 0x8, 0x0,/* LDO_VIO28_CON1 */
	0x1DD1, 0x80, 0x0,/* LDO_VIO28_CON2 */
	0x1DD6, 0x80, 0x80,/* LDO_VIO28_OP_EN2 */
	0x1E07, 0x3, 0x0,/* LDO_VFP_CON0 */
	0x1E08, 0x8, 0x0,/* LDO_VFP_CON1 */
	0x1E09, 0x80, 0x0,/* LDO_VFP_CON2 */
	0x1E0E, 0x80, 0x80,/* LDO_VFP_OP_EN2 */
	0x1E15, 0x3, 0x0,/* LDO_VTP_CON0 */
	0x1E16, 0x8, 0x0,/* LDO_VTP_CON1 */
	0x1E17, 0x80, 0x0,/* LDO_VTP_CON2 */
	0x1E1C, 0x80, 0x80,/* LDO_VTP_OP_EN2 */
	0x1E87, 0x3, 0x1,/* LDO_VSRAM_MD_CON0 */
	0x1E88, 0x8, 0x0,/* LDO_VSRAM_MD_CON1 */
	0x1E89, 0x80, 0x0,/* LDO_VSRAM_MD_CON2 */
	0x1E94, 0x1, 0x1,/* LDO_VSRAM_MD_OP_EN0 */
	0x1E96, 0x80, 0x80,/* LDO_VSRAM_MD_OP_EN2 */
	0x1E97, 0x1, 0x1,/* LDO_VSRAM_MD_OP_CFG0 */
	0x1E9A, 0x1, 0x1,/* LDO_VSRAM_MD_OP_MODE0 */
	0x1E9D, 0x3, 0x0,/* LDO_VSRAM_PROC1_CON0 */
	0x1E9E, 0x8, 0x0,/* LDO_VSRAM_PROC1_CON1 */
	0x1E9F, 0x80, 0x0,/* LDO_VSRAM_PROC1_CON2 */
	0x1EAC, 0x80, 0x80,/* LDO_VSRAM_PROC1_OP_EN2 */
	0x1F07, 0x3, 0x0,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8, 0x0,/* LDO_VSRAM_PROC2_CON1 */
	0x1F09, 0x80, 0x0,/* LDO_VSRAM_PROC2_CON2 */
	0x1F16, 0x80, 0x80,/* LDO_VSRAM_PROC2_OP_EN2 */
	0x1F1D, 0x3, 0x1,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F1E, 0x8, 0x0,/* LDO_VSRAM_OTHERS_CON1 */
	0x1F1F, 0x80, 0x0,/* LDO_VSRAM_OTHERS_CON2 */
	0x1F2C, 0x81, 0x81,/* LDO_VSRAM_OTHERS_OP_EN2 */
	0x1F2F, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_CFG2 */
	0x1F32, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_MODE2 */
	0x218E, 0x1E, 0x1E,/* XPP_TOP_CKPDN_CON0 */
	0x220B, 0x77, 0x0,/* ISINK_EN_CTRL0 */
0x220C, 0x70, 0x0 /* ISINK_EN_CTRL1 */
};

const unsigned int *AP_PMIC_REG_6377_gs_suspend_32kless =
				AP_PMIC_REG_MT6377_gs_suspend_32kless_data;

unsigned int AP_PMIC_REG_6377_gs_suspend_32kless_len = 954;

const unsigned int AP_PMIC_REG_MT6377_gs_deepidle___lp_mp3_32kless_data[] = {
	/*  Address     Mask        Golden Setting Value */
	0x1A, 0xA, 0xA,/* TOP_CON */
	0x1D, 0x1F, 0x1F,/* TEST_CON0 */
	0x25, 0x1, 0x1,/* SMT_CON0 */
	0x10F, 0x10, 0x10,/* TOP_CKPDN_CON1 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON2 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON1 */
	0x11B, 0xC, 0x0,/* TOP_CKSEL_CON2 */
	0x128, 0x55, 0x0,/* TOP_CLK_CON0 */
	0x129, 0x5, 0x0,/* TOP_CLK_CON1 */
	0x141, 0x1, 0x0,/* TOP_DCXO_CKEN_SW */
	0x142, 0x1, 0x0,/* TOP_SPMI_CON0 */
	0x143, 0x1, 0x0,/* TOP_SPMI_CON1 */
	0x243, 0x8, 0x0,/* TOP_VRCTL_VR1_EN */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0_L */
	0x51A, 0x1F, 0x10,/* SCK_TOP_CKHWEN_CON0_L */
	0x54C, 0xFF, 0x21,/* XO_BUF_CTL0_L */
	0x54D, 0x3F, 0x0,/* XO_BUF_CTL0_H */
	0x54E, 0xFF, 0xC0,/* XO_BUF_CTL1_L */
	0x54F, 0x3F, 0x3,/* XO_BUF_CTL1_H */
	0x550, 0xFF, 0x0,/* XO_BUF_CTL2_L */
	0x551, 0x3F, 0x8,/* XO_BUF_CTL2_H */
	0x552, 0xFF, 0x40,/* XO_BUF_CTL3_L */
	0x553, 0x3F, 0x0,/* XO_BUF_CTL3_H */
	0x554, 0xFF, 0xC0,/* XO_BUF_CTL4_L */
	0x555, 0x3F, 0x3,/* XO_BUF_CTL4_H */
	0x788, 0xDB, 0x49,/* DCXO_CW00 */
	0x78B, 0x6, 0x4,/* DCXO_CW00_H */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x79C, 0x4, 0x0,/* DCXO_CW08 */
	0x7A1, 0x36, 0x12,/* DCXO_CW09_H */
	0x7A8, 0x80, 0x0,/* DCXO_CW12 */
	0x7A9, 0x10, 0x10,/* DCXO_CW12_H */
	0xC0B, 0x5, 0x1,/* BM_TOP_CKPDN_CON0 */
	0xC0E, 0xFD, 0xE4,/* BM_TOP_CKPDN_CON1 */
	0xC17, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC21, 0x1, 0x1,/* BM_TOP_RST_CON2 */
	0xD07, 0x3, 0x3,/* FGADC_CON0 */
	0xD08, 0x1, 0x1,/* FGADC_CON1 */
	0xD0D, 0x7F, 0x78,/* FGADC_CON6 */
	0xD37, 0xFF, 0x1F,/* FGADC_SON_CON0_L */
	0xD38, 0xFF, 0x0,/* FGADC_SON_CON0_H */
	0xD39, 0xFF, 0x14,/* FGADC_SON_CON1_L */
	0xD3A, 0x3, 0x0,/* FGADC_SON_CON1_H */
	0xD3B, 0xFF, 0xFF,/* FGADC_SON_CON2_L */
	0xD3C, 0x3, 0x0,/* FGADC_SON_CON2_H */
	0xD3D, 0xFF, 0x1F,/* FGADC_SOFF_CON0_L */
	0xD3E, 0xFF, 0x0,/* FGADC_SOFF_CON0_H */
	0xD3F, 0xFF, 0x14,/* FGADC_SOFF_CON1_L */
	0xD40, 0x3, 0x0,/* FGADC_SOFF_CON1_H */
	0xD41, 0xFF, 0xFF,/* FGADC_SOFF_CON2_L */
	0xD42, 0x3, 0x0,/* FGADC_SOFF_CON2_H */
	0xF8C, 0xFF, 0x15,/* HK_TOP_CLK_CON0 */
	0xF8D, 0x3, 0x1,/* HK_TOP_CLK_CON1 */
	0x1188, 0xE0, 0x0,/* AUXADC_CON0 */
	0x1189, 0x3F, 0x14,/* AUXADC_CON1 */
	0x1199, 0x70, 0x60,/* AUXADC_AVG_CON6 */
	0x1208, 0x1, 0x0,/* AUXADC_IMP0 */
	0x120C, 0x2, 0x0,/* AUXADC_IMP4 */
	0x120D, 0x1, 0x0,/* AUXADC_LBAT0 */
	0x1218, 0x2, 0x0,/* AUXADC_LBAT11 */
	0x1219, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x121A, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x121B, 0x2, 0x2,/* AUXADC_LBAT2_2 */
	0x121E, 0x2, 0x2,/* AUXADC_LBAT2_5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT2_11 */
	0x1225, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x1227, 0x3, 0x2,/* AUXADC_BAT_TEMP_2 */
	0x1228, 0x2, 0x2,/* AUXADC_BAT_TEMP_3 */
	0x122B, 0x2, 0x2,/* AUXADC_BAT_TEMP_6 */
	0x1231, 0x2, 0x0,/* AUXADC_BAT_TEMP_12 */
	0x1232, 0x1, 0x0,/* AUXADC_THR0 */
	0x123D, 0x2, 0x0,/* AUXADC_THR11 */
	0x1247, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x1259, 0x2, 0x0,/* AUXADC_NAG_18 */
	0x140B, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x140E, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1488, 0x10, 0x0,/* BUCK_VBUCK1_SLP_CON */
	0x148F, 0x80, 0x80,/* BUCK_VBUCK1_OP_EN_2 */
	0x1496, 0x3, 0x0,/* BUCK_VBUCK1_CON0 */
	0x1508, 0x10, 0x0,/* BUCK_VBUCK2_SLP_CON */
	0x150F, 0x80, 0x80,/* BUCK_VBUCK2_OP_EN_2 */
	0x1516, 0x3, 0x0,/* BUCK_VBUCK2_CON0 */
	0x1588, 0x10, 0x0,/* BUCK_VBUCK3_SLP_CON */
	0x158F, 0x84, 0x84,/* BUCK_VBUCK3_OP_EN_2 */
	0x1592, 0x4, 0x4,/* BUCK_VBUCK3_OP_CFG_2 */
	0x1595, 0x4, 0x4,/* BUCK_VBUCK3_OP_MODE_2 */
	0x1596, 0x3, 0x1,/* BUCK_VBUCK3_CON0 */
	0x1608, 0x10, 0x0,/* BUCK_VBUCK4_SLP_CON */
	0x160F, 0x80, 0x80,/* BUCK_VBUCK4_OP_EN_2 */
	0x1616, 0x3, 0x0,/* BUCK_VBUCK4_CON0 */
	0x1688, 0x10, 0x0,/* BUCK_VBUCK5_SLP_CON */
	0x168F, 0x84, 0x84,/* BUCK_VBUCK5_OP_EN_2 */
	0x1692, 0x4, 0x4,/* BUCK_VBUCK5_OP_CFG_2 */
	0x1695, 0x4, 0x4,/* BUCK_VBUCK5_OP_MODE_2 */
	0x1696, 0x3, 0x1,/* BUCK_VBUCK5_CON0 */
	0x1708, 0x10, 0x0,/* BUCK_VBUCK6_SLP_CON */
	0x170F, 0x84, 0x84,/* BUCK_VBUCK6_OP_EN_2 */
	0x1712, 0x4, 0x4,/* BUCK_VBUCK6_OP_CFG_2 */
	0x1715, 0x4, 0x4,/* BUCK_VBUCK6_OP_MODE_2 */
	0x1716, 0x3, 0x1,/* BUCK_VBUCK6_CON0 */
	0x1788, 0x10, 0x0,/* BUCK_VS1_SLP_CON */
	0x178F, 0x84, 0x84,/* BUCK_VS1_OP_EN_2 */
	0x1792, 0x4, 0x4,/* BUCK_VS1_OP_CFG_2 */
	0x1795, 0x4, 0x4,/* BUCK_VS1_OP_MODE_2 */
	0x1796, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1808, 0x10, 0x0,/* BUCK_VS2_SLP_CON */
	0x180F, 0x84, 0x84,/* BUCK_VS2_OP_EN_2 */
	0x1812, 0x4, 0x4,/* BUCK_VS2_OP_CFG_2 */
	0x1815, 0x4, 0x4,/* BUCK_VS2_OP_MODE_2 */
	0x1816, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1B0D, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B0E, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B3E, 0x2, 0x2,/* LDO_VRTC28_CON */
	0x1B87, 0x3, 0x1,/* LDO_VA12_CON0 */
	0x1B88, 0x8, 0x0,/* LDO_VA12_CON1 */
	0x1B89, 0x80, 0x0,/* LDO_VA12_CON2 */
	0x1B8E, 0x84, 0x84,/* LDO_VA12_OP_EN2 */
	0x1B91, 0x4, 0x4,/* LDO_VA12_OP_CFG2 */
	0x1B94, 0x4, 0x4,/* LDO_VA12_OP_MODE2 */
	0x1B95, 0x3, 0x0,/* LDO_VAUD18_CON0 */
	0x1B96, 0x8, 0x0,/* LDO_VAUD18_CON1 */
	0x1B97, 0x80, 0x0,/* LDO_VAUD18_CON2 */
	0x1B9C, 0x80, 0x80,/* LDO_VAUD18_OP_EN2 */
	0x1BA3, 0x3, 0x0,/* LDO_VAUD28_CON0 */
	0x1BA4, 0x8, 0x0,/* LDO_VAUD28_CON1 */
	0x1BA5, 0x80, 0x0,/* LDO_VAUD28_CON2 */
	0x1BAA, 0x80, 0x80,/* LDO_VAUD28_OP_EN2 */
	0x1BB1, 0x3, 0x1,/* LDO_VAUX18_CON0 */
	0x1BB2, 0x8, 0x0,/* LDO_VAUX18_CON1 */
	0x1BB3, 0x80, 0x0,/* LDO_VAUX18_CON2 */
	0x1BB8, 0x84, 0x84,/* LDO_VAUX18_OP_EN2 */
	0x1BBB, 0x4, 0x4,/* LDO_VAUX18_OP_CFG2 */
	0x1BBE, 0x4, 0x4,/* LDO_VAUX18_OP_MODE2 */
	0x1BBF, 0x3, 0x1,/* LDO_VBIF28_CON0 */
	0x1BC0, 0x8, 0x0,/* LDO_VBIF28_CON1 */
	0x1BC1, 0x80, 0x0,/* LDO_VBIF28_CON2 */
	0x1BC6, 0x84, 0x84,/* LDO_VBIF28_OP_EN2 */
	0x1BC9, 0x4, 0x0,/* LDO_VBIF28_OP_CFG2 */
	0x1BCC, 0x4, 0x4,/* LDO_VBIF28_OP_MODE2 */
	0x1BCD, 0x3, 0x0,/* LDO_VCN33_1_CON0 */
	0x1BCE, 0x8, 0x0,/* LDO_VCN33_1_CON1 */
	0x1BCF, 0x80, 0x0,/* LDO_VCN33_1_CON2 */
	0x1BD4, 0x80, 0x80,/* LDO_VCN33_1_OP_EN2 */
	0x1C07, 0x3, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C08, 0x8, 0x0,/* LDO_VCN33_2_CON1 */
	0x1C09, 0x80, 0x0,/* LDO_VCN33_2_CON2 */
	0x1C0E, 0x80, 0x80,/* LDO_VCN33_2_OP_EN2 */
	0x1C15, 0x3, 0x0,/* LDO_VCN18_CON0 */
	0x1C16, 0x8, 0x0,/* LDO_VCN18_CON1 */
	0x1C17, 0x80, 0x0,/* LDO_VCN18_CON2 */
	0x1C1C, 0x80, 0x80,/* LDO_VCN18_OP_EN2 */
	0x1C23, 0x3, 0x0,/* LDO_VRFCK_CON0 */
	0x1C24, 0x8, 0x0,/* LDO_VRFCK_CON1 */
	0x1C25, 0x80, 0x0,/* LDO_VRFCK_CON2 */
	0x1C2A, 0xC0, 0xC0,/* LDO_VRFCK_OP_EN2 */
	0x1C2D, 0x40, 0x0,/* LDO_VRFCK_OP_CFG2 */
	0x1C30, 0x40, 0x0,/* LDO_VRFCK_OP_MODE2 */
	0x1C31, 0x3, 0x0,/* LDO_VBBCK_CON0 */
	0x1C32, 0x8, 0x0,/* LDO_VBBCK_CON1 */
	0x1C33, 0x80, 0x0,/* LDO_VBBCK_CON2 */
	0x1C38, 0xC0, 0xC0,/* LDO_VBBCK_OP_EN2 */
	0x1C3B, 0x40, 0x0,/* LDO_VBBCK_OP_CFG2 */
	0x1C3E, 0x40, 0x0,/* LDO_VBBCK_OP_MODE2 */
	0x1C3F, 0x3, 0x1,/* LDO_VXO22_CON0 */
	0x1C40, 0x8, 0x0,/* LDO_VXO22_CON1 */
	0x1C41, 0x80, 0x0,/* LDO_VXO22_CON2 */
	0x1C46, 0x84, 0x84,/* LDO_VXO22_OP_EN2 */
	0x1C49, 0x4, 0x4,/* LDO_VXO22_OP_CFG2 */
	0x1C4C, 0x4, 0x4,/* LDO_VXO22_OP_MODE2 */
	0x1C4D, 0x3, 0x1,/* LDO_VM18_CON0 */
	0x1C4E, 0x8, 0x0,/* LDO_VM18_CON1 */
	0x1C4F, 0x80, 0x0,/* LDO_VM18_CON2 */
	0x1C54, 0x84, 0x84,/* LDO_VM18_OP_EN2 */
	0x1C57, 0x4, 0x4,/* LDO_VM18_OP_CFG2 */
	0x1C5A, 0x4, 0x4,/* LDO_VM18_OP_MODE2 */
	0x1C87, 0x3, 0x1,/* LDO_VMDDR_CON0 */
	0x1C88, 0x8, 0x0,/* LDO_VMDDR_CON1 */
	0x1C89, 0x80, 0x0,/* LDO_VMDDR_CON2 */
	0x1C8E, 0x84, 0x84,/* LDO_VMDDR_OP_EN2 */
	0x1C91, 0x4, 0x4,/* LDO_VMDDR_OP_CFG2 */
	0x1C94, 0x4, 0x4,/* LDO_VMDDR_OP_MODE2 */
	0x1C95, 0x3, 0x0,/* LDO_VMDDQ_CON0 */
	0x1C96, 0x8, 0x0,/* LDO_VMDDQ_CON1 */
	0x1C97, 0x80, 0x0,/* LDO_VMDDQ_CON2 */
	0x1C9C, 0x80, 0x80,/* LDO_VMDDQ_OP_EN2 */
	0x1CA3, 0x3, 0x0,/* LDO_VEFUSE_CON0 */
	0x1CA4, 0x8, 0x0,/* LDO_VEFUSE_CON1 */
	0x1CA5, 0x80, 0x0,/* LDO_VEFUSE_CON2 */
	0x1CAA, 0x80, 0x80,/* LDO_VEFUSE_OP_EN2 */
	0x1CB1, 0x3, 0x0,/* LDO_VEMC_CON0 */
	0x1CB2, 0x8, 0x0,/* LDO_VEMC_CON1 */
	0x1CB3, 0x80, 0x0,/* LDO_VEMC_CON2 */
	0x1CB8, 0x80, 0x80,/* LDO_VEMC_OP_EN2 */
	0x1CBF, 0x3, 0x1,/* LDO_VUFS_CON0 */
	0x1CC0, 0x8, 0x0,/* LDO_VUFS_CON1 */
	0x1CC1, 0x80, 0x0,/* LDO_VUFS_CON2 */
	0x1CC6, 0x80, 0x80,/* LDO_VUFS_OP_EN2 */
	0x1CCD, 0x3, 0x1,/* LDO_VIO18_CON0 */
	0x1CCE, 0x8, 0x0,/* LDO_VIO18_CON1 */
	0x1CCF, 0x80, 0x0,/* LDO_VIO18_CON2 */
	0x1CD4, 0x80, 0x80,/* LDO_VIO18_OP_EN2 */
	0x1D07, 0x3, 0x1,/* LDO_VRF18_CON0 */
	0x1D08, 0x8, 0x0,/* LDO_VRF18_CON1 */
	0x1D09, 0x80, 0x0,/* LDO_VRF18_CON2 */
	0x1D0E, 0x82, 0x82,/* LDO_VRF18_OP_EN2 */
	0x1D11, 0x2, 0x0,/* LDO_VRF18_OP_CFG2 */
	0x1D14, 0x2, 0x2,/* LDO_VRF18_OP_MODE2 */
	0x1D15, 0x3, 0x1,/* LDO_VRF12_CON0 */
	0x1D16, 0x8, 0x0,/* LDO_VRF12_CON1 */
	0x1D17, 0x80, 0x0,/* LDO_VRF12_CON2 */
	0x1D1C, 0x82, 0x82,/* LDO_VRF12_OP_EN2 */
	0x1D1F, 0x2, 0x0,/* LDO_VRF12_OP_CFG2 */
	0x1D22, 0x2, 0x2,/* LDO_VRF12_OP_MODE2 */
	0x1D23, 0x3, 0x1,/* LDO_VRF09_CON0 */
	0x1D24, 0x8, 0x0,/* LDO_VRF09_CON1 */
	0x1D25, 0x80, 0x0,/* LDO_VRF09_CON2 */
	0x1D2A, 0x82, 0x82,/* LDO_VRF09_OP_EN2 */
	0x1D2D, 0x2, 0x0,/* LDO_VRF09_OP_CFG2 */
	0x1D30, 0x2, 0x2,/* LDO_VRF09_OP_MODE2 */
	0x1D31, 0x3, 0x1,/* LDO_VRFVA12_CON0 */
	0x1D32, 0x8, 0x0,/* LDO_VRFVA12_CON1 */
	0x1D33, 0x80, 0x0,/* LDO_VRFVA12_CON2 */
	0x1D38, 0x84, 0x84,/* LDO_VRFVA12_OP_EN2 */
	0x1D3B, 0x4, 0x4,/* LDO_VRFVA12_OP_CFG2 */
	0x1D3E, 0x4, 0x4,/* LDO_VRFVA12_OP_MODE2 */
	0x1D3F, 0x3, 0x1,/* LDO_VRFIO18_CON0 */
	0x1D40, 0x8, 0x0,/* LDO_VRFIO18_CON1 */
	0x1D41, 0x80, 0x0,/* LDO_VRFIO18_CON2 */
	0x1D46, 0x84, 0x84,/* LDO_VRFIO18_OP_EN2 */
	0x1D49, 0x4, 0x4,/* LDO_VRFIO18_OP_CFG2 */
	0x1D4C, 0x4, 0x4,/* LDO_VRFIO18_OP_MODE2 */
	0x1D4D, 0x3, 0x0,/* LDO_VMCH_CON0 */
	0x1D4E, 0x8, 0x0,/* LDO_VMCH_CON1 */
	0x1D4F, 0x80, 0x0,/* LDO_VMCH_CON2 */
	0x1D54, 0x80, 0x80,/* LDO_VMCH_OP_EN2 */
	0x1D87, 0x3, 0x0,/* LDO_VMC_CON0 */
	0x1D88, 0x8, 0x0,/* LDO_VMC_CON1 */
	0x1D89, 0x80, 0x0,/* LDO_VMC_CON2 */
	0x1D8E, 0x80, 0x80,/* LDO_VMC_OP_EN2 */
	0x1D95, 0x3, 0x0,/* LDO_VSIM1_CON0 */
	0x1D96, 0x8, 0x0,/* LDO_VSIM1_CON1 */
	0x1D97, 0x80, 0x0,/* LDO_VSIM1_CON2 */
	0x1D9C, 0x80, 0x80,/* LDO_VSIM1_OP_EN2 */
	0x1DA4, 0x3, 0x0,/* LDO_VSIM2_CON0 */
	0x1DA5, 0x8, 0x0,/* LDO_VSIM2_CON1 */
	0x1DA6, 0x80, 0x0,/* LDO_VSIM2_CON2 */
	0x1DAB, 0x80, 0x80,/* LDO_VSIM2_OP_EN2 */
	0x1DB3, 0x3, 0x1,/* LDO_VUSB_CON0 */
	0x1DB4, 0x8, 0x0,/* LDO_VUSB_CON1 */
	0x1DB5, 0x80, 0x0,/* LDO_VUSB_CON2 */
	0x1DBA, 0x84, 0x84,/* LDO_VUSB_OP_EN2 */
	0x1DBD, 0x4, 0x4,/* LDO_VUSB_OP_CFG2 */
	0x1DC0, 0x4, 0x4,/* LDO_VUSB_OP_MODE2 */
	0x1DC1, 0x3, 0x0,/* LDO_VIBR_CON0 */
	0x1DC2, 0x8, 0x0,/* LDO_VIBR_CON1 */
	0x1DC3, 0x80, 0x0,/* LDO_VIBR_CON2 */
	0x1DC8, 0x80, 0x80,/* LDO_VIBR_OP_EN2 */
	0x1DCF, 0x3, 0x1,/* LDO_VIO28_CON0 */
	0x1DD0, 0x8, 0x0,/* LDO_VIO28_CON1 */
	0x1DD1, 0x80, 0x0,/* LDO_VIO28_CON2 */
	0x1DD6, 0x80, 0x80,/* LDO_VIO28_OP_EN2 */
	0x1E07, 0x3, 0x0,/* LDO_VFP_CON0 */
	0x1E08, 0x8, 0x0,/* LDO_VFP_CON1 */
	0x1E09, 0x80, 0x0,/* LDO_VFP_CON2 */
	0x1E0E, 0x80, 0x80,/* LDO_VFP_OP_EN2 */
	0x1E15, 0x3, 0x0,/* LDO_VTP_CON0 */
	0x1E16, 0x8, 0x0,/* LDO_VTP_CON1 */
	0x1E17, 0x80, 0x0,/* LDO_VTP_CON2 */
	0x1E1C, 0x80, 0x80,/* LDO_VTP_OP_EN2 */
	0x1E87, 0x3, 0x1,/* LDO_VSRAM_MD_CON0 */
	0x1E88, 0x8, 0x0,/* LDO_VSRAM_MD_CON1 */
	0x1E89, 0x80, 0x0,/* LDO_VSRAM_MD_CON2 */
	0x1E96, 0x84, 0x84,/* LDO_VSRAM_MD_OP_EN2 */
	0x1E99, 0x4, 0x4,/* LDO_VSRAM_MD_OP_CFG2 */
	0x1E9C, 0x4, 0x4,/* LDO_VSRAM_MD_OP_MODE2 */
	0x1E9D, 0x3, 0x0,/* LDO_VSRAM_PROC1_CON0 */
	0x1E9E, 0x8, 0x0,/* LDO_VSRAM_PROC1_CON1 */
	0x1E9F, 0x80, 0x0,/* LDO_VSRAM_PROC1_CON2 */
	0x1EAC, 0x80, 0x80,/* LDO_VSRAM_PROC1_OP_EN2 */
	0x1F07, 0x3, 0x0,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8, 0x0,/* LDO_VSRAM_PROC2_CON1 */
	0x1F09, 0x80, 0x0,/* LDO_VSRAM_PROC2_CON2 */
	0x1F16, 0x80, 0x80,/* LDO_VSRAM_PROC2_OP_EN2 */
	0x1F1D, 0x3, 0x1,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F1E, 0x8, 0x0,/* LDO_VSRAM_OTHERS_CON1 */
	0x1F1F, 0x80, 0x0,/* LDO_VSRAM_OTHERS_CON2 */
	0x1F2C, 0x84, 0x84,/* LDO_VSRAM_OTHERS_OP_EN2 */
	0x1F2F, 0x4, 0x4,/* LDO_VSRAM_OTHERS_OP_CFG2 */
	0x1F32, 0x4, 0x4,/* LDO_VSRAM_OTHERS_OP_MODE2 */
	0x218E, 0x1E, 0x1E,/* XPP_TOP_CKPDN_CON0 */
	0x220B, 0x77, 0x0,/* ISINK_EN_CTRL0 */
0x220C, 0x70, 0x0 /* ISINK_EN_CTRL1 */
};

const unsigned int *AP_PMIC_REG_6377_gs_deepidle___lp_mp3_32kless =
			AP_PMIC_REG_MT6377_gs_deepidle___lp_mp3_32kless_data;

unsigned int AP_PMIC_REG_6377_gs_deepidle___lp_mp3_32kless_len = 876;

const unsigned int AP_PMIC_REG_MT6377_gs_sodi3p0_32kless_data[] = {
	/*  Address     Mask        Golden Setting Value */
	0x1A, 0xA, 0xA,/* TOP_CON */
	0x1D, 0x1F, 0x1F,/* TEST_CON0 */
	0x25, 0x1, 0x1,/* SMT_CON0 */
	0x10F, 0x10, 0x10,/* TOP_CKPDN_CON1 */
	0x112, 0x4, 0x4,/* TOP_CKPDN_CON2 */
	0x118, 0x8, 0x8,/* TOP_CKSEL_CON1 */
	0x11B, 0xC, 0x0,/* TOP_CKSEL_CON2 */
	0x128, 0x55, 0x0,/* TOP_CLK_CON0 */
	0x129, 0x5, 0x0,/* TOP_CLK_CON1 */
	0x141, 0x1, 0x0,/* TOP_DCXO_CKEN_SW */
	0x142, 0x1, 0x0,/* TOP_SPMI_CON0 */
	0x143, 0x1, 0x0,/* TOP_SPMI_CON1 */
	0x243, 0x8, 0x0,/* TOP_VRCTL_VR1_EN */
	0x514, 0xFF, 0x7F,/* SCK_TOP_CKPDN_CON0_L */
	0x51A, 0x1F, 0x10,/* SCK_TOP_CKHWEN_CON0_L */
	0x54C, 0xFF, 0x21,/* XO_BUF_CTL0_L */
	0x54D, 0x3F, 0x0,/* XO_BUF_CTL0_H */
	0x54E, 0xFF, 0xC0,/* XO_BUF_CTL1_L */
	0x54F, 0x3F, 0x3,/* XO_BUF_CTL1_H */
	0x550, 0xFF, 0x0,/* XO_BUF_CTL2_L */
	0x551, 0x3F, 0x8,/* XO_BUF_CTL2_H */
	0x552, 0xFF, 0x40,/* XO_BUF_CTL3_L */
	0x553, 0x3F, 0x0,/* XO_BUF_CTL3_H */
	0x554, 0xFF, 0xC0,/* XO_BUF_CTL4_L */
	0x555, 0x3F, 0x3,/* XO_BUF_CTL4_H */
	0x788, 0xDB, 0x49,/* DCXO_CW00 */
	0x78B, 0x6, 0x4,/* DCXO_CW00_H */
	0x790, 0x1, 0x0,/* DCXO_CW02 */
	0x79C, 0x4, 0x0,/* DCXO_CW08 */
	0x7A1, 0x36, 0x12,/* DCXO_CW09_H */
	0x7A8, 0x80, 0x0,/* DCXO_CW12 */
	0x7A9, 0x10, 0x10,/* DCXO_CW12_H */
	0xC0B, 0x5, 0x1,/* BM_TOP_CKPDN_CON0 */
	0xC0E, 0xFD, 0xE4,/* BM_TOP_CKPDN_CON1 */
	0xC17, 0x3, 0x3,/* BM_TOP_CKHWEN_CON0 */
	0xC21, 0x1, 0x1,/* BM_TOP_RST_CON2 */
	0xD07, 0x3, 0x3,/* FGADC_CON0 */
	0xD08, 0x1, 0x1,/* FGADC_CON1 */
	0xD0D, 0x7F, 0x78,/* FGADC_CON6 */
	0xD37, 0xFF, 0x1F,/* FGADC_SON_CON0_L */
	0xD38, 0xFF, 0x0,/* FGADC_SON_CON0_H */
	0xD39, 0xFF, 0x14,/* FGADC_SON_CON1_L */
	0xD3A, 0x3, 0x0,/* FGADC_SON_CON1_H */
	0xD3B, 0xFF, 0xFF,/* FGADC_SON_CON2_L */
	0xD3C, 0x3, 0x0,/* FGADC_SON_CON2_H */
	0xD3D, 0xFF, 0x1F,/* FGADC_SOFF_CON0_L */
	0xD3E, 0xFF, 0x0,/* FGADC_SOFF_CON0_H */
	0xD3F, 0xFF, 0x14,/* FGADC_SOFF_CON1_L */
	0xD40, 0x3, 0x0,/* FGADC_SOFF_CON1_H */
	0xD41, 0xFF, 0xFF,/* FGADC_SOFF_CON2_L */
	0xD42, 0x3, 0x0,/* FGADC_SOFF_CON2_H */
	0xF8C, 0xFF, 0x15,/* HK_TOP_CLK_CON0 */
	0xF8D, 0x3, 0x1,/* HK_TOP_CLK_CON1 */
	0x1188, 0xE0, 0x0,/* AUXADC_CON0 */
	0x1189, 0x3F, 0x14,/* AUXADC_CON1 */
	0x1199, 0x70, 0x60,/* AUXADC_AVG_CON6 */
	0x1208, 0x1, 0x0,/* AUXADC_IMP0 */
	0x120C, 0x2, 0x0,/* AUXADC_IMP4 */
	0x120D, 0x1, 0x0,/* AUXADC_LBAT0 */
	0x1218, 0x2, 0x0,/* AUXADC_LBAT11 */
	0x1219, 0x1, 0x1,/* AUXADC_LBAT2_0 */
	0x121A, 0x3, 0x2,/* AUXADC_LBAT2_1 */
	0x121B, 0x2, 0x2,/* AUXADC_LBAT2_2 */
	0x121E, 0x2, 0x2,/* AUXADC_LBAT2_5 */
	0x1224, 0x2, 0x0,/* AUXADC_LBAT2_11 */
	0x1225, 0x1, 0x1,/* AUXADC_BAT_TEMP_0 */
	0x1227, 0x3, 0x2,/* AUXADC_BAT_TEMP_2 */
	0x1228, 0x2, 0x2,/* AUXADC_BAT_TEMP_3 */
	0x122B, 0x2, 0x2,/* AUXADC_BAT_TEMP_6 */
	0x1231, 0x2, 0x0,/* AUXADC_BAT_TEMP_12 */
	0x1232, 0x1, 0x0,/* AUXADC_THR0 */
	0x123D, 0x2, 0x0,/* AUXADC_THR11 */
	0x1247, 0x19, 0x11,/* AUXADC_NAG_0 */
	0x1259, 0x2, 0x0,/* AUXADC_NAG_18 */
	0x140B, 0x7, 0x0,/* BUCK_TOP_CLK_CON0 */
	0x140E, 0x7, 0x7,/* BUCK_TOP_CLK_HWEN_CON0 */
	0x1488, 0x10, 0x0,/* BUCK_VBUCK1_SLP_CON */
	0x148F, 0x80, 0x80,/* BUCK_VBUCK1_OP_EN_2 */
	0x1496, 0x3, 0x0,/* BUCK_VBUCK1_CON0 */
	0x1508, 0x10, 0x0,/* BUCK_VBUCK2_SLP_CON */
	0x150F, 0x80, 0x80,/* BUCK_VBUCK2_OP_EN_2 */
	0x1516, 0x3, 0x0,/* BUCK_VBUCK2_CON0 */
	0x1588, 0x10, 0x0,/* BUCK_VBUCK3_SLP_CON */
	0x158F, 0x81, 0x81,/* BUCK_VBUCK3_OP_EN_2 */
	0x1592, 0x1, 0x1,/* BUCK_VBUCK3_OP_CFG_2 */
	0x1595, 0x1, 0x1,/* BUCK_VBUCK3_OP_MODE_2 */
	0x1596, 0x3, 0x1,/* BUCK_VBUCK3_CON0 */
	0x1608, 0x10, 0x0,/* BUCK_VBUCK4_SLP_CON */
	0x160F, 0x80, 0x80,/* BUCK_VBUCK4_OP_EN_2 */
	0x1616, 0x3, 0x0,/* BUCK_VBUCK4_CON0 */
	0x1688, 0x10, 0x0,/* BUCK_VBUCK5_SLP_CON */
	0x168D, 0x1, 0x1,/* BUCK_VBUCK5_OP_EN_0 */
	0x168F, 0x80, 0x80,/* BUCK_VBUCK5_OP_EN_2 */
	0x1690, 0x1, 0x1,/* BUCK_VBUCK5_OP_CFG_0 */
	0x1693, 0x1, 0x1,/* BUCK_VBUCK5_OP_MODE_0 */
	0x1696, 0x3, 0x1,/* BUCK_VBUCK5_CON0 */
	0x1708, 0x10, 0x0,/* BUCK_VBUCK6_SLP_CON */
	0x170F, 0x81, 0x81,/* BUCK_VBUCK6_OP_EN_2 */
	0x1712, 0x1, 0x1,/* BUCK_VBUCK6_OP_CFG_2 */
	0x1715, 0x1, 0x1,/* BUCK_VBUCK6_OP_MODE_2 */
	0x1716, 0x3, 0x1,/* BUCK_VBUCK6_CON0 */
	0x1788, 0x10, 0x0,/* BUCK_VS1_SLP_CON */
	0x178D, 0x1, 0x1,/* BUCK_VS1_OP_EN_0 */
	0x178E, 0x2, 0x2,/* BUCK_VS1_OP_EN_1 */
	0x178F, 0x80, 0x80,/* BUCK_VS1_OP_EN_2 */
	0x1790, 0x1, 0x1,/* BUCK_VS1_OP_CFG_0 */
	0x1791, 0x2, 0x0,/* BUCK_VS1_OP_CFG_1 */
	0x1793, 0x1, 0x1,/* BUCK_VS1_OP_MODE_0 */
	0x1794, 0x2, 0x0,/* BUCK_VS1_OP_MODE_1 */
	0x1796, 0x3, 0x1,/* BUCK_VS1_CON0 */
	0x1808, 0x10, 0x0,/* BUCK_VS2_SLP_CON */
	0x180D, 0x1, 0x1,/* BUCK_VS2_OP_EN_0 */
	0x180E, 0x2, 0x2,/* BUCK_VS2_OP_EN_1 */
	0x180F, 0x80, 0x80,/* BUCK_VS2_OP_EN_2 */
	0x1810, 0x1, 0x1,/* BUCK_VS2_OP_CFG_0 */
	0x1811, 0x2, 0x0,/* BUCK_VS2_OP_CFG_1 */
	0x1813, 0x1, 0x1,/* BUCK_VS2_OP_MODE_0 */
	0x1814, 0x2, 0x0,/* BUCK_VS2_OP_MODE_1 */
	0x1816, 0x3, 0x1,/* BUCK_VS2_CON0 */
	0x1B0D, 0x3, 0x3,/* TOP_TOP_CKHWEN_CON0 */
	0x1B0E, 0x1, 0x1,/* LDO_TOP_CLK_DCM_CON0 */
	0x1B3E, 0x2, 0x2,/* LDO_VRTC28_CON */
	0x1B87, 0x3, 0x1,/* LDO_VA12_CON0 */
	0x1B88, 0x8, 0x0,/* LDO_VA12_CON1 */
	0x1B89, 0x80, 0x0,/* LDO_VA12_CON2 */
	0x1B8C, 0x1, 0x1,/* LDO_VA12_OP_EN0 */
	0x1B8D, 0x2, 0x2,/* LDO_VA12_OP_EN1 */
	0x1B8E, 0x80, 0x80,/* LDO_VA12_OP_EN2 */
	0x1B8F, 0x1, 0x1,/* LDO_VA12_OP_CFG0 */
	0x1B90, 0x2, 0x0,/* LDO_VA12_OP_CFG1 */
	0x1B92, 0x1, 0x1,/* LDO_VA12_OP_MODE0 */
	0x1B93, 0x2, 0x0,/* LDO_VA12_OP_MODE1 */
	0x1B95, 0x3, 0x0,/* LDO_VAUD18_CON0 */
	0x1B96, 0x8, 0x0,/* LDO_VAUD18_CON1 */
	0x1B97, 0x80, 0x0,/* LDO_VAUD18_CON2 */
	0x1B9C, 0x80, 0x80,/* LDO_VAUD18_OP_EN2 */
	0x1BA3, 0x3, 0x0,/* LDO_VAUD28_CON0 */
	0x1BA4, 0x8, 0x0,/* LDO_VAUD28_CON1 */
	0x1BA5, 0x80, 0x0,/* LDO_VAUD28_CON2 */
	0x1BAA, 0x80, 0x80,/* LDO_VAUD28_OP_EN2 */
	0x1BB1, 0x3, 0x1,/* LDO_VAUX18_CON0 */
	0x1BB2, 0x8, 0x0,/* LDO_VAUX18_CON1 */
	0x1BB3, 0x80, 0x0,/* LDO_VAUX18_CON2 */
	0x1BB6, 0x1, 0x1,/* LDO_VAUX18_OP_EN0 */
	0x1BB8, 0x80, 0x80,/* LDO_VAUX18_OP_EN2 */
	0x1BB9, 0x1, 0x1,/* LDO_VAUX18_OP_CFG0 */
	0x1BBC, 0x1, 0x1,/* LDO_VAUX18_OP_MODE0 */
	0x1BBF, 0x3, 0x1,/* LDO_VBIF28_CON0 */
	0x1BC0, 0x8, 0x0,/* LDO_VBIF28_CON1 */
	0x1BC1, 0x80, 0x0,/* LDO_VBIF28_CON2 */
	0x1BC6, 0x81, 0x81,/* LDO_VBIF28_OP_EN2 */
	0x1BC9, 0x1, 0x0,/* LDO_VBIF28_OP_CFG2 */
	0x1BCC, 0x1, 0x1,/* LDO_VBIF28_OP_MODE2 */
	0x1BCD, 0x3, 0x0,/* LDO_VCN33_1_CON0 */
	0x1BCE, 0x8, 0x0,/* LDO_VCN33_1_CON1 */
	0x1BCF, 0x80, 0x0,/* LDO_VCN33_1_CON2 */
	0x1BD4, 0x80, 0x80,/* LDO_VCN33_1_OP_EN2 */
	0x1C07, 0x3, 0x0,/* LDO_VCN33_2_CON0 */
	0x1C08, 0x8, 0x0,/* LDO_VCN33_2_CON1 */
	0x1C09, 0x80, 0x0,/* LDO_VCN33_2_CON2 */
	0x1C0E, 0x80, 0x80,/* LDO_VCN33_2_OP_EN2 */
	0x1C15, 0x3, 0x0,/* LDO_VCN18_CON0 */
	0x1C16, 0x8, 0x0,/* LDO_VCN18_CON1 */
	0x1C17, 0x80, 0x0,/* LDO_VCN18_CON2 */
	0x1C1C, 0x80, 0x80,/* LDO_VCN18_OP_EN2 */
	0x1C23, 0x3, 0x0,/* LDO_VRFCK_CON0 */
	0x1C24, 0x8, 0x0,/* LDO_VRFCK_CON1 */
	0x1C25, 0x80, 0x0,/* LDO_VRFCK_CON2 */
	0x1C2A, 0xC0, 0xC0,/* LDO_VRFCK_OP_EN2 */
	0x1C2D, 0x40, 0x0,/* LDO_VRFCK_OP_CFG2 */
	0x1C30, 0x40, 0x0,/* LDO_VRFCK_OP_MODE2 */
	0x1C31, 0x3, 0x0,/* LDO_VBBCK_CON0 */
	0x1C32, 0x8, 0x0,/* LDO_VBBCK_CON1 */
	0x1C33, 0x80, 0x0,/* LDO_VBBCK_CON2 */
	0x1C38, 0xC0, 0xC0,/* LDO_VBBCK_OP_EN2 */
	0x1C3B, 0x40, 0x0,/* LDO_VBBCK_OP_CFG2 */
	0x1C3E, 0x40, 0x0,/* LDO_VBBCK_OP_MODE2 */
	0x1C3F, 0x3, 0x1,/* LDO_VXO22_CON0 */
	0x1C40, 0x8, 0x0,/* LDO_VXO22_CON1 */
	0x1C41, 0x80, 0x0,/* LDO_VXO22_CON2 */
	0x1C46, 0x81, 0x81,/* LDO_VXO22_OP_EN2 */
	0x1C49, 0x1, 0x1,/* LDO_VXO22_OP_CFG2 */
	0x1C4C, 0x1, 0x1,/* LDO_VXO22_OP_MODE2 */
	0x1C4D, 0x3, 0x1,/* LDO_VM18_CON0 */
	0x1C4E, 0x8, 0x0,/* LDO_VM18_CON1 */
	0x1C4F, 0x80, 0x0,/* LDO_VM18_CON2 */
	0x1C52, 0x1, 0x1,/* LDO_VM18_OP_EN0 */
	0x1C54, 0x80, 0x80,/* LDO_VM18_OP_EN2 */
	0x1C55, 0x1, 0x1,/* LDO_VM18_OP_CFG0 */
	0x1C58, 0x1, 0x1,/* LDO_VM18_OP_MODE0 */
	0x1C87, 0x3, 0x1,/* LDO_VMDDR_CON0 */
	0x1C88, 0x8, 0x0,/* LDO_VMDDR_CON1 */
	0x1C89, 0x80, 0x0,/* LDO_VMDDR_CON2 */
	0x1C8C, 0x1, 0x1,/* LDO_VMDDR_OP_EN0 */
	0x1C8E, 0x80, 0x80,/* LDO_VMDDR_OP_EN2 */
	0x1C8F, 0x1, 0x1,/* LDO_VMDDR_OP_CFG0 */
	0x1C92, 0x1, 0x1,/* LDO_VMDDR_OP_MODE0 */
	0x1C95, 0x3, 0x0,/* LDO_VMDDQ_CON0 */
	0x1C96, 0x8, 0x0,/* LDO_VMDDQ_CON1 */
	0x1C97, 0x80, 0x0,/* LDO_VMDDQ_CON2 */
	0x1C9C, 0x80, 0x80,/* LDO_VMDDQ_OP_EN2 */
	0x1CA3, 0x3, 0x0,/* LDO_VEFUSE_CON0 */
	0x1CA4, 0x8, 0x0,/* LDO_VEFUSE_CON1 */
	0x1CA5, 0x80, 0x0,/* LDO_VEFUSE_CON2 */
	0x1CAA, 0x80, 0x80,/* LDO_VEFUSE_OP_EN2 */
	0x1CB1, 0x3, 0x0,/* LDO_VEMC_CON0 */
	0x1CB2, 0x8, 0x0,/* LDO_VEMC_CON1 */
	0x1CB3, 0x80, 0x0,/* LDO_VEMC_CON2 */
	0x1CB8, 0x80, 0x80,/* LDO_VEMC_OP_EN2 */
	0x1CBF, 0x3, 0x1,/* LDO_VUFS_CON0 */
	0x1CC0, 0x8, 0x0,/* LDO_VUFS_CON1 */
	0x1CC1, 0x80, 0x0,/* LDO_VUFS_CON2 */
	0x1CC6, 0x80, 0x80,/* LDO_VUFS_OP_EN2 */
	0x1CCD, 0x3, 0x1,/* LDO_VIO18_CON0 */
	0x1CCE, 0x8, 0x0,/* LDO_VIO18_CON1 */
	0x1CCF, 0x80, 0x0,/* LDO_VIO18_CON2 */
	0x1CD4, 0x80, 0x80,/* LDO_VIO18_OP_EN2 */
	0x1D07, 0x3, 0x1,/* LDO_VRF18_CON0 */
	0x1D08, 0x8, 0x0,/* LDO_VRF18_CON1 */
	0x1D09, 0x80, 0x0,/* LDO_VRF18_CON2 */
	0x1D0E, 0x82, 0x82,/* LDO_VRF18_OP_EN2 */
	0x1D11, 0x2, 0x0,/* LDO_VRF18_OP_CFG2 */
	0x1D14, 0x2, 0x2,/* LDO_VRF18_OP_MODE2 */
	0x1D15, 0x3, 0x1,/* LDO_VRF12_CON0 */
	0x1D16, 0x8, 0x0,/* LDO_VRF12_CON1 */
	0x1D17, 0x80, 0x0,/* LDO_VRF12_CON2 */
	0x1D1C, 0x82, 0x82,/* LDO_VRF12_OP_EN2 */
	0x1D1F, 0x2, 0x0,/* LDO_VRF12_OP_CFG2 */
	0x1D22, 0x2, 0x2,/* LDO_VRF12_OP_MODE2 */
	0x1D23, 0x3, 0x1,/* LDO_VRF09_CON0 */
	0x1D24, 0x8, 0x0,/* LDO_VRF09_CON1 */
	0x1D25, 0x80, 0x0,/* LDO_VRF09_CON2 */
	0x1D2A, 0x82, 0x82,/* LDO_VRF09_OP_EN2 */
	0x1D2D, 0x2, 0x0,/* LDO_VRF09_OP_CFG2 */
	0x1D30, 0x2, 0x2,/* LDO_VRF09_OP_MODE2 */
	0x1D31, 0x3, 0x1,/* LDO_VRFVA12_CON0 */
	0x1D32, 0x8, 0x0,/* LDO_VRFVA12_CON1 */
	0x1D33, 0x80, 0x0,/* LDO_VRFVA12_CON2 */
	0x1D36, 0x1, 0x1,/* LDO_VRFVA12_OP_EN0 */
	0x1D38, 0x80, 0x80,/* LDO_VRFVA12_OP_EN2 */
	0x1D39, 0x1, 0x1,/* LDO_VRFVA12_OP_CFG0 */
	0x1D3C, 0x1, 0x1,/* LDO_VRFVA12_OP_MODE0 */
	0x1D3F, 0x3, 0x1,/* LDO_VRFIO18_CON0 */
	0x1D40, 0x8, 0x0,/* LDO_VRFIO18_CON1 */
	0x1D41, 0x80, 0x0,/* LDO_VRFIO18_CON2 */
	0x1D44, 0x1, 0x1,/* LDO_VRFIO18_OP_EN0 */
	0x1D46, 0x80, 0x80,/* LDO_VRFIO18_OP_EN2 */
	0x1D47, 0x1, 0x1,/* LDO_VRFIO18_OP_CFG0 */
	0x1D4A, 0x1, 0x1,/* LDO_VRFIO18_OP_MODE0 */
	0x1D4D, 0x3, 0x0,/* LDO_VMCH_CON0 */
	0x1D4E, 0x8, 0x0,/* LDO_VMCH_CON1 */
	0x1D4F, 0x80, 0x0,/* LDO_VMCH_CON2 */
	0x1D54, 0x80, 0x80,/* LDO_VMCH_OP_EN2 */
	0x1D87, 0x3, 0x0,/* LDO_VMC_CON0 */
	0x1D88, 0x8, 0x0,/* LDO_VMC_CON1 */
	0x1D89, 0x80, 0x0,/* LDO_VMC_CON2 */
	0x1D8E, 0x80, 0x80,/* LDO_VMC_OP_EN2 */
	0x1D95, 0x3, 0x0,/* LDO_VSIM1_CON0 */
	0x1D96, 0x8, 0x0,/* LDO_VSIM1_CON1 */
	0x1D97, 0x80, 0x0,/* LDO_VSIM1_CON2 */
	0x1D9C, 0x80, 0x80,/* LDO_VSIM1_OP_EN2 */
	0x1DA4, 0x3, 0x0,/* LDO_VSIM2_CON0 */
	0x1DA5, 0x8, 0x0,/* LDO_VSIM2_CON1 */
	0x1DA6, 0x80, 0x0,/* LDO_VSIM2_CON2 */
	0x1DAB, 0x80, 0x80,/* LDO_VSIM2_OP_EN2 */
	0x1DB3, 0x3, 0x1,/* LDO_VUSB_CON0 */
	0x1DB4, 0x8, 0x0,/* LDO_VUSB_CON1 */
	0x1DB5, 0x80, 0x0,/* LDO_VUSB_CON2 */
	0x1DB8, 0x1, 0x1,/* LDO_VUSB_OP_EN0 */
	0x1DBA, 0x80, 0x80,/* LDO_VUSB_OP_EN2 */
	0x1DBB, 0x1, 0x1,/* LDO_VUSB_OP_CFG0 */
	0x1DBE, 0x1, 0x1,/* LDO_VUSB_OP_MODE0 */
	0x1DC1, 0x3, 0x0,/* LDO_VIBR_CON0 */
	0x1DC2, 0x8, 0x0,/* LDO_VIBR_CON1 */
	0x1DC3, 0x80, 0x0,/* LDO_VIBR_CON2 */
	0x1DC8, 0x80, 0x80,/* LDO_VIBR_OP_EN2 */
	0x1DCF, 0x3, 0x1,/* LDO_VIO28_CON0 */
	0x1DD0, 0x8, 0x0,/* LDO_VIO28_CON1 */
	0x1DD1, 0x80, 0x0,/* LDO_VIO28_CON2 */
	0x1DD6, 0x80, 0x80,/* LDO_VIO28_OP_EN2 */
	0x1E07, 0x3, 0x0,/* LDO_VFP_CON0 */
	0x1E08, 0x8, 0x0,/* LDO_VFP_CON1 */
	0x1E09, 0x80, 0x0,/* LDO_VFP_CON2 */
	0x1E0E, 0x80, 0x80,/* LDO_VFP_OP_EN2 */
	0x1E15, 0x3, 0x0,/* LDO_VTP_CON0 */
	0x1E16, 0x8, 0x0,/* LDO_VTP_CON1 */
	0x1E17, 0x80, 0x0,/* LDO_VTP_CON2 */
	0x1E1C, 0x80, 0x80,/* LDO_VTP_OP_EN2 */
	0x1E87, 0x3, 0x1,/* LDO_VSRAM_MD_CON0 */
	0x1E88, 0x8, 0x0,/* LDO_VSRAM_MD_CON1 */
	0x1E89, 0x80, 0x0,/* LDO_VSRAM_MD_CON2 */
	0x1E94, 0x1, 0x1,/* LDO_VSRAM_MD_OP_EN0 */
	0x1E96, 0x80, 0x80,/* LDO_VSRAM_MD_OP_EN2 */
	0x1E97, 0x1, 0x1,/* LDO_VSRAM_MD_OP_CFG0 */
	0x1E9A, 0x1, 0x1,/* LDO_VSRAM_MD_OP_MODE0 */
	0x1E9D, 0x3, 0x0,/* LDO_VSRAM_PROC1_CON0 */
	0x1E9E, 0x8, 0x0,/* LDO_VSRAM_PROC1_CON1 */
	0x1E9F, 0x80, 0x0,/* LDO_VSRAM_PROC1_CON2 */
	0x1EAC, 0x80, 0x80,/* LDO_VSRAM_PROC1_OP_EN2 */
	0x1F07, 0x3, 0x0,/* LDO_VSRAM_PROC2_CON0 */
	0x1F08, 0x8, 0x0,/* LDO_VSRAM_PROC2_CON1 */
	0x1F09, 0x80, 0x0,/* LDO_VSRAM_PROC2_CON2 */
	0x1F16, 0x80, 0x80,/* LDO_VSRAM_PROC2_OP_EN2 */
	0x1F1D, 0x3, 0x1,/* LDO_VSRAM_OTHERS_CON0 */
	0x1F1E, 0x8, 0x0,/* LDO_VSRAM_OTHERS_CON1 */
	0x1F1F, 0x80, 0x0,/* LDO_VSRAM_OTHERS_CON2 */
	0x1F2C, 0x81, 0x81,/* LDO_VSRAM_OTHERS_OP_EN2 */
	0x1F2F, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_CFG2 */
	0x1F32, 0x1, 0x1,/* LDO_VSRAM_OTHERS_OP_MODE2 */
	0x218E, 0x1E, 0x1E,/* XPP_TOP_CKPDN_CON0 */
	0x220B, 0x77, 0x0,/* ISINK_EN_CTRL0 */
0x220C, 0x70, 0x0 /* ISINK_EN_CTRL1 */
};

const unsigned int *AP_PMIC_REG_6377_gs_sodi3p0_32kless =
				AP_PMIC_REG_MT6377_gs_sodi3p0_32kless_data;

unsigned int AP_PMIC_REG_6377_gs_sodi3p0_32kless_len = 936;
