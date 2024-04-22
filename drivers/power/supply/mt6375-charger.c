// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 MediaTek Inc.
 *
 * Author: ShuFan Lee <shufan_lee@richtek.com>
 */

#include <linux/completion.h>
#include <linux/iio/consumer.h>
#include <linux/atomic.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/reboot.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/workqueue.h>

#include "charger_class.h"
#include "mtk_charger.h"

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
#include <tcpm.h>
#if IS_ENABLED(CONFIG_PDIC_NOTIFIER)
#include <linux/usb/typec/common/pdic_notifier.h>
#endif
#if IS_ENABLED(CONFIG_USB_NOTIFY_LAYER)
#include <linux/usb_notify.h>
#endif
#include <../drivers/battery/common/sec_charging_common.h>
#include <linux/battery/sec_pd.h>

static char __read_mostly *f_mode;
module_param(f_mode, charp, 0444);
#endif
#if IS_ENABLED(CONFIG_PDIC_NOTIFIER)
#include <linux/usb/typec/common/pdic_param.h>
#include <linux/usb/typec/common/pdic_notifier.h>
#endif
#if IS_ENABLED(CONFIG_MUIC_NOTIFIER)
#include <linux/muic/common/muic.h>
#endif

#if IS_ENABLED(CONFIG_MTK_CHGLOG_SUPPORT)
static bool dbg_log_en = true;
#else
static bool dbg_log_en;
#endif
module_param(dbg_log_en, bool, 0644);
#define mt_dbg(dev, fmt, ...) \
	do { \
		if (dbg_log_en) \
			dev_info(dev, "%s " fmt, __func__, ##__VA_ARGS__); \
	} while (0)
#define PHY_MODE_BC11_SET 1
#define PHY_MODE_BC11_CLR 2

#define M_TO_U(val)	((val) * 1000)
#define U_TO_M(val)	((val) / 1000)

#define MT6375_MANUFACTURER	"Mediatek"

/* To support AFC TA detection */
#if IS_ENABLED(CONFIG_MTK_AFC_SUPPORT)
#define MT6375_AFC_SUPPORT
#endif

#define MT6375_REG_CORE_CTRL2	0x106
#define MT6375_REG_TM_PAS_CODE1	0x107
#define MT6375_REG_CHG_BATPRO	0x117
#define MT6375_REG_CHG_TOP1	0x120
#define MT6375_REG_CHG_TOP2	0x121
#define MT6375_REG_CHG_AICR	0x122
#define MT6375_REG_CHG_MIVR	0x123
#define MT6375_REG_CHG_VCHG	0x125
#define MT6375_REG_CHG_ICHG	0x126
#define MT6375_REG_CHG_TMR	0x127
#define MT6375_REG_CHG_EOC	0x128
#define MT6375_REG_CHG_VSYS	0x129
#define MT6375_REG_CHG_WDT	0x12A
#define MT6375_REG_CHG_PUMPX	0x12B
#define MT6375_REG_CHG_AICC1	0x12C
#define MT6375_REG_CHG_AICC2	0x12D
#define MT6375_REG_OTG_LBP	0x130
#define MT6375_REG_OTG_V	0x131
#define MT6375_REG_OTG_C	0x132
#define MT6375_REG_BAT_COMP	0x133
#define MT6375_REG_CHG_STAT	0x134
#define MT6375_REG_CHG_DUMY0	0x135
#define MT6375_REG_CHG_HD_TOP1	0x13B
#define MT6375_REG_CHG_HD_BUCK5	0x140
#define MT6375_REG_BC12_FUNC	0x150
#define MT6375_REG_BC12_STAT	0x151
#define MT6375_REG_DPDM_CTRL1	0x153
#define MT6375_REG_DPDM_CTRL2	0x154
#define MT6375_REG_DPDM_CTRL3   0x155
#define MT6375_REG_DPDM_CTRL4	0x156
#define MT6375_REG_DPDM_CTRL5	0x157
#define MT6375_REG_USBID_CTRL1	0x15D
#define MT6375_REG_USBID_CTRL2	0x15E
#define MT6375_REG_CHRD_CTRL2	0x161
#define MT6375_REG_AFC_CONFIG	0x164
#define MT6375_REG_AFC_CONTROL	0x165
#define MT6375_REG_AFC_CMD	0x16A
#define MT6375_REG_VBAT_MON_RPT	0x19C
#define MT6375_REG_BATEND_CODE	0x19E
#define MT6375_REG_ADC_CONFG1	0x1A4
#define MT6375_REG_ADC_ZCV_RPT	0x1CA
#define MT6375_REG_AFC_EVENT	0x1D6
#define MT6375_REG_CHG_STAT0	0x1E0
#define MT6375_REG_CHG_STAT1	0x1E1
#define MT6375_REG_AFC_MASK	0x1F6

#define MT6375_MSK_BATFET_DIS	0x40
#define MT6375_MSK_BLEED_DIS_EN	BIT(7)
#define MT6375_MSK_OTG_EN	0x04
#define MT6375_MSK_OTG_CV	0x3F
#define MT6375_MSK_OTG_CC	0x07
#define MT6375_MSK_CLK_FREQ	0xC0
#define MT6375_MSK_COMP_CLAMP	0x03
#define MT6375_MSK_BUCK_RAMPOFT	0xC0
#define MT6375_MSK_RESP		0x80
#define MT6375_MSK_CMDCPL	0x20
#define MT6375_MSK_ST_CHG_BATOV	BIT(1)
#define MT6375_MSK_ST_CHG_SYSOV	BIT(2)
#define MT6375_MSK_ST_CHG_TOUT	BIT(3)
#define MT6375_MSK_ST_WDT_IN	BIT(7)
#define MT6375_MSK_CHRDET_UV	BIT(0)
#define MT6375_MSK_CHRDET_OV	BIT(1)
#define MT6375_MSK_OTP0_STAT	BIT(0)

#define ADC_CONV_TIME_US	2200
#define ADC_VBAT_SCALE		1250
#define ADC_TO_VBAT_RAW(vbat)	((vbat) * 1000 / ADC_VBAT_SCALE)
#define ADC_FROM_VBAT_RAW(raw)	((raw) * ADC_VBAT_SCALE / 1000)

#define NORMAL_CHARGING_CURR_UA	500000
#define FAST_CHARGING_CURR_UA	1500000
#define RECHG_THRESHOLD		100
#define DEFAULT_PMIC_UVLO_mV	2000

#define POWER_SUPPLY_EXT_HEALTH	0

enum mt6375_chg_reg_field {
	/* MT6375_REG_CORE_CTRL2 */
	F_SHIP_RST_DIS, F_PD_MDEN,
	/* MT6375_REG_CHG_BATPRO */
	F_BATINT, F_BATPROTECT_EN,
	/* MT6375_REG_CHG_TOP1 */
	F_CHG_EN, F_BUCK_EN, F_HZ, F_BATFET_DISDLY, F_BATFET_DIS, F_PP_PG_FLAG,
	/* MT6375_REG_CHG_TOP2 */
	F_VBUS_OV,
	/* MT6375_REG_CHG_AICR */
	F_ILIM_EN, F_IAICR,
	/* MT6375_REG_CHG_MIVR */
	F_VMIVR,
	/* MT6375_REG_CHG_VCHG */
	F_CV, F_VREC,
	/* MT6375_REG_CHG_ICHG */
	F_CC,
	/* MT6375_REG_CHG_TMR */
	F_CHG_TMR, F_CHG_TMR_EN,
	/* MT6375_REG_CHG_EOC */
	F_EOC_RST, F_TE, F_EOC_TIME, F_IEOC,
	/* MT6375_REG_CHG_VSYS */
	F_VSYSMIN, F_BLEED_DIS_EN,
	/* MT6375_REG_CHG_WDT */
	F_WDT, F_WDT_RST, F_WDT_EN,
	/* MT6375_REG_CHG_PUMPX */
	F_PE20_CODE, F_PE10_INC, F_PE_SEL, F_PE_EN,
	/* MT6375_REG_CHG_AICC1 */
	F_AICC_VTH, F_AICC_EN,
	/* MT6375_REG_CHG_AICC2 */
	F_AICC_RPT, F_AICC_ONESHOT,
	/* MT6375_REG_OTG_LBP */
	F_OTG_LBP,
	/* MT6375_REG_OTG_C */
	F_OTG_CC,
	/* MT6375_REG_BAT_COMP */
	F_IRCMP_V, F_IRCMP_R,
	/* MT6375_REG_CHG_STAT */
	F_IC_STAT,
	/* MT6375_REG_CHG_HD_TOP1 */
	F_FORCE_VBUS_SINK,
	/* MT6375_REG_BC12_FUNC */
	F_DCDT_SEL, F_SPEC_TA_EN, F_BC12_EN,
	/* MT6375_REG_BC12_STAT */
	F_PORT_STAT,
	/* MT6375_REG_DPDM_CTRL1 */
	F_DM_DET_EN, F_DP_DET_EN, F_DPDM_SW_VCP_EN, F_MANUAL_MODE,
	/* MT6375_REG_DPDM_CTRL2 */
	F_DM_LDO_VSEL, F_DM_LDO_EN, F_DP_LDO_VSEL, F_DP_LDO_EN,
	/* MT6375_REG_DPDM_CTRL3 */
	F_DM_DIS_RSEL, F_DM_DIS_REN, F_DP_DIS_RSEL, F_DP_DIS_REN,
	/* MT6375_REG_DPDM_CTRL4 */
	F_DP_PULL_RSEL, F_DP_PULL_REN,
	/* MT6375_REG_DPDM_CTRL5 */
	F_DM_PULL_RSEL, F_DM_PULL_REN,
	/* MT6375_REG_CHRD_CTRL2 */
	F_CHRD_OV_VREF,
	/* MT6375_REG_ADC_CONFG1 */
	F_VBAT_MON_EN,
	/* MT6375_REG_CHG_STAT0 */
	F_ST_PWR_RDY,
	/* MT6375_REG_CHG_STAT1 */
	F_ST_MIVR,
	/* MT6375_REG_AFC_CONFIG */
	F_AFC_EN,
	/* MT6375_REG_AFC_CONTROL */
	F_AFC_CONTROL,
	/* MT6375_REG_AFC_CMD */
	F_AFC_CMD,
	/* MT6375_REG_AFC_EVENT */
	F_CMDCPL_EVT, F_RESP_EVT,
	/* MT6375_REG_USBID_CTRL1 */
	F_IS_TDET, F_ID_RUPSEL, F_USBID_EN,
	/* MT6375_REG_USBID_CTRL2 */
	F_USBID_FLOATING,
	F_MAX,
};

enum {
	CHG_STAT_SLEEP,
	CHG_STAT_VBUS_RDY,
	CHG_STAT_TRICKLE,
	CHG_STAT_PRE,
	CHG_STAT_FAST,
	CHG_STAT_EOC,
	CHG_STAT_BKGND,
	CHG_STAT_DONE,
	CHG_STAT_FAULT,
	CHG_STAT_OTG = 15,
	CHG_STAT_MAX,
};

enum {
	PORT_STAT_NOINFO,
	PORT_STAT_APPLE_10W = 8,
	PORT_STAT_SAMSUNG,
	PORT_STAT_APPLE_5W,
	PORT_STAT_APPLE_12W,
	PORT_STAT_UNKNOWN_TA,
	PORT_STAT_SDP,
	PORT_STAT_CDP,
	PORT_STAT_DCP,
};

enum mt6375_adc_chan {
	ADC_CHAN_CHGVINDIV5,
	ADC_CHAN_VSYS,
	ADC_CHAN_VBAT,
	ADC_CHAN_IBUS,
	ADC_CHAN_IBAT,
	ADC_CHAN_TEMP_JC,
	ADC_CHAN_USBDP,
	ADC_CHAN_USBDM,
	ADC_CHAN_SBU2,
	ADC_CHAN_MAX,
};

/* map with mtk_chg_type_det.c */
enum attach_type {
	ATTACH_TYPE_NONE,
	ATTACH_TYPE_PWR_RDY,
	ATTACH_TYPE_TYPEC,
	ATTACH_TYPE_PD,
	ATTACH_TYPE_PD_SDP,
	ATTACH_TYPE_PD_DCP,
	ATTACH_TYPE_PD_NONSTD,
};

enum mt6375_attach_trigger {
	ATTACH_TRIG_IGNORE,
	ATTACH_TRIG_PWR_RDY,
	ATTACH_TRIG_TYPEC,
};

enum mt6375_usbsw {
	USBSW_CHG = 0,
	USBSW_USB,
};

enum mt6375_chg_dtprop_type {
	DTPROP_U32,
	DTPROP_BOOL,
};

enum mt6375_chg_hvdet_type {
	HV_IRQ_NONE,
	HV_IRQ_DET,
	HV_AFC_DET,
};

struct mt6375_chg_data {
	struct device *dev;
	struct regmap *rmap;
	struct regmap_field *rmap_fields[F_MAX];
	struct power_supply *psy;
	struct power_supply_desc psy_desc;
	struct regulator_dev *rdev;
	struct regulator_desc rdesc;
	struct iio_channel *iio_adcs;
	struct mutex attach_lock;
	struct mutex pe_lock;
	struct mutex cv_lock;
	struct mutex hm_lock;
	struct workqueue_struct *wq;
	struct work_struct bc12_work;
	struct work_struct hvdcp_work;
	struct completion afc_done;
	struct completion pe_done;
	struct completion aicc_done;
	struct charger_device *chgdev;
	struct work_struct afcv_work;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	struct delayed_work sec_cable_work;
#endif
	struct delayed_work cc_hi_check_work;

	enum power_supply_usb_type psy_usb_type;
	bool pwr_rdy;
	atomic_t attach;
	bool bc12_dn;
	bool bc12_nsdp_retried;
	bool batprotect_en;
	u32 hm_use_cnt;
	u32 zcv;
	u32 cv;
	atomic_t eoc_cnt;
	atomic_t tchg;
	int vbat0_flag;
	unsigned int detach_irq;
	atomic_t no_6pin_used;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	unsigned int f_mode;
	int sec_cable_type;
#endif
	int afc_disable;
	int hvdcp_irq_status;
	int afc_set_voltage;
	int typec_rp_level;
	int lpm_bootmode;
	int hiccup_mode;
	int cc_hi_state;
	bool wd_state;
};

struct mt6375_chg_platform_data {
	u32 aicr;
	u32 mivr;
	u32 ichg;
	u32 ieoc;
	u32 cv;
	u32 wdt;
	u32 otg_lbp;
	u32 ircmp_v;
	u32 ircmp_r;
	u32 vbus_ov;
	u32 vrec;
	u32 chg_tmr;
	u32 dcdt_sel;
	u32 bc12_sel;
	u32 boot_mode;
	u32 boot_type;
	u32 pmic_uvlo;
	enum mt6375_attach_trigger attach_trig;
	const char *chg_name;
	bool chg_tmr_en;
	bool wdt_en;
	bool te_en;
	bool usb_killer_detect;
	bool spec_ta_en;
	int vctrl_otg_gpio;
};

struct mt6375_chg_range {
	u32 min;
	u32 max;
	u16 step;
	u8 offset;
	const u32 *table;
	u16 num_table;
	bool round_up;
};

struct mt6375_chg_field {
	const char *name;
	const struct mt6375_chg_range *range;
	struct reg_field field;
};

static const char *const mt6375_port_stat_names[] = {
	[PORT_STAT_NOINFO] = "No Info",
	[PORT_STAT_APPLE_10W] = "Apple 10W",
	[PORT_STAT_SAMSUNG] = "Samsung",
	[PORT_STAT_APPLE_5W] = "Apple 5W",
	[PORT_STAT_APPLE_12W] = "Apple 12W",
	[PORT_STAT_UNKNOWN_TA] = "Unknown TA",
	[PORT_STAT_SDP] = "SDP",
	[PORT_STAT_CDP] = "CDP",
	[PORT_STAT_DCP] = "DCP",
};

static const char *const mt6375_attach_trig_names[] = {
	"ignore", "pwr_rdy", "typec",
};

static const u32 mt6375_chg_vbus_ov[] = {
	5800, 6500, 11000, 14500,
};

static const u32 mt6375_chg_wdt[] = {
	8000, 40000, 80000, 160000,
};

static const u32 mt6375_chg_otg_cc[] = {
	500, 700, 1100, 1300, 1800, 2100, 2400,
};

static const u32 mt6375_usbid_rup[] = {
	500000, 75000, 5000, 1000,
};

static const u32 mt6375_usbid_src_ton[] = {
	400, 1000, 4000, 10000, 40000, 100000, 400000,
};

/* for regulator usage */
static const u32 mt6375_chg_otg_cc_micro[] = {
	500000, 700000, 1100000, 1300000, 1800000, 2100000, 2400000,
};

static const u32 mt6375_chg_dpdm_ldo_vsel[] = {
	600, 650, 700, 750, 1800, 2800, 3300,
};

static const u32 mt6375_chrd_ov_vref[] = {
	6000, 6500, 7000, 7500, 8500, 9500, 10500, 11500, 12500, 14500,
};

static const u32 mt6375_chg_eoc_time[] = {
	0, 15, 30, 45,
}; // unit : mins

#define MT6375_CHG_RANGE(_min, _max, _step, _offset, _ru) \
{ \
	.min = _min, \
	.max = _max, \
	.step = _step, \
	.offset = _offset, \
	.round_up = _ru, \
}

#define MT6375_CHG_RANGE_T(_table, _ru) \
	{ .table = _table, .num_table = ARRAY_SIZE(_table), .round_up = _ru, }

static const struct mt6375_chg_range mt6375_chg_ranges[F_MAX] = {
	[F_BATINT] = MT6375_CHG_RANGE(3900, 4710, 10, 0, false),
	[F_VBUS_OV] = MT6375_CHG_RANGE_T(mt6375_chg_vbus_ov, false),
	[F_IAICR] = MT6375_CHG_RANGE(100, 3225, 25, 2, false),
	[F_VMIVR] = MT6375_CHG_RANGE(3900, 13400, 100, 0, true),
	[F_CV] = MT6375_CHG_RANGE(3900, 4710, 10, 0, false),
	[F_VREC] = MT6375_CHG_RANGE(100, 200, 100, 0, false),
	[F_CC] = MT6375_CHG_RANGE(300, 3150, 50, 6, false),
	[F_CHG_TMR] = MT6375_CHG_RANGE(5, 20, 5, 0, false),
	[F_EOC_TIME] = MT6375_CHG_RANGE_T(mt6375_chg_eoc_time, false),
	[F_IEOC] = MT6375_CHG_RANGE(100, 800, 50, 1, false),
	[F_VSYSMIN] = MT6375_CHG_RANGE(3300, 4000, 100, 5, false),
	[F_WDT] = MT6375_CHG_RANGE_T(mt6375_chg_wdt, false),
	[F_PE20_CODE] = MT6375_CHG_RANGE(5500, 20000, 500, 0, false),
	[F_AICC_VTH] = MT6375_CHG_RANGE(3900, 13400, 100, 0, true),
	[F_AICC_RPT] = MT6375_CHG_RANGE(100, 3225, 25, 2, false),
	[F_OTG_LBP] = MT6375_CHG_RANGE(2700, 3800, 100, 4, false),
	[F_OTG_CC] = MT6375_CHG_RANGE_T(mt6375_chg_otg_cc, true),
	[F_IRCMP_V] = MT6375_CHG_RANGE(0, 224, 32, 0, false),
	[F_IRCMP_R] = MT6375_CHG_RANGE(0, 116900, 16700, 0, false),
	[F_DCDT_SEL] = MT6375_CHG_RANGE(0, 600, 300, 0, false),
	[F_DM_LDO_VSEL] = MT6375_CHG_RANGE_T(mt6375_chg_dpdm_ldo_vsel, false),
	[F_DP_LDO_VSEL] = MT6375_CHG_RANGE_T(mt6375_chg_dpdm_ldo_vsel, false),
	[F_CHRD_OV_VREF] = MT6375_CHG_RANGE_T(mt6375_chrd_ov_vref, false),
	[F_AFC_EN] = MT6375_CHG_RANGE(0, 3, 1, 0, false),
	[F_AFC_CONTROL] = MT6375_CHG_RANGE(0, 255, 1, 0, false),
	[F_AFC_CMD] = MT6375_CHG_RANGE(0, 255, 1, 0, false),
};

#define MT6375_CHG_FIELD_RANGE(_fd, _reg, _lsb, _msb, _range) \
	[_fd] = { \
		.name = #_fd, \
		.range = _range ? &mt6375_chg_ranges[_fd] : NULL, \
		.field = REG_FIELD(_reg, _lsb, _msb) \
	}

#define MT6375_CHG_FIELD(_fd, _reg, _lsb, _msb) \
	MT6375_CHG_FIELD_RANGE(_fd, _reg, _lsb, _msb, (_msb > _lsb))

static const struct mt6375_chg_field mt6375_chg_fields[F_MAX] = {
	MT6375_CHG_FIELD(F_SHIP_RST_DIS, MT6375_REG_CORE_CTRL2, 0, 0),
	MT6375_CHG_FIELD(F_PD_MDEN, MT6375_REG_CORE_CTRL2, 1, 1),
	MT6375_CHG_FIELD(F_BATINT, MT6375_REG_CHG_BATPRO, 0, 6),
	MT6375_CHG_FIELD(F_BATPROTECT_EN, MT6375_REG_CHG_BATPRO, 7, 7),
	MT6375_CHG_FIELD(F_CHG_EN, MT6375_REG_CHG_TOP1, 0, 0),
	MT6375_CHG_FIELD(F_BUCK_EN, MT6375_REG_CHG_TOP1, 1, 1),
	MT6375_CHG_FIELD(F_HZ, MT6375_REG_CHG_TOP1, 3, 3),
	MT6375_CHG_FIELD(F_BATFET_DISDLY, MT6375_REG_CHG_TOP1, 5, 5),
	MT6375_CHG_FIELD(F_BATFET_DIS, MT6375_REG_CHG_TOP1, 6, 6),
	MT6375_CHG_FIELD(F_PP_PG_FLAG, MT6375_REG_CHG_TOP1, 7, 7),
	MT6375_CHG_FIELD(F_VBUS_OV, MT6375_REG_CHG_TOP2, 0, 1),
	MT6375_CHG_FIELD(F_IAICR, MT6375_REG_CHG_AICR, 0, 6),
	MT6375_CHG_FIELD(F_ILIM_EN, MT6375_REG_CHG_AICR, 7, 7),
	MT6375_CHG_FIELD(F_VMIVR, MT6375_REG_CHG_MIVR, 0, 6),
	MT6375_CHG_FIELD(F_CV, MT6375_REG_CHG_VCHG, 0, 6),
	MT6375_CHG_FIELD_RANGE(F_VREC, MT6375_REG_CHG_VCHG, 7, 7, true),
	MT6375_CHG_FIELD(F_CC, MT6375_REG_CHG_ICHG, 0, 5),
	MT6375_CHG_FIELD(F_CHG_TMR, MT6375_REG_CHG_TMR, 4, 5),
	MT6375_CHG_FIELD(F_CHG_TMR_EN, MT6375_REG_CHG_TMR, 7, 7),
	MT6375_CHG_FIELD(F_EOC_RST, MT6375_REG_CHG_EOC, 0, 0),
	MT6375_CHG_FIELD(F_TE, MT6375_REG_CHG_EOC, 1, 1),
	MT6375_CHG_FIELD(F_EOC_TIME, MT6375_REG_CHG_EOC, 2, 3),
	MT6375_CHG_FIELD(F_IEOC, MT6375_REG_CHG_EOC, 4, 7),
	MT6375_CHG_FIELD(F_VSYSMIN, MT6375_REG_CHG_VSYS, 0, 3),
	MT6375_CHG_FIELD(F_BLEED_DIS_EN, MT6375_REG_CHG_VSYS, 7, 7),
	MT6375_CHG_FIELD(F_WDT, MT6375_REG_CHG_WDT, 0, 1),
	MT6375_CHG_FIELD(F_WDT_RST, MT6375_REG_CHG_WDT, 2, 2),
	MT6375_CHG_FIELD(F_WDT_EN, MT6375_REG_CHG_WDT, 3, 3),
	MT6375_CHG_FIELD(F_PE20_CODE, MT6375_REG_CHG_PUMPX, 0, 4),
	MT6375_CHG_FIELD(F_PE10_INC, MT6375_REG_CHG_PUMPX, 5, 5),
	MT6375_CHG_FIELD(F_PE_SEL, MT6375_REG_CHG_PUMPX, 6, 6),
	MT6375_CHG_FIELD(F_PE_EN, MT6375_REG_CHG_PUMPX, 7, 7),
	MT6375_CHG_FIELD(F_AICC_VTH, MT6375_REG_CHG_AICC1, 0, 6),
	MT6375_CHG_FIELD(F_AICC_EN, MT6375_REG_CHG_AICC1, 7, 7),
	MT6375_CHG_FIELD(F_AICC_RPT, MT6375_REG_CHG_AICC2, 0, 6),
	MT6375_CHG_FIELD(F_AICC_ONESHOT, MT6375_REG_CHG_AICC2, 7, 7),
	MT6375_CHG_FIELD(F_OTG_CC, MT6375_REG_OTG_C, 0, 2),
	MT6375_CHG_FIELD(F_OTG_LBP, MT6375_REG_OTG_LBP, 0, 3),
	MT6375_CHG_FIELD(F_IRCMP_V, MT6375_REG_BAT_COMP, 0, 2),
	MT6375_CHG_FIELD(F_IRCMP_R, MT6375_REG_BAT_COMP, 4, 6),
	MT6375_CHG_FIELD_RANGE(F_IC_STAT, MT6375_REG_CHG_STAT, 0, 3, false),
	MT6375_CHG_FIELD(F_FORCE_VBUS_SINK, MT6375_REG_CHG_HD_TOP1, 6, 6),
	MT6375_CHG_FIELD(F_DCDT_SEL, MT6375_REG_BC12_FUNC, 4, 5),
	MT6375_CHG_FIELD(F_SPEC_TA_EN, MT6375_REG_BC12_FUNC, 6, 6),
	MT6375_CHG_FIELD(F_BC12_EN, MT6375_REG_BC12_FUNC, 7, 7),
	MT6375_CHG_FIELD_RANGE(F_PORT_STAT, MT6375_REG_BC12_STAT, 0, 3, false),
	MT6375_CHG_FIELD(F_DM_DET_EN, MT6375_REG_DPDM_CTRL1, 0, 0),
	MT6375_CHG_FIELD(F_DP_DET_EN, MT6375_REG_DPDM_CTRL1, 1, 1),
	MT6375_CHG_FIELD(F_DPDM_SW_VCP_EN, MT6375_REG_DPDM_CTRL1, 5, 5),
	MT6375_CHG_FIELD(F_MANUAL_MODE, MT6375_REG_DPDM_CTRL1, 7, 7),
	MT6375_CHG_FIELD(F_DM_LDO_VSEL, MT6375_REG_DPDM_CTRL2, 0, 2),
	MT6375_CHG_FIELD(F_DM_LDO_EN, MT6375_REG_DPDM_CTRL2, 3, 3),
	MT6375_CHG_FIELD(F_DP_LDO_VSEL, MT6375_REG_DPDM_CTRL2, 4, 6),
	MT6375_CHG_FIELD(F_DP_LDO_EN, MT6375_REG_DPDM_CTRL2, 7, 7),
	MT6375_CHG_FIELD(F_DM_DIS_RSEL, MT6375_REG_DPDM_CTRL3, 0, 0),
	MT6375_CHG_FIELD(F_DM_DIS_REN, MT6375_REG_DPDM_CTRL3, 1, 1),
	MT6375_CHG_FIELD(F_DP_DIS_RSEL, MT6375_REG_DPDM_CTRL3, 4, 4),
	MT6375_CHG_FIELD(F_DP_DIS_REN, MT6375_REG_DPDM_CTRL3, 5, 5),
	MT6375_CHG_FIELD(F_DP_PULL_RSEL, MT6375_REG_DPDM_CTRL4, 6, 6),
	MT6375_CHG_FIELD(F_DP_PULL_REN, MT6375_REG_DPDM_CTRL4, 7, 7),
	MT6375_CHG_FIELD(F_DM_PULL_RSEL, MT6375_REG_DPDM_CTRL5, 6, 6),
	MT6375_CHG_FIELD(F_DM_PULL_REN, MT6375_REG_DPDM_CTRL5, 7, 7),
	MT6375_CHG_FIELD(F_CHRD_OV_VREF, MT6375_REG_CHRD_CTRL2, 0, 3),
	MT6375_CHG_FIELD(F_VBAT_MON_EN, MT6375_REG_ADC_CONFG1, 5, 5),
	MT6375_CHG_FIELD(F_ST_PWR_RDY, MT6375_REG_CHG_STAT0, 0, 0),
	MT6375_CHG_FIELD(F_ST_MIVR, MT6375_REG_CHG_STAT1, 7, 7),
	MT6375_CHG_FIELD(F_AFC_EN, MT6375_REG_AFC_CONFIG, 5, 6),
	MT6375_CHG_FIELD(F_AFC_CONTROL, MT6375_REG_AFC_CONTROL, 0, 7),
	MT6375_CHG_FIELD(F_AFC_CMD, MT6375_REG_AFC_CMD, 0, 7),
	MT6375_CHG_FIELD(F_CMDCPL_EVT, MT6375_REG_AFC_EVENT, 5, 5),
	MT6375_CHG_FIELD(F_RESP_EVT, MT6375_REG_AFC_EVENT, 7, 7),
	MT6375_CHG_FIELD_RANGE(F_IS_TDET, MT6375_REG_USBID_CTRL1, 2, 4, false),
	MT6375_CHG_FIELD_RANGE(F_ID_RUPSEL, MT6375_REG_USBID_CTRL1,
			       5, 6, false),
	MT6375_CHG_FIELD(F_USBID_EN, MT6375_REG_USBID_CTRL1, 7, 7),
	MT6375_CHG_FIELD(F_USBID_FLOATING, MT6375_REG_USBID_CTRL2, 1, 1),
};

#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
static int mt6375_get_vsys(struct charger_device *chgdev, u32 *vsys);
static int mt6375_get_adc(struct charger_device *chgdev, enum adc_channel chan,
					int *min, int *max);
#endif
#if IS_ENABLED(CONFIG_SEC_FACTORY)
static void mt6375_usbid_init(struct mt6375_chg_data *ddata, bool en);
#endif

static inline int mt6375_chg_field_set(struct mt6375_chg_data *ddata,
				       enum mt6375_chg_reg_field fd, u32 val);
static int mt6375_enable_hm(struct mt6375_chg_data *ddata, bool en)
{
	int ret = 0;

	mutex_lock(&ddata->hm_lock);
	if (en) {
		if (ddata->hm_use_cnt == 0) {
			ret = regmap_write(ddata->rmap, MT6375_REG_TM_PAS_CODE1,
					   0x69);
			if (ret < 0)
				goto out;
		}
		ddata->hm_use_cnt++;
	} else {
		if (ddata->hm_use_cnt == 1) {
			ret = regmap_write(ddata->rmap, MT6375_REG_TM_PAS_CODE1,
					   0x00);
			if (ret < 0)
				goto out;
		}
		if (ddata->hm_use_cnt > 0)
			ddata->hm_use_cnt--;
	}
out:
	mutex_unlock(&ddata->hm_lock);
	return ret;
}

static int mt6375_set_boost_param(struct mt6375_chg_data *ddata, bool bst)
{
	int i, ret;
	u8 val;

	static const u16 regs[] = {
		MT6375_REG_CHG_TOP2,
		MT6375_REG_CHG_DUMY0,
		MT6375_REG_CHG_HD_BUCK5,
		MT6375_REG_CHG_VSYS,
	};
	static const u8 msks[] = {
		MT6375_MSK_CLK_FREQ,
		MT6375_MSK_COMP_CLAMP,
		MT6375_MSK_BUCK_RAMPOFT,
		MT6375_MSK_BLEED_DIS_EN,
	};
	static const u8 buck[] = {
		0x01, 0x00, 0x01, 0x01,
	};
	static const u8 boost[] = {
		0x00, 0x03, 0x03, 0x00,
	};

	ret = mt6375_enable_hm(ddata, true);
	if (ret < 0)
		return ret;
	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		val = bst ? boost[i] : buck[i];
		val <<= ffs(msks[i]) - 1;
		ret = regmap_update_bits(ddata->rmap, regs[i], msks[i], val);
		if (ret < 0) {
			dev_err(ddata->dev,
				"failed to set reg0x%02X=0x%02X, msk0x%02X\n",
				regs[i], val, msks[i]);
			goto recover;
		}
	}
	goto out;
recover:
	/*
	 * we do not guarantee the recovery is OK
	 * keep the error code from above
	 */
	for (; i >= 0; i--) {
		val = bst ? buck[i] : boost[i];
		val <<= ffs(msks[i]) - 1;
		if (regmap_update_bits(ddata->rmap, regs[i], msks[i],
				       val) < 0) {
			dev_err(ddata->dev,
				"failed to set reg0x%02X=0x%02X, msk0x%02X\n",
				regs[i], val, msks[i]);
		}
	}
out:
	mt6375_enable_hm(ddata, false);
	return ret;
}

