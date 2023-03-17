/********************************************************************************************************
 * @file	frame.h
 *
 * @brief	This is the header file for TLSR8231
 *
 * @author	Telink
 * @date	May 12, 2019
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#define MAX_PAIRED_REMOTER  8
#define RX_PACKAGE_SIZE     64
unsigned char rx_packet[RX_PACKAGE_SIZE*2] __attribute__((aligned(4)));

#define REMOTE_ID_ADDR         0
#define PAIRE_INDEX_ADDR       MAX_PAIRED_REMOTER*4
#define LUMI_INDEX_ADDR        PAIRE_INDEX_ADDR+1
#define CHROMA_INDEX_ADDR      LUMI_INDEX_ADDR+1
#define LED_STATE_ADDR         CHROMA_INDEX_ADDR+1
#define LED_RED_VALUE_ADDR     LED_STATE_ADDR+1
#define LED_GREEN_VALUE_ADDR   LED_RED_VALUE_ADDR+2
#define LED_BLUE_VALUE_ADDR    LED_GREEN_VALUE_ADDR+2



unsigned char  g_package_new;
unsigned char  g_package_cmd;
unsigned int   g_package_pid;
unsigned char  g_package_seq;
unsigned short g_package_lumi;
unsigned short g_package_chroma;
unsigned short g_package_red;
unsigned short g_package_green;
unsigned short g_package_blue;

typedef struct{
	unsigned int   dma_len;
	unsigned char  rf_len;
	unsigned char  rf_len1;
	unsigned short vid;
	unsigned int   pid;
	unsigned char  pkt_seq;
	unsigned char  key_control;
	unsigned short value[3];
}LED_Package_t;

typedef struct{
	unsigned int   remote_id[MAX_PAIRED_REMOTER];
	unsigned char  paire_index;
	unsigned char  luminance_index;//亮度下标
	unsigned char  chroma_index;//色温下标
	unsigned char  led_state;
	unsigned short rgb_value[3];
}LED_Control_Info_t;

LED_Control_Info_t led_control __attribute__((aligned(4)));
