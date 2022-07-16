#include "eos_plugin_shell.h"	
#if (_USE_SHELL_ == 1)

//ʹ���ֽ��жϽ�������ʱ��ָ��������ʼ�ַ�������enter	�Իس�����
//ʹ�ÿ����жϽ�������ʱ��ָ����eos$:Ϊ��ʼ�ַ�������eos$:enter	�Իس�����

//ϵͳ��ָ��
const char cmd_enter[]={"enter"}; //����ָ��ϵͳ
const char cmd_exit[]={"exit"};   //�˳�ָ��ϵͳ

//ϵͳ��ָ��
const char cmd_osinformation[]={"information"};//�鿴ϵͳ״̬
const char cmd_shutdown_r_now[]={"shutdown-r now"}; //Ӳ������
const char cmd_shutdown_h_now[]={"shutdown-h now"};  //ϵͳ�ر�
const char cmd_shutdown_td[]={"shutdown-td=%d"};  //ϵͳ��ʱ�ػ� //��
const char cmd_shutdown_th[]={"shutdown-th=%d"};  //ϵͳ��ʱ�ػ� //ʱ
const char cmd_shutdown_tm[]={"shutdown-tm=%d"};  //ϵͳ��ʱ�ػ� //��
const char cmd_shutdown_ts[]={"shutdown-ts=%d"};  //ϵͳ��ʱ�ػ� //��
const char cmd_shutdown_hd[]={"shutdown-hd=%d"};//����ʱ�ر�ϵͳ //��
const char cmd_shutdown_hh[]={"shutdown-hh=%d"};//����ʱ�ر�ϵͳ //ʱ
const char cmd_shutdown_hm[]={"shutdown-hm=%d"};//����ʱ�ر�ϵͳ //��
const char cmd_shutdown_hs[]={"shutdown-hs=%d"};//����ʱ�ر�ϵͳ //��
const char cmd_shutdown_ht[]={"shutdown-ht=%d-%d-%d %d:%d:%d"};//����ʱ�ر�ϵͳ   //��/��/��/ʱ.��.��
const char cmd_shutdown_c[]={"shutdown-c"};//ȡ����ʱ�ر�
const char cmd_shutdown_j[]={"shutdown-j"};//�鿴�ر�ϵͳ�Ķ�ʱʱ��
const char cmd_reboot[]={"reboot"};        //ϵͳ����
const char cmd_startup[]={"startup"};      //ϵͳ��
const char cmd_system_iap[]={"system-iap"};//ϵͳ��������
const char cmd_system_rap[]={"system-rap"};//��ȡϵͳ����
const char cmd_user_load[]={"load"};//�û���¼
const char cmd_user_unload[]={"unload"};//�˳��û���¼
const char cmd_access[]={"access"};//�鿴��¼״̬(�Ƿ���Ȩ)
const char cmd_rtc_set[]={"rtc=%d-%d-%d %d:%d:%d w%d"};//����ϵͳʵʱʱ��
const char cmd_rtc_read[]={"rtc"};//�鿴ϵͳʵʱʱ��
/********************************************************************/


//�û��Զ���ָ���õ����ڲ���������
os_u8 os_shell_compare(char* p);//�Ƚ��ַ���ָ��
os_u8 os_shell_compare_dig(char* p,char ch);//�Ƚ��ַ���ָ��
os_u8 os_shell_contain(char* p1,char* p2);//ָ��ؼ��ַ������
double os_shell_float_get(os_u8 n);//��ȡ�ַ�������Ӧλ�õĸ�������
int os_shell_int_get(os_u8 n);//��ȡ�ַ�������Ӧλ�õ��з�����������
u32 os_shell_uint_get(os_u8 n);//��ȡ�ַ�������Ӧλ�õ��޷�����������
u8 os_shell_hex_get(os_u8 n);//��ȡ�ַ�������Ӧλ�õ��޷�����������
u32 os_shell_hex32_get(os_u8 n);//��ȡ�ַ�������Ӧλ�õ��޷�����������
//


//�û��˺�
char user01[]={"admin 123456"};//ʾ��
char user02[]={"user abc123"};//ʾ��
/********************************************************************/
//��������user_cmd_add
//�β�  ����
//����ֵ����
//����  ���û��Զ���ָ��Ա�/������
//����  ���û��Զ��庯��
//����  ���û�ָ��Ա�/�����������˺������˺������Զ���ϵͳ���á�
/********************************************************************/
/*-----------�û��ⲿ�ļ�����ͷ�ļ���------------*/
#include "led.h"

/*-----------------------------------------------*/
void user_cmd_add(void)
{
	double double_value=0.0;
	int int_value=0;
	
	if(os_shell_compare("debug on"))  { os_debug_print_enable(); os_shell_printf("ok debug on\r\n");  } 		//debug��Ϣ��ʾ
	if(os_shell_compare("debug off")) { os_debug_print_disable(); os_shell_printf("ok debug off\r\n"); }		//debug��Ϣ����
	
	if(os_shell_compare_dig("led time=%d",'='))
	{
		double_value=os_shell_float_get(0); //��ȡָ��'='�ź����1������(0��Ӧ��1��)
		if(double_value>0) os_app_value_post(OS_VALUE_ID_LED_TIME_SET,&double_value);
		else
		{
			os_shell_printf("led time must be greater than 0\r\n");
		}			
	}
	if(os_shell_compare("led time read"))
	{
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_TIME_GET,&double_value))
		{
			os_shell_printf("led time read=%d ms\r\n",(u32)double_value);
		}
	}
	if(os_shell_compare("led on"))
	{
		double_value=LED_STATE_ON;
		os_app_value_post(OS_VALUE_ID_LED_STATE_SET,&double_value);
	}
	if(os_shell_compare("led off"))
	{
		double_value=LED_STATE_OFF;
		os_app_value_post(OS_VALUE_ID_LED_STATE_SET,&double_value);
	}
	if(os_shell_compare("led toggle"))
	{
		double_value=LED_STATE_TOGGLE;
		os_app_value_post(OS_VALUE_ID_LED_STATE_SET,&double_value);
	}
	if(os_shell_compare("led state read"))
	{
		if(os_app_value_pend(APP_UNBLOCK,OS_VALUE_ID_LED_STATE_GET,&double_value))
		{
			os_shell_printf("led state read=");
			int_value=double_value;
			switch((u8)int_value)
			{
				case LED_STATE_OFF:{ os_shell_printf("OFF\r\n"); }break;
				case LED_STATE_ON:{ os_shell_printf("ON\r\n"); }break;
				case LED_STATE_TOGGLE:{ os_shell_printf("TOGGLE\r\n"); }break;
				default:{ os_shell_printf("UNKNOW\r\n"); }break;
			}
		}
	}
}
/*
ע��: (�ַ���ָ�������eos$:��ͷ���س���β����)
   1.��ָ�
      if(os_shell_compare("�ַ���ָ��")) { ��Ӧ�Ĵ�����(); }

   2.�����Ͳ���ָ�
      if(os_shell_compare("�ַ���ָ��=����")) { ��Ӧ�Ĵ�����(  ��ȡ��������Ϊ����  ); }


1.��ָ��
��1��	if(os_shell_compare("led on"))  { led_on();   }   //�Ƚ��ַ����Ƿ�һ�£�����ִ�к���ĺ�����
        if(os_shell_compare("led off")) { led_off();  }   //led_on()/led_off()������ͨ��������Ӧͷ�ļ�����ʹ��


2.�����Ͳ���ָ��
��1��	if(os_shell_compare_dig("led time=%d",'=')) { led_timer_set( os_shell_float_get(0) ); }
      //�Ƚ��ַ�'='��ǰ���ַ����Ƿ�һ�£�һ����ִ�к���ĺ�����
      //os_shell_float_get(0)Ϊ��ȡ���յ����ַ���ָ���еĵ�0�����֣�
      //PS:os_shell_float_get(n)Ϊ��ȡ���յ����ַ���ָ���еĵ�n�����֣���ָ�time=2020.02.05����ʾ��3�����֣�
      //�ֱ�Ϊ2020��2��5�����������ַ�������������Ϊ��ͬ���֣���ȡ��������ͳһΪu32���͡����ָ����Ҫ����������
      //�뽫�����͸������ָ������������Ϊ�����������͡�

��2��	if(os_shell_compare_dig("timeroff=%d/%d/%d %d.%d.%d",'=')) 
        {
					os_shutdown_delaytime_set_ymdhms( os_shell_float_get(0),\
													  os_shell_float_get(1),\
													  os_shell_float_get(2),\
													  os_shell_float_get(3),\
													  os_shell_float_get(4),\
													  os_shell_float_get(5)\
													  );
        }
*/



