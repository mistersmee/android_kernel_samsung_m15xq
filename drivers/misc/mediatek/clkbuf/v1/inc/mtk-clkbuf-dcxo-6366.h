/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 MediaTek Inc.
 * Author: ky.liu <ky.liu@mediatek.com>
 */
#ifndef CLKBUF_DCXO_6366_H
#define CLKBUF_DCXO_6366_H

#include "mtk-clkbuf-dcxo.h"

#define MT6366_DCXO_CW00			(0x788)
#define MT6366_DCXO_CW03			(0x792)
#define MT6366_DCXO_CW08			(0x79c)
#define MT6366_DCXO_CW09			(0x79e)
#define MT6366_DCXO_CW11			(0x7a2)
#define MT6366_DCXO_CW12			(0x7a8)
#define MT6366_DCXO_CW13			(0x7aa)
#define MT6366_DCXO_CW15			(0x7ae)
#define MT6366_DCXO_CW16			(0x7b0)
#define MT6366_DCXO_CW18			(0x7b4)
#define MT6366_DCXO_CW19			(0x7b6)
#define MT6366_DCXO_CW23			(0x7be)

/* Register_DCXO_REG*/
#define MT6366_XO_EXTBUF1_MODE_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF1_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF1_MODE_SHIFT		(0)
#define MT6366_XO_EXTBUF1_EN_M_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF1_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF1_EN_M_SHIFT		(2)
#define MT6366_XO_EXTBUF2_MODE_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF2_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF2_MODE_SHIFT		(3)
#define MT6366_XO_EXTBUF2_EN_M_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF2_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF2_EN_M_SHIFT		(5)
#define MT6366_XO_EXTBUF3_MODE_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF3_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF3_MODE_SHIFT		(6)
#define MT6366_XO_EXTBUF3_EN_M_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF3_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF3_EN_M_SHIFT		(8)
#define MT6366_XO_EXTBUF4_MODE_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF4_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF4_MODE_SHIFT		(9)
#define MT6366_XO_EXTBUF4_EN_M_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_EXTBUF4_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF4_EN_M_SHIFT		(11)

#define MT6366_XO_EXTBUF6_MODE_ADDR		(MT6366_DCXO_CW11)
#define MT6366_XO_EXTBUF6_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF6_MODE_SHIFT		(8)
#define MT6366_XO_EXTBUF6_EN_M_ADDR		(MT6366_DCXO_CW11)
#define MT6366_XO_EXTBUF6_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF6_EN_M_SHIFT		(10)
#define MT6366_XO_EXTBUF7_MODE_ADDR		(MT6366_DCXO_CW11)
#define MT6366_XO_EXTBUF7_MODE_MASK		(0x3)
#define MT6366_XO_EXTBUF7_MODE_SHIFT		(11)
#define MT6366_XO_EXTBUF7_EN_M_ADDR		(MT6366_DCXO_CW11)
#define MT6366_XO_EXTBUF7_EN_M_MASK		(0x1)
#define MT6366_XO_EXTBUF7_EN_M_SHIFT		(13)

#define MT6366_XO_BB_LPM_EN_M_ADDR		(MT6366_DCXO_CW00)
#define MT6366_XO_BB_LPM_EN_M_MASK		(0x1)
#define MT6366_XO_BB_LPM_EN_M_SHIFT		(12)
#define MT6366_XO_BB_LPM_EN_SEL_ADDR		(MT6366_DCXO_CW23)
#define MT6366_XO_BB_LPM_EN_SEL_MASK		(0x1)
#define MT6366_XO_BB_LPM_EN_SEL_SHIFT		(0)
#define MT6366_XO_EXTBUF1_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF1_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF1_BBLPM_EN_MASK_SHIFT	(1)
#define MT6366_XO_EXTBUF2_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF2_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF2_BBLPM_EN_MASK_SHIFT	(2)
#define MT6366_XO_EXTBUF3_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF3_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF3_BBLPM_EN_MASK_SHIFT	(3)
#define MT6366_XO_EXTBUF4_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF4_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF4_BBLPM_EN_MASK_SHIFT	(4)
#define MT6366_XO_EXTBUF6_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF6_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF6_BBLPM_EN_MASK_SHIFT	(5)
#define MT6366_XO_EXTBUF7_BBLPM_EN_MASK_ADDR	(MT6366_DCXO_CW23)
#define MT6366_XO_EXTBUF7_BBLPM_EN_MASK_MASK	(0x1)
#define MT6366_XO_EXTBUF7_BBLPM_EN_MASK_SHIFT	(6)


#define MT6366_RG_XO_EXTBUF1_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF1_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF1_HD_SHIFT	(2)
#define MT6366_RG_XO_EXTBUF2_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF2_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF2_HD_SHIFT	(4)
#define MT6366_RG_XO_EXTBUF3_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF3_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF3_HD_SHIFT	(6)
#define MT6366_RG_XO_EXTBUF4_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF4_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF4_HD_SHIFT	(8)
#define MT6366_RG_XO_EXTBUF6_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF6_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF6_HD_SHIFT	(12)
#define MT6366_RG_XO_EXTBUF7_HD_ADDR		(MT6366_DCXO_CW15)
#define MT6366_RG_XO_EXTBUF7_HD_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF7_HD_SHIFT	(14)

#define MT6366_XO_STATIC_AUXOUT_SEL_ADDR	(MT6366_DCXO_CW18)
#define MT6366_XO_STATIC_AUXOUT_SEL_MASK	(0x3f)
#define MT6366_XO_STATIC_AUXOUT_SEL_SHIFT	(0)
#define MT6366_XO_AUXOUT_SEL_ADDR		(MT6366_DCXO_CW18)
#define MT6366_XO_AUXOUT_SEL_MASK		(0x3ff)
#define MT6366_XO_AUXOUT_SEL_SHIFT		(6)
#define MT6366_XO_STATIC_AUXOUT_ADDR		(MT6366_DCXO_CW19)
#define MT6366_XO_STATIC_AUXOUT_MASK		(0xffff)
#define MT6366_XO_STATIC_AUXOUT_SHIFT		(0)

#define MT6366_RG_XO_EXTBUF1_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF1_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF1_ISET_SHIFT	(0)
#define MT6366_RG_XO_EXTBUF2_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF2_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF2_ISET_SHIFT	(2)
#define MT6366_RG_XO_EXTBUF3_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF3_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF3_ISET_SHIFT	(4)
#define MT6366_RG_XO_EXTBUF4_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF4_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF4_ISET_SHIFT	(6)
#define MT6366_RG_XO_EXTBUF6_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF6_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF6_ISET_SHIFT	(10)
#define MT6366_RG_XO_EXTBUF7_ISET_ADDR		(MT6366_DCXO_CW16)
#define MT6366_RG_XO_EXTBUF7_ISET_MASK		(0x3)
#define MT6366_RG_XO_EXTBUF7_ISET_SHIFT	(12)

enum MT6366_XO_NAME {
	XO_SOC = 0,
	XO_WCN,
	XO_NFC,
	XO_CEL,
	XO_RSV1,
	XO_RSV2,
	XO_EXT,
	MT6366_XO_NUM,
};

#endif /* CLKBUF_DCXO_6366_H */
