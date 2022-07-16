#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "type.h"

//APP声明
void app_01(void);
void app_02(void);
void app_03(void);


//数值量(ID从0开始使用,最大255)
#define OS_VALUE_ID_LED_STATE_SET 	0    // LED状态设置
#define OS_VALUE_ID_LED_TIME_SET 	1    // LED闪烁时间设置
#define OS_VALUE_ID_LED_STATE_GET 	2    // LED状态获取
#define OS_VALUE_ID_LED_TIME_GET 	3    // LED闪烁时间获取

#endif