//�������д����û������޸Ļ�ʹ��


//��������
OS_SHELL_CMD os_shell_cmd={0};
OS_SHELL_TIME os_shell_time={0};
OS_SHELL_DATA os_shell_data={0};
SECURE_PARAM secure_param={0};

void app_shell(void);
void user_cmd_add(void);
void os_shell_handle_process(void);
void os_cpu_occrate_calculation_process(void); 
void os_shell_cpu_occrate_counter_process(void); 
void os_stk_calculation_process(void);
void os_shell_run_time_counter_process(void); 
void os_shutdown_delaytime_set_ymdhms(os_u32 year,os_u8 mouth,os_u8 day,os_u8 hour,os_u8 minute,os_u8 second); 
void os_shell_grab_dig(char* p);
void os_shell_string_move(char *str,int mlen);

/***************************************/
//������: access_exist
//�β�  : ��
//����ֵ����
//����  ���ӳ���Ȩʱ��
//����  ����ȫ�ӿ�
/***************************************/
void access_exist(void)
{
	secure_param.timeout=1000*60*5; //Ȩ�޳���5����
}
/***************************************/
//������: os_cmd_enter
//�β�  ���� 
//����ֵ����
//����  ����������ϵͳ
//����  �������
/***************************************/
void os_cmd_enter(void) 
{
	os_shell_cmd.bool_enter_sys=OS_TRUE;  
	os_sys.enter_shell=OS_TRUE;
}
void os_cmd_enter_tips(void)
{
	os_shell_printf("\r\neos$:\r\n");
}
/***************************************/
//������: os_cmd_exit
//�β�  ���� 
//����ֵ����
//����  ���˳�����ϵͳ
//����  �������
/***************************************/
void os_cmd_exit(void)
{
	os_shell_cmd.bool_enter_sys=OS_FALSE; 
	os_sys.enter_shell=OS_FALSE;
}
void os_cmd_exit_tips(void)
{
	os_shell_printf("eos$:exit\r\n");
}
/***************************************/
//������: os_hard_reset
//�β�  ���� 
//����ֵ����
//����  ��Ӳ������
//����  �����ƺ���
/***************************************/
void os_hard_reset(void) 
{
	API_os_irq_disable();
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}
void os_hard_reset_tips(void)
{
	os_shell_printf("-----$$ HardReset $$-----\r\n");
}