static bool mt6375_chg_is_usb_killer_dp(struct mt6375_chg_data *ddata)
{
	int i, ret, vdp, vdm;
	bool killer = false;
	static const u32 vdiff = 100;
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);
	static const struct {
		enum mt6375_chg_reg_field fd;
		u32 val;
	} settings[] = {
		{ F_MANUAL_MODE, 1 },
		{ F_DPDM_SW_VCP_EN, 1 },
		{ F_DP_DET_EN, 1 },
		{ F_DM_DET_EN, 1 },
		{ F_DP_LDO_VSEL, 1800 },
		{ F_DP_LDO_EN, 1 },
		{ F_DP_PULL_RSEL, 1 },
		{ F_DP_PULL_REN, 1 },
	};

	if (!pdata->usb_killer_detect) {
		mt_dbg(ddata->dev, "disabled\n");
		return false;
	}

	/* turn on usb dp 1.8V */
	for (i = 0; i < ARRAY_SIZE(settings); i++) {
		ret = mt6375_chg_field_set(ddata, settings[i].fd,
					   settings[i].val);
		if (ret < 0)
			goto recover;
	}
	--i;
	usleep_range(1000, 1100);
	/* check usb dpdm */
	ret = iio_read_channel_processed(&ddata->iio_adcs[ADC_CHAN_USBDP],
					 &vdp);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to read usbdp voltage\n");
		goto recover;
	}
	ret = iio_read_channel_processed(&ddata->iio_adcs[ADC_CHAN_USBDM],
					 &vdm);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to read usbdm voltage\n");
		goto recover;
	}
	vdp = U_TO_M(vdp);
	vdm = U_TO_M(vdm);
	mt_dbg(ddata->dev, "dp=%dmV, dm=%dmV, vdiff=%dmV\n", vdp, vdm,
	       abs(vdp - vdm));
	if (abs(vdp - vdm) < vdiff) {
		dev_info(ddata->dev, "%s: suspect usb killer\n", __func__);
		killer = true;
	}
recover:
	/* we do not guarantee the recovery is OK */
	for (; i >= 0; i--)
		mt6375_chg_field_set(ddata, settings[i].fd, 0);
	return killer;
}

static bool mt6375_chg_is_usb_killer_dm(struct mt6375_chg_data *ddata)
{
	int i, ret, vdp, vdm;
	bool killer = false;
	static const u32 vdiff = 100;
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);
	static const struct {
		enum mt6375_chg_reg_field fd;
		u32 val;
	} settings[] = {
		{ F_MANUAL_MODE, 1 },
		{ F_DPDM_SW_VCP_EN, 1 },
		{ F_DP_DET_EN, 1 },
		{ F_DM_DET_EN, 1 },
		{ F_DM_LDO_VSEL, 1800 },
		{ F_DM_LDO_EN, 1 },
		{ F_DM_PULL_RSEL, 1 },
		{ F_DM_PULL_REN, 1 },
	};

	if (!pdata->usb_killer_detect) {
		mt_dbg(ddata->dev, "disabled\n");
		return false;
	}

	/* turn on usb dm 1.8V */
	for (i = 0; i < ARRAY_SIZE(settings); i++) {
		ret = mt6375_chg_field_set(ddata, settings[i].fd,
					   settings[i].val);
		if (ret < 0)
			goto recover;
	}
	--i;
	usleep_range(1000, 1100);
	/* check usb dpdm */
	ret = iio_read_channel_processed(&ddata->iio_adcs[ADC_CHAN_USBDP],
					 &vdp);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to read usbdp voltage\n");
		goto recover;
	}
	ret = iio_read_channel_processed(&ddata->iio_adcs[ADC_CHAN_USBDM],
					 &vdm);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to read usbdm voltage\n");
		goto recover;
	}
	vdp = U_TO_M(vdp);
	vdm = U_TO_M(vdm);
	mt_dbg(ddata->dev, "dp=%dmV, dm=%dmV, vdiff=%dmV\n", vdp, vdm,
	       abs(vdp - vdm));
	if (abs(vdm - vdp) < vdiff) {
		dev_info(ddata->dev, "%s: suspect usb killer\n", __func__);
		killer = true;
	}
