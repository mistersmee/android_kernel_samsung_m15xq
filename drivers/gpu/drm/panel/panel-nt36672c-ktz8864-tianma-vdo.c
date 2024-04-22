// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2022 MediaTek Inc.
 */

#include <linux/backlight.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>
#include <drm/drm_modes.h>
#include <linux/delay.h>
#include <drm/drm_device.h>

#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>

#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>

#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>

#define CONFIG_MTK_PANEL_EXT
#if defined(CONFIG_MTK_PANEL_EXT)
#include "../mediatek/mediatek_v2/mtk_panel_ext.h"
#include "../mediatek/mediatek_v2/mtk_drm_graphics_base.h"
#endif

#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
#include "../mediatek/mtk_corner_pattern/mtk_data_hw_roundedpattern.h"
#endif

int mtk_panel_i2c_write_bytes(unsigned char addr, unsigned char value);

struct lcm {
	struct device *dev;
	struct drm_panel panel;
	struct backlight_device *backlight;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *bias_pos, *bias_neg;

	bool prepared;
	bool enabled;

	int error;
};

#define lcm_dcs_write_seq(ctx, seq...) \
({\
	const u8 d[] = { seq };\
	BUILD_BUG_ON_MSG(ARRAY_SIZE(d) > 64, "DCS sequence too big for stack");\
	lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

#define lcm_dcs_write_seq_static(ctx, seq...) \
({\
	static const u8 d[] = { seq };\
	lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

static inline struct lcm *panel_to_lcm(struct drm_panel *panel)
{
	return container_of(panel, struct lcm, panel);
}

static void lcm_dcs_write(struct lcm *ctx, const void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;
	char *addr;

	if (ctx->error < 0)
		return;

	addr = (char *)data;
	if ((int)*addr < 0xB0)
		ret = mipi_dsi_dcs_write_buffer(dsi, data, len);
	else
		ret = mipi_dsi_generic_write(dsi, data, len);
	if (ret < 0) {
		dev_info(ctx->dev, "error %zd writing seq: %ph\n", ret, data);
		ctx->error = ret;
	}
}

#ifdef PANEL_SUPPORT_READBACK
static int lcm_dcs_read(struct lcm *ctx, u8 cmd, void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;

	if (ctx->error < 0)
		return 0;

	ret = mipi_dsi_dcs_read(dsi, cmd, data, len);
	if (ret < 0) {
		dev_info(ctx->dev, "error %d reading dcs seq:(%#x)\n", ret, cmd);
		ctx->error = ret;
	}

	return ret;
}

static void lcm_panel_get_data(struct lcm *ctx)
{
	u8 buffer[3] = {0};
	static int ret;

	if (ret == 0) {
		ret = lcm_dcs_read(ctx,  0x0A, buffer, 1);
		dev_info(ctx->dev, "return %d data(0x%08x) to dsi engine\n",
			 ret, buffer[0] | (buffer[1] << 8));
	}
}
#endif

static struct regulator *disp_vdd_lcd_1p8;

static int lcm_panel_bias_regulator_init(void)
{
	static int regulator_inited;
	int ret = 0;

	if (regulator_inited)
		return ret;

	/* please only get regulator once in a driver */
	disp_vdd_lcd_1p8 = regulator_get(NULL, "mt6377_vaud18");
	if (IS_ERR(disp_vdd_lcd_1p8)) { /* handle return value */
		ret = PTR_ERR(disp_vdd_lcd_1p8);
		pr_info("get vaud18 fail, error: %d\n", ret);
		return ret;
	}

	/* set voltage with min & max*/
	ret = regulator_set_voltage(disp_vdd_lcd_1p8, 1800000, 1800000);
	if (ret < 0)
		pr_info("set voltage disp_vdd_lcd_1p8 fail, ret = %d\n", ret);

	/* enable regulator */
	ret = regulator_enable(disp_vdd_lcd_1p8);
	if (ret < 0)
		pr_info("enable regulator disp_vdd_lcd_1p8 fail, ret = %d\n", ret);

	regulator_inited = 1;

	return ret; /* must be 0 */

}

static int lcm_panel_bias_enable(void)
{
	int ret = 0;
	int retval = 0;

	/* enable regulator */
	ret = regulator_enable(disp_vdd_lcd_1p8);
	if (ret < 0)
		pr_info("enable regulator disp_vdd_lcd_1p8 fail, ret = %d\n", ret);
	retval |= ret;

	return retval;
}

static int lcm_panel_bias_disable(void)
{
	int ret = 0;
	int retval = 0;

	ret = regulator_disable(disp_vdd_lcd_1p8);
	if (ret < 0)
		pr_info("disable regulator disp_vdd_lcd_1p8 fail, ret = %d\n", ret);
	retval |= ret;

	return retval;
}

static void _ktz8864_init(void)
{
	mtk_panel_i2c_write_bytes(0x0C, 0x2C);
	mtk_panel_i2c_write_bytes(0x0D, 0x26);
	mtk_panel_i2c_write_bytes(0x0E, 0x26);
	mtk_panel_i2c_write_bytes(0x09, 0x99);
	mtk_panel_i2c_write_bytes(0x02, 0x6B);
	mtk_panel_i2c_write_bytes(0x03, 0x0D);
	mtk_panel_i2c_write_bytes(0x11, 0x74);
	mtk_panel_i2c_write_bytes(0x04, 0x05);
	mtk_panel_i2c_write_bytes(0x05, 0xCC);
	mtk_panel_i2c_write_bytes(0x10, 0x67);
	mtk_panel_i2c_write_bytes(0x08, 0x13);

	udelay(2 * 1000);
}

static void lcm_panel_init(struct lcm *ctx)
{
	ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_info(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return;
	}

	gpiod_set_value(ctx->reset_gpio, 0);
	udelay(15 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	udelay(5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 0);
	udelay(5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	udelay(15 * 1000);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x10);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x36, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x3B, 0x03, 0X0C, 0X17, 0X04, 0X04);
	lcm_dcs_write_seq_static(ctx, 0xB0, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xC0, 0x00);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x20);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x01, 0x66);
	lcm_dcs_write_seq_static(ctx, 0x06, 0x64);
	lcm_dcs_write_seq_static(ctx, 0x07, 0x28);
	lcm_dcs_write_seq_static(ctx, 0x17, 0x55);
	lcm_dcs_write_seq_static(ctx, 0x19, 0x55);
	lcm_dcs_write_seq_static(ctx, 0x1B, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x2F, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x5C, 0x90);
	lcm_dcs_write_seq_static(ctx, 0x5E, 0x8B);
	lcm_dcs_write_seq_static(ctx, 0x69, 0xD0);
	lcm_dcs_write_seq_static(ctx, 0xF2, 0x64);
	lcm_dcs_write_seq_static(ctx, 0xF3, 0x44);
	lcm_dcs_write_seq_static(ctx, 0xF4, 0x64);
	lcm_dcs_write_seq_static(ctx, 0xF5, 0x44);
	lcm_dcs_write_seq_static(ctx, 0xF6, 0x64);
	lcm_dcs_write_seq_static(ctx, 0xF7, 0x44);
	lcm_dcs_write_seq_static(ctx, 0xF8, 0x64);
	lcm_dcs_write_seq_static(ctx, 0xF9, 0x44);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x23);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x00, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x07, 0x60);
	lcm_dcs_write_seq_static(ctx, 0x08, 0x06);
	lcm_dcs_write_seq_static(ctx, 0x09, 0x1C);
	lcm_dcs_write_seq_static(ctx, 0x0A, 0x2B);
	lcm_dcs_write_seq_static(ctx, 0x0B, 0x2B);
	lcm_dcs_write_seq_static(ctx, 0x0C, 0x2B);
	lcm_dcs_write_seq_static(ctx, 0x0D, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x10, 0x50);
	lcm_dcs_write_seq_static(ctx, 0x11, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x12, 0x95);
	lcm_dcs_write_seq_static(ctx, 0x15, 0x68);
	lcm_dcs_write_seq_static(ctx, 0x16, 0x0B);
	lcm_dcs_write_seq_static(ctx, 0x19, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x1A, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x1B, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x1C, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x1D, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x1E, 0x03);
	lcm_dcs_write_seq_static(ctx, 0x1F, 0x05);
	lcm_dcs_write_seq_static(ctx, 0x20, 0x0C);
	lcm_dcs_write_seq_static(ctx, 0x21, 0x13);
	lcm_dcs_write_seq_static(ctx, 0x22, 0x17);
	lcm_dcs_write_seq_static(ctx, 0x23, 0x1D);
	lcm_dcs_write_seq_static(ctx, 0x24, 0x23);
	lcm_dcs_write_seq_static(ctx, 0x25, 0x2C);
	lcm_dcs_write_seq_static(ctx, 0x26, 0x33);
	lcm_dcs_write_seq_static(ctx, 0x27, 0x39);
	lcm_dcs_write_seq_static(ctx, 0x28, 0x3F);
	lcm_dcs_write_seq_static(ctx, 0x29, 0x3F);
	lcm_dcs_write_seq_static(ctx, 0x2A, 0x3F);
	lcm_dcs_write_seq_static(ctx, 0x2B, 0x3F);
	lcm_dcs_write_seq_static(ctx, 0x30, 0xFF);
	lcm_dcs_write_seq_static(ctx, 0x31, 0xFE);
	lcm_dcs_write_seq_static(ctx, 0x32, 0xFD);
	lcm_dcs_write_seq_static(ctx, 0x33, 0xFC);
	lcm_dcs_write_seq_static(ctx, 0x34, 0xFB);
	lcm_dcs_write_seq_static(ctx, 0x35, 0xFA);
	lcm_dcs_write_seq_static(ctx, 0x36, 0xF9);
	lcm_dcs_write_seq_static(ctx, 0x37, 0xF7);
	lcm_dcs_write_seq_static(ctx, 0x38, 0xF5);
	lcm_dcs_write_seq_static(ctx, 0x39, 0xF3);
	lcm_dcs_write_seq_static(ctx, 0x3A, 0xF1);
	lcm_dcs_write_seq_static(ctx, 0x3B, 0xEE);
	lcm_dcs_write_seq_static(ctx, 0x3D, 0xEC);
	lcm_dcs_write_seq_static(ctx, 0x3F, 0xEA);
	lcm_dcs_write_seq_static(ctx, 0x40, 0xE8);
	lcm_dcs_write_seq_static(ctx, 0x41, 0xE6);
	lcm_dcs_write_seq_static(ctx, 0x04, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xA0, 0x11);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x24);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x4E, 0x78);
	lcm_dcs_write_seq_static(ctx, 0x4F, 0x78);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x78);
	lcm_dcs_write_seq_static(ctx, 0x77, 0x80);
	lcm_dcs_write_seq_static(ctx, 0x79, 0x03);
	lcm_dcs_write_seq_static(ctx, 0x7A, 0x02);
	lcm_dcs_write_seq_static(ctx, 0x7B, 0xA3);
	lcm_dcs_write_seq_static(ctx, 0x7D, 0x05);
	lcm_dcs_write_seq_static(ctx, 0x80, 0x05);
	lcm_dcs_write_seq_static(ctx, 0x81, 0x05);
	lcm_dcs_write_seq_static(ctx, 0x82, 0x13);
	lcm_dcs_write_seq_static(ctx, 0x84, 0x31);
	lcm_dcs_write_seq_static(ctx, 0x85, 0x13);
	lcm_dcs_write_seq_static(ctx, 0x86, 0x22);
	lcm_dcs_write_seq_static(ctx, 0x87, 0x31);
	lcm_dcs_write_seq_static(ctx, 0x90, 0x13);
	lcm_dcs_write_seq_static(ctx, 0x92, 0x31);
	lcm_dcs_write_seq_static(ctx, 0x93, 0x13);
	lcm_dcs_write_seq_static(ctx, 0x94, 0x22);
	lcm_dcs_write_seq_static(ctx, 0x95, 0x31);
	lcm_dcs_write_seq_static(ctx, 0x9C, 0xF4);
	lcm_dcs_write_seq_static(ctx, 0x9D, 0x01);
	lcm_dcs_write_seq_static(ctx, 0xA0, 0x21);
	lcm_dcs_write_seq_static(ctx, 0xA2, 0x21);
	lcm_dcs_write_seq_static(ctx, 0xA3, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xA4, 0x05);
	lcm_dcs_write_seq_static(ctx, 0xA5, 0x05);
	lcm_dcs_write_seq_static(ctx, 0xC4, 0x40);
	lcm_dcs_write_seq_static(ctx, 0xC6, 0x40);
	lcm_dcs_write_seq_static(ctx, 0xC9, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xD9, 0x80);
	lcm_dcs_write_seq_static(ctx, 0xE9, 0x02);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x25);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x0F, 0x1B);
	lcm_dcs_write_seq_static(ctx, 0x18, 0x20);
	lcm_dcs_write_seq_static(ctx, 0x19, 0xE4);
	lcm_dcs_write_seq_static(ctx, 0x21, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x66, 0x40);
	lcm_dcs_write_seq_static(ctx, 0x67, 0x29);
	lcm_dcs_write_seq_static(ctx, 0x68, 0x50);
	lcm_dcs_write_seq_static(ctx, 0x69, 0x20);
	lcm_dcs_write_seq_static(ctx, 0x6B, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x71, 0x6D);
	lcm_dcs_write_seq_static(ctx, 0x77, 0x60);
	lcm_dcs_write_seq_static(ctx, 0x78, 0xA5);
	lcm_dcs_write_seq_static(ctx, 0x7D, 0x40);
	lcm_dcs_write_seq_static(ctx, 0x7E, 0x2D);
	lcm_dcs_write_seq_static(ctx, 0x84, 0x70);
	lcm_dcs_write_seq_static(ctx, 0xC0, 0x4D);
	lcm_dcs_write_seq_static(ctx, 0xC1, 0xA9);
	lcm_dcs_write_seq_static(ctx, 0xC2, 0xD2);
	lcm_dcs_write_seq_static(ctx, 0xC4, 0x11);
	lcm_dcs_write_seq_static(ctx, 0xD6, 0x80);
	lcm_dcs_write_seq_static(ctx, 0xD7, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xDA, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xDD, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xE0, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xF0, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xF1, 0x04);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x26);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x00, 0x10);
	lcm_dcs_write_seq_static(ctx, 0x01, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0x03, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x04, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0x05, 0x08);
	lcm_dcs_write_seq_static(ctx, 0x06, 0x21);
	lcm_dcs_write_seq_static(ctx, 0x08, 0x21);
	lcm_dcs_write_seq_static(ctx, 0x14, 0x06);
	lcm_dcs_write_seq_static(ctx, 0x15, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x74, 0xAF);
	lcm_dcs_write_seq_static(ctx, 0x81, 0x23);
	lcm_dcs_write_seq_static(ctx, 0x83, 0x02);
	lcm_dcs_write_seq_static(ctx, 0x84, 0x04);
	lcm_dcs_write_seq_static(ctx, 0x85, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x86, 0x04);
	lcm_dcs_write_seq_static(ctx, 0x87, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x88, 0x08);
	lcm_dcs_write_seq_static(ctx, 0x8A, 0x1A);
	lcm_dcs_write_seq_static(ctx, 0x8B, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x8C, 0x24);
	lcm_dcs_write_seq_static(ctx, 0x8E, 0x42);
	lcm_dcs_write_seq_static(ctx, 0x8F, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x90, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x91, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x9A, 0x80);
	lcm_dcs_write_seq_static(ctx, 0x9B, 0x04);
	lcm_dcs_write_seq_static(ctx, 0x9C, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x9D, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x9E, 0x00);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x27);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x01, 0x68);
	lcm_dcs_write_seq_static(ctx, 0x20, 0x82);
	lcm_dcs_write_seq_static(ctx, 0x21, 0xE7);
	lcm_dcs_write_seq_static(ctx, 0x25, 0x83);
	lcm_dcs_write_seq_static(ctx, 0x26, 0x37);
	lcm_dcs_write_seq_static(ctx, 0x6E, 0x23);
	lcm_dcs_write_seq_static(ctx, 0x6F, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x70, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x71, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x72, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x73, 0x21);
	lcm_dcs_write_seq_static(ctx, 0x74, 0x03);
	lcm_dcs_write_seq_static(ctx, 0x75, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x76, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x77, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x7D, 0x09);
	lcm_dcs_write_seq_static(ctx, 0x7E, 0x6B);
	lcm_dcs_write_seq_static(ctx, 0x80, 0x23);
	lcm_dcs_write_seq_static(ctx, 0x82, 0x09);
	lcm_dcs_write_seq_static(ctx, 0x83, 0x6B);
	lcm_dcs_write_seq_static(ctx, 0x88, 0x03);
	lcm_dcs_write_seq_static(ctx, 0x89, 0x01);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x2A);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x00, 0x91);
	lcm_dcs_write_seq_static(ctx, 0x03, 0x20);
	lcm_dcs_write_seq_static(ctx, 0x07, 0x64);
	lcm_dcs_write_seq_static(ctx, 0x0A, 0x70);
	lcm_dcs_write_seq_static(ctx, 0x0C, 0x09);
	lcm_dcs_write_seq_static(ctx, 0x0D, 0x40);
	lcm_dcs_write_seq_static(ctx, 0x0E, 0x02);
	lcm_dcs_write_seq_static(ctx, 0x0F, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x11, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0x15, 0x0F);
	lcm_dcs_write_seq_static(ctx, 0x16, 0x0A);
	lcm_dcs_write_seq_static(ctx, 0x19, 0x0E);
	lcm_dcs_write_seq_static(ctx, 0x1A, 0xDE);
	lcm_dcs_write_seq_static(ctx, 0x1B, 0x0C);
	lcm_dcs_write_seq_static(ctx, 0x1D, 0x17);
	lcm_dcs_write_seq_static(ctx, 0x1E, 0x81);
	lcm_dcs_write_seq_static(ctx, 0x1F, 0x81);
	lcm_dcs_write_seq_static(ctx, 0x20, 0x81);
	lcm_dcs_write_seq_static(ctx, 0x27, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x28, 0x08);
	lcm_dcs_write_seq_static(ctx, 0x29, 0x1C);
	lcm_dcs_write_seq_static(ctx, 0x2A, 0x4C);
	lcm_dcs_write_seq_static(ctx, 0x2B, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x2D, 0x09);
	lcm_dcs_write_seq_static(ctx, 0x2F, 0x03);
	lcm_dcs_write_seq_static(ctx, 0x30, 0x1E);
	lcm_dcs_write_seq_static(ctx, 0x31, 0x40);
	lcm_dcs_write_seq_static(ctx, 0x33, 0x2E);
	lcm_dcs_write_seq_static(ctx, 0x34, 0x74);
	lcm_dcs_write_seq_static(ctx, 0x35, 0x28);
	lcm_dcs_write_seq_static(ctx, 0x36, 0xC8);
	lcm_dcs_write_seq_static(ctx, 0x37, 0x72);
	lcm_dcs_write_seq_static(ctx, 0x38, 0x28);
	lcm_dcs_write_seq_static(ctx, 0x39, 0xC8);
	lcm_dcs_write_seq_static(ctx, 0x3A, 0x1E);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0xE0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x35, 0x82);
	lcm_dcs_write_seq_static(ctx, 0x85, 0x30);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x1C, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x33, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x5A, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xD2, 0x50);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0xC0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x9C, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x9D, 0x11);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0xD0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x25, 0xA9);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x22);
	lcm_dcs_write_seq_static(ctx, 0x54, 0x02);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0x10);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x2C);
	lcm_dcs_write_seq_static(ctx, 0x55, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x68, 0x00, 0x01);

	lcm_dcs_write_seq_static(ctx, 0x11, 0x00);	/* sleep_out */
	msleep(150);
	lcm_dcs_write_seq_static(ctx, 0x29, 0x00);	/* display_on */
	msleep(50);
	lcm_dcs_write_seq_static(ctx, 0x51, 0xFF);	/* display_on */
}

