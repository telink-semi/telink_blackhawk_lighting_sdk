/********************************************************************************************************
 * @file	pairing_op.h
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
#include "frame.h"


unsigned char paired_ID_match(unsigned int pid,unsigned char grp);
void clear_pared_code_func(void);
void pair_id_save_func(void);

#define EEPROM_INFO_START            0
#define EEPROM_PAIR_ID_ADDR          EEPROM_INFO_START
#define EEPROM_ID_INDEX_ADDR         EEPROM_PAIR_ID_ADDR+MAX_PAIRED_REMOTER*5
#define EEPROM_LUMINACE_INDEX_ADDR   EEPROM_ID_INDEX_ADDR+1
#define EEPROM_CHROMA_INDEX_ADDR     EEPROM_LUMINACE_INDEX_ADDR+1
#define EEPROM_LED_ON_ADDR           EEPROM_CHROMA_INDEX_ADDR+1
#define EEPROM_POWER_ON_RECOVER_ADDR EEPROM_LED_ON_ADDR+1
#define EEPROM_SEG_INDEX_ADDR        EEPROM_POWER_ON_RECOVER_ADDR+1