recover:
	/* we do not guarantee the recovery is OK */
	for (; i >= 0; i--)
		mt6375_chg_field_set(ddata, settings[i].fd, 0);
	return killer;
}

static bool mt6375_chg_is_usb_killer(struct mt6375_chg_data *ddata)
{
	if (!mt6375_chg_is_usb_killer_dp(ddata))
		return false;
	else
		return mt6375_chg_is_usb_killer_dm(ddata);
}

static void mt6375_chg_set_vctrl_otg_gpio(struct mt6375_chg_data *ddata, int val)
{
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

	if (!gpio_is_valid(pdata->vctrl_otg_gpio))
		return;
	pr_info("%s: val(%d)\n", __func__, val);
	gpio_direction_output(pdata->vctrl_otg_gpio, val);
}

static int mt6375_chg_regulator_enable(struct regulator_dev *rdev)
{
	int ret;
	struct mt6375_chg_data *ddata = rdev->reg_data;
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
	int event;
#endif
#if defined(CONFIG_USB_HW_PARAM)
	struct otg_notify *o_notify = get_otg_notify();
#endif

	if (mt6375_chg_is_usb_killer(ddata)) {
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		event = NOTIFY_EXTRA_USBKILLER;
		store_usblog_notify(NOTIFY_EXTRA, (void *)&event, NULL);
#endif
#if defined(CONFIG_USB_HW_PARAM)
		if (o_notify)
			inc_hw_param(o_notify, USB_CCIC_USB_KILLER_COUNT);
#endif
		return -EIO;
	}
	mt6375_chg_set_vctrl_otg_gpio(ddata, 1);
	ret = mt6375_set_boost_param(ddata, true);
	if (ret < 0)
		return ret;
	ret = regulator_enable_regmap(rdev);
	if (ret < 0) {
		mt6375_set_boost_param(ddata, false);
		return ret;
	}
	return 0;
}

static int mt6375_chg_regulator_disable(struct regulator_dev *rdev)
{
	int ret;
	struct mt6375_chg_data *ddata = rdev->reg_data;

	ret = mt6375_set_boost_param(ddata, false);
	if (ret < 0)
		return ret;
	ret = regulator_disable_regmap(rdev);
	if (ret < 0) {
		mt6375_set_boost_param(ddata, true);
		return ret;
	}
	mt6375_chg_set_vctrl_otg_gpio(ddata, 0);
	return 0;
}

static const struct regulator_ops mt6375_chg_otg_rops = {
	.enable = mt6375_chg_regulator_enable,
	.disable = mt6375_chg_regulator_disable,
	.is_enabled = regulator_is_enabled_regmap,
	.list_voltage = regulator_list_voltage_linear,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_current_limit = regulator_set_current_limit_regmap,
	.get_current_limit = regulator_get_current_limit_regmap,
};

static const struct regulator_desc mt6375_chg_otg_rdesc = {
	.of_match = "mt6375,otg-vbus",
	.ops = &mt6375_chg_otg_rops,
	.owner = THIS_MODULE,
	.type = REGULATOR_VOLTAGE,
	.min_uV = 4850000,
	.uV_step = 25000,
	.n_voltages = 47,
	.linear_min_sel = 20,
	.curr_table = mt6375_chg_otg_cc_micro,
	.n_current_limits = ARRAY_SIZE(mt6375_chg_otg_cc_micro),
	.csel_reg = MT6375_REG_OTG_C,
	.csel_mask = MT6375_MSK_OTG_CC,
	.vsel_reg = MT6375_REG_OTG_V,
	.vsel_mask = MT6375_MSK_OTG_CV,
	.enable_reg = MT6375_REG_CHG_TOP1,
	.enable_mask = MT6375_MSK_OTG_EN,
};

static const struct mt6375_chg_platform_data mt6375_chg_pdata_def = {
#if !IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	.aicr = 3225,
	.mivr = 4400,
	.ichg = 2000,
	.ieoc = 150,
	.cv = 4200,
	.te_en = true,
#endif
	.wdt = 40000,
	.vbus_ov = 14500,
	.vrec = 100,
	.ircmp_v = 0,
	.ircmp_r = 0,	/* uOhm */
	.chg_tmr = 10,	/* hr */
	.dcdt_sel = 600,
	.wdt_en = false,
	.chg_tmr_en = true,
	.chg_name = "primary_chg",
	.usb_killer_detect = false,
	.spec_ta_en = true,
};

static inline u8 mt6375_chg_val_toreg(const struct mt6375_chg_range *range,
				      u32 val)
{
	int i;
	u8 reg;

	if (!range)
		return val;

	if (range->table) {
		if (val <= range->table[0])
			return 0;
		for (i = 1; i < range->num_table - 1; i++) {
			if (val == range->table[i])
				return i;
			if (val > range->table[i] &&
			     val < range->table[i + 1])
				return range->round_up ? i + 1 : i;
		}
		return range->num_table - 1;
	}
	if (val <= range->min)
		reg = 0;
	else if (val >= range->max)
		reg = (range->max - range->min) / range->step;
	else if (range->round_up)
		reg = DIV_ROUND_UP(val - range->min, range->step);
	else
		reg = (val - range->min) / range->step;
	return reg + range->offset;
}

static inline u32 mt6375_chg_reg_toval(const struct mt6375_chg_range *range,
				       u8 reg)
{
	if (!range)
		return reg;
	return range->table ? range->table[reg] :
			      range->min + range->step * (reg - range->offset);
}

static inline int mt6375_chg_field_get(struct mt6375_chg_data *ddata,
				       enum mt6375_chg_reg_field fd, u32 *val)
{
	int ret;
	u32 regval;

	ret = regmap_field_read(ddata->rmap_fields[fd], &regval);
	if (ret < 0)
		return ret;
	*val = mt6375_chg_reg_toval(mt6375_chg_fields[fd].range, regval);
	mt_dbg(ddata->dev, "%s, reg=0x%02X, val=%d\n",
	       mt6375_chg_fields[fd].name, regval, *val);
	return 0;
}

static inline int mt6375_chg_field_set(struct mt6375_chg_data *ddata,
				       enum mt6375_chg_reg_field fd, u32 val)
{
	mt_dbg(ddata->dev, "%s, val=%d\n", mt6375_chg_fields[fd].name,
	       val);
	val = mt6375_chg_val_toreg(mt6375_chg_fields[fd].range, val);
	return regmap_field_write(ddata->rmap_fields[fd], val);
}

static int mt6375_chg_enable_charging(struct mt6375_chg_data *ddata, bool en)
{
	int ret;

	mutex_lock(&ddata->cv_lock);
	ret = mt6375_chg_field_set(ddata, F_CHG_EN, en);
	mutex_unlock(&ddata->cv_lock);
	return ret;
}

static int mt6375_chg_is_enabled(struct mt6375_chg_data *ddata, bool *en)
{
	int ret = 0;
	u32 val = 0;

	ret = mt6375_chg_field_get(ddata, F_CHG_EN, &val);
	if (ret < 0)
		return ret;
	*en = val;
	return 0;
}

static int mt6375_chg_is_charge_done(struct mt6375_chg_data *ddata, bool *done)
{
	int ret;
	union power_supply_propval val;

	ret = power_supply_get_property(ddata->psy, POWER_SUPPLY_PROP_STATUS,
					&val);
	if (ret < 0)
		return ret;
	*done = (val.intval == POWER_SUPPLY_STATUS_FULL);
	return 0;
}

static int mt6375_chg_set_cv(struct mt6375_chg_data *ddata, u32 mV)
{
	int ret = 0;
	bool done = false, enabled = false;

	mutex_lock(&ddata->cv_lock);
	if (ddata->batprotect_en) {
		dev_notice(ddata->dev,
			   "batprotect enabled, should not set cv\n");
		goto out;
	}
	if (mV <= ddata->cv || mV >= ddata->cv + RECHG_THRESHOLD)
		goto out_cv;
	ret = mt6375_chg_is_charge_done(ddata, &done);
	if (ret < 0 || !done)
		goto out_cv;
	ret = mt6375_chg_is_enabled(ddata, &enabled);
	if (ret < 0 || !enabled)
		goto out_cv;
	if (mt6375_chg_field_set(ddata, F_CHG_EN, false) < 0)
		dev_notice(ddata->dev, "failed to disable charging\n");
out_cv:
	ret = mt6375_chg_field_set(ddata, F_CV, mV);
	if (!ret)
		ddata->cv = mV;
	if (done && enabled)
		mt6375_chg_field_set(ddata, F_CHG_EN, true);
out:
	mutex_unlock(&ddata->cv_lock);
	return ret;
}

static int mt6375_get_chg_status(struct mt6375_chg_data *ddata)
{
	int ret = 0, attach;
	u32 stat;
	bool chg_en = false;

	attach = atomic_read(&ddata->attach);
	if (!attach)
		return POWER_SUPPLY_STATUS_NOT_CHARGING;

	ret = mt6375_chg_is_enabled(ddata, &chg_en);
	if (ret < 0)
		return ret;
	ret = mt6375_chg_field_get(ddata, F_IC_STAT, &stat);
	if (ret < 0)
		return ret;
	switch (stat) {
	case CHG_STAT_OTG:
		return POWER_SUPPLY_STATUS_DISCHARGING;
	case CHG_STAT_SLEEP:
	case CHG_STAT_VBUS_RDY:
	case CHG_STAT_TRICKLE:
	case CHG_STAT_PRE:
	case CHG_STAT_FAST:
	case CHG_STAT_EOC:
	case CHG_STAT_BKGND:
		if (chg_en)
			return POWER_SUPPLY_STATUS_CHARGING;
		else
			return POWER_SUPPLY_STATUS_NOT_CHARGING;
	case CHG_STAT_DONE:
		return POWER_SUPPLY_STATUS_FULL;
	case CHG_STAT_FAULT:
		return POWER_SUPPLY_STATUS_NOT_CHARGING;
	default:
		return POWER_SUPPLY_STATUS_UNKNOWN;
	}
}

static void mt6375_chg_attach_pre_process(struct mt6375_chg_data *ddata,
					  enum mt6375_attach_trigger trig,
					  int attach)
{
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);
	bool bc12_dn;

	mt_dbg(ddata->dev, "trig=%s,attach=%d\n",
	       mt6375_attach_trig_names[trig], attach);
	/* if attach trigger is not match, ignore it */
	if (pdata->attach_trig != trig) {
		mt_dbg(ddata->dev, "trig=%s ignored\n",
		       mt6375_attach_trig_names[trig]);
		return;
	}

	mutex_lock(&ddata->attach_lock);
	if (attach == ATTACH_TYPE_NONE)
		ddata->bc12_dn = false;

	bc12_dn = ddata->bc12_dn;
	if (!bc12_dn)
		atomic_set(&ddata->attach, attach);
	mutex_unlock(&ddata->attach_lock);

	if (attach > ATTACH_TYPE_PD && bc12_dn)
		return;

	if (!queue_work(ddata->wq, &ddata->bc12_work))
		dev_notice(ddata->dev, "%s bc12 work already queued\n", __func__);
}

static void mt6375_chg_pwr_rdy_process(struct mt6375_chg_data *ddata)
{
	int ret;
	u32 val;

	ret = mt6375_chg_field_get(ddata, F_ST_PWR_RDY, &val);
	if (ret < 0 || ddata->pwr_rdy == val)
		return;
	ddata->pwr_rdy = val;
	mt_dbg(ddata->dev, "pwr_rdy=%d\n", val);
	ret = mt6375_chg_field_set(ddata, F_BLEED_DIS_EN, !ddata->pwr_rdy);
	if (ret < 0)
		dev_err(ddata->dev, "failed to set bleed discharge = %d\n",
			!ddata->pwr_rdy);
	mt6375_chg_attach_pre_process(ddata, ATTACH_TRIG_PWR_RDY,
				val ? ATTACH_TYPE_PWR_RDY : ATTACH_TYPE_NONE);
}

static int mt6375_chg_set_usbsw(struct mt6375_chg_data *ddata,
				enum mt6375_usbsw usbsw)
{
	struct phy *phy;
	int ret, mode = (usbsw == USBSW_CHG) ? PHY_MODE_BC11_SET :
					       PHY_MODE_BC11_CLR;

	mt_dbg(ddata->dev, "usbsw=%d\n", usbsw);
	phy = phy_get(ddata->dev, "usb2-phy");
	if (IS_ERR_OR_NULL(phy)) {
		dev_err(ddata->dev, "failed to get usb2-phy\n");
		return -ENODEV;
	}
	ret = phy_set_mode_ext(phy, PHY_MODE_USB_DEVICE, mode);
	if (ret)
		dev_err(ddata->dev, "failed to set phy ext mode\n");
	phy_put(ddata->dev, phy);
	return ret;
}

static bool is_usb_rdy(struct device *dev)
{
	bool ready = true;
	struct device_node *node;

	node = of_parse_phandle(dev->of_node, "usb", 0);
	if (node) {
		ready = !of_property_read_bool(node, "cdp-block");
		mt_dbg(dev, "usb ready = %d\n", ready);
	} else
		dev_warn(dev, "usb node missing or invalid\n");
	return ready;
}

static int mt6375_chg_enable_bc12(struct mt6375_chg_data *ddata, bool en)
{
	int i, ret, attach;
	static const int max_wait_cnt = 250;

	mt_dbg(ddata->dev, "en=%d\n", en);
	if (en) {
		/* CDP port specific process */
		dev_info(ddata->dev, "check CDP block\n");
		for (i = 0; i < max_wait_cnt; i++) {
			if (is_usb_rdy(ddata->dev))
				break;
			attach = atomic_read(&ddata->attach);
			if (attach == ATTACH_TYPE_TYPEC)
				msleep(100);
			else {
				dev_notice(ddata->dev, "%s: change attach:%d, disable bc12\n",
					   __func__, attach);
				en = false;
				break;
			}
		}
		if (i == max_wait_cnt)
			dev_notice(ddata->dev, "CDP timeout\n", __func__);
		else
			dev_info(ddata->dev, "CDP free\n", __func__);
	}
	ret = mt6375_chg_set_usbsw(ddata, en ? USBSW_CHG : USBSW_USB);
	if (ret)
		return ret;
	return mt6375_chg_field_set(ddata, F_BC12_EN, en);
}

static void mt6375_wake_up_charger(struct mt6375_chg_data *ddata)
{
	struct power_supply *chg_psy = NULL;
	struct mtk_charger *info = NULL;

	chg_psy = power_supply_get_by_name("mtk-master-charger");
	if (chg_psy == NULL || IS_ERR(chg_psy)) {
		dev_err(ddata->dev, "%s Couldn't get chg_psy\n", __func__);
	} else {
		info = (struct mtk_charger *)power_supply_get_drvdata(chg_psy);
		if (info == NULL)
			return;
		if (info->wake_up_charger)
			info->wake_up_charger(info);
	}
}

static void mt6375_chg_bc12_work_func(struct work_struct *work)
{
	struct mt6375_chg_data *ddata = container_of(work,
						     struct mt6375_chg_data,
						     bc12_work);
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);
	bool bc12_ctrl = true, bc12_en = false, rpt_psy = true;
	int ret, attach;
	u32 val = 0;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	int cable_work_delay = 0;
#endif

	mutex_lock(&ddata->attach_lock);
	attach = atomic_read(&ddata->attach);
	dev_info(ddata->dev, "vt_muic : attach=%d\n", attach);

	if (attach > ATTACH_TYPE_NONE && pdata->boot_mode == 5) {
		/* skip bc12 to speed up ADVMETA_BOOT */
		dev_notice(ddata->dev, "force SDP in meta mode\n");
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
		goto out;
	}

	switch (attach) {
	case ATTACH_TYPE_NONE:
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_UNKNOWN;
		ddata->bc12_nsdp_retried = false;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_NONE;
#endif
		goto out;
	case ATTACH_TYPE_TYPEC:
		if (!ddata->bc12_dn) {
			bc12_en = true;
			rpt_psy = false;
			goto out;
		}
		ret = mt6375_chg_field_get(ddata, F_PORT_STAT, &val);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to get port stat\n");
			rpt_psy = false;
			goto out;
		}
		break;
	case ATTACH_TYPE_PD_SDP:
		val = PORT_STAT_SDP;
		break;
	case ATTACH_TYPE_PD_DCP:
		/* not to enable bc12 */
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_TA;
#endif
		goto out;
	case ATTACH_TYPE_PD_NONSTD:
		val = PORT_STAT_UNKNOWN_TA;
		break;
	default:
		dev_info(ddata->dev,
			 "%s: using tradtional bc12 flow!\n", __func__);
		break;
	}

	/* BC12 workaround (NONSTD) */
	if (val == PORT_STAT_UNKNOWN_TA && !ddata->bc12_nsdp_retried) {
		ddata->bc12_nsdp_retried = true;
		dev_info(ddata->dev, "%s: nsdp workaround\n", __func__);
		ret = mt6375_chg_enable_bc12(ddata, false);
		if (ret < 0)
			dev_err(ddata->dev, "failed to disable bc12\n");
		bc12_en = true;
		rpt_psy = false;
		goto out;
	}

	switch (val) {
	case PORT_STAT_NOINFO:
		bc12_ctrl = false;
		rpt_psy = false;
		dev_info(ddata->dev, "%s no info\n", __func__);
		goto out;
	case PORT_STAT_APPLE_5W:
	case PORT_STAT_APPLE_10W:
	case PORT_STAT_APPLE_12W:
	case PORT_STAT_SAMSUNG:
	case PORT_STAT_DCP:
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB_DCP;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_TA;
#endif
		bc12_en = true;
		break;
	case PORT_STAT_SDP:
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_SDP;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_USB;
#endif
		break;
	case PORT_STAT_CDP:
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB_CDP;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_CDP;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_USB_CDP;
#endif
		break;
	case PORT_STAT_UNKNOWN_TA:
		ddata->psy_desc.type = POWER_SUPPLY_TYPE_USB;
		ddata->psy_usb_type = POWER_SUPPLY_USB_TYPE_DCP;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		ddata->sec_cable_type = SEC_BATTERY_CABLE_TIMEOUT;
#endif
		break;
	default:
		bc12_ctrl = false;
		rpt_psy = false;
		dev_info(ddata->dev, "Unknown port stat %d\n", val);
		goto out;
	}
	mt_dbg(ddata->dev, "port stat = %s\n", mt6375_port_stat_names[val]);
