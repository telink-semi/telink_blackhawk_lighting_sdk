/********************************************************************************************************
 * @file	app.c
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
#include "app_config.h"
#include "../../drivers.h"
#include "frame.h"
#include "rf_control.h"
#include "key_def.h"


#define  DEBUG          0
#define  LED_PIN        GPIO_SWSC7
#define  LED_OFF        gpio_write(LED_PIN,0)
#define  LED_ON         gpio_write(LED_PIN,1)
//const unsigned int gpio_row[]={GPIO_PC4,GPIO_PC5,GPIO_PC1,GPIO_PB2};//矩阵行的IO
//const unsigned int gpio_column[]={GPIO_PB1,GPIO_PB3,GPIO_PB4,GPIO_PB5};//矩阵列的IO
const unsigned int gpio_row[]={GPIO_PC2,GPIO_PC3,GPIO_PC1,GPIO_PB0,GPIO_PD2};//矩阵行的IO
const unsigned int gpio_column[]={GPIO_PB3,GPIO_PB4,GPIO_PB5};//矩阵列的IO
unsigned char pre_key;//记录上次按键按下的值
unsigned char current_active_group;//色温亮度调试时的组别
unsigned char key_down_cnt;//按键按下计数值
unsigned char key_up_cnt;//按键弹起计数值
unsigned char key_off_cnt;//关灯健按下的计数值，短按为开灯，长按为夜灯
unsigned char key_lumi_chro_cnt;//色温亮度按键按下的计数值，一直按下则色温值不断增加
unsigned char led_night_cmd_flag;//夜灯命令执行标志
unsigned int loop;
#if 1
const unsigned char key_table[5][3] = {//按键表格
		{((KEY_LUMINANT_DECREASE<<4)|GROUP_ALL),((KEY_OFF<<4)|GROUP_2),((KEY_ON<<4)|GROUP_2)},
		{((KEY_CHROMA_DECREASE<<4)|GROUP_ALL),((KEY_OFF<<4)|GROUP_1),((KEY_ON<<4)|GROUP_1)},
		{((KEY_ON<<4)|GROUP_3),((KEY_OFF<<4)|GROUP_3),((KEY_OFF<<4)|GROUP_4)},
		{KEY_NONE,((KEY_ON<<4)|GROUP_4),((KEY_CHROMA_INCREASE<<4)|GROUP_ALL)},
		{((KEY_OFF<<4)|GROUP_ALL),((KEY_LUMINANT_INCREASE<<4)|GROUP_ALL),((KEY_ON<<4)|GROUP_ALL)}
};
#else
const unsigned char key_table[4][4] = {//按键表格
		{((KEY_ON<<4)|GROUP_1),                   ((KEY_LUMINANT_DECREASE<<4)|GROUP_ALL), ((KEY_ON<<4)|GROUP_2),                ((KEY_ON<<4)|GROUP_3)  },
		{((KEY_OFF<<4)|GROUP_1),                  0x15,                                   ((KEY_OFF<<4)|GROUP_2),               ((KEY_OFF<<4)|GROUP_3) },
		{((KEY_CHROMA_DECREASE<<4)|GROUP_ALL),    0x19,                                   ((KEY_CHROMA_INCREASE<<4)|GROUP_ALL), ((KEY_OFF<<4)|GROUP_4) },
		{((KEY_ON<<4)|GROUP_ALL),                 ((KEY_LUMINANT_INCREASE<<4)|GROUP_ALL), ((KEY_OFF<<4)|GROUP_ALL),             ((KEY_ON<<4)|GROUP_4)  }
};
#endif
/*******************************************************************
 * 函数功能：GPIO初始化
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void gpio_init_func(void)
{
	unsigned char i;
	gpio_set_func(LED_PIN,AS_GPIO);     //IO设为普通IO
	gpio_set_output_en(LED_PIN,LEVEL_HIGH); //输出使能关掉
	gpio_set_input_en(LED_PIN,LEVEL_LOW);  //输入使能关掉
	for(i=0;i<5;i++){
		gpio_set_func(gpio_row[i],AS_GPIO);     //IO设为普通IO
		gpio_set_output_en(gpio_row[i],LEVEL_LOW); //输出使能关掉
		gpio_set_input_en(gpio_row[i],LEVEL_LOW);  //输入使能关掉
		gpio_write(gpio_row[i],LEVEL_LOW);        //IO输出设为低电平
		gpio_set_up_down_resistor(gpio_row[i],GPIO_PULL_NONE);        //IO设为悬浮状态
	}

	for(i=0;i<3;i++){
		gpio_set_func(gpio_column[i],AS_GPIO);        //IO设为普通IO
		gpio_set_output_en(gpio_column[i],LEVEL_LOW);    //输出使能关掉
		gpio_set_input_en(gpio_column[i],LEVEL_HIGH);       //使能输入
		gpio_set_up_down_resistor(gpio_column[i],GPIO_PULL_UP_1M);          //设置上拉1M电阻
		gpio_write(gpio_column[i],LEVEL_LOW);           //输出设为0
		pm_set_gpio_wakeup(gpio_column[i],0,1);       //设置IO低电平唤醒，第一参数为IO，第而参数为唤醒电平，第三参数为使能
	}
}
/*******************************************************************
 * 函数功能：进入deepsleep前设置参数
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void set_wakeup_func(void)
{
	unsigned char i;
	for(i=0;i<5;i++)
		gpio_set_up_down_resistor(gpio_row[i],GPIO_PULL_DN_100K);      //设置行IO下拉100K，当按键按下时，列IO为低电平，则可唤醒MCU
	analog_write(0x3a, current_active_group);         //进入deepsleep前保存组别值
	analog_write(0x3b, led_remote.rf_seq_no);         //进入deepsleep前保存包的序列号
}
/*******************************************************************
 * 函数功能：发送数据包的数据初始化
 * 参       数：
 * 返 回 值：
 ******************************************************************/