static int lcm_disable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	if (!ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = false;

	return 0;
}

static int lcm_unprepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	if (!ctx->prepared)
		return 0;

	lcm_dcs_write_seq_static(ctx, 0x28);
	msleep(50);
	lcm_dcs_write_seq_static(ctx, 0x10);
	msleep(120);

	ctx->error = 0;
	ctx->prepared = false;
	ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_info(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	/* BL IC OFF */
	// 0x09 (LCD off)	(@0x09) 0x9C -> 1ms -> 0x18 -> 1ms -> (@0x08) 0x00

	ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
		"bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_info(ctx->dev, "%s: cannot get bias_neg %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	gpiod_set_value(ctx->bias_neg, 0);
	devm_gpiod_put(ctx->dev, ctx->bias_neg);

	udelay(1000);

	ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
		"bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_info(ctx->dev, "%s: cannot get bias_pos %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	gpiod_set_value(ctx->bias_pos, 0);
	devm_gpiod_put(ctx->dev, ctx->bias_pos);

	lcm_panel_bias_disable();

	return 0;
}

static int lcm_prepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	int ret;

	if (ctx->prepared)
		return 0;

	lcm_panel_bias_enable();

	ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
		"bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_info(ctx->dev, "%s: cannot get bias_pos %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	gpiod_set_value(ctx->bias_pos, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_pos);

	udelay(2000);

	ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
		"bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_info(ctx->dev, "%s: cannot get bias_neg %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	gpiod_set_value(ctx->bias_neg, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_neg);

	_ktz8864_init();

	lcm_panel_init(ctx);

	ret = ctx->error;
	if (ret < 0)
		lcm_unprepare(panel);

	ctx->prepared = true;

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_rst(panel);
#endif
#ifdef PANEL_SUPPORT_READBACK
	lcm_panel_get_data(ctx);
