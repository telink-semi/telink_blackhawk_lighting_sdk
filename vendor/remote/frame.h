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

typedef struct{
	unsigned int   dma_len;				// 0~3 DMA length
	unsigned char  rf_len;					// 4 rf data length = 0x10
	unsigned char  rf_len1;
	unsigned short vid;					// 5~6 vendor ID
	unsigned int   pid;					// 7~10 product ID

	unsigned char  control_key;			// 11 function control key
	unsigned char  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

	unsigned short button_keep_counter;	// 13~14 sequence number in one certain channel.
	unsigned short control_key_value[3];	// 15, 16, 17, 18
	unsigned char  reserved;
}rf_packet_led_remote_t;		//rf data packet from remoter end.

rf_packet_led_remote_t  led_remote __attribute__((aligned(4)));