out:
	mutex_unlock(&ddata->attach_lock);

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	if (rpt_psy) {
#if !IS_ENABLED(CONFIG_SEC_FACTORY)		
		if (ddata->sec_cable_type != SEC_BATTERY_CABLE_NONE)
			cable_work_delay = 250;
#endif
		cancel_delayed_work(&ddata->sec_cable_work);
		queue_delayed_work(ddata->wq, &ddata->sec_cable_work, msecs_to_jiffies(cable_work_delay));
	}
#endif

#if IS_ENABLED(CONFIG_SEC_FACTORY)
	if (attach > ATTACH_TYPE_NONE)
		mt6375_usbid_init(ddata, true);
	else
		mt6375_usbid_init(ddata, false);
#endif

	if (bc12_ctrl && (mt6375_chg_enable_bc12(ddata, bc12_en) < 0))
		dev_err(ddata->dev, "failed to set bc12 = %d\n", bc12_en);

	if (rpt_psy)
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		mt6375_wake_up_charger(ddata);
#else
		power_supply_changed(ddata->psy);
#endif
}

#ifdef MT6375_AFC_SUPPORT
#define AFC_VBUS_READ_COUNT	3

static void mt6375_clear_afc_evt(struct mt6375_chg_data *ddata)
{
	/* EVT clear */
	regmap_field_write(ddata->rmap_fields[F_CMDCPL_EVT], 1);
}

static int mt6375_send_afc_data(struct charger_device *chgdev, u32 val)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret;

	/* Unmask interrupts of AFC_EVENT */
	ret = regmap_update_bits(ddata->rmap, MT6375_REG_AFC_MASK,
			MT6375_MSK_CMDCPL, 0x0);

	ret = mt6375_chg_field_set(ddata, F_AFC_CMD, val);
	ret = mt6375_chg_field_set(ddata, F_AFC_CONTROL, 0x80);

	reinit_completion(&ddata->afc_done);
	ret = wait_for_completion_interruptible_timeout(&ddata->afc_done,
		msecs_to_jiffies(100));

	if (ret == 0)
		ret = -ETIMEDOUT;
	else if (ret < 0)
		ret = -EINTR;

	mt6375_clear_afc_evt(ddata);
	/* Mask interrupts of AFC_EVENT */
	ret = regmap_update_bits(ddata->rmap, MT6375_REG_AFC_MASK,
			MT6375_MSK_CMDCPL, 0xFF);

	if (ret < 0) {
		dev_info(ddata->dev, "cmdcpl event not occurred(%d)\n", ret);
		return ret;
	}

	return 0;
}

static void mt6375_chg_hvdcp_work_func(struct work_struct *work)
{
	struct mt6375_chg_data *ddata = container_of(work,
						     struct mt6375_chg_data,
						     hvdcp_work);
	int ret, attach, i, vbus;
	u32 val;

	mutex_lock(&ddata->attach_lock);
	attach = atomic_read(&ddata->attach);
	dev_info(ddata->dev, "%s: attach=%d\n", __func__, attach);

	if (!attach || ddata->wd_state) {
		mutex_unlock(&ddata->attach_lock);
		return;
	}

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	if (ddata->typec_rp_level != TYPEC_CC_VOLT_SNK_DFT) {
		dev_info(ddata->dev, "%s: typec_rp_level=%d\n", __func__,
			ddata->typec_rp_level);
		mutex_unlock(&ddata->attach_lock);
		return;
	}
#endif

	ddata->hvdcp_irq_status = HV_IRQ_DET;

	if (ddata->hiccup_mode) {
		dev_info(ddata->dev, "%s: hiccup_mode\n", __func__);
		mutex_unlock(&ddata->attach_lock);
		return;
	}

	/* Try AFC 5V */
	ret = mt6375_chg_field_set(ddata, F_AFC_EN, 0x3);
	ret = charger_dev_send_afc_data(ddata->chgdev, 0x8);

	/* Check */
	ret = mt6375_chg_field_get(ddata, F_AFC_CONTROL, &val);
	if ((val&0xE)>>1 != 0x7) {
		dev_info(ddata->dev, "%s: AFC not detected 0x%x\n", __func__, val);
		ddata->hvdcp_irq_status = HV_IRQ_NONE;
		goto out;
	}
	ddata->hvdcp_irq_status = HV_AFC_DET;

	if (ddata->afc_disable) {
		dev_info(ddata->dev, "%s: afc_disable\n", __func__);
#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
		vt_muic_set_attached_afc_dev(ATTACHED_DEV_AFC_CHARGER_DISABLED_MUIC);
#endif
		goto out;
	}

	mt_dbg(ddata->dev, "AFC detected. try 9v.\n");
	for (i=0; i<3; i++) {
		ret = charger_dev_send_afc_data(ddata->chgdev, 0x46);
		msleep(50);
	}

	/* Check VBUS 9V */
	for (i=0; i<AFC_VBUS_READ_COUNT; i++) {
		ret = charger_dev_get_vbus_level(ddata->chgdev, &vbus);
		if (ret < 0)
			goto out;

		if (vbus == 9) {
#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
			vt_muic_set_attached_afc_dev(ATTACHED_DEV_AFC_CHARGER_9V_MUIC);
#endif
			break;
		} else if(i == AFC_VBUS_READ_COUNT-1)
			dev_info(ddata->dev, "%s: VBUS 9v not confirmed\n", __func__);

		usleep_range(9000, 10000);
	}

out:
	mt6375_chg_field_set(ddata, F_AFC_EN, 0x0);
	mutex_unlock(&ddata->attach_lock);
	mt_dbg(ddata->dev, "done %d\n", ret);
}

static int mt6375_afc_set_voltage(struct charger_device *chgdev, int val)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int vbus = 0;

	mutex_lock(&ddata->attach_lock);
	charger_dev_get_vbus_level(chgdev, &vbus);
	dev_info(ddata->dev, "%s: set %d to %d\n", __func__, vbus, val);

	if (ddata->afc_disable && val == 9) {
		mutex_unlock(&ddata->attach_lock);
		return 0;
	}

	if (vbus && vbus != val) {
		ddata->afc_set_voltage = val;
		queue_work(ddata->wq, &ddata->afcv_work);
	}
	mutex_unlock(&ddata->attach_lock);

	return 0;
}

static int mt6375_afc_disable_set(struct charger_device *chgdev, int val)
 {
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	dev_info(ddata->dev, "%s: set to %d\n", __func__, val);
	ddata->afc_disable = val;

	return 0;
}

static int mt6375_get_vbus_level(struct charger_device *chgdev, int *vbus)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret;
	u32 vbus_adc;

	ret = charger_dev_get_vbus(chgdev, &vbus_adc);
	if (ret < 0) {
		dev_err(ddata->dev, "get VBUS fail %d\n", ret);
		return ret;
	} else
		vbus_adc /= 1000;

	if (vbus_adc > 3800 && vbus_adc < 6500)
		*vbus = 0x5;
	else if (vbus_adc > 7500 && vbus_adc < 10500)
		*vbus = 0x9;
	else
		*vbus = 0x0;

	pr_info("%s %dmV >> %d\n", __func__, vbus_adc, *vbus);
	return ret;
}

static void mt6375_chg_afcv_work_func(struct work_struct *work)
{
	struct mt6375_chg_data *ddata = container_of(work,
						     struct mt6375_chg_data,
						     afcv_work);
	int ret, attach, i, vbus;
	u32 val;

	mutex_lock(&ddata->attach_lock);
	attach = atomic_read(&ddata->attach);
	dev_info(ddata->dev, "%s: attach=%d/irq_status=%d/hiccup_mode=%d\n", __func__,
		attach, ddata->hvdcp_irq_status, ddata->hiccup_mode);

	if(!attach || ddata->hiccup_mode) {
		mutex_unlock(&ddata->attach_lock);
		return;
	}

	/* hvdcp irq processing status check  */
	if(ddata->hvdcp_irq_status == HV_IRQ_DET) {
		queue_work(ddata->wq, &ddata->hvdcp_work);
		mutex_unlock(&ddata->attach_lock);
		return;
	}

	mt6375_chg_field_set(ddata, F_AFC_EN, 0x3);

	/* Request AFC 9V or 5V. */
	val = (ddata->afc_set_voltage == 9) ? 0x46 : 0x8;
	mt_dbg(ddata->dev, "AFC try %dv.\n", ddata->afc_set_voltage);
	for (i=0; i<3; i++) {
		ret = charger_dev_send_afc_data(ddata->chgdev, val);
		msleep(50);
	}

	/* Check VBUS Level */
	for (i=0; i<AFC_VBUS_READ_COUNT; i++) {
		ret = charger_dev_get_vbus_level(ddata->chgdev, &vbus);
		if (ret < 0)
			goto out;

		if (vbus == ddata->afc_set_voltage) {
#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
			if (vbus == 9)
				vt_muic_set_attached_afc_dev(ATTACHED_DEV_AFC_CHARGER_9V_MUIC);
			else if (vbus == 5) {
				if (ddata->afc_disable)
					vt_muic_set_attached_afc_dev(ATTACHED_DEV_AFC_CHARGER_DISABLED_MUIC);
				else
					vt_muic_set_attached_afc_dev(ATTACHED_DEV_AFC_CHARGER_5V_MUIC);
			}
#endif
			break;
		} else if(i == AFC_VBUS_READ_COUNT-1)
			dev_info(ddata->dev, "%s: VBUS %dv not confirmed\n", __func__,
					ddata->afc_set_voltage);

		usleep_range(9000, 10000);
	}
out:
	mt6375_chg_field_set(ddata, F_AFC_EN, 0x0);
	mutex_unlock(&ddata->attach_lock);
	mt_dbg(ddata->dev, "done %d\n", ret);
}
#endif

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
static void mt6375_sec_cable_work_func(struct work_struct *work)
{
	struct mt6375_chg_data *ddata = container_of(work,
						     struct mt6375_chg_data,
						     sec_cable_work.work);
	struct tcpc_device *tcpc_dev;
	union power_supply_propval value = {0, };

	tcpc_dev = tcpc_dev_get_by_name("type_c_port0");
	if (!tcpc_dev)
		pr_notice("%s get tcpc device type_c_port0 fail\n", __func__);
	else {
		if ((tcpm_inquire_pd_connected(tcpc_dev)) &&
			(ddata->sec_cable_type != SEC_BATTERY_CABLE_NONE)) {
			value.intval = 1;
			psy_do_property("battery", set,
					POWER_SUPPLY_EXT_PROP_SRCCAP, value);
		}
	}
	pr_info("%s: vt_muic with: sec_cable_type(%d), psy_desc.type(%d), psy_usb_type(%d)\n",
		__func__, ddata->sec_cable_type, ddata->psy_desc.type, ddata->psy_usb_type);
	value.intval = ddata->sec_cable_type;

#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
	psy_do_property("bc12", set,
		POWER_SUPPLY_PROP_ONLINE, value);
#endif
}
#endif

static int mt6375_chg_enable_hiccup(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret;

	mutex_lock(&ddata->attach_lock);
	dev_info(ddata->dev, "%s[%d]\n", __func__, en);

	if (ddata->lpm_bootmode) {
		pr_err("%s: lpm boot mode\n", __func__);
		mutex_unlock(&ddata->attach_lock);
		return 0;
	}

#if IS_ENABLED(CONFIG_VIRTUAL_MUIC)
	if (!ddata->wd_state)
		vt_muic_set_attached_afc_dev(ATTACHED_DEV_HICCUP_MUIC);
#endif

	cancel_delayed_work(&ddata->sec_cable_work);
	if (ddata->wd_state && !en && !ddata->hiccup_mode)
		queue_delayed_work(ddata->wq, &ddata->cc_hi_check_work, msecs_to_jiffies(1000));

	ddata->hiccup_mode = en;

	ret = mt6375_chg_field_set(ddata,  F_MANUAL_MODE, en);
	ret |= mt6375_chg_field_set(ddata, F_DPDM_SW_VCP_EN, en);
	ret |= mt6375_chg_field_set(ddata, F_DM_DIS_RSEL, en);
	ret |= mt6375_chg_field_set(ddata, F_DM_DIS_REN, en);
	ret |= mt6375_chg_field_set(ddata, F_DP_DIS_RSEL, en);
	ret |= mt6375_chg_field_set(ddata, F_DP_DIS_REN, en);

	mutex_unlock(&ddata->attach_lock);
	return ret;
}

static int mt6375_chg_set_cc_hi_state(struct charger_device *chgdev, int val)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mutex_lock(&ddata->attach_lock);

	if(ddata->cc_hi_state == val) {
		mutex_unlock(&ddata->attach_lock);
		return 0;
	}

	ddata->cc_hi_state = val;
	dev_info(ddata->dev, "%s[cc_hi:%d/hiccup:%d]\n", __func__,
		ddata->cc_hi_state, ddata->hiccup_mode);
	cancel_delayed_work(&ddata->sec_cable_work);
	if (ddata->wd_state && ddata->cc_hi_state && !ddata->hiccup_mode)
		queue_delayed_work(ddata->wq, &ddata->cc_hi_check_work, msecs_to_jiffies(100));

	mutex_unlock(&ddata->attach_lock);
	return 0;
}

static void mt6375_chg_cc_hi_check_work_func(struct work_struct *work)
{
	struct mt6375_chg_data *ddata = container_of(work,
						     struct mt6375_chg_data,
						     cc_hi_check_work.work);

	dev_info(ddata->dev, "%s[cc_hi:%d/hiccup:%d]\n", __func__,
		ddata->cc_hi_state, ddata->hiccup_mode);

	if (!ddata->wd_state)
		return;

	if (ddata->cc_hi_state && !ddata->hiccup_mode) {
#if IS_ENABLED(CONFIG_PDIC_NOTIFIER)
		PD_NOTI_TYPEDEF pdic_noti;

		pdic_noti.src = PDIC_NOTIFY_DEV_PDIC;
		pdic_noti.dest = PDIC_NOTIFY_DEV_MUIC;
		pdic_noti.id = PDIC_NOTIFY_ID_WATER_CABLE;
		pdic_noti.sub1 = 1;
		pdic_notifier_notify((PD_NOTI_TYPEDEF *)&pdic_noti, 0, 0);
#endif
	}
}

static int mt6375_chg_set_water_state(struct charger_device *chgdev, bool val)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "val=%d\n", val);
	ddata->wd_state = val;

	return 0;
}

static enum power_supply_usb_type mt6375_chg_psy_usb_types[] = {
	POWER_SUPPLY_USB_TYPE_UNKNOWN,
	POWER_SUPPLY_USB_TYPE_SDP,
	POWER_SUPPLY_USB_TYPE_CDP,
	POWER_SUPPLY_USB_TYPE_DCP,
};

static enum power_supply_property mt6375_chg_psy_properties[] = {
	POWER_SUPPLY_PROP_MANUFACTURER,
	POWER_SUPPLY_PROP_ONLINE,
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE,
	POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT,
	POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_USB_TYPE,
	POWER_SUPPLY_PROP_CURRENT_MAX,
	POWER_SUPPLY_PROP_VOLTAGE_MAX,
#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
	POWER_SUPPLY_PROP_VOLTAGE_MIN,
#endif
	POWER_SUPPLY_PROP_CALIBRATE,
	POWER_SUPPLY_PROP_ENERGY_EMPTY,
};

static int mt6375_chg_property_is_writeable(struct power_supply *psy,
					    enum power_supply_property psp)
{
	switch (psp) {
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
	case POWER_SUPPLY_PROP_STATUS:
	case POWER_SUPPLY_PROP_ONLINE:
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
		return 1;
	default:
		return 0;
	}
	return 0;
}

static int mt6375_chg_get_property(struct power_supply *psy,
				   enum power_supply_property psp,
				   union power_supply_propval *val)
{
	struct mt6375_chg_data *ddata = power_supply_get_drvdata(psy);
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	enum power_supply_ext_property ext_psp =
					(enum power_supply_ext_property) psp;
	u8 rp_currentlvl = 0;
#endif
	int ret = 0;
	u16 data;
	u32 _val = 0;

