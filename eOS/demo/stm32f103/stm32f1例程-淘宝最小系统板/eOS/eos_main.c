#include "main.h"
#include "eos.h"
#include "eos_plugin_shell.h" //�Ƿ�ʹ��shell���
#include "led.h" //ʾ��
#include "usart.h"


void os_user_init(void) //�û������ʹ������(delay_us/ms()�ڴ���Ч,os_app_sleep()��ֹ)
{
	//�û�ģ��\�����ʼ��,���������û���ʼ������������
	/*------------*/
	os_debug_print_enable();
	
	led_init(); //ʾ��
	usart1_init(115200);
	
	os_debug_printf("eos v3.1.3 inside..\r\n");
	/*------------*/
}
/*
����˵����
*APP_ID:��ѡֵ��1 -> (OS_APP_USER_MAX-1),����OS_APP_MAX��eos.h�����û��Զ���,IDԽ�����ȼ�Խ�ߡ�
*APP�ַ������ƣ����ڴ�ӡ�������Ϣ�е�APP����
*APP�������ƣ�APP�������ƣ�ע�ⲻ������
*ջ��С����ջ�ڴ����eos.h�����ã���ջ��С��������Ӧ��ջ�ܺ�ȷ������Ӧ�õ�ջ��С����APP�еĴ�����ת��Ⱥ;ֲ������Ĵ�С��������һ��
		ȡ��������󳤶�+32����APP01���õ����к����У�����ĳ�������ľֲ�����Ϊһ��char buff[512],��ջ��������Ϊ512/4+32=160��
*��ʱʱ�䣺APP�ڸ�����ʱ���ڸ���ԭ��û�еõ����еĳ�ʱʱ�䣬�������ʱ���OSǿ��ʹ������һ�Ρ�
APP״̬��APP������ĳ�ʼ״̬����ȡֵ��APP_STATE_RUNNING-����̬��APP_STATE_PENDING-��̬ͣ����̬ͣ����APP���������������У��ȴ�����
		APP���й���������һ�����������os_app_resume()��os_app_restart()ʹ�俪ʼ���С�
*/
void os_main(void)
{   
	os_init();//ϵͳ��ʹ��
	
	/*         	APP_ID  APP�ַ�������  APP��������    ջ��С  ��ʱʱ�� 	  	 	APP״̬     */
	os_app_create(1, 	"app_01", 		app_01, 		128,	500,		APP_STATE_RUNNING);
	os_app_create(2, 	"app_02", 		app_02,			128,	500,		APP_STATE_RUNNING);
	os_app_create(3, 	"app_03", 		app_03, 		128, 	1000,		APP_STATE_RUNNING);
	
    os_startup();//ϵͳ����
}

//��ʾ������APP��while(1)��ѭ���ڱ�����������os_app_sleep();����OS����ֻ�������ID��APP��os_app_sleep���ߵ�ʱ�����⣬����һ��Ҫ���á�

void app_01(void)  //LED��˸
{ 
	double value=0;
	u32 time_1ms_count=0;
	LED_STATE led_state=LED_STATE_ON;
	int led_toggle_time=100; 	
	while(1)
	{	
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_STATE_SET,&value)) //ʹ�÷�����OS_VALUE��ֵ�� ��ȡ LED״̬����ֵ
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
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_TIME_SET,&value)) //ʹ�÷�����OS_VALUE��ֵ�� ���� LED��˸���ʱ������ֵ
		{
			led_toggle_time=value;
			if(led_toggle_time>0)
			{
				led_state = LED_STATE_TOGGLE;
				os_shell_printf("led time set=%d ms\r\n",led_toggle_time); //ʹ��os_printf��os_debug_printf��os_shell_printfʱ��ǰ����ifʱҪ��{}������	
			}				
		}	
		value=led_state;
		if(os_app_value_post(OS_VALUE_ID_LED_STATE_GET,&value)==FALSE) //ʹ�÷�����OS_VALUE��־�� ���� LED״̬
		{
			os_shell_printf("led state post error\r\n");
		}
		value=led_toggle_time;
		if(os_app_value_post(OS_VALUE_ID_LED_TIME_GET,&value)==FALSE) //ʹ�÷�����OS_VALUE��ֵ�� ���� LED��˸���ʱ��
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
void app_02(void) //��������
{
	u32 count=0;
	while(1)
	{
		os_debug_printf("app_02 count=%d\r\n",count++);//debug�����ӡ
		os_app_sleep(1000);
	}
}
void app_03(void)  //�û��Զ���ʹ��
{  
    while(1)
	{

		os_app_sleep(500);
    }
}

/*
os_printf--���������������(���ɶ�̬�����Ƿ��ӡ������ڽ���os_shell״̬�²��ܴ�ӡ���)
os_debug_printf--���debug����(�ɶ�̬�����Ƿ��ӡ���) os_debug_print_enable()��os_debug_print_disable()
os_shell_printf--�ڽ���os_shell״̬�µĴ�ӡ���
*/