/***************************************/
//������: os_off
//�β�  ���� 
//����ֵ����
//����  ��ϵͳ��ͣ/ֹͣ����
//����  �����ƺ���
/***************************************/
void os_off(void) 
{
	os_u8 i;
	os_lock();
	for(i=1;i<os_app_shell_prio;i++)
	{
		if(os_acb_list[i].state==APP_STATE_RUNNING) os_acb_list[i].state=APP_STATE_PENDING;
	}
	os_sys.run_sta=OS_FALSE;
	os_unlock();
}
void os_off_tips(void)
{
	os_shell_printf("-----$$ OS SLEEP  $$-----\r\n");
}
/***************************************/
//������: os_on
//�β�  ���� 
//����ֵ����
//����  ��ϵͳ��ʼ/��������
//����  �����ƺ���
/***************************************/
void os_on(void) 
{
	os_u8 i;
	API_os_irq_disable();
	os_shell_printf("on...\r\n");
	for(i=1;i<os_app_shell_prio;i++)
	{
		if(os_acb_list[i].state==APP_STATE_PENDING) os_acb_list[i].state=APP_STATE_RUNNING;
	}
	os_sys.run_sta=OS_TRUE;
	API_os_irq_enable();
}
void os_on_tips(void)
{
	os_shell_printf("-----$$ OS AWAKEN  $$-----\r\n");
}
/***************************************/
//������: os_information_process
//�β�  ���� 
//����ֵ����
//����  ��ϵͳ��Ϣ��ӡ
//����  ����Ϣ��ӡ����
/***************************************/
void os_information_process(void)
{	
	os_u8 i;
	os_lock();
	os_cpu_occrate_calculation_process();
	os_stk_calculation_process();
	os_shell_printf("++++++++++++++++++++++++++++++++++++++ϵͳ״̬��Ϣ++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
	os_shell_printf("==================================================================================================\r\n");
	os_shell_printf("  %s @Angle_145 %s			\r\n",\
					EOS_VERSION,EOS_DATE\
					);
	os_shell_printf("  ����ģʽ��prio + timeout				����ʱ�䣺%-d��%-d��%-d�� %-dʱ%-d��%-d�� %-dms\r\n",																				 
					 os_shell_time.runtime_year,
					 os_shell_time.runtime_mouth,
					 os_shell_time.runtime_day,
					 os_shell_time.runtime_hour,
					 os_shell_time.runtime_minute,
					 os_shell_time.runtime_second,
					 os_shell_time.runtime_ms); 
	os_shell_printf("  ʱ�ӽ��ģ�%-4dms					ϵͳ�رյ���ʱ��%-d��%-d��%-d�� %-dʱ%-d��%-d��\r\n",
					 OS_APP_SLICETIME,
					 os_shell_time.shutdowntime_year,
					 os_shell_time.shutdowntime_mouth,
					 os_shell_time.shutdowntime_day,
					 os_shell_time.shutdowntime_hour,
					 os_shell_time.shutdowntime_minute,
					 os_shell_time.shutdowntime_second); 
	os_shell_printf("  CPU��ǰռ���ʣ�%-4.1f%%                        		��ʷ���ռ���ʣ�%-4.1f%% \r\n",os_shell_data.cpu_occrate_per1000/10.0,os_shell_data.cpu_occrate_per1000_max/10.0);
	os_shell_printf("--------------------------------------------------------------------------------------------------\r\n");
	os_shell_printf("ռ����   ʹ��ջ   ����ջ   �ٷֱ�    ״̬   ����ms    ��ʱ    ���ʱ   ���ȼ�   Ӧ������\r\n");
	os_shell_printf("  CPU    Used     Free      Per     State    Sleep   TimeOut  TimeMax    AppId    AppName\r\n\r\n");
	for(i=0;i<OS_APP_MAX;i++)
	{
		os_shell_printf(" %4.1f%%    %-6d   %-6d %5.1f%%      %1d      %-6d   %-6d   %-6d  %5d      %s\r\n",
						 os_shell_data.app_cpu_occrate_per1000[i]/10.0,
						 os_shell_data.stk_size_used[i],
						 os_shell_data.stk_size_free[i],
						 os_shell_data.stk_size_used[i]*100.0/os_acb_list[i].stk_size,
						 os_acb_list[i].state,
						 os_acb_list[i].delay_clock,
						 os_acb_list[i].timeout_load-os_acb_list[i].timeout_clock,
						 os_acb_list[i].timeout_load-os_acb_list[i].timeout_min,
						 os_acb_list[i].prio,
						 os_acb_list[i].name);
	}
	os_shell_printf("==================================================================================================\r\n");
	os_unlock();
}
/***************************************/
//������: os_stk_calculation_process
//�β�  ���� 
//����ֵ����
//����  ����������ջʹ����
//����  ����Ϣ������
/***************************************/
void os_stk_calculation_process(void) 
{
	os_u8 i;
	os_u32 j;
	for(i=0;i<OS_APP_MAX;i++)
	{
		j=0;
		os_shell_data.stk_size_free[i]=0;
		while(*(os_shell_data.stk_ptr[i]+j)==0x00) 
		{
			os_shell_data.stk_size_free[i]++;
			j++;
		}
		os_shell_data.stk_size_used[i]=os_acb_list[i].stk_size-os_shell_data.stk_size_free[i];
	}	
}
/**********************************************/
//������: os_shell_cpu_occrate_counter_process
//�β�  ���� 
//����ֵ����
//����  ��CPUʹ���ʼ���������systick�жϹ����У�
//����  ��αϵͳ����
/**********************************************/
void os_shell_cpu_occrate_counter_process(void) 
{	
	 os_u8 i;	
	 if(os_sys.clock_counter!=0)
	 {
		for(i=0;i<OS_APP_MAX;i++)
		{
			if(os_acb_cur==&os_acb_list[i]) 
			{
				os_shell_data.app_cpu_occrate_counter[i]++;
				break;
			}					
		}
	 }
	 else 
	 {
		for(i=0;i<OS_APP_MAX;i++)
		{
			os_shell_data.app_cpu_occrate_counter[i]=0;        
		}
		os_sys.clock_counter=1;//��1��ʼ����(0����������)		 
	 } 	 
}
/**********************************************/
//������: os_cpu_occrate_calculation_process
//�β�  ���� 
//����ֵ����
//����  ������cpuռ����
//����  ����Ϣ������
/**********************************************/
void os_cpu_occrate_calculation_process(void) 
{
	os_u8 i;
	if(os_sys.clock_counter>1000)
	{
		for(i=0;i<OS_APP_MAX;i++)
		{
			if(os_acb_list[i].state==APP_STATE_RUNNING)
			{
			  os_shell_data.app_cpu_occrate_per1000[i]=os_shell_data.app_cpu_occrate_counter[i]*1000/os_sys.clock_counter;
			}					
		}
		os_shell_data.cpu_occrate_per1000=1000-os_shell_data.app_cpu_occrate_per1000[0];
		if(os_shell_data.cpu_occrate_per1000_max<os_shell_data.cpu_occrate_per1000)
		{
		  os_shell_data.cpu_occrate_per1000_max=os_shell_data.cpu_occrate_per1000;
		}
	} 
}
/********************************************************/
//������: os_cpu_occrate_calculation_process
//�β�  ���� 
//����ֵ����
//����  ������ϵͳ����ʱ�估����ʱ(����systick�жϹ�����)
//����  ��αϵͳ����
/*******************************************************/
void os_shell_run_time_counter_process(void) 
{
	 if(os_shell_time.runtime_ms<1000) os_shell_time.runtime_ms++;
	 else
	 {
		 os_shell_time.runtime_ms=0;
		 os_shell_time.runtime_second++;
		 if(os_shell_time.runtime_second>=60) {
  		  os_shell_time.runtime_second=0;os_shell_time.runtime_minute++;
		 if(os_shell_time.runtime_minute>=60) { 
		    os_shell_time.runtime_minute=0;os_shell_time.runtime_hour++;
		 if(os_shell_time.runtime_hour>=24)   { 
		    os_shell_time.runtime_hour=0;os_shell_time.runtime_day++;
		 if(os_shell_time.runtime_hour>=24)   { 
		    os_shell_time.runtime_hour=0;os_shell_time.runtime_day++;
		 if(os_shell_time.runtime_day>=30)    { 
		    os_shell_time.runtime_day=0;os_shell_time.runtime_mouth++;
		 if(os_shell_time.runtime_mouth>=12)  { 
		    os_shell_time.runtime_mouth=0;os_shell_time.runtime_year++;
		 }}}}}} 
  
		 if((os_shell_time.shutdowntime_second==0)&&(\
        (os_shell_time.shutdowntime_minute!=0)||\
				(os_shell_time.shutdowntime_hour  !=0)||\
				(os_shell_time.shutdowntime_day   !=0)||\
				(os_shell_time.shutdowntime_mouth !=0)||\
				(os_shell_time.shutdowntime_year  !=0)))
		 {	
		os_shell_time.shutdowntime_second=1;
		 }	
		 if((os_shell_time.shutdowntime_second>2)&&\
				(os_shell_time.shutdowntime_second<13)&&\
				(os_shell_time.shutdowntime_minute==0)&&\
				(os_shell_time.shutdowntime_hour  ==0)&&\
				(os_shell_time.shutdowntime_day   ==0)&&\
				(os_shell_time.shutdowntime_mouth ==0)&&\
				(os_shell_time.shutdowntime_year  ==0))
			{
				os_shell_printf("Counting down: %0.2d\r\n",os_shell_time.shutdowntime_second-2);
			}				
		 if((os_shell_time.shutdowntime_second==2)&&\
				(os_shell_time.shutdowntime_minute==0)&&\
				(os_shell_time.shutdowntime_hour  ==0)&&\
				(os_shell_time.shutdowntime_day   ==0)&&\
				(os_shell_time.shutdowntime_mouth ==0)&&\
				(os_shell_time.shutdowntime_year  ==0))
			{
				os_shell_time.shutdowntime_second=0;
				os_shell_printf("timing completed, system has been shut down\r\n");
				os_off();
				os_off_tips();
			}
			else
			{
				if(os_shell_time.shutdowntime_second>1) os_shell_time.shutdowntime_second--;	
				if(os_shell_time.shutdowntime_second==1) 
				{
					os_shell_time.shutdowntime_second=60;
					if(os_shell_time.shutdowntime_minute>0) 
					{
						os_shell_time.shutdowntime_minute--;
					}
					else if(os_shell_time.shutdowntime_hour>0) 
					{
						os_shell_time.shutdowntime_hour--;
						os_shell_time.shutdowntime_minute=59;
					}
					else if(os_shell_time.shutdowntime_day>1) 
					{
						os_shell_time.shutdowntime_day--;
						os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}		
					else if(os_shell_time.shutdowntime_mouth>1) 
					{
						os_shell_time.shutdowntime_mouth--;
						os_shell_time.shutdowntime_day=30;
						os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}		
					else if(os_shell_time.shutdowntime_year>1) 
					{
						os_shell_time.shutdowntime_year--;
						os_shell_time.shutdowntime_mouth=12;
						os_shell_time.shutdowntime_day=30;
						os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}												
				}			
			}
	 }
}
/**********************************************************/
//������: os_shutdown_delaytime_set_ymdhms
//�β�   year��, mouth��, day��, hourʱ, minute��, second�� 
//����ֵ����
//����  �����ö�ʱ�ر�ϵͳʱ��
//����  ��ָ�����
/*********************************************************/
void os_shutdown_delaytime_set_ymdhms(os_u32 year,os_u8 mouth,os_u8 day,os_u8 hour,os_u8 minute,os_u8 second) 
{			 
	os_shell_time.shutdowntime_second=second;
	os_shell_time.shutdowntime_minute=minute;
	os_shell_time.shutdowntime_hour  =hour;
	os_shell_time.shutdowntime_day   =day;	
	os_shell_time.shutdowntime_mouth =mouth;
	os_shell_time.shutdowntime_year  =year;
}
/**********************************************************/
//������: os_shutdown_delaytime_add_ymdhms
//�β�   year��, mouth��, day��, hourʱ, minute��, second�� 
//����ֵ����
//����  �����Ӷ�ʱ�ر�ϵͳʱ��
//����  ��ָ�����
/*********************************************************/
void os_shutdown_delaytime_add_ymdhms(os_u32 year,os_u8 mouth,os_u8 day,os_u8 hour,os_u8 minute,os_u8 second) 
{			 
	os_shell_time.shutdowntime_second+=second;
	os_shell_time.shutdowntime_minute+=minute;
	os_shell_time.shutdowntime_hour  +=hour;
	os_shell_time.shutdowntime_day   +=day;	
	os_shell_time.shutdowntime_mouth +=mouth;
	os_shell_time.shutdowntime_year  +=year;
}
/***************************************/
//������: os_shutdown_tips
//�β�  ���� 
//����ֵ����
//����  ���ػ�ָʾ
//����  ��ָʾ����
/***************************************/
void os_shutdown_tips(void)
{
	os_shell_printf("shutdow delay:%d.%d.%d %d:%d:%d\r\n",\
	os_shell_time.shutdowntime_year,\
	os_shell_time.shutdowntime_mouth,\
	os_shell_time.shutdowntime_day,\
	os_shell_time.shutdowntime_hour,\
	os_shell_time.shutdowntime_minute,\
	os_shell_time.shutdowntime_second\
	);
}
/***************************************/
//������: os_shutdown_cancel_tips
//�β�  ���� 
//����ֵ����
//����  ��ȡ���ػ�ָʾ
//����  ��ָʾ����
/***************************************/
void os_shutdown_cancel_tips(void)
{
	os_shell_printf("shutdow cancel:%d.%d.%d %d:%d:%d\r\n",\
	os_shell_time.shutdowntime_year,\
	os_shell_time.shutdowntime_mouth,\
	os_shell_time.shutdowntime_day,\
	os_shell_time.shutdowntime_hour,\
	os_shell_time.shutdowntime_minute,\
	os_shell_time.shutdowntime_second\
	);
}
/*****************************************/
//������: os_shell_rx_buff_clean
//�β�  ���� 
//����ֵ����
//����  ������ַ���ָ����ջ���
//����  ���ڴ洦����
/*****************************************/
void os_shell_rx_buff_clean(void)
{
	memset(os_shell_cmd.rx_buff,0,SHELL_RXBUFF_LEN);
}
/*********************************************/
//������: os_shell_compare
//�β�  ��char* p �ַ�����ַ
//����ֵ: OS_TRUE��һ�£�OS_FALSE:��һ�¡�
//����  ���ַ���p�ͽ���ָ���ַ����Աȡ�
//        �Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF
//        ���ַ���p�����Ƿ�һ�¡�
//����  ���ַ���������
/*********************************************/
os_u8 os_shell_compare(char* p)
{
	if(strncmp(os_shell_cmd.rx_buff,p,os_shell_string_len_get(p))==0) return OS_TRUE;
	else return OS_FALSE;
}
/*********************************************/
//������: os_shell_compares
//�β�  ��char* p �ַ�����ַ
//����ֵ: ==1һ�£�!=1��һ��
//����  ���ַ���p�ͽ���ָ���ַ���ƫ��offset�Աȡ�
//        �Ƚ��ַ���OS_Perip.OS_USART_RX_BUFFƫ��offset
//        ���ַ���p�����Ƿ�һ�¡�
//����  ���ַ���������
/*********************************************/
os_u8 os_shell_compares(u32 offset,char* p)
{
	return strncmp(os_shell_cmd.rx_buff+offset,p,os_shell_string_len_get(p));
}
/************************************************************/
//������: os_shell_dig_grab
//�β�  ��char* p�ַ���
//����ֵ����
//����  �����ַ���p����ȡ���ֱ��浽ȫ�ֱ���float_buff[]������
//����  ���ַ���������
/************************************************************/
void os_shell_dig_grab(char* p)
{
	string_float_convert(p,os_shell_cmd.float_buff,SHELL_RXBUFF_DIG_MAX);
	string_int_convert(p,os_shell_cmd.int_buff,SHELL_RXBUFF_DIG_MAX);
	string_uint_convert(p,os_shell_cmd.uint_buff,SHELL_RXBUFF_DIG_MAX);
	string_hex_convert(p,os_shell_cmd.hex_buff,SHELL_RXBUFF_DIG_MAX);
	string_hex32_convert(p,os_shell_cmd.hex32_buff,SHELL_RXBUFF_DIG_MAX);
}
/************************************************************/
//������: os_shell_compare_dig
//�β�  ����char* p�ַ���,char ch�ַ�
//����ֵ��OS_TRUE��һ�£�OS_FALSE:��һ�¡�
//����  ���Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�У�
//        �ַ�ch��ǰ�������Ƿ�һ��
//����  ���ַ���������
/************************************************************/
os_u8 os_shell_compare_dig(char* p,char ch)
{
	char *ptr=NULL;
	char strbuff[2]={0};
	
	if(strncmp(os_shell_cmd.rx_buff,p,os_char_in_string_position_get(p,ch)+1)==0) 
	{
		strbuff[0]=ch;
		strbuff[1]='\0';
		ptr=strstr(os_shell_cmd.rx_buff,strbuff);
		if(ptr!=NULL) 
		{
			os_shell_dig_grab(ptr);
			return OS_TRUE;
		}
		else return OS_FALSE;
	}
	else return OS_FALSE;
}
/************************************************************/
//������: os_shell_contain
//�β�  ����char* p�ַ���
//����ֵ��OS_TRUE��һ�£�OS_FALSE:��һ�¡�
//����  �����shell�������Ƿ����ĳ�ַ���
//����  ���ַ���������
/************************************************************/
os_u8 os_shell_contain(char* p1,char* p2)
{
	char *ptr=NULL;
	ptr=strstr(os_shell_cmd.rx_buff,p1);
	if(ptr==NULL) return OS_FALSE;
	if(strstr(ptr+strlen(p1)-1,p2)) return OS_TRUE;
	return OS_FALSE;
}
/*************************************************/
//������: os_shell_float_get
//�β�  ��os_u8 nλ��
//����ֵ��double�͵�һ����(���ͻ򸡵��;�����)
//����  ����ȡ�ַ���p�еĵ�n������(n=0��ʼ)
//����  ���ַ���������
/*************************************************/
double os_shell_float_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.float_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.float_buff[n];
}
/*************************************************/
//������: os_shell_int_get
//�β�  ��os_u8 nλ��
//����ֵ��int�͵�һ���з���������
//����  ����ȡ�ַ���p�еĵ�n������(n=0��ʼ)
//����  ���ַ���������
/*************************************************/
int os_shell_int_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.int_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.int_buff[n];
}
/*************************************************/
//������: os_shell_uint_get
//�β�  ��os_u8 nλ��
//����ֵ��u32�͵�һ���޷���������
//����  ����ȡ�ַ���p�еĵ�n������(n=0��ʼ)
//����  ���ַ���������
/*************************************************/
u32 os_shell_uint_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.uint_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.uint_buff[n];
}
/*************************************************/
//������: os_shell_hex8_get
//�β�  ��os_u8 nλ��
//����ֵ��u32�͵�һ���޷���������
//����  ����ȡ�ַ���p�еĵ�n������(n=0��ʼ)
//����  ���ַ���������
/*************************************************/
u8 os_shell_hex_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.hex_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.hex_buff[n];
}
/*************************************************/
//������: os_shell_hex32_get
//�β�  ��os_u8 nλ��
//����ֵ��u32�͵�һ���޷���������
//����  ����ȡ�ַ���p�еĵ�n������(n=0��ʼ)
//����  ���ַ���������
/*************************************************/
u32 os_shell_hex32_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.hex32_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.hex32_buff[n];
}
/*************************************************/
//������: os_shell_handle_process
//�β�  ����
//����ֵ����
//����  ��ָ��ԱȺʹ���
//����  ��ָ�����
/*************************************************/
void os_shell_handle_process(void)
{
	if(os_shell_cmd.bool_rx_ok==OS_TRUE)
	{	
		if(os_shell_compare((char*)cmd_exit))   { os_cmd_exit();os_cmd_exit_tips();  }	
		if(os_shell_cmd.bool_enter_sys==OS_FALSE)
		{	
			//os_shell_printf("OS_FALSE...\r\n");	
		}		
		else 
		{
			//�û�������Ϣ
			/*******************************************************************************************/
			//��Ϣ�ж�
			if(secure_param.bool_access==OS_FALSE)
			{
				if(secure_param.bool_requesting==OS_TRUE)//�����¼
				{
					if(secure_param.request_count<0xFFFFFFFF) secure_param.request_count++;
					if(secure_param.request_times==0) secure_param.request_times=3;//����3��

					if(os_shell_compare(user01))
					{
						access_exist();
						secure_param.request_count=0;
						secure_param.bool_access=OS_TRUE;
						secure_param.bool_qflash=OS_TRUE;	
						secure_param.bool_super=OS_TRUE;
						//usart0_receive_clear();
						
						os_shell_printf("�����û�\r\n");
						os_shell_printf("���ڽ�������..0%%\r\n");
						os_cmd_exit();
					}
					else if(os_shell_compare(user02))
					{
						access_exist();
						secure_param.request_count=0;
						secure_param.bool_access=OS_TRUE;
						os_shell_printf("��ͨ�û�\r\n");
					}
					else if(secure_param.request_times==1)
					{
						secure_param.bool_requesting=OS_FALSE;
					    //����bug����secure_param.cooltime�ۼӵ�������0��ʼ
						secure_param.cooltime+=30000*secure_param.request_count;
						os_shell_printf("���������������%d�������\r\n",secure_param.cooltime/1000);
					}
					else if(secure_param.request_times>1) 
					{
						secure_param.request_times--;
						os_shell_printf("�û�������\r\n<ʣ�ೢ�Դ���:%d��>\r\n",secure_param.request_times);
					}
				}
			}
			
			//�û�ָ��
			/*******************************************************************************************/
			//ָ���ж�
			if(os_shell_compare((char*)cmd_system_iap))
			{
				if(secure_param.bool_access==OS_FALSE)
				{
					if(secure_param.cooltime==0) 
					{
						secure_param.bool_requesting=OS_TRUE;
						os_shell_printf("�������û���������(�ո����)��\r\n");
					}
					else
					{
						os_shell_printf("���������������%d�������\r\n",secure_param.cooltime/1000);
					}
				}
				else
				{
					access_exist();
					if(secure_param.bool_qflash==OS_TRUE) 
					{
						os_shell_printf("IAP���ڽ���..\r\n");
					}
					else 
					{
						if(secure_param.bool_super==OS_TRUE)
						{						
							access_exist();
							secure_param.request_count=0;
							secure_param.bool_access=OS_TRUE;
							secure_param.bool_qflash=OS_TRUE;	
							secure_param.bool_super=OS_TRUE;
							//usart0_receive_clear();
							
							os_shell_printf("�����û�\r\n");
							os_shell_printf("���ڽ�������..0%%\r\n");
							os_cmd_exit();
						}
						else
						{
							os_shell_printf("��ͨ�û�>>����Ȩ��\r\n");
						}
					}
				}
			}
			else if(os_shell_compare((char*)cmd_system_rap))
			{
				if(secure_param.bool_access==OS_FALSE)
				{
					if(secure_param.cooltime==0) 
					{
						secure_param.bool_requesting=OS_TRUE;
						os_shell_printf("�������û���������(�ո����)��\r\n");
					}
					else
					{
						os_shell_printf("���������������%d�������\r\n",secure_param.cooltime/1000);
					}
				}
				else
				{
					access_exist();
					if(secure_param.bool_qflash==OS_TRUE) 
					{
						os_shell_printf("�޷���ȡ��IAP���ڽ���..\r\n");
					}
					else 
					{
						os_shell_printf("���ڶ�ȡ..\r\n");
						os_cmd_exit();
					}
				}
			}
			else if(os_shell_compare((char*)cmd_user_load))
			{
				if(os_shell_compares(sizeof(cmd_user_load)-1,user01))
				{
					access_exist();
					secure_param.bool_access=OS_TRUE;	
					secure_param.bool_super=OS_TRUE;					
					os_shell_printf("��¼�ɹ���>>�����û�\r\n");
				}
				else if(os_shell_compares(sizeof(cmd_user_load)-1,user02))
				{
					access_exist();
					secure_param.bool_access=OS_TRUE;	
					secure_param.bool_super=OS_FALSE;					
					os_shell_printf("��¼�ɹ���>>��ͨ�û�\r\n");
				}
				else
				{
					secure_param.bool_access=OS_FALSE;	
					secure_param.bool_super=OS_FALSE;						
					os_shell_printf("��¼ʧ�ܣ�>>�û�������\r\n");
				}
			}
			else if(os_shell_compare((char*)cmd_user_unload))
			{
				secure_param.bool_access=OS_FALSE;	
				secure_param.bool_super=OS_FALSE;						
				os_shell_printf("���˳���¼��\r\n");
			}
			else if(os_shell_compare((char*)cmd_access))
			{
				if(secure_param.bool_access==OS_TRUE)
				{
					if(secure_param.bool_super==OS_TRUE)
					{
						os_shell_printf("�ѵ�¼>>�����û�\r\n");
					}
					else
					{
						os_shell_printf("�ѵ�¼>>��ͨ�û�\r\n");
					}
				}
				else
				{
					os_shell_printf("δ��¼>>����Ȩ��\r\n");
				}
			}
			else if(os_shell_compare((char*)cmd_reboot)){ os_hard_reset_tips();  os_hard_reset(); }
			else if(os_shell_compare((char*)cmd_osinformation)) { os_information_process();  }		
			else if(os_shell_compare((char*)cmd_shutdown_r_now)){ os_hard_reset_tips();  os_hard_reset(); }
			else if(os_shell_compare((char*)cmd_startup))           { os_on(); os_on_tips(); }
			else if(os_shell_compare((char*)cmd_shutdown_h_now)){ os_off(); os_off_tips(); }	
			else if(os_shell_compare((char*)cmd_shutdown_j)) { os_shutdown_tips();  }
			else if(os_shell_compare((char*)cmd_shutdown_c)) 
			{ 
					os_shutdown_cancel_tips(); 
					os_shutdown_delaytime_set_ymdhms(\
					0,\
					0,\
					0,\
					0,\
					0,\
					0);
			}	
			//��ʱ�ػ�
			else if(os_shell_compare_dig((char*)cmd_shutdown_hd,'=')) 
			{ 
					os_shutdown_delaytime_add_ymdhms(\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0,\
					0,\
					0);
					os_shutdown_tips(); 
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hh,'=')) 
			{ 
					os_shutdown_delaytime_add_ymdhms(\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0,\
					0);
					os_shutdown_tips(); 
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hm,'=')) 
			{ 
					os_shutdown_delaytime_add_ymdhms(\
					0,\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0);
					os_shutdown_tips(); 
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hs,'=')) 
			{ 
					os_shutdown_delaytime_add_ymdhms(\
					0,\
					0,\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0));
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_ht,'=')) 
			{ 
					os_shutdown_delaytime_add_ymdhms(\
					os_shell_uint_get(0),\
					os_shell_uint_get(1),\
					os_shell_uint_get(2),\
					os_shell_uint_get(3),\
					os_shell_uint_get(4),\
					os_shell_uint_get(5));
			}	

			//����ʱ�����ػ�
			else if(os_shell_compare_dig((char*)cmd_shutdown_hd,'=')) 
			{ 
					os_shutdown_delaytime_set_ymdhms(\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0,\
					0,\
					0);
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hh,'=')) 
			{ 
					os_shutdown_delaytime_set_ymdhms(\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0,\
					0);
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hm,'=')) 
			{ 
					os_shutdown_delaytime_set_ymdhms(\
					0,\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0),\
					0);
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_hs,'=')) 
			{ 
					os_shutdown_delaytime_set_ymdhms(\
					0,\
					0,\
					0,\
					0,\
					0,\
					(os_u8)os_shell_uint_get(0));
			}
			else if(os_shell_compare_dig((char*)cmd_shutdown_ht,'=')) 
			{ 
					os_shutdown_delaytime_set_ymdhms(\
					(os_u8)os_shell_uint_get(0),\
					(os_u8)os_shell_uint_get(1),\
					(os_u8)os_shell_uint_get(2),\
					(os_u8)os_shell_uint_get(3),\
					(os_u8)os_shell_uint_get(4),\
					(os_u8)os_shell_uint_get(5));
			}	
			else
			{
				//os_shell_printf("\r\ncmd not found\r\n");
			}
			user_cmd_add();//�û�ָ��ԱȺʹ���
		}
		
		//ϵͳָ��			
		if(os_shell_compare((char*)cmd_enter))  { os_cmd_enter();os_cmd_enter_tips();  }		
	
		os_shell_rx_buff_clean();
		os_shell_cmd.rx_counter=0;
		os_shell_cmd.bool_rx_ok=OS_FALSE;	
	}	
}	
/******************************************************/
//������: os_systick_handler_hook
//�β�  ����
//����ֵ����
//����  ��systick�жϹ��Ӻ���(����ϵͳ�Դ���ͬ��������)
//����  ��αϵͳ����
/*****************************************************/
void os_systick_handler_hook(void)
{
	os_shell_cpu_occrate_counter_process();
	os_shell_run_time_counter_process();
	if(secure_param.cooltime>0) secure_param.cooltime--;
	if(secure_param.bool_access==OS_TRUE)
	{
		if(secure_param.timeout>0) secure_param.timeout--;
		if(secure_param.timeout==0) secure_param.bool_access=OS_FALSE;
	}
}
/***************************************/
//������: os_app_shell_create
//�β�  ���� 
//����ֵ����
//����  ��shell����(����ϵͳ�Դ���ͬ
//        ��������)
//����  ��������
/***************************************/
os_astk APP_SHELL_STK[STK_SHELL_SIZE];
void os_app_shell_create(void)//shellAPP��������
{
#if OS_APP_NAME_SIZE>0
	strcpy(os_acb_list[os_app_shell_prio].name,"shell");
#endif
	os_acb_list[os_app_shell_prio].timeout_load=0;
	os_acb_list[os_app_shell_prio].timeout_clock=0;
	os_acb_list[os_app_shell_prio].app_ptr=(os_u32*)app_shell;
	os_acb_list[os_app_shell_prio].stk_ptr_bpk=APP_SHELL_STK;
    os_acb_list[os_app_shell_prio].stk_ptr = os_app_stk_init(app_shell,(os_stk*)&APP_SHELL_STK[STK_SHELL_SIZE - 1]);
	os_acb_list[os_app_shell_prio].stk_size=STK_SHELL_SIZE;
	os_acb_list[os_app_shell_prio].prio=os_app_shell_prio;
	os_acb_list[os_app_shell_prio].delay_clock=1;//����Ϊ1
	os_acb_list[os_app_shell_prio].state=APP_STATE_RUNNING;
	app_rdy_tbl_set(os_app_shell_prio); //APP����
}
/***************************************/
//������: app_ide
//�β�  ���� 
//����ֵ����
//����  ����������(����ϵͳ�Դ���ͬ
//        ��������)
//����  ��������
/***************************************/
void app_shell(void)
{
	os_u8 i;
	os_lock();
	os_debug_printf("eos>>the highest prio app used to app_shell\r\n");
	os_shell_data.stk_ptr[0]=APP_IDLE_STK; //����ջ����ջ��ַ
	for(i=1;i<OS_APP_MAX;i++)
	{
		os_shell_data.stk_ptr[i]=os_acb_list[i].stk_ptr_bpk; //����ջ����ջ��ַ
	}
	os_unlock();
	while(1)
	{	 
		os_shell_handle_process();	
		os_app_suspend(app_shell);
	}
}
void HardFault_Handler(void)
{
	os_sys.enter_shell=OS_FALSE;
	os_shell_printf("\r\n-----$$ HardFault $$-----\r\n");
	os_information_process();
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1){
    }
}