	mt_dbg(ddata->dev, "psp=%d\n", psp);
	switch (psp) {
	case POWER_SUPPLY_PROP_MANUFACTURER:
		val->strval = MT6375_MANUFACTURER;
		break;
	case POWER_SUPPLY_PROP_ONLINE:
		val->intval = atomic_read(&ddata->attach);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		ret = mt6375_get_chg_status(ddata);
		if (ret < 0)
			return ret;
		val->intval = ret;
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_get(ddata, F_CC, &val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		mutex_lock(&ddata->cv_lock);
		ret = mt6375_chg_field_get(ddata, F_CV, &val->intval);
		mutex_unlock(&ddata->cv_lock);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_get(ddata, F_IAICR, &val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_get(ddata, F_VMIVR, &val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		ret = mt6375_chg_field_get(ddata, F_IEOC, &val->intval);
		break;
	case POWER_SUPPLY_PROP_USB_TYPE:
		mutex_lock(&ddata->attach_lock);
		val->intval = ddata->psy_usb_type;
		mutex_unlock(&ddata->attach_lock);
		break;
	case POWER_SUPPLY_PROP_CURRENT_MAX:
		if (ddata->psy_desc.type == POWER_SUPPLY_TYPE_USB)
			val->intval = NORMAL_CHARGING_CURR_UA;
		else if (ddata->psy_desc.type == POWER_SUPPLY_TYPE_USB_DCP)
			val->intval = FAST_CHARGING_CURR_UA;
		break;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX:
		if (ddata->psy_desc.type == POWER_SUPPLY_TYPE_USB)
			val->intval = 5000000;
		break;
#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
	case POWER_SUPPLY_PROP_VOLTAGE_MIN:
		ret = mt6375_get_vsys(ddata->chgdev, &val->intval);
		break;
#endif
	case POWER_SUPPLY_PROP_TYPE:
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		if (val->intval == SEC_BATTERY_CABLE_TYPE_FROM_MTK)
			val->intval = ddata->sec_cable_type;
		else
#endif
			val->intval = ddata->psy_desc.type;
		break;
	case POWER_SUPPLY_PROP_CALIBRATE:
		mutex_lock(&ddata->cv_lock);
		ret = mt6375_chg_field_get(ddata, F_VBAT_MON_EN, &_val);
		if (_val) {
			ret = -EBUSY;
			dev_notice(ddata->dev, "vbat_mon is enabled\n");
			mutex_unlock(&ddata->cv_lock);
			break;
		}
		ret = mt6375_chg_field_set(ddata, F_VBAT_MON_EN, 1);
		if (ret < 0) {
			dev_notice(ddata->dev, "failed to enable vbat monitor\n");
			mutex_unlock(&ddata->cv_lock);
			break;
		}
		usleep_range(ADC_CONV_TIME_US * 2, ADC_CONV_TIME_US * 3);
		ret = regmap_bulk_read(ddata->rmap, MT6375_REG_VBAT_MON_RPT, &data, 2);
		if (ret < 0)
			dev_notice(ddata->dev, "failed to get vbat monitor report\n");
		else
			val->intval = ADC_FROM_VBAT_RAW(be16_to_cpu(data));
		if (mt6375_chg_field_set(ddata, F_VBAT_MON_EN, 0) < 0)
			dev_notice(ddata->dev, "failed to disable vbat monitor\n");
		mutex_unlock(&ddata->cv_lock);
		break;
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
		val->intval = ddata->vbat0_flag;
		break;
	default:
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		switch (ext_psp) {
		case POWER_SUPPLY_EXT_PROP_RP_LEVEL:
			switch (ddata->typec_rp_level) {
			case TYPEC_CC_VOLT_SNK_1_5:
				rp_currentlvl = RP_CURRENT_LEVEL2;
				break;
			case TYPEC_CC_VOLT_SNK_3_0:
				rp_currentlvl = RP_CURRENT_LEVEL3;
				break;
			case TYPEC_CC_VOLT_SNK_DFT:
				rp_currentlvl = RP_CURRENT_LEVEL_DEFAULT;
				break;
			}
			 val->intval = rp_currentlvl;
			break;
		default:
			ret = -EINVAL;
			break;
		}
#else
		ret = -EINVAL;
#endif
		break;
	}
	return ret;
}

static int mt6375_chg_set_property(struct power_supply *psy,
				   enum power_supply_property psp,
				   const union power_supply_propval *val)
{
	int ret = 0;
	struct mt6375_chg_data *ddata = power_supply_get_drvdata(psy);
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	enum power_supply_ext_property ext_psp =
					(enum power_supply_ext_property) psp;
#endif

	mt_dbg(ddata->dev, "psp=%d\n", psp);
	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		mt6375_chg_attach_pre_process(ddata, ATTACH_TRIG_TYPEC,
					      val->intval);
		break;
	case POWER_SUPPLY_PROP_STATUS:
		ret = mt6375_chg_enable_charging(ddata, val->intval);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_set(ddata, F_CC, val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE:
		ret = mt6375_chg_set_cv(ddata, val->intval);
		break;
	case POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_set(ddata, F_IAICR, val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT:
		mutex_lock(&ddata->pe_lock);
		ret = mt6375_chg_field_set(ddata, F_VMIVR, val->intval);
		mutex_unlock(&ddata->pe_lock);
		break;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		ret = mt6375_chg_field_set(ddata, F_IEOC, val->intval);
		break;
	case POWER_SUPPLY_PROP_ENERGY_EMPTY:
		ddata->vbat0_flag = val->intval;
		break;
	default:
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
		switch (ext_psp) {
		case POWER_SUPPLY_EXT_PROP_RP_LEVEL:
			ddata->typec_rp_level = val->intval;
			break;
		default:
			ret = -EINVAL;
			break;
		}
#else
		ret = -EINVAL;
#endif
		break;
	}
	return ret;
}

static char *mt6375_psy_supplied_to[] = {
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	"mtk-fg-battery",
#else
	"battery",
#endif
	"mtk-master-charger",
};

static const struct power_supply_desc mt6375_psy_desc = {
	.type = POWER_SUPPLY_TYPE_USB,
	.usb_types = mt6375_chg_psy_usb_types,
	.num_usb_types = ARRAY_SIZE(mt6375_chg_psy_usb_types),
	.properties = mt6375_chg_psy_properties,
	.num_properties = ARRAY_SIZE(mt6375_chg_psy_properties),
	.property_is_writeable = mt6375_chg_property_is_writeable,
	.get_property = mt6375_chg_get_property,
	.set_property = mt6375_chg_set_property,
};

/* The following functions are for mtk charger device */

static int mt6375_enable_charging(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_enable_charging(ddata, en);
}

static int mt6375_is_enabled(struct charger_device *chgdev, bool *en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_is_enabled(ddata, en);
}

static int mt6375_set_ichg(struct charger_device *chgdev, u32 uA)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val = { .intval = U_TO_M(uA) };

	mt_dbg(ddata->dev, "ichg=%d\n", uA);
	return power_supply_set_property(ddata->psy,
		POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT, &val);
}

static int mt6375_get_ichg(struct charger_device *chgdev, u32 *uA)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val;

	ret = power_supply_get_property(ddata->psy,
		POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT, &val);
	if (ret < 0)
		return ret;
	*uA = M_TO_U(val.intval);
	return 0;
}

static int mt6375_get_min_ichg(struct charger_device *chgdev, u32 *uA)
{
	*uA = M_TO_U(mt6375_chg_fields[F_CC].range->min);
	return 0;
}

static int mt6375_set_cv(struct charger_device *chgdev, u32 uV)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "cv=%d\n", uV);
	return mt6375_chg_set_cv(ddata, U_TO_M(uV));
}

static int mt6375_get_cv(struct charger_device *chgdev, u32 *uV)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val;

	ret = power_supply_get_property(ddata->psy,
		POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE, &val);
	if (ret < 0)
		return ret;
	*uV = M_TO_U(val.intval);
	return 0;
}

static int mt6375_set_aicr(struct charger_device *chgdev, u32 uA)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val = { .intval = U_TO_M(uA) };

#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
	if (ddata->f_mode == OB_MODE) {
		dev_err(ddata->dev, "%s: do not set aicr for no bat boot support\n", __func__);
		return 0;
	}
#endif /* CONFIG_MTK_NO_BAT_BOOT_SUPPORT */
	dev_err(ddata->dev, "aicr=%d\n", uA);
	return power_supply_set_property(ddata->psy,
		POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT, &val);
}

static int mt6375_get_aicr(struct charger_device *chgdev, u32 *uA)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val;

	ret = power_supply_get_property(ddata->psy,
		POWER_SUPPLY_PROP_INPUT_CURRENT_LIMIT, &val);
	if (ret < 0)
		return ret;
	*uA = M_TO_U(val.intval);
	return 0;
}

static int mt6375_get_min_aicr(struct charger_device *chgdev, u32 *uA)
{
	*uA = M_TO_U(mt6375_chg_fields[F_IAICR].range->min);
	return 0;
}

static int mt6375_set_mivr(struct charger_device *chgdev, u32 uV)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val = { .intval = U_TO_M(uV) };

#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
#if IS_ENABLED(CONFIG_SEC_FACTORY)
	if (ddata->f_mode == OB_MODE)
	{
		dev_err(ddata->dev, "%s: do not set mivr for no bat boot support\n", __func__);
		return 0;
	}
#endif
#endif /* CONFIG_MTK_NO_BAT_BOOT_SUPPORT */

	dev_err(ddata->dev, "mivr=%d\n", uV);
	return power_supply_set_property(ddata->psy,
		POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT, &val);
}

static int mt6375_get_mivr(struct charger_device *chgdev, u32 *uV)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val;

	ret = power_supply_get_property(ddata->psy,
		POWER_SUPPLY_PROP_INPUT_VOLTAGE_LIMIT, &val);
	if (ret < 0)
		return ret;
	*uV = M_TO_U(val.intval);
	return 0;
}

static int mt6375_get_mivr_state(struct charger_device *chgdev, bool *active)
{
	int ret;
	u32 val;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	*active = false;
	ret = mt6375_chg_field_get(ddata, F_ST_MIVR, &val);
	if (ret < 0)
		return ret;
	*active = val;
	return 0;
}

static int mt6375_get_adc(struct charger_device *chgdev, enum adc_channel chan,
			  int *min, int *max)
{
	int ret;
	enum mt6375_adc_chan adc_chan;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	switch (chan) {
	case ADC_CHANNEL_VBUS:
		adc_chan = ADC_CHAN_CHGVINDIV5;
		break;
	case ADC_CHANNEL_VSYS:
		adc_chan = ADC_CHAN_VSYS;
		break;
	case ADC_CHANNEL_VBAT:
		adc_chan = ADC_CHAN_VBAT;
		break;
	case ADC_CHANNEL_IBUS:
		adc_chan = ADC_CHAN_IBUS;
		break;
	case ADC_CHANNEL_IBAT:
		adc_chan = ADC_CHAN_IBAT;
		break;
	case ADC_CHANNEL_TEMP_JC:
		adc_chan = ADC_CHAN_TEMP_JC;
		break;
	case ADC_CHANNEL_USBID:
		adc_chan = ADC_CHAN_SBU2;
		break;
	default:
		return -EINVAL;
	}
	ret = iio_read_channel_processed(&ddata->iio_adcs[adc_chan], min);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to read adc\n");
		return ret;
	}
	*max = *min;
	return 0;
}

static int mt6375_get_vbus(struct charger_device *chgdev, u32 *vbus)
{
	return mt6375_get_adc(chgdev, ADC_CHANNEL_VBUS, vbus, vbus);
}

static int mt6375_get_ibus(struct charger_device *chgdev, u32 *ibus)
{
	return mt6375_get_adc(chgdev, ADC_CHANNEL_IBUS, ibus, ibus);
}

static int mt6375_get_ibat(struct charger_device *chgdev, u32 *ibat)
{
	return mt6375_get_adc(chgdev, ADC_CHANNEL_IBAT, ibat, ibat);
}

#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
static int mt6375_get_vsys(struct charger_device *chgdev, u32 *vsys)
{
	return mt6375_get_adc(chgdev, ADC_CHANNEL_VSYS, vsys, vsys);
}
#endif

#define ABNORMAL_TEMP_JC	120
static int mt6375_get_tchg(struct charger_device *chgdev,
			   int *tchg_min, int *tchg_max)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int temp_jc = 0, ret = 0, retry_cnt = 3;

	/* temp abnormal workaround */
	do {
		ret = mt6375_get_adc(chgdev, ADC_CHANNEL_TEMP_JC, &temp_jc,
				     &temp_jc);
	} while ((ret < 0 || temp_jc >= ABNORMAL_TEMP_JC) && (--retry_cnt) > 0);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get temp jc\n");
		return ret;
	}
	/* use last result if temp_jc is abnormal */
	if (temp_jc >= ABNORMAL_TEMP_JC)
		temp_jc = atomic_read(&ddata->tchg);
	else
		atomic_set(&ddata->tchg, temp_jc);
	*tchg_min = *tchg_max = temp_jc;
	mt_dbg(ddata->dev, "tchg=%d\n", temp_jc);
	return 0;
}

static int mt6375_get_zcv(struct charger_device *chgdev, u32 *uV)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	*uV = M_TO_U(ddata->zcv);
	return 0;
}

static int mt6375_set_ieoc(struct charger_device *chgdev, u32 uA)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	union power_supply_propval val = { .intval = U_TO_M(uA) };

	mt_dbg(ddata->dev, "ieoc=%d\n", uA);
	return power_supply_set_property(ddata->psy,
		POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT, &val);
}

static int mt6375_reset_eoc_state(struct charger_device *chgdev)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_field_set(ddata, F_EOC_RST, 1);
}

static int mt6375_sw_check_eoc(struct charger_device *chgdev, u32 uA)
{
	int ret, ibat;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	ret = mt6375_get_ibat(chgdev, &ibat);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get ibat\n");
		return ret;
	}

	if (ibat <= uA) {
		/* if it happens 3 times, trigger EOC event */
		if (atomic_read(&ddata->eoc_cnt) == 2) {
			atomic_set(&ddata->eoc_cnt, 0);
			mt_dbg(ddata->dev, "ieoc=%d, ibat=%d\n", uA, ibat);
			charger_dev_notify(ddata->chgdev,
					   CHARGER_DEV_NOTIFY_EOC);
		} else
			atomic_inc(&ddata->eoc_cnt);
	} else
		atomic_set(&ddata->eoc_cnt, 0);
	return 0;
}

static int mt6375_is_charge_done(struct charger_device *chgdev, bool *done)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_is_charge_done(ddata, done);
}

static int mt6375_enable_te(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "en=%d\n", en);
	return mt6375_chg_field_set(ddata, F_TE, en);
}

static int mt6375_enable_buck(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
	if (ddata->f_mode == OB_MODE) {
		dev_err(ddata->dev, "%s: do not set power path for no bat boot support\n", __func__);
		return 0;
	}
#endif /* CONFIG_MTK_NO_BAT_BOOT_SUPPORT */
	mt_dbg(ddata->dev, "en=%d\n", en);
	return mt6375_chg_field_set(ddata, F_BUCK_EN, en);
}

static int mt6375_is_buck_enabled(struct charger_device *chgdev, bool *en)
{
	int ret;
	u32 val;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	ret = mt6375_chg_field_get(ddata, F_BUCK_EN, &val);
	if (ret < 0)
		return ret;
	*en = val;
	return 0;
}

static int mt6375_enable_chg_timer(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "en=%d\n", en);
	return mt6375_chg_field_set(ddata, F_CHG_TMR_EN, en);
}

static int mt6375_is_chg_timer_enabled(struct charger_device *chgdev, bool *en)
{
	int ret;
	u32 val;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	ret = mt6375_chg_field_get(ddata, F_CHG_TMR_EN, &val);
	if (ret < 0)
		return ret;
	*en = val;
	return 0;
}

static int mt6375_enable_hz(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_field_set(ddata, F_HZ, en ? 1 : 0);
}

static int mt6375_kick_wdt(struct charger_device *chgdev)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_field_set(ddata, F_WDT_RST, 1);
}

static int mt6375_run_aicc(struct charger_device *chgdev, u32 *uA)
{
	int ret;
	bool active;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	long ret_comp;
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	union power_supply_propval propval = {0, };
#endif

	ret = mt6375_get_mivr_state(chgdev, &active);
	if (ret < 0)
		return ret;
	if (!active) {
		mt_dbg(ddata->dev, "mivr loop is not active\n");
		return 0;
	}

	mutex_lock(&ddata->pe_lock);
	disable_irq(ddata->detach_irq);
	if (!ddata->pwr_rdy) {
		enable_irq(ddata->detach_irq);
		dev_info(ddata->dev, "detach\n");
		ret = -EPERM;
		goto out;
	}
	reinit_completion(&ddata->aicc_done);
	enable_irq(ddata->detach_irq);
	ret = mt6375_chg_field_set(ddata, F_AICC_EN, 1);
	if (ret < 0)
		goto out;

	/* worst case = 128steps * 52msec = 6656ms */
	ret_comp = wait_for_completion_interruptible_timeout(&ddata->aicc_done,
		msecs_to_jiffies(7000));
	if (ret_comp == 0)
		ret = -ETIMEDOUT;
	else if (ret_comp < 0)
		ret = -EINTR;
	else
		ret = 0;
	if (ret < 0) {
		dev_err(ddata->dev, "failed to wait aicc (%d)\n", ret);
		goto out;
	}
	if (!ddata->pwr_rdy) {
		dev_info(ddata->dev, "detach\n");
		ret = -EPERM;
		goto out;
	}
	ret = mt6375_chg_field_get(ddata, F_AICC_RPT, uA);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get aicc report\n");
		goto out;
	}
	*uA = M_TO_U(*uA);
out:
	mt6375_chg_field_set(ddata, F_AICC_EN, 0);
	mutex_unlock(&ddata->pe_lock);

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	if (*uA > 0) {
		propval.intval = (*uA / 1000);
		psy_do_property("sec-mtk-charger", set,
				POWER_SUPPLY_EXT_PROP_AICL_CURRENT,
				propval);
		if (ret < 0)
			pr_err("%s: Fail to set property\n", __func__);
	}
#endif

	return ret;
}

static int mt6375_run_pe(struct mt6375_chg_data *ddata, bool pe20)
{
	int ret;
	unsigned long timeout = pe20 ? 1400 : 2800;
	long ret_comp;

	ret = mt6375_chg_field_set(ddata, F_IAICR, 800);
	if (ret < 0)
		return ret;
	ret = mt6375_chg_field_set(ddata, F_CC, 2000);
	if (ret < 0)
		return ret;
	ret = mt6375_chg_field_set(ddata, F_CHG_EN, 1);
	if (ret < 0)
		return ret;
	ret = mt6375_chg_field_set(ddata, F_PE_SEL, pe20);
	if (ret < 0)
		return ret;
	disable_irq(ddata->detach_irq);
	if (!ddata->pwr_rdy) {
		enable_irq(ddata->detach_irq);
		dev_info(ddata->dev, "detach\n");
		ret = -EPERM;
		goto out;
	}
	reinit_completion(&ddata->pe_done);
	enable_irq(ddata->detach_irq);
	ret = mt6375_chg_field_set(ddata, F_PE_EN, 1);
	if (ret < 0)
		goto out;

	ret_comp = wait_for_completion_interruptible_timeout(&ddata->pe_done,
		msecs_to_jiffies(timeout));
	if (ret_comp == 0)
		ret = -ETIMEDOUT;
	else if (ret_comp < 0)
		ret = -EINTR;
	else
		ret = 0;
	if (ret < 0) {
		dev_err(ddata->dev, "failed to wait pe (%d)\n", ret);
		goto out;
	}
	if (!ddata->pwr_rdy) {
		dev_info(ddata->dev, "detach\n");
		ret = -EPERM;
		goto out;
	}
out:
	mt6375_chg_field_set(ddata, F_PE_EN, 0);
	return ret;
}