#endif

	return ret;
}

static int lcm_enable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	if (ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = true;

	return 0;
}

#define HFP (86)
#define HSA (10)
#define HBP (26)
#define VFP (23)
#define VSA (2)
#define VBP (10)
#define VAC (2408)
#define HAC (1080)
static const struct drm_display_mode default_mode = {
	.clock = 176189,
	.hdisplay = HAC,
	.hsync_start = HAC + HFP,
	.hsync_end = HAC + HFP + HSA,
	.htotal = HAC + HFP + HSA + HBP,//1202
	.vdisplay = VAC,
	.vsync_start = VAC + VFP,
	.vsync_end = VAC + VFP + VSA,
	.vtotal = VAC + VFP + VSA + VBP,//2443
};

#if defined(CONFIG_MTK_PANEL_EXT)
static int panel_ext_reset(struct drm_panel *panel, int on)
{
	struct lcm *ctx = panel_to_lcm(panel);

	ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_info(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, on);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	return 0;
}

static int panel_ata_check(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	unsigned char data[3] = {0x00, 0x00, 0x00};
	unsigned char id[3] = {0x00, 0x80, 0x00};
	ssize_t ret;

	ret = mipi_dsi_dcs_read(dsi, 0x4, data, 3);
	if (ret < 0) {
		pr_info("%s error\n", __func__);
		return 0;
	}

	pr_info("ATA read data %x %x %x\n", data[0], data[1], data[2]);

	if (data[0] == id[0] &&
			data[1] == id[1] &&
			data[2] == id[2])
		return 1;

	pr_info("ATA expect read data is %x %x %x\n",
			id[0], id[1], id[2]);

	return 0;
}

