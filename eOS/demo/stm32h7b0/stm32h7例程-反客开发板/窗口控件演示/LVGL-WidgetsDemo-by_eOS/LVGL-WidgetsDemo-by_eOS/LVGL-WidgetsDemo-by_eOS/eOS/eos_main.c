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
#include "lv_demo_widgets.h"

#define OS_SOFTTIMER_ID_HAL		0

void app_system_server(void);
void app_gui_server(void);
void app_data_refresh_server(void);
void app_led(void);

void hal_softtimer_handler(void)
{
	HAL_IncTick();//HAL库时钟节拍
	lv_tick_inc(1);	// LVGL时钟节拍	
}

void os_user_init(void)
{
	os_debug_print_enable();
	
	os_timer_reload_set(OS_SOFTTIMER_ID_HAL,1);
	os_timer_count_interrupt_set(OS_SOFTTIMER_ID_HAL,hal_softtimer_handler);
	
	LED_Init();					// 初始化LED引脚
	KEY_Init();					// 初始化按键引脚

// 因为 LVGL 需要用__aeabi_assert，因此不能再勾选 microLib，	
// 若需要使用串口打印，需要添加部分代码，详见 usart.c 第102行	
	usart1_init(115200);				// USART1初始化	，
	
	MX_LTDC_Init();			// LTDC以及层初始化
//	LCD_PWMinit(40);			// 背光引脚PWM初始化，占空比40%		
	Touch_Init();				// 触摸屏初始化		

	lv_init();					//	LVGL初始化
	lv_port_disp_init();		//	LVGL显示接口初始化
	lv_port_indev_init();	// LVGL触摸接口初始化				
	
	lv_demo_widgets();    // 运行官方例程
}
void os_main(void)
{   
	os_init();
	
	/*         APP_ID  函数名字符串     函数名 		栈大小   超时时间 	APP状态     */
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
		lv_task_handler();		// LVGL进程
		Touch_Scan();			// 触摸扫描>=10ms
		os_unlock();
		os_app_sleep(10);
	}
}
void app_data_refresh_server(void)
{
    while(1)
	{
		os_lock();
		/*----------更新数据源---------*/

		
		/*-----------------------------*/
		/*-------发送数据更新事件------*/		
		//lv_event_send(decparam_batper,LV_EVENT_VALUE_CHANGED,NULL);//电池电量
		
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
