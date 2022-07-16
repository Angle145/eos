#include "main.h"
#include "eos.h"
#include "eos_plugin_shell.h" //是否使用shell插件
#include "led.h" //示例
#include "usart.h"


void os_user_init(void) //用户代码初使化函数(delay_us/ms()在此有效,os_app_sleep()禁止)
{
	//用户模块\外设初始化,建议所有用户初始化都放在这里
	/*------------*/
	os_debug_print_enable();
	
	led_init(); //示例
	usart1_init(115200);
	
	os_debug_printf("eos v3.1.3 inside..\r\n");
	/*------------*/
}
/*
参数说明：
*APP_ID:可选值：1 -> (OS_APP_USER_MAX-1),其中OS_APP_MAX在eos.h中由用户自定义,ID越大，优先级越高。
*APP字符串名称：用于打印或调试信息中的APP名称
*APP函数名称：APP函数名称，注意不带括号
*栈大小：总栈内存池在eos.h里设置，总栈大小根据所有应用栈总和确定，单应用的栈大小根据APP中的代码跳转深度和局部变量的大小来决定。一般
		取变量的最大长度+32，如APP01调用的所有函数中，其中某个函数的局部变量为一个char buff[512],则栈最少设置为512/4+32=160。
*超时时间：APP在该运行时由于各种原因没有得到运行的超时时间，超过这个时间后OS强制使其运行一次。
APP状态：APP创建后的初始状态，可取值：APP_STATE_RUNNING-运行态、APP_STATE_PENDING-暂停态，暂停态用于APP创建后不能立即运行，等待其他
		APP运行过程中满足一定条件后调用os_app_resume()或os_app_restart()使其开始运行。
*/
void os_main(void)
{   
	os_init();//系统初使化
	
	/*         	APP_ID  APP字符串名称  APP函数名称    栈大小  超时时间 	  	 	APP状态     */
	os_app_create(1, 	"app_01", 		app_01, 		128,	500,		APP_STATE_RUNNING);
	os_app_create(2, 	"app_02", 		app_02,			128,	500,		APP_STATE_RUNNING);
	os_app_create(3, 	"app_03", 		app_03, 		128, 	1000,		APP_STATE_RUNNING);
	
    os_startup();//系统启动
}

//提示：所有APP的while(1)死循环内必须在最后调用os_app_sleep();否则OS可能只运行最大ID的APP，os_app_sleep休眠的时间任意，但是一定要调用。

void app_01(void)  //LED闪烁
{ 
	double value=0;
	u32 time_1ms_count=0;
	LED_STATE led_state=LED_STATE_ON;
	int led_toggle_time=100; 	
	while(1)
	{	
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_STATE_SET,&value)) //使用非阻塞OS_VALUE数值量 获取 LED状态设置值
		{
			led_state = (LED_STATE)value;
			led_state_control(led_state);
			os_shell_printf("led state set=");
			switch(led_state)
			{
				case LED_STATE_OFF:{ os_shell_printf("OFF\r\n"); }break;
				case LED_STATE_ON:{ os_shell_printf("ON\r\n"); }break;
				case LED_STATE_TOGGLE:{ os_shell_printf("TOGGLE\r\n"); }break;
				default:{ os_shell_printf("UNKNOW\r\n"); }break;
			}
		}
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_TIME_SET,&value)) //使用非阻塞OS_VALUE数值量 设置 LED闪烁间隔时间设置值
		{
			led_toggle_time=value;
			if(led_toggle_time>0)
			{
				led_state = LED_STATE_TOGGLE;
				os_shell_printf("led time set=%d ms\r\n",led_toggle_time); //使用os_printf或os_debug_printf或os_shell_printf时，前面有if时要用{}括起来	
			}				
		}	
		value=led_state;
		if(os_app_value_post(OS_VALUE_ID_LED_STATE_GET,&value)==FALSE) //使用非阻塞OS_VALUE标志量 发送 LED状态
		{
			os_shell_printf("led state post error\r\n");
		}
		value=led_toggle_time;
		if(os_app_value_post(OS_VALUE_ID_LED_TIME_GET,&value)==FALSE) //使用非阻塞OS_VALUE数值量 发送 LED闪烁间隔时间
		{
			os_shell_printf("led time post error\r\n");
		}		
		if(time_1ms_count>=led_toggle_time) 
		{
			time_1ms_count=0;
			led_state_control(led_state);
		}
		time_1ms_count++;
		os_app_sleep(1); 
	} 
}
void app_02(void) //用来计数
{
	u32 count=0;
	while(1)
	{
		os_debug_printf("app_02 count=%d\r\n",count++);//debug输出打印
		os_app_sleep(1000);
	}
}
void app_03(void)  //用户自定义使用
{  
    while(1)
	{

		os_app_sleep(500);
    }
}

/*
os_printf--正常输出串口数据(不可动态控制是否打印输出，在进入os_shell状态下不能打印输出)
os_debug_printf--输出debug数据(可动态控制是否打印输出) os_debug_print_enable()和os_debug_print_disable()
os_shell_printf--在进入os_shell状态下的打印输出
*/