#endif
/***************************************/
//������: os_shell_input
//�β�  : os_u8 rx_byte ���ڽ����ֽ�����
//����ֵ����
//����  ��eOS����ָ������ӿ�
//����  ���������ӿ�
/***************************************/
void os_shell_input(os_u8 rx_byte)
{
#if (_USE_SHELL_ == 1)
	if(os_shell_cmd.bool_rx_ok==OS_FALSE)
	{
		if(os_shell_cmd.rx_counter<SHELL_RXBUFF_LEN-1) 
		{
			os_shell_cmd.rx_buff[os_shell_cmd.rx_counter++]=rx_byte;
			switch(os_shell_cmd.rx_counter)
			{
				case 1: if(os_shell_cmd.rx_buff[0]!='e') os_shell_cmd.rx_counter=0;
					break;
				case 2: if(strcmp(os_shell_cmd.rx_buff,"eo")<0) os_shell_cmd.rx_counter=0;
					break;
				case 3: if(strcmp(os_shell_cmd.rx_buff,"eos")<0) os_shell_cmd.rx_counter=0;
					break;
				case 4: if(strcmp(os_shell_cmd.rx_buff,"eos$")<0) os_shell_cmd.rx_counter=0;
					break;
				case 5: if(strcmp(os_shell_cmd.rx_buff,"eos$:")<0) os_shell_cmd.rx_counter=0;
					break;
			}	
			if(os_shell_cmd.rx_counter>=sizeof("eos$:"))
			{
				if((os_shell_cmd.rx_buff[os_shell_cmd.rx_counter-2]=='\r')&&(os_shell_cmd.rx_buff[os_shell_cmd.rx_counter-1]=='\n'))
				{
					os_shell_cmd.rx_counter=os_shell_cmd.rx_counter-sizeof("eos$:")+1;
					os_shell_string_move(os_shell_cmd.rx_buff,(int)(0-sizeof("eos$:")+1));
					os_shell_cmd.rx_buff[os_shell_cmd.rx_counter-2]='\0';
					os_shell_cmd.bool_rx_ok=OS_TRUE;
					os_app_resume(app_shell);
				}
			}
		}
		else 
		{ 
			os_shell_cmd.rx_counter=0; os_shell_cmd.rx_buff[os_shell_cmd.rx_counter]=rx_byte; 
		}
	}
#endif
}
/***************************************/
//������: os_shell_inputs
//�β�  : os_u8 rx_byte ���ڽ�����������
//����ֵ��os_bool OS_TRUE-��ָ�� OS_FALSE-��ָ��
//����  ��eOS����ָ������ӿ�
//����  ���������ӿ�
/***************************************/
void os_shell_inputs(os_u8 *rx_bytes,os_u32 len)
{
#if (_USE_SHELL_ == 1)
	char* rxbuff;
	char* end;
	os_u16 rxlen;
		
	if(os_shell_cmd.bool_rx_ok==OS_FALSE)
	{
		if(len>SHELL_RXBUFF_LEN) return;
		rxbuff=strstr((char*)rx_bytes,"eos$:");
		if(rxbuff==NULL) return;
		end=strstr((char*)rx_bytes,"\r\n");
		if(end==NULL) return;
		*(end+2)='\0';
		rxlen=len+(os_u32)rx_bytes-(os_u32)rxbuff-sizeof("eos$:")+1;
		os_shell_cmd.rx_counter=rxlen;
		if(os_shell_cmd.rx_counter<SHELL_RXBUFF_LEN) 
		{
			memcpy(os_shell_cmd.rx_buff,rxbuff+sizeof("eos$:")-1,os_shell_cmd.rx_counter);
			os_shell_cmd.bool_rx_ok=OS_TRUE;				
			os_app_resume(app_shell);
		}
		else 
		{ 
			os_shell_cmd.rx_counter=0;
		}
	}
#endif
}

