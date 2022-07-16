#include "eos.h"
#include "usart.h"
#include "led.h"

void app_01(void);
void app_02(void);
void app_led(void);


void os_user_init(void)
{
	os_debug_print_enable();
	
	uart_init(115200);			//���ڳ�ʼ��
	LED_Init();					//��ʼ��LED����

	os_debug_printf("eOS V3.1.3 inside...\r\n");
}
void os_main(void)
{   
	os_init();
	
	/*         APP_ID  �������ַ���     ������ 		ջ��С   ��ʱʱ�� 	APP״̬     */
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
		GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
		os_app_sleep(500);  		   //��ʱ300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
		os_app_sleep(500);                     //��ʱ300ms
    }
}