void package_data_init_func(void)
{
	led_remote.dma_len = sizeof(rf_packet_led_remote_t)-sizeof(led_remote.dma_len);//设置包的dma长度
	led_remote.rf_len = led_remote.dma_len-1;
	led_remote.rf_len1 = led_remote.dma_len-2;
	led_remote.vid = 0x5453;//设置VID值，目前灯设置为0x5453，客户可自定义
//	led_remote.pid = 0x12345678;//设置遥控器ID，一般采用滚码方式
	led_remote.pid = otp_read(PID_ADDR) | otp_read(PID_ADDR+1)<<8 | otp_read(PID_ADDR+2)<<16 | otp_read(PID_ADDR+3)<<24;
	current_active_group = analog_read(0x3a);//读上次保存的组别值，若为第一次上电，则为0
	led_remote.rf_seq_no = analog_read(0x3b);//读上次包的序列值，若为第一次上电，则为0
}
/*******************************************************************
 * 函数功能：按键扫描
 * 参       数：
 * 返 回 值：返回对应的按键值
 ******************************************************************/
unsigned char remote_key_scan_func(void)
{
	unsigned char i,j;
	for(i=0;i<5;i++){
		gpio_set_up_down_resistor(gpio_row[i],GPIO_PULL_DN_100K);//设置行IO为下拉100K
//		gpio_write(gpio_row[i],LEVEL_LOW);
		delay_us(20);
		for(j=0;j<3;j++){
			if(gpio_read(gpio_column[j])==0){//有按键按下
				gpio_set_up_down_resistor(gpio_row[i],GPIO_PULL_NONE);//悬浮
//				gpio_write(gpio_row[i],LEVEL_HIGH);
				return key_table[i][j];//查表，返回表值
			}
		}
		gpio_set_up_down_resistor(gpio_row[i],GPIO_PULL_NONE);//若无相应按键按下，则悬浮
//		gpio_write(gpio_row[i],LEVEL_HIGH);
		delay_us(20);
	}
	return 0;
}
void user_init(void)
{
	otp_init_read();//ZQ新增，为了初始化OTP的clock
	gpio_init_func();
	rf_init_func();
	package_data_init_func();
	irq_enable();//开系统总中断
}
unsigned int keyvalue;
void main_loop(void)
{
	unsigned short Cur_key=0;
	loop++;
	Cur_key=remote_key_scan_func();//读按键值
	keyvalue=Cur_key;
	if((Cur_key>>4)== KEY_ON){//是否为开灯健，开灯键则保存组别，给色温亮度按键使用
	    current_active_group = Cur_key&0xf;
	}else if(((Cur_key>>4)!= KEY_OFF) && Cur_key){//为色温亮度按键时，使用保存的组别
	    Cur_key = (Cur_key&0xf0)|current_active_group;
	}

	if(Cur_key){//有按键按下
		if(loop&0x08)
			LED_ON;
		else
			LED_OFF;
		if(Cur_key!=pre_key){//按键值是否变化
			key_down_cnt++;
			if(key_down_cnt>4){//4次消抖
				key_up_cnt=0;
				key_lumi_chro_cnt=0;
				key_off_cnt=0;
				pre_key=Cur_key;
				led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
			}
		}
		if((pre_key>>4)!=KEY_OFF){
			led_remote.control_key=pre_key;
			if((pre_key>>4)==KEY_LUMINANT_INCREASE||(pre_key>>4)==KEY_LUMINANT_DECREASE||(pre_key>>4)==KEY_CHROMA_INCREASE||(pre_key>>4)==KEY_CHROMA_DECREASE){
				key_lumi_chro_cnt++;
				if(key_lumi_chro_cnt&0x20){//色温亮度按键按下时，每320ms调节1级
					key_lumi_chro_cnt=0;
					led_remote.rf_seq_no++;//包的序列号更新，则认为时新命令
				}
			}
		}else{
			key_lumi_chro_cnt=0;
			key_off_cnt++;
			if(key_off_cnt&0x80){//关灯健按下1.28s，进入夜灯模式
				led_night_cmd_flag=1;//夜灯命令触发标志
				key_off_cnt=0x80;
				led_remote.control_key = (KEY_QUICK_LOW_LIGHT<<4) | (pre_key&0x0f);//夜灯命令
			}
		}
	}else{//按键弹起
		LED_OFF;
		key_up_cnt++;
		key_down_cnt=0;
		key_lumi_chro_cnt=0;
		key_off_cnt=0;
		if(key_up_cnt>4){
			if(((pre_key>>4)==KEY_OFF)&&(led_night_cmd_flag==0)){//为关灯按键，并未触发夜灯命令
				if(key_up_cnt<15){
					led_remote.control_key = pre_key;//连续发送15次关灯命令值
				}else if(key_up_cnt<25){
					led_remote.control_key = KEY_NONE;//发送10次按键弹起值
					pre_key = KEY_NONE;               //按键弹起后，清除保存的按键值
				}else{
#if DEBUG
					delay_us(100000);
#else
					set_wakeup_func();
					pm_sleep_wakeup(DEEPSLEEP_MODE,PM_WAKEUP_PAD,0);//进入deepsleep模式，设置PAD唤醒
#endif
				}
			}else{
				led_night_cmd_flag=0;
				if(key_up_cnt<15){//发送15次按键弹起值
					led_remote.control_key = KEY_NONE;
					pre_key = KEY_NONE;
				}else{
#if DEBUG
					delay_us(100000);
#else
					set_wakeup_func();
					pm_sleep_wakeup(DEEPSLEEP_MODE,PM_WAKEUP_PAD,0);
#endif
				}
			}
		}
	}
	send_package_data_func();//发送数据
#if DEBUG
	delay_us(10000);
#else
	pm_sleep_wakeup(SUSPEND_MODE,PM_WAKEUP_TIMER,get_sys_tick()+10*CLOCK_SYS_CLOCK_1MS);//进入suspend 10ms
#endif
}