static int mt6375_set_pe_current_pattern(struct charger_device *chgdev,
					 bool inc)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mutex_lock(&ddata->pe_lock);
	mt_dbg(ddata->dev, "inc=%d\n", inc);
	ret = mt6375_chg_field_set(ddata, F_PE10_INC, inc);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set pe10 up/down\n");
		goto out;
	}
	ret = mt6375_run_pe(ddata, false);
out:
	mutex_unlock(&ddata->pe_lock);
	return ret;
}

static int mt6375_set_pe20_efficiency_table(struct charger_device *chgdev)
{
	return 0;
}

static int mt6375_set_pe20_current_pattern(struct charger_device *chgdev,
					   u32 uV)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mutex_lock(&ddata->pe_lock);
	mt_dbg(ddata->dev, "pe20=%d\n", uV);
	ret = mt6375_chg_field_set(ddata, F_PE20_CODE, U_TO_M(uV));
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set pe20 code\n", __func__);
		goto out;
	}
	ret = mt6375_run_pe(ddata, true);
out:
	mutex_unlock(&ddata->pe_lock);
	return ret;
}

static int mt6375_enable_pe_cable_drop_comp(struct charger_device *chgdev,
					    bool en)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	if (en)
		return 0;
	mutex_lock(&ddata->pe_lock);
	mt_dbg(ddata->dev, "en=%d\n", en);
	ret = mt6375_chg_field_set(ddata, F_PE20_CODE, 0x1F);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set cable drop comp code\n");
		goto out;
	}
	ret = mt6375_run_pe(ddata, true);
out:
	mutex_unlock(&ddata->pe_lock);
	return ret;
}

static int mt6375_reset_pe_ta(struct charger_device *chgdev)
{
	int ret;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mutex_lock(&ddata->pe_lock);
	mt_dbg(ddata->dev, "++\n");
	ret = mt6375_chg_field_set(ddata, F_VMIVR, 4600);
	if (ret < 0)
		goto out;
	ret = mt6375_chg_field_set(ddata, F_IAICR, 100);
	if (ret < 0)
		goto out;
	msleep(250);
	ret = mt6375_chg_field_set(ddata, F_IAICR, 500);
out:
	mutex_unlock(&ddata->pe_lock);
	return ret;
}

static int mt6375_set_otg_cc(struct charger_device *chgdev, u32 uA)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "otg_cc=%d\n", uA);
	return mt6375_chg_field_set(ddata, F_OTG_CC, U_TO_M(uA));
}

static int mt6375_enable_otg(struct charger_device *chgdev, bool en)
{
	int ret;
	struct regulator *regulator;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "en=%d\n", en);
	regulator = devm_regulator_get(ddata->dev, "usb-otg-vbus");
	if (IS_ERR(regulator)) {
		dev_err(ddata->dev, "failed to get otg regulator\n");
		return PTR_ERR(regulator);
	}
	ret = en ? regulator_enable(regulator) : regulator_disable(regulator);
	devm_regulator_put(regulator);
	return ret;
}

static int mt6375_enable_discharge(struct charger_device *chgdev, bool en)
{
	int i, ret;
	u32 val;
	const int dischg_retry_cnt = 3;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	mt_dbg(ddata->dev, "en=%d\n", en);
	ret = mt6375_enable_hm(ddata, true);
	if (ret < 0)
		return ret;

	ret = mt6375_chg_field_set(ddata, F_FORCE_VBUS_SINK, en);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set dischg %d\n", __func__, en);
		goto out;
	}

	/* for disable, make sure it is disabled */
	if (!en) {
		for (i = 0; i < dischg_retry_cnt; i++) {
			ret = mt6375_chg_field_get(ddata, F_FORCE_VBUS_SINK,
						   &val);
			if (ret < 0)
				continue;
			if (!val)
				break;
			mt6375_chg_field_set(ddata, F_FORCE_VBUS_SINK, 0);
		}
		if (i == dischg_retry_cnt) {
			dev_err(ddata->dev, "failed to disable dischg\n");
			ret = -EINVAL;
		}
	}
out:
	mt6375_enable_hm(ddata, false);
	return ret;
}

static int mt6375_enable_chg_type_det(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int attach = en ? ATTACH_TYPE_TYPEC : ATTACH_TYPE_NONE;

	mt_dbg(ddata->dev, "en=%d\n", en);
	mt6375_chg_attach_pre_process(ddata, ATTACH_TRIG_TYPEC, attach);
	return 0;
}

#define DUMP_REG_BUF_SIZE	1024
static int mt6375_dump_registers(struct charger_device *chgdev)
{
	int i, ret;
	u32 val;
	u16 data;
	char buf[DUMP_REG_BUF_SIZE] = "\0";
	static struct {
		const char *name;
		const char *unit;
		enum mt6375_chg_reg_field fd;
	} settings[] = {
		{ .fd = F_CC, .name = "CC", .unit = "mA" },
		{ .fd = F_IAICR, .name = "AICR", .unit = "mA" },
		{ .fd = F_VMIVR, .name = "MIVR", .unit = "mV" },
		{ .fd = F_IEOC, .name = "IEOC", .unit = "mA" },
		{ .fd = F_CV, .name = "CV", .unit = "mV" },
	};
	static struct {
		const char *name;
		const char *unit;
		enum mt6375_adc_chan chan;
	} adcs[] = {
		{ .chan = ADC_CHAN_CHGVINDIV5, .name = "VBUS", .unit = "mV" },
		{ .chan = ADC_CHAN_IBUS, .name = "IBUS", .unit = "mA" },
		{ .chan = ADC_CHAN_VBAT, .name = "VBAT", .unit = "mV" },
		{ .chan = ADC_CHAN_IBAT, .name = "IBAT", .unit = "mA" },
		{ .chan = ADC_CHAN_VSYS, .name = "VSYS", .unit = "mV" },
	};
	static struct {
		const u16 reg;
		const char *name;
	} regs[] = {
		{ .reg = MT6375_REG_CHG_STAT, .name = "CHG_STAT" },
		{ .reg = MT6375_REG_CHG_STAT0, .name = "CHG_STAT0" },
		{ .reg = MT6375_REG_CHG_STAT1, .name = "CHG_STAT1" },
		{ .reg = MT6375_REG_CHG_TOP1, .name = "CHG_TOP1" },
		{ .reg = MT6375_REG_CHG_TOP2, .name = "CHG_TOP2" },
		{ .reg = MT6375_REG_CHG_EOC, .name = "CHG_EOC" },
	};
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	for (i = 0; i < ARRAY_SIZE(settings); i++) {
		ret = mt6375_chg_field_get(ddata, settings[i].fd, &val);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to get %s\n",
				settings[i].name);
			return ret;
		}
		if (i == ARRAY_SIZE(settings) - 1)
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = %d%s\n", settings[i].name, val,
				  settings[i].unit);
		else
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = %d%s, ", settings[i].name, val,
				  settings[i].unit);
	}

	for (i = 0; i < ARRAY_SIZE(adcs); i++) {
		ret = iio_read_channel_processed(&ddata->iio_adcs[adcs[i].chan],
						 &val);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to read adc %s\n",
				adcs[i].name);
			return ret;
		}
		val = U_TO_M(val);
		if (i == ARRAY_SIZE(adcs) - 1)
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = %d%s\n", adcs[i].name, val,
				  adcs[i].unit);
		else
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = %d%s, ", adcs[i].name, val,
				  adcs[i].unit);
	}

	/* No need to lock, it is only for information dump */
	if (ddata->batprotect_en) {
		ret = regmap_bulk_read(ddata->rmap, MT6375_REG_VBAT_MON_RPT,
				       &data, 2);
		if (ret < 0)
			dev_err(ddata->dev,
				"failed to get vbat monitor report\n");
		else {
			val = ADC_FROM_VBAT_RAW(be16_to_cpu(data));
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "VBATCELL = %dmV\n", val);
		}
	}

	for (i = 0; i < ARRAY_SIZE(regs); i++) {
		ret = regmap_read(ddata->rmap, regs[i].reg, &val);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to read %s\n",
				regs[i].name);
			return ret;
		}
		if (i == ARRAY_SIZE(regs) - 1)
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = 0x%02X\n", regs[i].name, val);
		else
			scnprintf(buf + strlen(buf), DUMP_REG_BUF_SIZE,
				  "%s = 0x%02X, ", regs[i].name, val);
	}
	dev_info(ddata->dev, "%s %s", __func__, buf);
	return 0;
}

static int mt6375_do_event(struct charger_device *chgdev, u32 event, u32 args)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	switch (event) {
	case EVENT_FULL:
	case EVENT_RECHARGE:
	case EVENT_DISCHARGE:
		power_supply_changed(ddata->psy);
		break;
	default:
		break;
	}
	return 0;
}

static int mt6375_plug_in(struct charger_device *chgdev)
{
	int ret = 0;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

	mt_dbg(ddata->dev, "++\n");
	if (pdata->wdt_en) {
		ret = mt6375_chg_field_set(ddata, F_WDT_EN, 1);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to enable WDT\n");
			return ret;
		}
	}

	return ret;
}

static int mt6375_plug_out(struct charger_device *chgdev)
{
	int ret = 0;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

	mt_dbg(ddata->dev, "++\n");
#ifdef MT6375_AFC_SUPPORT
	/* AFC disable */
	ddata->hvdcp_irq_status = HV_IRQ_NONE;
	mt6375_chg_field_set(ddata, F_AFC_EN, 0x0);
#endif
	ddata->typec_rp_level = 0;

	if (pdata->wdt_en) {
		ret = mt6375_chg_field_set(ddata, F_WDT_EN, 0);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to disable WDT\n");
			return ret;
		}
	}

	return 0;
}

static int mt6375_enable_6pin_battery_charging(struct charger_device *chgdev,
					       bool en)
{
	int ret = 0;
	u16 data, batend_code;
	u32 vbat, cv;
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

	mutex_lock(&ddata->cv_lock);
	if (ddata->batprotect_en == en)
		goto out;

	mt_dbg(ddata->dev, "en=%d\n", en);
	if (!en)
		goto dis_pro;

	/* If no 6pin used, always bypass this function */
	if (atomic_read(&ddata->no_6pin_used) == 1)
		goto dis_pro;

	ret = mt6375_chg_field_set(ddata, F_VBAT_MON_EN, 1);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to enable vbat monitor\n");
		goto out;
	}
	usleep_range(ADC_CONV_TIME_US * 2, ADC_CONV_TIME_US * 3);
	ret = regmap_bulk_read(ddata->rmap, MT6375_REG_VBAT_MON_RPT, &data, 2);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get vbat monitor report\n");
		goto dis_mon;
	}
	vbat = ADC_FROM_VBAT_RAW(be16_to_cpu(data));
	ret = mt6375_chg_field_get(ddata, F_CV, &cv);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get cv\n");
		goto dis_mon;
	}
	mt_dbg(ddata->dev, "vbat = %dmV, cv = %dmV\n", vbat, cv);
	if (vbat >= cv) {
		dev_warn(ddata->dev, "vbat(%d) >= cv(%d), should not start\n",
			 vbat, cv);
		goto dis_mon;
	} else if (vbat <= pdata->pmic_uvlo) {
		/*
		 * If no 6pin used, vbat detected by vbat mon will be much
		 * lower than the PMIC UVLO.
		 */
		atomic_set(&ddata->no_6pin_used, 1);
		dev_notice(ddata->dev, "vbat <= PMIC UVLO(%d mV), should not start\n",
			   pdata->pmic_uvlo);
		goto dis_mon;
	}

	ret = mt6375_chg_field_set(ddata, F_BATINT, cv);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set batint\n");
		goto dis_mon;
	}
	batend_code = ADC_TO_VBAT_RAW(cv);
	batend_code = cpu_to_be16(batend_code);
	mt_dbg(ddata->dev, "batend code = 0x%04X\n", batend_code);
	ret = regmap_bulk_write(ddata->rmap, MT6375_REG_BATEND_CODE,
				&batend_code, 2);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set batend code\n");
		goto dis_mon;
	}
	ret = mt6375_chg_field_set(ddata, F_BATPROTECT_EN, 1);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to enable bat protect\n");
		goto dis_mon;
	}
	ret = mt6375_chg_field_set(ddata, F_CV,
				   mt6375_chg_fields[F_CV].range->max);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set maximum cv\n");
		goto dis_pro;
	}
	ddata->batprotect_en = true;
	mt_dbg(ddata->dev, "successfully\n");
	goto out;
dis_pro:
	if (mt6375_chg_field_set(ddata, F_BATPROTECT_EN, 0) < 0)
		dev_notice(ddata->dev, "failed to disable bat protect\n");
	if (mt6375_chg_field_set(ddata, F_CV, ddata->cv) < 0)
		dev_notice(ddata->dev, "failed to set cv\n");
dis_mon:
	if (mt6375_chg_field_set(ddata, F_VBAT_MON_EN, 0) < 0)
		dev_notice(ddata->dev, "failed to disable vbat monitor\n");
	ddata->batprotect_en = false;
out:
	mutex_unlock(&ddata->cv_lock);
	return ret;
}

static int mt6375_enable_usbid(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret = 0;

	ret = mt6375_chg_field_set(ddata, F_PD_MDEN, en ? 0 : 1);
	if (ret < 0)
		return ret;
	return mt6375_chg_field_set(ddata, F_USBID_EN, en ? 1 : 0);
}

static inline u32 mt6375_trans_usbid_rup(u32 rup)
{
	int i = 0;
	int maxidx = ARRAY_SIZE(mt6375_usbid_rup) - 1;

	if (rup >= mt6375_usbid_rup[0])
		return 0;
	if (rup <= mt6375_usbid_rup[maxidx])
		return maxidx;

	for (i = 0; i < maxidx; i++) {
		if (rup == mt6375_usbid_rup[i])
			return i;
		if (rup < mt6375_usbid_rup[i] &&
		    rup > mt6375_usbid_rup[i + 1]) {
			if ((mt6375_usbid_rup[i] - rup) <=
			    (rup - mt6375_usbid_rup[i + 1]))
				return i;
			else
				return i + 1;
		}
	}
	return maxidx;
}

static int mt6375_set_usbid_rup(struct charger_device *chgdev, u32 rup)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	u32 val = mt6375_trans_usbid_rup(rup);

	return mt6375_chg_field_set(ddata, F_ID_RUPSEL, val);
}

static inline u32 mt6375_trans_usbid_src_ton(u32 src_ton)
{
	int i = 0;
	int maxidx = ARRAY_SIZE(mt6375_usbid_src_ton) - 1;

	/* There is actually an option, always on, after 400000 */
	if (src_ton == 0)
		return maxidx + 1;
	if (src_ton <= mt6375_usbid_src_ton[0])
		return 0;
	if (src_ton >= mt6375_usbid_src_ton[maxidx])
		return maxidx;

	for (i = 0; i < maxidx; i++) {
		if (src_ton == mt6375_usbid_src_ton[i])
			return i;
		if (src_ton > mt6375_usbid_src_ton[i] &&
		    src_ton < mt6375_usbid_src_ton[i + 1]) {
			if ((src_ton - mt6375_usbid_src_ton[i]) <=
			    (mt6375_usbid_src_ton[i + 1] - src_ton))
				return i;
			else
				return i + 1;
		}
	}
	return maxidx;
}

static int mt6375_set_usbid_src_ton(struct charger_device *chgdev, u32 src_ton)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	u32 val = mt6375_trans_usbid_src_ton(src_ton);

	return mt6375_chg_field_set(ddata, F_IS_TDET, val);
}

static int mt6375_enable_usbid_floating(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	return mt6375_chg_field_set(ddata, F_USBID_FLOATING, en ? 1 : 0);
}

static int mt6375_enable_ship_mode(struct charger_device *chgdev, bool batdet_dis_dly)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret;

	dev_notice(ddata->dev, "enable ship mode\n");

	ret = mt6375_chg_field_set(ddata, F_SHIP_RST_DIS, 1);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable ship reset\n");
		return ret;
	}

	/* if batdet_dis_dly= 1, enable BATFET delay (18s) */
	ret = mt6375_chg_field_set(ddata, F_BATFET_DISDLY, batdet_dis_dly);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable ship mode delay\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_BUCK_EN, 0);
	if (ret < 0) {
		dev_notice(ddata->dev, "failed to disable chg buck en\n");
		return ret;
	}

	return regmap_update_bits(ddata->rmap, MT6375_REG_CHG_TOP1,
				  MT6375_MSK_BATFET_DIS, 0xFF);
}

static int mt6375_set_eoc_timer(struct charger_device *chgdev, unsigned int mins)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	pr_info("%s: set eoc timer: %d\n", __func__, mins);
	return mt6375_chg_field_set(ddata, F_EOC_TIME, mins);
}

static int mt6375_enable_aicc(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	dev_notice(ddata->dev, "enable aicc: %d\n", en);
	return mt6375_chg_field_set(ddata, F_AICC_EN, en);
}

static int mt6375_get_health(struct charger_device *chgdev, int *health)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret = 0;
	u8 stat[9];

	*health = POWER_SUPPLY_HEALTH_UNKNOWN;

	ret = regmap_bulk_read(ddata->rmap, MT6375_REG_CHG_STAT0,
			       stat, sizeof(stat));
	if (ret < 0) {
		dev_notice(ddata->dev, "failed to get chg stat\n");
		return ret;
	}

	if (stat[8] & MT6375_MSK_OTP0_STAT)
		*health = POWER_SUPPLY_HEALTH_OVERHEAT;
#if POWER_SUPPLY_EXT_HEALTH
	else if (stat[1] & MT6375_MSK_ST_CHG_SYSOV)
		*health = POWER_SUPPLY_EXT_HEALTH_VSYS_OVP;
	else if (stat[1] & MT6375_MSK_ST_CHG_BATOV)
		*health = POWER_SUPPLY_EXT_HEALTH_VBAT_OVP;
