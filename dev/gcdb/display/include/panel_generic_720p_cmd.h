/* Copyright (c) 2013,2015, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*---------------------------------------------------------------------------
 * This file is autogenerated file using gcdb parser. Please do not edit it.
 * Update input XML file to add a new entry or update variable in this file
 * VERSION = "1.0"
 *---------------------------------------------------------------------------*/

#ifndef _PANEL_GENERIC_720P_CMD_H_

#define _PANEL_GENERIC_720P_CMD_H_
/*---------------------------------------------------------------------------*/
/* HEADER files                                                              */
/*---------------------------------------------------------------------------*/
#include "panel.h"

/*---------------------------------------------------------------------------*/
/* Panel configuration                                                       */
/*---------------------------------------------------------------------------*/

static struct panel_config generic_720p_cmd_panel_data = {
  "qcom,mdss_dsi_generic_720p_cmd", "dsi:0:", "qcom,mdss-dsi-panel",
  10, 1, "DISPLAY_1", 0, 0, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel resolution                                                          */
/*---------------------------------------------------------------------------*/
static struct panel_resolution generic_720p_cmd_panel_res = {
  768, 1280, 26, 26, 26, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel Color Information                                                   */
/*---------------------------------------------------------------------------*/
static struct color_info generic_720p_cmd_color = {
  24, 0, 0xff, 0, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Panel Command information                                                 */
/*---------------------------------------------------------------------------*/
static char generic_720p_cmd_on_cmd0[] = {
0x11, 0x00, 0x05, 0x80 };


static char generic_720p_cmd_on_cmd1[] = {
0x36, 0x00, 0x15, 0x80 };


static char generic_720p_cmd_on_cmd2[] = {
0x29, 0x00, 0x05, 0x80 };


static char generic_720p_cmd_on_cmd3[] = {
0x53, 0x24, 0x15, 0x80 };


static char generic_720p_cmd_on_cmd4[] = {
0x35, 0x00, 0x15, 0x80 };




static struct mipi_dsi_cmd generic_720p_cmd_on_command[] = {
{ 0x4 , generic_720p_cmd_on_cmd0},
{ 0x4 , generic_720p_cmd_on_cmd1},
{ 0x4 , generic_720p_cmd_on_cmd2},
{ 0x4 , generic_720p_cmd_on_cmd3},
{ 0x4 , generic_720p_cmd_on_cmd4}
};
#define GENERIC_720P_CMD_ON_COMMAND 5


static char generic_720p_cmdoff_cmd0[] = {
0x34, 0x00, 0x05, 0x80 };


static char generic_720p_cmdoff_cmd1[] = {
0x10, 0x00, 0x05, 0x80 };


static char generic_720p_cmdoff_cmd2[] = {
0x28, 0x00, 0x05, 0x80 };


static char generic_720p_cmdoff_cmd3[] = {
0x53, 0x00, 0x15, 0x80 };




static struct mipi_dsi_cmd generic_720p_cmd_off_command[] = {
{ 0x4 , generic_720p_cmdoff_cmd0},
{ 0x4 , generic_720p_cmdoff_cmd1},
{ 0x4 , generic_720p_cmdoff_cmd2},
{ 0x4 , generic_720p_cmdoff_cmd3}
};
#define GENERIC_720P_CMD_OFF_COMMAND 4


static struct command_state generic_720p_cmd_state = {
  0, 1
};

/*---------------------------------------------------------------------------*/
/* Command mode panel information                                            */
/*---------------------------------------------------------------------------*/

static struct commandpanel_info generic_720p_cmd_command_panel = {
  1, 1, 1, 0, 0, 0x2c, 0, 0, 0, 1, 0, 0
};

/*---------------------------------------------------------------------------*/
/* Video mode panel information                                              */
/*---------------------------------------------------------------------------*/

static struct videopanel_info generic_720p_cmd_video_panel = {
  0, 0, 0, 0, 1, 1, 1, 0, 0x9
};

/*---------------------------------------------------------------------------*/
/* Lane Configuration                                                        */
/*---------------------------------------------------------------------------*/

static struct lane_configuration generic_720p_cmd_lane_config = {
  4, 0, 1, 1, 1, 1, 0
};


/*---------------------------------------------------------------------------*/
/* Panel Timing                                                              */
/*---------------------------------------------------------------------------*/
static const uint32_t generic_720p_cmd_timings[] = {
  0x6e, 0x26, 0x1b, 0x00, 0x35, 0x34, 0x20, 0x28, 0x17, 0x03, 0x04, 0x00
};

static struct panel_timing generic_720p_cmd_timing_info = {
  0, 4, 0x20, 0x2a
};

static struct panel_reset_sequence generic_720p_cmd_reset_seq = {
 { 1, 0, 1, }, { 20, 2, 20, }, 2
};


/*---------------------------------------------------------------------------*/
/* Backlight Settings                                                        */
/*---------------------------------------------------------------------------*/

static struct backlight generic_720p_cmd_backlight = {
  1, 1, 4095, 100, 1, "PMIC_8941"
};

#define GENERIC_720P_CMD_SIGNATURE 0x210000

#endif /*_PANEL_GENERIC_720P_CMD_H_*/
