#include "eos.h"
//#include "led.h"


void app_system_server(void);
void app_led(void);

void os_user_init(void)
{
	os_debug_print_enable();
	
	//led_init();		

	
}
void os_main(void)
{   
	os_init();
	
	/*         APP_ID  函数名字符串     函数名 		栈大小   超时时间 	APP状态     */
	os_app_create(1, "system_server", app_system_server, 256,  500,	APP_STATE_RUNNING);
	os_app_create(4, "led", app_led, 128,  500,	APP_STATE_RUNNING);
    os_startup();
}

void app_system_server(void)  
{  
	while(1)
	{
		os_app_sleep(100);
	}
}
void app_led(void)  
{  
    while(1)
	{
		led_toggle(LED_RED);
		os_app_sleep(500);
    }
}
