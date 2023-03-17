/********************************************************************************************************
 * @file	pairing_op.c
 *
 * @brief	This is the source file for TLSR8231
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
//#include "../../common.h"
#include "../../drivers.h"
#include "../../user_drivers.h"
#include "frame.h"
/***********************************************************
 * 函数功能：把ID值写入eeprom
 * 参       数：Position   ID所在的下标值
 *        Id         写入的id值
 * 返 回  值：
 **********************************************************/
void write_id_direct(unsigned char Position,unsigned int Id)
{
	if(Position<MAX_PAIRED_REMOTER){
		fm24c02_write_func(REMOTE_ID_ADDR+Position*4,Id);
		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+1,Id>>8);
		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+2,Id>>16);
		fm24c02_write_func(REMOTE_ID_ADDR+Position*4+3,Id>>24);
	}
}
/***********************************************************
 * 函数功能：把数据写入到eeprom
 * 参       数：Addr     eeprom的地址
 *       Data      写入的数据值
 * 返 回  值：
 **********************************************************/
void write_data_direct_func(unsigned char Addr,unsigned char Data)
{
	fm24c02_write_func(Addr,Data);
}
/***********************************************************
 * 函数功能：保存遥控器ID
 * 参       数：Id    保存的Id值
 * 返 回  值：成功返回1  不成功返回0
 **********************************************************/
unsigned char save_remote_ID_func(unsigned int Id)
{
	unsigned char i;
	for(i=0;i<MAX_PAIRED_REMOTER;i++){
		if(Id==led_control.remote_id[i])
			return 1;
	}
	write_id_direct(led_control.paire_index,Id);
	led_control.remote_id[led_control.paire_index]=Id;
	led_control.paire_index++;
	if(led_control.paire_index>=MAX_PAIRED_REMOTER)
		led_control.paire_index=0;
	write_data_direct_func(PAIRE_INDEX_ADDR,led_control.paire_index);
	return 1;
}
/***********************************************************
 * 函数功能：清除保存的遥控器ID值
 * 参       数：
 * 返 回  值：
 **********************************************************/
void clear_remote_ID_func(void)
{
	unsigned char i;
	for(i=0;i<MAX_PAIRED_REMOTER;i++){
		led_control.remote_id[i]=0x00000000;//0x00000000
		write_id_direct(i,0x00000000);//0x00000000
	}
	led_control.paire_index=0;
	write_data_direct_func(PAIRE_INDEX_ADDR,led_control.paire_index);
}
/***********************************************************
 * 函数功能：查询ID是否跟已保存的id匹配
 * 参       数：
 * 返 回  值：匹配成功返回1  不成功返回0
 **********************************************************/
unsigned char paired_ID_match(unsigned int Id)
{
	unsigned char i;
	for(i=0;i<MAX_PAIRED_REMOTER;i++){
		if(Id==led_control.remote_id[i])
			return 1;
	}
	return 0;
}
/***********************************************************
 * 函数功能：保存灯的控制信息
 * 参       数：
 * 返 回  值：
 **********************************************************/
void save_led_control_info_func(void)
{
	unsigned char *Ptr=(void *)&led_control;
	unsigned char i;
	for(i=0;i<sizeof(LED_Control_Info_t);i++){
		fm24c02_write_func(i,*Ptr++);
	}
}
/***********************************************************
 * 函数功能：保存灯的状态信息
 * 参       数：
 * 返 回  值：
 **********************************************************/
void save_led_state_info_func(void)
{
	unsigned char *Ptr=(void *)&led_control.luminance_index;
	unsigned char i;
	unsigned char Len=sizeof(LED_Control_Info_t)-MAX_PAIRED_REMOTER-1;

	for(i=0;i<Len;i++){
		fm24c02_write_func(LUMI_INDEX_ADDR+i,*Ptr++);
	}
}
