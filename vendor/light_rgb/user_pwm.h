/********************************************************************************************************
 * @file	user_pwm.h
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
#include "../../drivers.h"

void PWM_MaxFqeSet(PWM_TypeDef pwmNumber,unsigned short MaxValue,unsigned short Fqe);
void PWM_DutyValueSet(PWM_TypeDef pwmNumber,unsigned short value);
void user_PWMInit(PWM_TypeDef pwmNumber,unsigned short MaxValue,unsigned short Fqe);