/*---------------------------------------------�ַ���������-----------------------------------------*/

/*****************************************/
//������: os_shell_string_move
//�β�  : mlen 
//����ֵ����
//����  ���ƶ��ַ���
//����  ���ַ���������
/*****************************************/
void os_shell_string_move(char *str,int mlen)
{
	os_u32 i=0;
	os_u32 len=0;
	os_u8 dir=0;
	os_u32 slen=0;
	
	slen=strlen(str);
	if(mlen<0) 
	{
		len=0-mlen;
		dir=0;
	}
	else
	{	
		len=mlen;
		dir=1;
	}
	if(dir==0) //����
	{
		for(i=0;i<(slen-len);i++)
		{
			str[i]=str[i+len];
		}
	}
	else //����
	{
		for(i=slen-1;i>(len-1);i--)
		{
			str[i]=str[i-len];
		}
	}
}
/****************************************/
//������: os_shell_string_len_get
//�β�  ��char* p �ַ�����ַ
//����ֵ��32bit���ͳ���ֵ
//����  ����ȡ�ַ���p�ĳ���
//����  ���ַ���������
/****************************************/
os_u32 os_shell_string_len_get(char* p)
{
	os_u32 i=0;
	while(*p!='\0')
	{
		i++;
		p++;
	}
	return i;
}
/********************************************/
//������: os_char_in_string_position_get
//�β�  ��char* p�ַ���,char ch�ַ�
//����ֵ��32bit���ͳ���ֵ
//����  ����ȡ�ַ�ch���ַ���p�е�λ��
//����  ���ַ���������
/********************************************/
os_u32 os_char_in_string_position_get(char* p,char ch)
{
	os_u32 i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
		i++;
		p++;
	}
	return i;
}
/********************************************/
//������: string_dig_convert
//�β�  ��ptr_source �ַ������룬*outbuff �������,len ת�����ָ���
//����ֵ����
//����  �����ַ���������ת����double������
//����  ���ַ���������
/********************************************/
void string_float_convert(char* ptr_source,double *outbuff,u32 len)
{
	u8 neg_flag=FALSE;
	u16 i=0;
	u16 dp_pos=0;
	char *p=NULL;

	p=ptr_source;
	
	//������memset(outbuff,0,len);��������ѭ����ֵ0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//����
		{
			if(*(p-1)=='-') neg_flag=TRUE;//�ж�����ǰ��ķ����ַ����Ƿ��Ǹ���
			while((*p>='0')&&(*p<='9'))
			{
				outbuff[i]=outbuff[i]*10.0+(*p-'0');
				p++;
			}
			if(*p=='.') //�ж����ֺ���ķ����ַ����Ƿ���С�������
			{
				dp_pos=1;
				p++;
				while((*p>='0')&&(*p<='9'))
				{
					outbuff[i]=outbuff[i]*10.0+(*p-'0');
					dp_pos*=10.0;
					p++;
				}
				outbuff[i]/=(double)dp_pos;
			}
			if(neg_flag==TRUE) outbuff[i]=0-outbuff[i];
			i++;
			if(i>=len) break;
		} 
		p++;
	}
}
/********************************************/
//������: string_int_convert
//�β�  ��ptr_source �ַ������룬*outbuff �������,len ת�����ָ���
//����ֵ����
//����  �����ַ���������ת����int������
//����  ���ַ���������
/********************************************/
void string_int_convert(char* ptr_source,int *outbuff,u32 len)
{
	u8 neg_flag=FALSE;
	u16 i=0;
	char *p=NULL;

	p=ptr_source;
	
	//������memset(outbuff,0,len);��������ѭ����ֵ0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//����
		{
			if(*(p-1)=='-') neg_flag=TRUE;//�ж�����ǰ��ķ����ַ����Ƿ��Ǹ���
			while((*p>='0')&&(*p<='9'))
			{
				outbuff[i]=outbuff[i]*10+(*p-'0');
				p++;
			}
			if(neg_flag==TRUE) outbuff[i]=0-outbuff[i];
			i++;
			if(i>=len) break;
		} 
		p++;
	}	
}
/********************************************/
//������: string_uint_convert
//�β�  ��ptr_source �ַ������룬*outbuff �������,len ת�����ָ���
//����ֵ����
//����  �����ַ���������ת����uint������
//����  ���ַ���������
/********************************************/
void string_uint_convert(char* ptr_source,os_u32 *outbuff,u32 len)
{
	u16 i=0;
	char *p=NULL;

	p=ptr_source;
	
	//������memset(outbuff,0,len);��������ѭ����ֵ0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//����
		{
			while((*p>='0')&&(*p<='9'))
			{
				outbuff[i]=outbuff[i]*10+(*p-'0');
				p++;
			}
			i++;
			if(i>=len) break;
		} 
		p++;
	}	
}
/********************************************/
//������: string_hex_convert
//�β�  ��ptr_source �ַ������룬*outbuff �������,len ת�����ָ���
//����ֵ����
//����  �����ַ���������ת����hex������
//����  ���ַ���������
/********************************************/
void string_hex_convert(char* ptr_source,u8 *outbuff,u32 len)
{
	u16 i=0,j=0;
	char *p=NULL;

	p=ptr_source;
	
	//������memset(outbuff,0,len);��������ѭ����ֵ0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		j=0;
		while(((*p>='0')&&(*p<='9'))||((*p>='A')&&(*p<='F'))||((*p>='a')&&(*p<='f')))
		{
			if((*p>='0')&&(*p<='9'))//16��������
			{
				outbuff[i]=outbuff[i]*16+(*p-'0');
			}
			else if((*p>='A')&&(*p<='F'))//16���ƴ�д��ĸ
			{
				outbuff[i]=outbuff[i]*16+(*p-'A'+10);
			}
			else if((*p>='a')&&(*p<='f'))//16����Сд��ĸ
			{
				outbuff[i]=outbuff[i]*16+(*p-'a'+10);
			}
			p++;
			j++;
			if(j%2==0) 
			{
				i++;
				if(i>=len) break;
			}
		}
		if(j%2!=0) 
		{
			i++;
			if(i>=len) break;
		}
		p++;		
	}	
}
/********************************************/
//������: string_hex32_convert
//�β�  ��ptr_source �ַ������룬*outbuff �������,len ת�����ָ���
//����ֵ����
//����  �����ַ���������ת����hex32������
//����  ���ַ���������
/********************************************/
void string_hex32_convert(char* ptr_source,u32 *outbuff,u32 len)
{
	u16 i=0,j=0;
	char *p=NULL;

	p=ptr_source;
	
	//������memset(outbuff,0,len);��������ѭ����ֵ0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		j=0;
		while(((*p>='0')&&(*p<='9'))||((*p>='A')&&(*p<='F'))||((*p>='a')&&(*p<='f')))
		{
			if((*p>='0')&&(*p<='9'))//16��������
			{
				outbuff[i]=outbuff[i]*16+(*p-'0');
			}
			else if((*p>='A')&&(*p<='F'))//16���ƴ�д��ĸ
			{
				outbuff[i]=outbuff[i]*16+(*p-'A'+10);
			}
			else if((*p>='a')&&(*p<='f'))//16����Сд��ĸ
			{
				outbuff[i]=outbuff[i]*16+(*p-'a'+10);
			}
			p++;
			j++;
			if(j%8==0) 
			{
				i++;
				if(i>=len) break;
			}
		}
		if(j%8!=0) 
		{
			i++;
			if(i>=len) break;
		}
		p++;		
	}
}


