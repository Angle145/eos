#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "type.h"

//APP����
void app_01(void);
void app_02(void);
void app_03(void);


//��ֵ��(ID��0��ʼʹ��,���255)
#define OS_VALUE_ID_LED_STATE_SET 	0    // LED״̬����
#define OS_VALUE_ID_LED_TIME_SET 	1    // LED��˸ʱ������
#define OS_VALUE_ID_LED_STATE_GET 	2    // LED״̬��ȡ
#define OS_VALUE_ID_LED_TIME_GET 	3    // LED��˸ʱ���ȡ

#endif
