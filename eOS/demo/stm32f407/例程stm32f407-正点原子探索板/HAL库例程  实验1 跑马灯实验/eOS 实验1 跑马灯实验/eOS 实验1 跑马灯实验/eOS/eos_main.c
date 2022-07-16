#include "eos.h"
#include "usart.h"
#include "led.h"

#define OS_SOFTTIMER_ID_HAL		0

void app_01(void);
void app_02(void);
void app_led(void);

void hal_softtimer_handler(void)
{
	HAL_IncTick();//HAL��ʱ�ӽ���
}

void os_user_init(void)
{
	os_debug_print_enable();
	
	//����1����ʱ����Hal��ʹ�ã�HAL_Delay()��������Ч�������ú�delay_ms()��ͬ
	//ͬʱeos.h��OS_TIMER_MAX Ҫ��Ϊ>=1
	os_timer_reload_set(OS_SOFTTIMER_ID_HAL,1);
	os_timer_count_interrupt_set(OS_SOFTTIMER_ID_HAL,hal_softtimer_handler);
	
	uart_init(115200);
	LED_Init();					// ��ʼ��LED����

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
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_RESET); 	//LED0��Ӧ����PF9���ͣ�������ͬ��LED0(0)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET);   	//LED1��Ӧ����PF10���ߣ��𣬵�ͬ��LED1(1)
        os_app_sleep(500);											//��ʱ500ms
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);   	//LED0��Ӧ����PF9���ߣ��𣬵�ͬ��LED0(1)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET); 	//LED1��Ӧ����PF10���ͣ�������ͬ��LED1(0)
        os_app_sleep(500);                                      	//��ʱ500ms 
    }
}