#endif /* POWER_SUPPLY_EXT_HEALTH */
	else if (stat[4] & MT6375_MSK_CHRDET_OV)
		*health = POWER_SUPPLY_HEALTH_OVERVOLTAGE;
	else if (stat[2] & MT6375_MSK_ST_WDT_IN)
		*health = POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE;
	else if (stat[1] & MT6375_MSK_ST_CHG_TOUT)
		*health = POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE;
#if POWER_SUPPLY_EXT_HEALTH
	else if (stat[4] & MT6375_MSK_CHRDET_UV)
		*health = POWER_SUPPLY_EXT_HEALTH_UNDERVOLTAGE;
#endif /* POWER_SUPPLY_EXT_HEALTH */
	else
		*health = POWER_SUPPLY_HEALTH_GOOD;

	return 0;
}

static int mt6375_get_charge_type(struct charger_device *chgdev, int *type)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);
	int ret = 0;
	u32 stat = 0, aicc = 0;

	*type = POWER_SUPPLY_CHARGE_TYPE_UNKNOWN;

	ret = mt6375_chg_field_get(ddata, F_IC_STAT, &stat);
	if (ret < 0) {
		dev_notice(ddata->dev, "failed to get ic stat\n");
		return ret;
	}

	switch (stat) {
	case CHG_STAT_TRICKLE:
	case CHG_STAT_PRE:
		*type = POWER_SUPPLY_CHARGE_TYPE_TRICKLE;
		break;
	case CHG_STAT_FAST:
		*type = POWER_SUPPLY_CHARGE_TYPE_FAST;
		ret = mt6375_run_aicc(chgdev, &aicc);
		if (ret < 0) {
			dev_notice(ddata->dev, "failed to run aicc\n");
			return ret;
		}
		if ((aicc > 0) && (aicc <= 400000))
			*type = POWER_SUPPLY_CHARGE_TYPE_TRICKLE;
		break;
	case CHG_STAT_EOC:
	case CHG_STAT_BKGND:
	case CHG_STAT_DONE:
		*type = POWER_SUPPLY_CHARGE_TYPE_LONGLIFE;
		break;
	default:
		*type = POWER_SUPPLY_CHARGE_TYPE_NONE;
		break;
	}

	return 0;

}

static int mt6375_get_charging_status(struct charger_device *chgdev, u32 *charging_status)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	*charging_status = mt6375_get_chg_status(ddata);
	return 0;
}

static int mt6375_en_wdt(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	dev_notice(ddata->dev, "enable wdt: %d\n", en);
	return mt6375_chg_field_set(ddata, F_WDT_EN, en);
}

static int mt6375_en_ilim(struct charger_device *chgdev, bool en)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	dev_notice(ddata->dev, "enable ilim: %d\n", en);
	return mt6375_chg_field_set(ddata, F_ILIM_EN, en);
}

static int mt6375_set_vsysmin(struct charger_device *chgdev, unsigned int mV)
{
	struct mt6375_chg_data *ddata = charger_get_data(chgdev);

	dev_notice(ddata->dev, "set vsysmin: %d\n", mV);
	return mt6375_chg_field_set(ddata, F_VSYSMIN, mV);
}

static int mt6375_set_iinlmtsel(struct charger_device *chgdev, bool en)
{
	return 0;
}

static const struct charger_properties mt6375_chg_props = {
	.alias_name = "mt6375_chg",
};

static const struct charger_ops mt6375_chg_ops = {
	/* cable plug in/out */
	.plug_in = mt6375_plug_in,
	.plug_out = mt6375_plug_out,
	/* enable */
	.enable = mt6375_enable_charging,
	.is_enabled = mt6375_is_enabled,
	/* charging current */
	.set_charging_current = mt6375_set_ichg,
	.get_charging_current = mt6375_get_ichg,
	.get_min_charging_current = mt6375_get_min_ichg,
	/* charging voltage */
	.set_constant_voltage = mt6375_set_cv,
	.get_constant_voltage = mt6375_get_cv,
	/* input current limit */
	.set_input_current = mt6375_set_aicr,
	.get_input_current = mt6375_get_aicr,
	.get_min_input_current = mt6375_get_min_aicr,
	/* MIVR */
	.set_mivr = mt6375_set_mivr,
	.get_mivr = mt6375_get_mivr,
	.get_mivr_state = mt6375_get_mivr_state,
	/* ADC */
	.get_adc = mt6375_get_adc,
	.get_vbus_adc = mt6375_get_vbus,
	.get_vbus_level = mt6375_get_vbus_level,
	.get_ibus_adc = mt6375_get_ibus,
	.get_ibat_adc = mt6375_get_ibat,
	.get_tchg_adc = mt6375_get_tchg,
	.get_zcv = mt6375_get_zcv,
	/* charing termination */
	.set_eoc_current = mt6375_set_ieoc,
	.enable_termination = mt6375_enable_te,
	.reset_eoc_state = mt6375_reset_eoc_state,
	.safety_check = mt6375_sw_check_eoc,
	.is_charging_done = mt6375_is_charge_done,
	/* power path */
	.enable_powerpath = mt6375_enable_buck,
	.is_powerpath_enabled = mt6375_is_buck_enabled,
	/* timer */
	.enable_safety_timer = mt6375_enable_chg_timer,
	.is_safety_timer_enabled = mt6375_is_chg_timer_enabled,
	.kick_wdt = mt6375_kick_wdt,
	/* AICL */
	.run_aicl = mt6375_run_aicc,
	/* PE+/PE+20 */
	.send_ta_current_pattern = mt6375_set_pe_current_pattern,
	.set_pe20_efficiency_table = mt6375_set_pe20_efficiency_table,
	.send_ta20_current_pattern = mt6375_set_pe20_current_pattern,
	.reset_ta = mt6375_reset_pe_ta,
	.enable_cable_drop_comp = mt6375_enable_pe_cable_drop_comp,
	/* OTG */
	.set_boost_current_limit = mt6375_set_otg_cc,
	.enable_otg = mt6375_enable_otg,
	.enable_discharge = mt6375_enable_discharge,
	/* charger type detection */
	.enable_chg_type_det = mt6375_enable_chg_type_det,
	/* misc */
	.dump_registers = mt6375_dump_registers,
	.enable_hz = mt6375_enable_hz,
	/* event */
	.event = mt6375_do_event,
	/* 6pin battery */
	.enable_6pin_battery_charging = mt6375_enable_6pin_battery_charging,
	/* TypeC */
	.enable_usbid = mt6375_enable_usbid,
	.set_usbid_rup = mt6375_set_usbid_rup,
	.set_usbid_src_ton = mt6375_set_usbid_src_ton,
	.enable_usbid_floating = mt6375_enable_usbid_floating,
#ifdef MT6375_AFC_SUPPORT
	.send_afc_data = mt6375_send_afc_data,
	.set_afc_voltage = mt6375_afc_set_voltage,
	.afc_disable_set = mt6375_afc_disable_set,
#endif
	.set_enable_hiccup = mt6375_chg_enable_hiccup,
	.set_cc_hi_state = mt6375_chg_set_cc_hi_state,
	.set_water_state = mt6375_chg_set_water_state,
	.set_eoc_timer = mt6375_set_eoc_timer,
	.enable_ship_mode = mt6375_enable_ship_mode,
	.get_vsys_adc = mt6375_get_vsys,
	.enable_aicc = mt6375_enable_aicc,
	.get_charging_status = mt6375_get_charging_status,
	.en_wdt = mt6375_en_wdt,
	.en_ilim = mt6375_en_ilim,
	.set_vsysmin = mt6375_set_vsysmin,
	.set_iinlmtsel = mt6375_set_iinlmtsel,

	.get_health = mt6375_get_health,
	.get_charge_type = mt6375_get_charge_type,

#ifdef MT6375_AFC_SUPPORT
	.send_afc_data = mt6375_send_afc_data,
#endif
};

static irqreturn_t mt6375_fl_wdt_handler(int irq, void *data)
{
	int ret;
	struct mt6375_chg_data *ddata = data;

	ret = mt6375_chg_field_set(ddata, F_WDT_RST, 1);
	if (ret < 0)
		dev_notice(ddata->dev, "failed to kick wdt\n");
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_pwr_rdy_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	mt6375_chg_pwr_rdy_process(ddata);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_detach_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	mt6375_chg_pwr_rdy_process(ddata);
	complete(&ddata->aicc_done);
	complete(&ddata->pe_done);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_vbus_ov_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	charger_dev_notify(ddata->chgdev, CHARGER_DEV_NOTIFY_VBUS_OVP);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_chg_tout_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	charger_dev_notify(ddata->chgdev, CHARGER_DEV_NOTIFY_SAFETY_TIMEOUT);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_bc12_dn_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;
	int attach;

	mt_dbg(ddata->dev, "++\n");
	mutex_lock(&ddata->attach_lock);
	attach = atomic_read(&ddata->attach);
	ddata->bc12_dn = (attach == ATTACH_TYPE_NONE) ? false : true;
	mutex_unlock(&ddata->attach_lock);

	if (!ddata->bc12_dn) {
		dev_notice(ddata->dev, "%s attach=%d, bc12_dn=%d",
			   __func__, attach, ddata->bc12_dn);
		return IRQ_HANDLED;
	}

	if (attach < ATTACH_TYPE_PD && !queue_work(ddata->wq, &ddata->bc12_work))
		dev_notice(ddata->dev, "%s bc12 work already queued\n", __func__);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_bc12_hvdcp_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
#ifdef MT6375_AFC_SUPPORT
	queue_work(ddata->wq, &ddata->hvdcp_work);
#endif

	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_pe_done_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	complete(&ddata->pe_done);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_aicc_done_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "++\n");
	complete(&ddata->aicc_done);
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_fl_batpro_done_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;
	int ret;

	mt_dbg(ddata->dev, "++\n");
	ret = mt6375_enable_6pin_battery_charging(ddata->chgdev, false);
	charger_dev_notify(ddata->chgdev, CHARGER_DEV_NOTIFY_BATPRO_DONE);
	return ret < 0 ? ret : IRQ_HANDLED;
}

static irqreturn_t mt6375_adc_vbat_mon_ov_handler(int irq, void *data)
{
	struct mt6375_chg_data *ddata = data;
	u32 cv;

	mt6375_get_cv(ddata->chgdev, &cv);
	mt_dbg(ddata->dev, "cv = %dmV\n", U_TO_M(cv));
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_cmdcpl_handler(int irq, void *data)
{
#ifdef MT6375_AFC_SUPPORT
	struct mt6375_chg_data *ddata = data;

	mt_dbg(ddata->dev, "%s\n", __func__);
	complete(&ddata->afc_done);
#endif
	return IRQ_HANDLED;
}

static irqreturn_t mt6375_resp_handler(int irq, void *data)
{
	return IRQ_HANDLED;
}

#if IS_ENABLED(CONFIG_SEC_FACTORY)
static void send_lcd_on_uevent(struct mt6375_chg_data *ddata)
{
	char *envp[2];
	char *type = {"TYPE=LCD_ON"};
	int index = 0;

	pr_info("%s\n", __func__);

	envp[index++] = type;
	envp[index++] = NULL;

	kobject_uevent_env(&ddata->dev->kobj,
		KOBJ_CHANGE, envp);
}

static void mt6375_usbid_init(struct mt6375_chg_data *ddata, bool en)
{
	if (en) {
		charger_dev_set_usbid_rup(ddata->chgdev, 75000);
		charger_dev_set_usbid_src_ton(ddata->chgdev, 0);
	}

	charger_dev_enable_usbid(ddata->chgdev, en);
}
#endif /* CONFIG_SEC_FACTORY */

static irqreturn_t mt6375_usbid_evt_handler(int irq, void *data)
{
#if IS_ENABLED(CONFIG_SEC_FACTORY)
	struct mt6375_chg_data *ddata = data;

	send_lcd_on_uevent(ddata);
#endif

	return IRQ_HANDLED;
}

struct mt6375_chg_dtprop {
	const char *name;
	const char *legacy_name;
	size_t offset;
	enum mt6375_chg_reg_field field;
	enum mt6375_chg_dtprop_type type;
	bool optional;
};

#define MT6375_CHG_DTPROP(_prop_name, _var, _field, _type, _opt) \
{ \
	.name = _prop_name, \
	.legacy_name = #_var, \
	.field = _field, \
	.type = _type, \
	.offset = offsetof(struct mt6375_chg_platform_data, _var), \
	.optional = _opt, \
}

static const struct mt6375_chg_dtprop mt6375_chg_dtprops[] = {
	MT6375_CHG_DTPROP("vbus-ov", vbus_ov, F_VBUS_OV, DTPROP_U32, false),
	MT6375_CHG_DTPROP("chg-tmr", chg_tmr, F_CHG_TMR, DTPROP_U32, false),
	MT6375_CHG_DTPROP("chg-tmr-en", chg_tmr_en, F_CHG_TMR_EN, DTPROP_BOOL, false),
	MT6375_CHG_DTPROP("otg-lbp", otg_lbp, F_OTG_LBP, DTPROP_U32, false),
	MT6375_CHG_DTPROP("ircmp-v", ircmp_v, F_IRCMP_V, DTPROP_U32, false),
	MT6375_CHG_DTPROP("ircmp-r", ircmp_r, F_IRCMP_R, DTPROP_U32, false),
	MT6375_CHG_DTPROP("wdt", wdt, F_WDT, DTPROP_U32, false),
	MT6375_CHG_DTPROP("wdt-en", wdt_en, F_WDT_EN, DTPROP_BOOL, false),
	MT6375_CHG_DTPROP("te-en", te_en, F_TE, DTPROP_BOOL, false),
	MT6375_CHG_DTPROP("mivr", mivr, F_VMIVR, DTPROP_U32, true),
	MT6375_CHG_DTPROP("aicr", aicr, F_IAICR, DTPROP_U32, true),
	MT6375_CHG_DTPROP("ichg", ichg, F_CC, DTPROP_U32, true),
	MT6375_CHG_DTPROP("ieoc", ieoc, F_IEOC, DTPROP_U32, true),
	MT6375_CHG_DTPROP("cv", cv, F_CV, DTPROP_U32, true),
	MT6375_CHG_DTPROP("vrec", vrec, F_VREC, DTPROP_U32, true),
	MT6375_CHG_DTPROP("dcdt-sel", dcdt_sel, F_DCDT_SEL, DTPROP_U32, true),
	MT6375_CHG_DTPROP("spec-ta-en", spec_ta_en, F_SPEC_TA_EN, DTPROP_BOOL, true),
};

static inline u32 pdata_get_val(void *pdata, const struct mt6375_chg_dtprop *dp)
{
	if (dp->type == DTPROP_BOOL)
		return *((bool *)(pdata + dp->offset));
	return *((u32 *)(pdata + dp->offset));
}

static int mt6375_chg_apply_dt(struct mt6375_chg_data *ddata)
{
	int i, ret;
	u32 val;
	const struct mt6375_chg_dtprop *dp;

	mt_dbg(ddata->dev, "++\n");
	pr_info("%s\n", __func__);
	for (i = 0; i < ARRAY_SIZE(mt6375_chg_dtprops); i++) {
		dp = &mt6375_chg_dtprops[i];
		if (dp->field >= F_MAX)
			continue;
		val = pdata_get_val(dev_get_platdata(ddata->dev), dp);
#if defined(CONFIG_MTK_NO_BAT_BOOT_SUPPORT)
#if IS_ENABLED(CONFIG_SEC_FACTORY)
	if (!strcmp(dp->name, "te_en")) {
		val = 0;
		dev_err(ddata->dev, "force disable [%s] for no bat boot support\n", dp->name);
	}
	if (!strcmp(dp->name, "aicr")) {
		val = 3225;
		dev_err(ddata->dev, "force set [%s] %d for no bat boot support\n", dp->name, val);
	}
	if (!strcmp(dp->name, "mivr")) {
		val = 4200;
		dev_err(ddata->dev, "force set [%s] %d for no bat boot support\n", dp->name, val);
	}
#else
	if (ddata->f_mode == OB_MODE) {
		if (!strcmp(dp->name, "te_en")) {
			val = 0;
			dev_err(ddata->dev, "force disable [%s] for no bat boot support\n", dp->name);
		}
		if (!strcmp(dp->name, "aicr")) {
			val = 3225;
			dev_err(ddata->dev, "force set [%s] %d for no bat boot support\n", dp->name, val);
		}
	}
#endif
#endif
		if (!strcmp(dp->name, "cv"))
			dev_err(ddata->dev, "cv set [%s] %d\n", dp->name, val);

		ret = mt6375_chg_field_set(ddata, dp->field, val);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to write dtprop %s\n",
				dp->name);
			if (!dp->optional)
				return ret;
		}
	}
	return 0;
}

static void mt6375_chg_parse_dt_helper(struct device *dev, void *pdata,
				       const struct mt6375_chg_dtprop *dp)
{
	int ret;
	void *val = pdata + dp->offset;

	if (dp->type == DTPROP_BOOL)
		*((bool *)val) = device_property_read_bool(dev, dp->name) ||
				 device_property_read_bool(dev, dp->legacy_name);
	else {
		ret = device_property_read_u32(dev, dp->name, val) ?
		      device_property_read_u32(dev, dp->legacy_name, val) : 0;
		if (ret < 0)
			dev_info(dev, "property %s (legacy_name: %s) not found\n",
				 dp->name, dp->legacy_name);
	}
}

