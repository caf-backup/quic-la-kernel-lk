/* Copyright (c) 2020, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PANEL_AUO_416P_CMD_H_
#define _PANEL_AUO_416P_CMD_H_
/*---------------------------------------------------------------------------*/
/* HEADER files                                                              */
/*---------------------------------------------------------------------------*/
#include "panel.h"

/*---------------------------------------------------------------------------*/
/* Panel configuration                                                       */
/*---------------------------------------------------------------------------*/

static struct panel_config auo_416p_cmd_panel_data = {
	"qcom,dsi_auo_416p_amoled_cmd", "dsi:0:", "qcom,mdss-dsi-panel",
	10, 1, "DISPLAY_1", 0, 0, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL
};

/*---------------------------------------------------------------------------*/
/* Panel resolution                                                          */
/*---------------------------------------------------------------------------*/
static struct panel_resolution auo_416p_cmd_panel_res = {
	416, 416, 140, 164, 8, 0, 6, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel color information                                                   */
/*---------------------------------------------------------------------------*/
static struct color_info auo_416p_cmd_color = {
	24, 0, 0xff, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel on/off command information                                          */
/*---------------------------------------------------------------------------*/
static char auo_416p_cmd_on_cmd0[] = {
	0xFE, 0x01, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd1[] = {
	0x0A, 0xF0, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd2[] = {
	0xFE, 0x00, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd3[] = {
	0x05, 0x00, 0x29, 0xC0,
	0x2A, 0x00, 0x1E, 0x01,
	0xBD, 0xFF, 0xFF, 0xFF,
};

static char auo_416p_cmd_on_cmd4[] = {
	0x05, 0x00, 0x29, 0xC0,
	0x2B, 0x00, 0x00, 0x01,
	0x9F, 0xFF, 0xFF, 0xFF,
};

static char auo_416p_cmd_on_cmd5[] = {
	0x05, 0x00, 0x29, 0xC0,
	0x30, 0x00, 0x00, 0x01,
	0x9F, 0xFF, 0xFF, 0xFF,
};

static char auo_416p_cmd_on_cmd6[] = {
	0x05, 0x00, 0x29, 0xC0,
	0x31, 0x00, 0x1E, 0x01,
	0xBD, 0xFF, 0xFF, 0xFF,
};

static char auo_416p_cmd_on_cmd7[] = {
	0x12, 0x00, 0x05, 0x80,
};

static char auo_416p_cmd_on_cmd8[] = {
	0x35, 0x02, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd9[] = {
	0x53, 0x20, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd10[] = {
	0x51, 0xFF, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd11[] = {
	0x66, 0x00, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd12[] = {
	0x63, 0xFF, 0x15, 0x80,
};

static char auo_416p_cmd_on_cmd13[] = {
	0x11, 0x00, 0x05, 0x80,
};

static char auo_416p_cmd_on_cmd14[] = {
	0x29, 0x00, 0x05, 0x80,
};

static struct mipi_dsi_cmd auo_416p_cmd_on_command[] = {
	{0x4, auo_416p_cmd_on_cmd0, 0x00},
	{0x4, auo_416p_cmd_on_cmd1, 0x00},
	{0x4, auo_416p_cmd_on_cmd2, 0x00},
	{0xc, auo_416p_cmd_on_cmd3, 0x00},
	{0xc, auo_416p_cmd_on_cmd4, 0x00},
	{0xc, auo_416p_cmd_on_cmd5, 0x00},
	{0xc, auo_416p_cmd_on_cmd6, 0x00},
	{0x4, auo_416p_cmd_on_cmd7, 0x00},
	{0x4, auo_416p_cmd_on_cmd8, 0x00},
	{0x4, auo_416p_cmd_on_cmd9, 0x00},
	{0x4, auo_416p_cmd_on_cmd10, 0x00},
	{0x4, auo_416p_cmd_on_cmd11, 0x28},
	{0x4, auo_416p_cmd_on_cmd12, 0x00},
	{0x4, auo_416p_cmd_on_cmd13, 0x32},
	{0x4, auo_416p_cmd_on_cmd14, 0x1E}
};

#define AUO_416P_CMD_ON_COMMAND 15


static char auo_416p_cmdoff_cmd0[] = {
	0x28, 0x00, 0x05, 0x80
};

static char auo_416p_cmdoff_cmd1[] = {
	0x10, 0x00, 0x05, 0x80
};

static struct mipi_dsi_cmd auo_416p_cmd_off_command[] = {
	{0x4, auo_416p_cmdoff_cmd0, 0x00},
	{0x4, auo_416p_cmdoff_cmd1, 0x78}
};

#define AUO_416P_CMD_OFF_COMMAND 2

static struct command_state auo_416p_cmd_state = {
	0, 1
};

/*---------------------------------------------------------------------------*/
/* Command mode panel information                                            */
/*---------------------------------------------------------------------------*/
static struct commandpanel_info auo_416p_cmd_command_panel = {
	1, 1, 1, 1, 2, 0x2c, 0, 0, 0, 1, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Video mode panel information                                              */
/*---------------------------------------------------------------------------*/
static struct videopanel_info auo_416p_cmd_video_panel = {
	1, 0, 0, 0, 1, 1, 2, 0, 0x9
};

/*---------------------------------------------------------------------------*/
/* Lane configuration                                                        */
/*---------------------------------------------------------------------------*/
static struct lane_configuration auo_416p_cmd_lane_config = {
	1, 0, 1, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel timing                                                              */
/*---------------------------------------------------------------------------*/
static const uint32_t auo_416p_cmd_12nm_timings[] = {
	0x06, 0x05, 0x01, 0x0A, 0x00, 0x03, 0x01, 0x0F
};

static struct panel_timing auo_416p_cmd_timing_info = {
	0, 4, 0x05, 0x12
};

/*---------------------------------------------------------------------------*/
/* Panel reset sequence                                                      */
/*---------------------------------------------------------------------------*/
static struct panel_reset_sequence auo_416p_cmd_reset_seq = {
	{1, 0, 1, }, {20, 20, 20, }, 2
};

/*---------------------------------------------------------------------------*/
/* Backlight setting                                                         */
/*---------------------------------------------------------------------------*/
static struct backlight auo_416p_cmd_backlight = {
	2, 1, 255, 100, 2, "BL_CTL_DCS"
};

#endif /*_PANEL_AUO_416P_CMD_H_*/