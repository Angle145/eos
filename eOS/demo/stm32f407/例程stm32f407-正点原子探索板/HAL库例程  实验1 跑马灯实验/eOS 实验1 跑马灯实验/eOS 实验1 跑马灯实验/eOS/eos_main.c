#include "eos.h"
#include "usart.h"
#include "led.h"

#define OS_SOFTTIMER_ID_HAL		0

void app_01(void);
void app_02(void);
void app_led(void);

void hal_softtimer_handler(void)
{
	HAL_IncTick();//HAL库时钟节拍
}

void os_user_init(void)
{
	os_debug_print_enable();
	
	//创建1个软定时器给Hal库使用，HAL_Delay()函数仍有效，其作用和delay_ms()相同
	//同时eos.h的OS_TIMER_MAX 要设为>=1
	os_timer_reload_set(OS_SOFTTIMER_ID_HAL,1);
	os_timer_count_interrupt_set(OS_SOFTTIMER_ID_HAL,hal_softtimer_handler);
	
	uart_init(115200);
	LED_Init();					// 初始化LED引脚

	os_debug_printf("eOS V3.1.3 inside...\r\n");
}
void os_main(void)
{   
	os_init();
	
	/*         APP_ID  函数名字符串     函数名 		栈大小   超时时间 	APP状态     */
	os_app_create(1, "app_01", app_01, 256,  2000,	APP_STATE_RUNNING);
	os_app_create(2, "app_02", app_02, 256,  2000,	APP_STATE_RUNNING);
	os_app_create(3, "app_led", app_led, 128,  2000,	APP_STATE_RUNNING);
    os_startup();
}

void app_01(void)  
{  
	u32 t=0;
	while(1)
	{
		os_debug_printf("app_01 running %d\r\n",t++);
		os_app_sleep(1000);
	}
}
void app_02(void)
{
	u32 t=0;
	while(1)
	{
		os_debug_printf("app_02 running %d\r\n",t++);
		os_app_sleep(500);
	}
}
void app_led(void)  
{  
    while(1)
	{
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET); 	//LED0对应引脚PF9拉低，亮，等同于LED0(0)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);   	//LED1对应引脚PF10拉高，灭，等同于LED1(1)
        os_app_sleep(500);											//延时500ms
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);   	//LED0对应引脚PF9拉高，灭，等同于LED0(1)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET); 	//LED1对应引脚PF10拉低，亮，等同于LED1(0)
        os_app_sleep(500);                                      	//延时500ms 
    }
}
