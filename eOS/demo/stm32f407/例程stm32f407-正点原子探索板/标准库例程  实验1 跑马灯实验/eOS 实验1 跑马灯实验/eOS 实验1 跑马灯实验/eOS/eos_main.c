#include "eos.h"
#include "usart.h"
#include "led.h"

void app_01(void);
void app_02(void);
void app_led(void);


void os_user_init(void)
{
	os_debug_print_enable();
	
	uart_init(115200);			//串口初始化
	LED_Init();					//初始化LED引脚

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
		GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
		os_app_sleep(500);  		   //延时300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
		os_app_sleep(500);                     //延时300ms
    }
}