static int lcm_setbacklight_cmdq(void *dsi, dcs_write_gce cb,
	void *handle, unsigned int level)
{
	char bl_tb0[] = {0x51, 0xFF};

	bl_tb0[1] = level;

	if (!cb)
		return -1;

	cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));

	return 0;
}

static struct mtk_panel_params ext_params = {
	.pll_clk = 562,
	.vfp_low_power = 1294,
	.cust_esd_check = 0,
	.esd_check_enable = 0,
	.lcm_esd_check_table[0] = {
		.cmd = 0x0a,
		.count = 1,
		.para_list[0] = 0x9c,
	},
	.vdo_per_frame_lp_enable = 1,
};

static struct mtk_panel_funcs ext_funcs = {
	.reset = panel_ext_reset,
	.set_backlight_cmdq = lcm_setbacklight_cmdq,
	.ata_check = panel_ata_check,
};
#endif

struct panel_desc {
	const struct drm_display_mode *modes;
	unsigned int num_modes;

	unsigned int bpc;

	struct {
		unsigned int width;
		unsigned int height;
	} size;

	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
	} delay;
};

static int lcm_get_modes(struct drm_panel *panel, struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &default_mode);
	if (!mode) {
		dev_info(connector->dev->dev, "failed to add mode %ux%ux@%u\n",
			default_mode.hdisplay, default_mode.vdisplay,
			drm_mode_vrefresh(&default_mode));
		return -ENOMEM;
	}

	drm_mode_set_name(mode);
	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(connector, mode);

	connector->display_info.width_mm = 64;
	connector->display_info.height_mm = 129;

	return 1;
}

