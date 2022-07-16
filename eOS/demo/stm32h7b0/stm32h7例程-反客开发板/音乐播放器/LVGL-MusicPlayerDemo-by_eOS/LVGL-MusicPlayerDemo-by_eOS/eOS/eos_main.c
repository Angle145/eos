#include "main.h"
#include "eos.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "lcd_rgb.h"
#include "lcd_pwm.h"
#include "touch_800x480.h"

#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lv_demo_music.h"

#define OS_SOFTTIMER_ID_HAL		0

void app_system_server(void);
void app_gui_server(void);
void app_data_refresh_server(void);
void app_led(void);

void hal_softtimer_handler(void)
{
	HAL_IncTick();//HAL��ʱ�ӽ���
	lv_tick_inc(1);	// LVGLʱ�ӽ���	
}

void os_user_init(void)
{
	os_debug_print_enable();
	
	os_timer_reload_set(OS_SOFTTIMER_ID_HAL,1);
	os_timer_count_interrupt_set(OS_SOFTTIMER_ID_HAL,hal_softtimer_handler);
	
	LED_Init();					// ��ʼ��LED����
	KEY_Init();					// ��ʼ����������

// ��Ϊ LVGL ��Ҫ��__aeabi_assert����˲����ٹ�ѡ microLib��	
// ����Ҫʹ�ô��ڴ�ӡ����Ҫ��Ӳ��ִ��룬��� usart.c ��102��	
	usart1_init(115200);				// USART1��ʼ��	��
	
	MX_LTDC_Init();			// LTDC�Լ����ʼ��
//	LCD_PWMinit(40);			// ��������PWM��ʼ����ռ�ձ�40%		
	Touch_Init();				// ��������ʼ��		

	lv_init();					//	LVGL��ʼ��
	lv_port_disp_init();		//	LVGL��ʾ�ӿڳ�ʼ��
	lv_port_indev_init();	// LVGL�����ӿڳ�ʼ��				
	
	lv_demo_music();    // ���йٷ�����	
}
void os_main(void)
{   
	os_init();
	
	/*         APP_ID  �������ַ���     ������ 		ջ��С   ��ʱʱ�� 	APP״̬     */
	os_app_create(1, "system_server", app_system_server, 256,  500,	APP_STATE_RUNNING);
	os_app_create(2, "gui_server", app_gui_server, 1024*2,  100,	APP_STATE_RUNNING);
	os_app_create(3, "refresh_server", app_data_refresh_server, 512,  5000,	APP_STATE_RUNNING);
	os_app_create(4, "led", app_led, 128,  1000,	APP_STATE_RUNNING);
    os_startup();
}

void app_system_server(void)  
{  
	while(1)
	{
		os_app_sleep(100);
	}
}
void app_gui_server(void)
{
	while(1)
	{
		os_lock();
		lv_task_handler();		// LVGL����
		Touch_Scan();			// ����ɨ��>=10ms
		os_unlock();
		os_app_sleep(10);
	}
}
void app_data_refresh_server(void)
{
    while(1)
	{
		os_lock();
		/*----------��������Դ---------*/

		
		/*-----------------------------*/
		/*-------�������ݸ����¼�------*/		
		//lv_event_send(decparam_batper,LV_EVENT_VALUE_CHANGED,NULL);//��ص���
		
		/*-----------------------------*/
		os_unlock();
		os_app_sleep(1000);
    }	
}
void app_led(void)  
{  
    while(1)
	{
		LED1_Toggle;
		os_app_sleep(500);
    }
}