static int mt6375_chg_get_pdata(struct device *dev)
{
	int i;
	u32 val;
	const struct {
		u32 size;
		u32 tag;
		u32 boot_mode;
		u32 boot_type;
	} *tag;
	struct device_node *bc12_np, *boot_np, *pmic_uvlo_np, *np = dev->of_node;
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(dev);
	int ret;

	mt_dbg(dev, "%s: entry. Get pdata now.\n", __func__);
	if (np) {
		pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
		if (!pdata)
			return -ENOMEM;
		memcpy(pdata, &mt6375_chg_pdata_def, sizeof(*pdata));
		for (i = 0; i < ARRAY_SIZE(mt6375_chg_dtprops); i++)
			mt6375_chg_parse_dt_helper(dev, pdata,
						   &mt6375_chg_dtprops[i]);
		pdata->usb_killer_detect = device_property_read_bool(dev, "usb-killer-detect") ||
					   device_property_read_bool(dev, "usb_killer_detect");

		/* mediatek chgdev name */
		if (of_property_read_string(np, "chg_name", &pdata->chg_name) &&
		    of_property_read_string(np, "chg-name", &pdata->chg_name))
			dev_notice(dev, "failed to get chg_name\n");

		/* mediatek boot mode */
		boot_np = of_parse_phandle(np, "boot-mode", 0);
		if (!boot_np)
			boot_np = of_parse_phandle(np, "boot_mode", 0);

		if (!boot_np) {
			dev_err(dev, "failed to get bootmode phandle\n");
			return -ENODEV;
		}
		tag = of_get_property(boot_np, "atag,boot", NULL);
		if (!tag) {
			dev_err(dev, "failed to get atag,boot\n");
			return -EINVAL;
		}
		dev_info(dev, "sz:0x%x tag:0x%x mode:0x%x type:0x%x\n",
			 tag->size, tag->tag, tag->boot_mode, tag->boot_type);
		pdata->boot_mode = tag->boot_mode;
		pdata->boot_type = tag->boot_type;

		/*
		 * mediatek bc12_sel
		 * 0 means bc12 owner is THIS_MODULE,
		 * if it is not 0, always ignore
		 */
		bc12_np = of_parse_phandle(np, "bc12_sel", 0);
		if (!bc12_np) {
			dev_err(dev, "failed to get bc12_sel phandle\n");
			return -ENODEV;
		}
		if (of_property_read_u32(bc12_np, "bc12_sel", &val) < 0) {
			dev_err(dev, "property bc12_sel not found\n");
			return -EINVAL;
		}
		if (val != 0)
			pdata->attach_trig = ATTACH_TRIG_IGNORE;
		else if (IS_ENABLED(CONFIG_TCPC_CLASS))
			pdata->attach_trig = ATTACH_TRIG_TYPEC;
		else
			pdata->attach_trig = ATTACH_TRIG_PWR_RDY;

		pmic_uvlo_np = of_parse_phandle(np, "pmic-uvlo", 0);
		if (!pmic_uvlo_np)
			dev_notice(dev, "Failed to get pmic-uvlo phandle\n");

		if (of_property_read_u32(pmic_uvlo_np, "uvlo-level", &val) < 0)
			dev_notice(dev, "property uvlo-level not found, use default: %d mv\n",
				   DEFAULT_PMIC_UVLO_mV);

		if (val != 0)
			pdata->pmic_uvlo = val;
		else
			pdata->pmic_uvlo = DEFAULT_PMIC_UVLO_mV;

		pdata->vctrl_otg_gpio = of_get_named_gpio(np, "vctrl_otg_gpio", 0);
		if (gpio_is_valid(pdata->vctrl_otg_gpio)) {
			dev_info(dev, "vctrl_otg_gpio is used\n");
			ret = devm_gpio_request(dev, pdata->vctrl_otg_gpio, "vctrl_otg");
			if (ret) {
				dev_err(dev, "failed vctrl_otg_gpio request\n");
				return ret;
			}
			gpio_direction_output(pdata->vctrl_otg_gpio, 0);
		} else
			dev_info(dev, "vctrl_otg_gpio is not used\n");

		dev->platform_data = pdata;
	}
	return pdata ? 0 : -ENODEV;
}

static int mt6375_chg_init_setting(struct mt6375_chg_data *ddata)
{
	int ret;
	u32 val;
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	if (!f_mode)
		ddata->f_mode = NO_MODE;
	else if ((strncmp(f_mode, "OB", 2) == 0) || (strncmp(f_mode, "DL", 2) == 0))
		ddata->f_mode = OB_MODE;
	else if (strncmp(f_mode, "IB", 2) == 0)
		ddata->f_mode = IB_MODE;
	else
		ddata->f_mode = NO_MODE;
	pr_info("[BAT] %s: f_mode: %s\n", __func__, BOOT_MODE_STRING[ddata->f_mode]);
#endif

	mt_dbg(ddata->dev, "%s: entry. Init setting now.\n", __func__);
	ret = mt6375_chg_field_set(ddata, F_AICC_ONESHOT, 1);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set aicc oneshot\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_BC12_EN, 0);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable bc12\n");
		return ret;
	}

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	if (ddata->f_mode != OB_MODE)
#endif
	{
		/* set aicr = 200mA in 1:META_BOOT 5:ADVMETA_BOOT */
		if (pdata->boot_mode == 1 || pdata->boot_mode == 5) {
			ret = mt6375_chg_field_set(ddata, F_IAICR, 200);
			if (ret < 0) {
				dev_err(ddata->dev, "failed to set aicr 200mA\n");
				return ret;
			}
		}
	}

	ret = mt6375_chg_apply_dt(ddata);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to apply dt property\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_ILIM_EN, 0);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable ilim\n");
		return ret;
	}

	/*
	 * disable wdt to save 1mA power consumption
	 * it will be turned back on later
	 * if it is enabled in dt property and TA attached
	 */
	ret = mt6375_chg_field_set(ddata, F_WDT_EN, 0);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable WDT\n");
		return ret;
	}

	/* chrdet ovp limit set 12.5V */
	ret = mt6375_chg_field_set(ddata, F_CHRD_OV_VREF, 12500);
	if (ret < 0) {
		dev_warn(ddata->dev, "failed to set chrdet ovp\n");
		return ret;
	}

#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	/* ALPS05385820 : recharging VREC set to MAX, */
	/* to inform topoff timer and */
	/* for using samsung's recharging routine */
	ret = mt6375_chg_field_set(ddata, F_VREC, mt6375_chg_fields[F_VREC].range->max);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to set F_VREC as 200mV\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_CHG_EN, 0);
	if (ret < 0){
		dev_err(ddata->dev, "failed to set F_CHG_EN as disable\n");
		return ret;
	}
#endif

	/* if get failed, just ignore it */
	ret = mt6375_chg_field_get(ddata, F_PP_PG_FLAG, &val);
	if (ret >= 0 && val)
		dev_warn(ddata->dev, "BATSYSUV occurred\n");
	return mt6375_chg_field_set(ddata, F_PP_PG_FLAG, 1);
}

static int mt6375_chg_get_iio_adc(struct mt6375_chg_data *ddata)
{
	int ret;
	u16 zcv;

	mt_dbg(ddata->dev, "%s: entry. Get 6375 ADC data.\n", __func__);
	ddata->iio_adcs = devm_iio_channel_get_all(ddata->dev);
	if (IS_ERR(ddata->iio_adcs))
		return PTR_ERR(ddata->iio_adcs);
	ret = regmap_bulk_read(ddata->rmap, MT6375_REG_ADC_ZCV_RPT, &zcv, 2);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to get zcv\n");
		return ret;
	}
	ddata->zcv = ADC_FROM_VBAT_RAW(be16_to_cpu(zcv));
	return 0;
}

static int mt6375_chg_init_psy(struct mt6375_chg_data *ddata)
{
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);
	struct power_supply_config cfg = {
		.drv_data = ddata,
		.of_node = ddata->dev->of_node,
		.supplied_to = mt6375_psy_supplied_to,
		.num_supplicants = ARRAY_SIZE(mt6375_psy_supplied_to),
	};

	mt_dbg(ddata->dev, "%s: entry. Init power supply now.(%s)\n", __func__, pdata->chg_name);
	memcpy(&ddata->psy_desc, &mt6375_psy_desc, sizeof(ddata->psy_desc));
	ddata->psy_desc.name = pdata->chg_name;
	ddata->psy = devm_power_supply_register(ddata->dev, &ddata->psy_desc,
						&cfg);
	return IS_ERR(ddata->psy) ? PTR_ERR(ddata->psy) : 0;
}

static int mt6375_chg_init_regulator(struct mt6375_chg_data *ddata)
{
	struct regulator_config cfg = {
		.dev = ddata->dev,
		.driver_data = ddata,
		.regmap = ddata->rmap,
	};

	mt_dbg(ddata->dev, "%s: entry. Init regulator now.\n", __func__);
	memcpy(&ddata->rdesc, &mt6375_chg_otg_rdesc, sizeof(ddata->rdesc));
	ddata->rdesc.name = dev_name(ddata->dev);
	ddata->rdev = devm_regulator_register(ddata->dev, &ddata->rdesc, &cfg);
	return IS_ERR(ddata->rdev) ? PTR_ERR(ddata->rdev) : 0;
}

static int mt6375_chg_init_chgdev(struct mt6375_chg_data *ddata)
{
	struct mt6375_chg_platform_data *pdata = dev_get_platdata(ddata->dev);

	mt_dbg(ddata->dev, "%s: entry. Init charger device now.\n", __func__);
	ddata->chgdev = charger_device_register(pdata->chg_name, ddata->dev,
						ddata, &mt6375_chg_ops,
						&mt6375_chg_props);
	return IS_ERR(ddata->chgdev) ? PTR_ERR(ddata->chgdev) : 0;
}

#define MT6375_CHG_IRQ(_name) \
{ \
	.name = #_name, \
	.hdlr = mt6375_##_name##_handler, \
}

static int mt6375_chg_init_irq(struct mt6375_chg_data *ddata)
{
	int i, ret;
	struct {
		const char * const name;
		irq_handler_t const hdlr;
		unsigned int irq;
	} mt6375_chg_irqs[] = {
		MT6375_CHG_IRQ(fl_wdt),
		MT6375_CHG_IRQ(fl_pwr_rdy),
		MT6375_CHG_IRQ(fl_vbus_ov),
		MT6375_CHG_IRQ(fl_chg_tout),
		MT6375_CHG_IRQ(fl_detach),
		MT6375_CHG_IRQ(fl_bc12_dn),
		MT6375_CHG_IRQ(fl_bc12_hvdcp),
		MT6375_CHG_IRQ(fl_pe_done),
		MT6375_CHG_IRQ(fl_aicc_done),
		MT6375_CHG_IRQ(fl_batpro_done),
		MT6375_CHG_IRQ(adc_vbat_mon_ov),
		MT6375_CHG_IRQ(cmdcpl),
		MT6375_CHG_IRQ(usbid_evt),
		MT6375_CHG_IRQ(resp),
	};

	mt_dbg(ddata->dev, "%s: entry. Init irq now.\n", __func__);
	for (i = 0; i < ARRAY_SIZE(mt6375_chg_irqs); i++) {
		ret = platform_get_irq_byname(to_platform_device(ddata->dev),
					      mt6375_chg_irqs[i].name);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to get irq %s\n",
				mt6375_chg_irqs[i].name);
			return ret;
		}
		mt6375_chg_irqs[i].irq = ret;
		ret = devm_request_threaded_irq(ddata->dev, ret, NULL,
						mt6375_chg_irqs[i].hdlr,
						IRQF_ONESHOT,
						dev_name(ddata->dev), ddata);
		if (ret < 0) {
			dev_err(ddata->dev, "failed to request irq %s\n",
				mt6375_chg_irqs[i].name);
			return ret;
		}
	}
	ddata->detach_irq = mt6375_chg_irqs[4].irq;
	return 0;
}

static int mt6375_set_shipping_mode(struct mt6375_chg_data *ddata)
{
	int ret;

	ret = mt6375_chg_field_set(ddata, F_SHIP_RST_DIS, 1);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable ship reset\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_BATFET_DISDLY, 0);
	if (ret < 0) {
		dev_err(ddata->dev, "failed to disable ship mode delay\n");
		return ret;
	}

	ret = mt6375_chg_field_set(ddata, F_BUCK_EN, 0);
	if (ret < 0) {
		dev_notice(ddata->dev, "failed to disable chg buck en\n");
		return ret;
	}

	return regmap_update_bits(ddata->rmap, MT6375_REG_CHG_TOP1,
				  MT6375_MSK_BATFET_DIS, 0xFF);
}

static ssize_t shipping_mode_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	int ret;
	unsigned long magic;
	struct mt6375_chg_data *ddata = dev_get_drvdata(dev);

	ret = kstrtoul(buf, 0, &magic);
	if (ret < 0) {
		dev_warn(dev, "parsing number fail\n");
		return ret;
	}
	if (magic != 5526789)
		return -EINVAL;
	ret = mt6375_set_shipping_mode(ddata);
	return ret < 0 ? ret : count;
}
static const DEVICE_ATTR_WO(shipping_mode);

static int mt6375_chg_probe(struct platform_device *pdev)
{
	int i, ret;
	struct mt6375_chg_data *ddata;
	struct device *dev = &pdev->dev;
	const struct mt6375_chg_field *fds = mt6375_chg_fields;

	dev_info(dev, "%s: entry. 6375 charger probe now.\n", __func__);
	ddata = devm_kzalloc(dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->rmap = dev_get_regmap(dev->parent, NULL);
	if (!ddata->rmap) {
		dev_err(dev, "failed to get regmap\n");
		return -ENODEV;
	}

	for (i = 0; i < F_MAX; i++) {
		ddata->rmap_fields[i] = devm_regmap_field_alloc(dev,
								ddata->rmap,
								fds[i].field);
		if (IS_ERR(ddata->rmap_fields[i])) {
			dev_err(dev, "failed to allocate regmap field\n");
			return PTR_ERR(ddata->rmap_fields[i]);
		}
	}

	ret = mt6375_chg_get_pdata(dev);
	if (ret < 0) {
		dev_err(dev, "failed to get platform data\n");
		return ret;
	}

	ddata->dev = dev;
	init_completion(&ddata->pe_done);
	init_completion(&ddata->aicc_done);
	mutex_init(&ddata->attach_lock);
	mutex_init(&ddata->pe_lock);
	mutex_init(&ddata->cv_lock);
	mutex_init(&ddata->hm_lock);
	atomic_set(&ddata->attach, 0);
	atomic_set(&ddata->eoc_cnt, 0);
	atomic_set(&ddata->no_6pin_used, 0);
	ddata->wq = create_singlethread_workqueue(dev_name(dev));
	if (!ddata->wq) {
		dev_err(dev, "failed to create workqueue\n");
		ret = -ENOMEM;
		goto out;
	}
	INIT_WORK(&ddata->bc12_work, mt6375_chg_bc12_work_func);
#ifdef MT6375_AFC_SUPPORT
#if IS_ENABLED(CONFIG_MUIC_NOTIFIER)
	if (get_afc_mode() == CH_MODE_AFC_DISABLE_VAL) {
		pr_info("AFC mode disabled\n");
		ddata->afc_disable = true;
	} else {
		pr_info("AFC mode enabled\n");
		ddata->afc_disable = false;
	}
#endif
	init_completion(&ddata->afc_done);
	INIT_WORK(&ddata->hvdcp_work, mt6375_chg_hvdcp_work_func);
	INIT_WORK(&ddata->afcv_work, mt6375_chg_afcv_work_func);
#endif
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	INIT_DELAYED_WORK(&ddata->sec_cable_work, mt6375_sec_cable_work_func);
#endif
	INIT_DELAYED_WORK(&ddata->cc_hi_check_work, mt6375_chg_cc_hi_check_work_func);
	platform_set_drvdata(pdev, ddata);

#if IS_ENABLED(CONFIG_PDIC_NOTIFIER)
	ddata->lpm_bootmode = is_lpcharge_pdic_param();
#endif

	ret = device_create_file(dev, &dev_attr_shipping_mode);
	if (ret < 0) {
		dev_err(dev, "failed to create shipping mode attribute\n");
		goto out_wq;
	}

	ret = mt6375_chg_init_setting(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to init setting\n");
		goto out_attr;
	}

	ret = mt6375_chg_get_iio_adc(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to get iio adc\n");
		goto out_attr;
	}

	ret = mt6375_chg_init_psy(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to init power supply\n");
		goto out_attr;
	}

	ret = mt6375_chg_init_regulator(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to init regulator\n");
		goto out_attr;
	}

	ret = mt6375_chg_init_chgdev(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to init chgdev\n");
		goto out_attr;
	}

	ret = mt6375_chg_init_irq(ddata);
	if (ret < 0) {
		dev_err(dev, "failed to init irq\n");
		goto out_chgdev;
	}
	mt6375_chg_pwr_rdy_process(ddata);
#if IS_ENABLED(CONFIG_BATTERY_SAMSUNG)
	/* samsung cable type init value = 1 */
	ddata->sec_cable_type = SEC_BATTERY_CABLE_NONE;

	dev_info(dev, "%s: entry. 6375 gauge probe ended.\n", __func__);
	sec_chg_set_dev_init(SC_DEV_FG);
#endif

	mt_dbg(dev, "successfully\n");
	return 0;
out_chgdev:
	charger_device_unregister(ddata->chgdev);
out_attr:
	device_remove_file(ddata->dev, &dev_attr_shipping_mode);
out_wq:
	destroy_workqueue(ddata->wq);
out:
	mutex_destroy(&ddata->hm_lock);
	mutex_destroy(&ddata->cv_lock);
	mutex_destroy(&ddata->pe_lock);
	mutex_destroy(&ddata->attach_lock);
	return ret;
}

static int mt6375_chg_remove(struct platform_device *pdev)
{
	struct mt6375_chg_data *ddata = platform_get_drvdata(pdev);

	mt_dbg(&pdev->dev, "%s: entry. 6375 charger remove now.\n", __func__);
	if (ddata) {
		charger_device_unregister(ddata->chgdev);
		device_remove_file(ddata->dev, &dev_attr_shipping_mode);
		destroy_workqueue(ddata->wq);
		mutex_destroy(&ddata->hm_lock);
		mutex_destroy(&ddata->cv_lock);
		mutex_destroy(&ddata->pe_lock);
		mutex_destroy(&ddata->attach_lock);
	}
	return 0;
}

static const struct of_device_id __maybe_unused mt6375_chg_of_match[] = {
	{ .compatible = "mediatek,mt6375-chg", },
	{ },
};
MODULE_DEVICE_TABLE(of, mt6375_chg_of_match);

static struct platform_driver mt6375_chg_driver = {
	.probe = mt6375_chg_probe,
	.remove = mt6375_chg_remove,
	.driver = {
		.name = "mt6375-chg",
		.of_match_table = of_match_ptr(mt6375_chg_of_match),
	},
};
module_platform_driver(mt6375_chg_driver);

MODULE_AUTHOR("ShuFan Lee <shufan_lee@richtek.com>");
MODULE_DESCRIPTION("MT6375 Charger Driver");
MODULE_LICENSE("GPL v2");