static const struct drm_panel_funcs lcm_drm_funcs = {
	.disable = lcm_disable,
	.unprepare = lcm_unprepare,
	.prepare = lcm_prepare,
	.enable = lcm_enable,
	.get_modes = lcm_get_modes,
};

static int lcm_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct lcm *ctx;
	struct device_node *backlight;
	int ret;
	struct device_node *dsi_node, *remote_node = NULL, *endpoint = NULL;

	dsi_node = of_get_parent(dev->of_node);
	if (dsi_node) {
		endpoint = of_graph_get_next_endpoint(dsi_node, NULL);
		if (endpoint) {
			remote_node = of_graph_get_remote_port_parent(endpoint);
			if (!remote_node) {
				pr_info("No panel connected,skip probe lcm\n");
				return -ENODEV;
			}
			pr_info("device node name:%s\n", remote_node->name);
		}
	}
	if (remote_node != dev->of_node) {
		pr_info("%s+ skip probe due to not current lcm\n", __func__);
		return -ENODEV;
	}

	ctx = devm_kzalloc(dev, sizeof(struct lcm), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	mipi_dsi_set_drvdata(dsi, ctx);

	ctx->dev = dev;
	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO
			 | MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_NO_EOT_PACKET
			 | MIPI_DSI_CLOCK_NON_CONTINUOUS;

	backlight = of_parse_phandle(dev->of_node, "backlight", 0);
	if (backlight) {
		ctx->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);

		if (!ctx->backlight)
			return -EPROBE_DEFER;
	}

	lcm_panel_bias_regulator_init();

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_info(dev, "%s: cannot get reset-gpios %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	devm_gpiod_put(dev, ctx->reset_gpio);

	ctx->bias_pos = devm_gpiod_get_index(dev, "bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_info(dev, "%s: cannot get bias-pos 0 %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	devm_gpiod_put(dev, ctx->bias_pos);

	ctx->bias_neg = devm_gpiod_get_index(dev, "bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_info(dev, "%s: cannot get bias-neg 1 %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	devm_gpiod_put(dev, ctx->bias_neg);

	ctx->prepared = true;
	ctx->enabled = true;

	drm_panel_init(&ctx->panel, dev, &lcm_drm_funcs, DRM_MODE_CONNECTOR_DSI);
	ctx->panel.dev = dev;
	ctx->panel.funcs = &lcm_drm_funcs;

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0)
		drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_handle_reg(&ctx->panel);
	ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);
	if (ret < 0)
		return ret;
#endif

	pr_info("%s-\n", __func__);

	return ret;
}

static int lcm_remove(struct mipi_dsi_device *dsi)
{
	struct lcm *ctx = mipi_dsi_get_drvdata(dsi);

	mipi_dsi_detach(dsi);
	drm_panel_remove(&ctx->panel);

	return 0;
}

static const struct of_device_id lcm_of_match[] = {
	{ .compatible = "nt36672c,ktz8864,tianma,vdo", },
	{ }
};

MODULE_DEVICE_TABLE(of, lcm_of_match);

static struct mipi_dsi_driver lcm_driver = {
	.probe = lcm_probe,
	.remove = lcm_remove,
	.driver = {
		.name = "panel-nt36672c-ktz8864-tianma-vdo",
		.owner = THIS_MODULE,
		.of_match_table = lcm_of_match,
	},
};

module_mipi_dsi_driver(lcm_driver);

MODULE_AUTHOR("Jenny Kim <Jenny.Kim@mediatek.com>");
MODULE_DESCRIPTION("nt36672c FHDP VDO LCD TIANMA Panel Driver");
MODULE_LICENSE("GPL v2");


