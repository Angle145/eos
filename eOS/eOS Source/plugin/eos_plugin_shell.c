#include "eos_plugin_shell.h"	
#if (_USE_SHELL_ == 1)

//使用字节中断接收数据时，指令以无起始字符串，如enter	以回车结束
//使用空闲中断接收数据时，指令以eos$:为起始字符串，如eos$:enter	以回车结束

//系统父指令
const char cmd_enter[]={"enter"}; //进入指令系统
const char cmd_exit[]={"exit"};   //退出指令系统

//系统子指令
const char cmd_osinformation[]={"information"};//查看系统状态
const char cmd_shutdown_r_now[]={"shutdown-r now"}; //硬件重启
const char cmd_shutdown_h_now[]={"shutdown-h now"};  //系统关闭
const char cmd_shutdown_td[]={"shutdown-td=%d"};  //系统延时关机 //日
const char cmd_shutdown_th[]={"shutdown-th=%d"};  //系统延时关机 //时
const char cmd_shutdown_tm[]={"shutdown-tm=%d"};  //系统延时关机 //分
const char cmd_shutdown_ts[]={"shutdown-ts=%d"};  //系统延时关机 //秒
const char cmd_shutdown_hd[]={"shutdown-hd=%d"};//倒计时关闭系统 //日
const char cmd_shutdown_hh[]={"shutdown-hh=%d"};//倒计时关闭系统 //时
const char cmd_shutdown_hm[]={"shutdown-hm=%d"};//倒计时关闭系统 //分
const char cmd_shutdown_hs[]={"shutdown-hs=%d"};//倒计时关闭系统 //秒
const char cmd_shutdown_ht[]={"shutdown-ht=%d-%d-%d %d:%d:%d"};//倒计时关闭系统   //年/月/日/时.分.秒
const char cmd_shutdown_c[]={"shutdown-c"};//取消定时关闭
const char cmd_shutdown_j[]={"shutdown-j"};//查看关闭系统的定时时间
const char cmd_reboot[]={"reboot"};        //系统重启
const char cmd_startup[]={"startup"};      //系统打开
const char cmd_system_iap[]={"system-iap"};//系统在线升级
const char cmd_system_rap[]={"system-rap"};//读取系统程序
const char cmd_user_load[]={"load"};//用户登录
const char cmd_user_unload[]={"unload"};//退出用户登录
const char cmd_access[]={"access"};//查看登录状态(是否授权)
const char cmd_rtc_set[]={"rtc=%d-%d-%d %d:%d:%d w%d"};//设置系统实时时间
const char cmd_rtc_read[]={"rtc"};//查看系统实时时间
/********************************************************************/


//用户自定义指令用到的内部函数声明
os_u8 os_shell_compare(char* p);//比较字符串指令
os_u8 os_shell_compare_dig(char* p,char ch);//比较字符串指令
os_u8 os_shell_contain(char* p1,char* p2);//指令关键字符串检测
double os_shell_float_get(os_u8 n);//提取字符串中相应位置的浮点数字
int os_shell_int_get(os_u8 n);//提取字符串中相应位置的有符号整型数字
u32 os_shell_uint_get(os_u8 n);//提取字符串中相应位置的无符号整型数字
u8 os_shell_hex_get(os_u8 n);//提取字符串中相应位置的无符号整型数字
u32 os_shell_hex32_get(os_u8 n);//提取字符串中相应位置的无符号整型数字
//


//用户账号
char user01[]={"admin 123456"};//示例
char user02[]={"user abc123"};//示例
/********************************************************************/
//函数名：user_cmd_add
//形参  ：无
//返回值：无
//作用  ：用户自定义指令对比/处理函数
//类型  ：用户自定义函数
//方法  ：用户指令对比/处理代码填入此函数，此函数会自动被系统调用。
/********************************************************************/
/*-----------用户外部文件包含头文件区------------*/
#include "userdef.h"
#include "rtc.h"
/*-----------------------------------------------*/
void user_cmd_add(void)
{
	
	if(os_shell_compare("debug on"))  { os_debug_print_enable(); os_shell_printf("ok debug on\r\n");  } 		//debug信息显示
	if(os_shell_compare("debug off")) { os_debug_print_disable(); os_shell_printf("ok debug off\r\n"); }		//debug信息隐藏
}
/*
注意: (字符串指令必须以eos$:开头，回车结尾发送)
   1.纯指令：
      if(os_shell_compare("字符串指令")) { 对应的处理函数(); }

   2.带整型参数指令：
      if(os_shell_compare("字符串指令=数字")) { 对应的处理函数(  提取出数字作为参数  ); }


1.纯指令
例1：	if(os_shell_compare("led on"))  { led_on();   }   //比较字符串是否一致，是则执行后面的函数。
        if(os_shell_compare("led off")) { led_off();  }   //led_on()/led_off()函数可通过包含相应头文件过来使用


2.带整型参数指令
例1：	if(os_shell_compare_dig("led time=%d",'=')) { led_timer_set( os_shell_float_get(0) ); }
      //比较字符'='以前的字符串是否一致，一致则执行后面的函数，
      //os_shell_float_get(0)为提取接收到的字符串指令中的第0个数字，
      //PS:os_shell_float_get(n)为提取接收到的字符串指令中的第n个数字，如指令”time=2020.02.05“表示有3个数字，
      //分别为2020、2、5，被非数字字符隔开的数字视为不同数字，提取到的数字统一为u32类型。如果指令需要带浮点数，
      //请将整数和浮点数分割成两个整数作为两个参数发送。

例2：	if(os_shell_compare_dig("timeroff=%d/%d/%d %d.%d.%d",'=')) 
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



//以下所有代码用户不能修改或使用


//变量定义
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
//函数名: access_exist
//形参  : 无
//返回值：无
//作用  ：延长授权时间
//类型  ：安全接口
/***************************************/
void access_exist(void)
{
	secure_param.timeout=1000*60*5; //权限持续5分钟
}
/***************************************/
//函数名: os_cmd_enter
//形参  ：无 
//返回值：无
//作用  ：进入命令系统
//类型  ：命令函数
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
//函数名: os_cmd_exit
//形参  ：无 
//返回值：无
//作用  ：退出命令系统
//类型  ：命令函数
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
//函数名: os_hard_reset
//形参  ：无 
//返回值：无
//作用  ：硬件重启
//类型  ：控制函数
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
//函数名: os_off
//形参  ：无 
//返回值：无
//作用  ：系统暂停/停止运行
//类型  ：控制函数
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
//函数名: os_on
//形参  ：无 
//返回值：无
//作用  ：系统开始/继续运行
//类型  ：控制函数
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
//函数名: os_information_process
//形参  ：无 
//返回值：无
//作用  ：系统信息打印
//类型  ：信息打印函数
/***************************************/
void os_information_process(void)
{	
	os_u8 i;
	os_lock();
	os_cpu_occrate_calculation_process();
	os_stk_calculation_process();
	os_shell_printf("++++++++++++++++++++++++++++++++++++++系统状态信息++++++++++++++++++++++++++++++++++++++++++++++++\r\n");
	os_shell_printf("==================================================================================================\r\n");
	os_shell_printf("  %s @Angle_145 %s			实时时间：%04d/%02d/%02d %02d:%02d:%02d w%01d\r\n",\
					EOS_VERSION,EOS_DATE,\
					sys_rtc.std_timenow.year+2000,\
					sys_rtc.std_timenow.month,\
					sys_rtc.std_timenow.day,\
					sys_rtc.std_timenow.hour,\
					sys_rtc.std_timenow.minute,\
					sys_rtc.std_timenow.second,\
					sys_rtc.std_timenow.week
					);
	os_shell_printf("  运行模式：prio + timeout				运行时间：%-d年%-d月%-d日 %-d时%-d分%-d秒 %-dms\r\n",																				 
					 os_shell_time.runtime_year,
					 os_shell_time.runtime_mouth,
					 os_shell_time.runtime_day,
					 os_shell_time.runtime_hour,
					 os_shell_time.runtime_minute,
					 os_shell_time.runtime_second,
					 os_shell_time.runtime_ms); 
	os_shell_printf("  时钟节拍：%-4dms					系统关闭倒计时：%-d年%-d月%-d日 %-d时%-d分%-d秒\r\n",
					 OS_APP_SLICETIME,
					 os_shell_time.shutdowntime_year,
					 os_shell_time.shutdowntime_mouth,
					 os_shell_time.shutdowntime_day,
					 os_shell_time.shutdowntime_hour,
					 os_shell_time.shutdowntime_minute,
					 os_shell_time.shutdowntime_second); 
	os_shell_printf("  CPU当前占用率：%-4.1f%%                        		历史最大占用率：%-4.1f%% \r\n",os_shell_data.cpu_occrate_per1000/10.0,os_shell_data.cpu_occrate_per1000_max/10.0);
	os_shell_printf("--------------------------------------------------------------------------------------------------\r\n");
	os_shell_printf("占用率   使用栈   空闲栈   百分比    状态   休眠ms    超时    最大超时   优先级   应用名称\r\n");
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
//函数名: os_stk_calculation_process
//形参  ：无 
//返回值：无
//作用  ：计算任务栈使用率
//类型  ：信息处理函数
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
//函数名: os_shell_cpu_occrate_counter_process
//形参  ：无 
//返回值：无
//作用  ：CPU使用率计数（放在systick中断钩子中）
//类型  ：伪系统函数
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
		os_sys.clock_counter=1;//从1开始计数(0不能作除数)		 
	 } 	 
}
/**********************************************/
//函数名: os_cpu_occrate_calculation_process
//形参  ：无 
//返回值：无
//作用  ：计算cpu占用率
//类型  ：信息处理函数
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
//函数名: os_cpu_occrate_calculation_process
//形参  ：无 
//返回值：无
//作用  ：计算系统运行时间及倒计时(放在systick中断钩子中)
//类型  ：伪系统函数
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
//函数名: os_shutdown_delaytime_set_ymdhms
//形参   year年, mouth月, day日, hour时, minute分, second秒 
//返回值：无
//作用  ：设置定时关闭系统时间
//类型  ：指令处理函数
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
//函数名: os_shutdown_delaytime_add_ymdhms
//形参   year年, mouth月, day日, hour时, minute分, second秒 
//返回值：无
//作用  ：增加定时关闭系统时间
//类型  ：指令处理函数
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
//函数名: os_shutdown_tips
//形参  ：无 
//返回值：无
//作用  ：关机指示
//类型  ：指示函数
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
//函数名: os_shutdown_cancel_tips
//形参  ：无 
//返回值：无
//作用  ：取消关机指示
//类型  ：指示函数
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
//函数名: os_shell_rx_buff_clean
//形参  ：无 
//返回值：无
//作用  ：清空字符串指令接收缓存
//类型  ：内存处理函数
/*****************************************/
void os_shell_rx_buff_clean(void)
{
	memset(os_shell_cmd.rx_buff,0,SHELL_RXBUFF_LEN);
}
/*********************************************/
//函数名: os_shell_compare
//形参  ：char* p 字符串地址
//返回值: OS_TRUE：一致，OS_FALSE:不一致。
//作用  ：字符串p和接收指令字符串对比。
//        比较字符串OS_Perip.OS_USART_RX_BUFF
//        和字符串p内容是否一致。
//类型  ：字符串处理函数
/*********************************************/
os_u8 os_shell_compare(char* p)
{
	if(strncmp(os_shell_cmd.rx_buff,p,os_shell_string_len_get(p))==0) return OS_TRUE;
	else return OS_FALSE;
}
/*********************************************/
//函数名: os_shell_compares
//形参  ：char* p 字符串地址
//返回值: ==1一致，!=1不一致
//作用  ：字符串p和接收指令字符串偏移offset对比。
//        比较字符串OS_Perip.OS_USART_RX_BUFF偏移offset
//        和字符串p内容是否一致。
//类型  ：字符串处理函数
/*********************************************/
os_u8 os_shell_compares(u32 offset,char* p)
{
	return strncmp(os_shell_cmd.rx_buff+offset,p,os_shell_string_len_get(p));
}
/************************************************************/
//函数名: os_shell_dig_grab
//形参  ：char* p字符串
//返回值：无
//作用  ：从字符串p中提取数字保存到全局变量float_buff[]数组中
//类型  ：字符串处理函数
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
//函数名: os_shell_compare_dig
//形参  ：，char* p字符串,char ch字符
//返回值：OS_TRUE：一致，OS_FALSE:不一致。
//作用  ：比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，
//        字符ch以前的内容是否一致
//类型  ：字符串处理函数
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
//函数名: os_shell_contain
//形参  ：，char* p字符串
//返回值：OS_TRUE：一致，OS_FALSE:不一致。
//作用  ：检测shell命令中是否包含某字符串
//类型  ：字符串处理函数
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
//函数名: os_shell_float_get
//形参  ：os_u8 n位置
//返回值：double型的一个数(整型或浮点型均包含)
//作用  ：获取字符串p中的第n个数字(n=0开始)
//类型  ：字符串处理函数
/*************************************************/
double os_shell_float_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.float_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.float_buff[n];
}
/*************************************************/
//函数名: os_shell_int_get
//形参  ：os_u8 n位置
//返回值：int型的一个有符号整型数
//作用  ：获取字符串p中的第n个数字(n=0开始)
//类型  ：字符串处理函数
/*************************************************/
int os_shell_int_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.int_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.int_buff[n];
}
/*************************************************/
//函数名: os_shell_uint_get
//形参  ：os_u8 n位置
//返回值：u32型的一个无符号整型数
//作用  ：获取字符串p中的第n个数字(n=0开始)
//类型  ：字符串处理函数
/*************************************************/
u32 os_shell_uint_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.uint_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.uint_buff[n];
}
/*************************************************/
//函数名: os_shell_hex8_get
//形参  ：os_u8 n位置
//返回值：u32型的一个无符号整型数
//作用  ：获取字符串p中的第n个数字(n=0开始)
//类型  ：字符串处理函数
/*************************************************/
u8 os_shell_hex_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.hex_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.hex_buff[n];
}
/*************************************************/
//函数名: os_shell_hex32_get
//形参  ：os_u8 n位置
//返回值：u32型的一个无符号整型数
//作用  ：获取字符串p中的第n个数字(n=0开始)
//类型  ：字符串处理函数
/*************************************************/
u32 os_shell_hex32_get(os_u8 n)
{
	if(n>=SHELL_RXBUFF_DIG_MAX) return os_shell_cmd.hex32_buff[SHELL_RXBUFF_DIG_MAX-1];
	return os_shell_cmd.hex32_buff[n];
}
/*************************************************/
//函数名: os_shell_handle_process
//形参  ：无
//返回值：无
//作用  ：指令对比和处理
//类型  ：指令处理函数
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
			//用户键入信息
			/*******************************************************************************************/
			//信息判断
			if(secure_param.bool_access==OS_FALSE)
			{
				if(secure_param.bool_requesting==OS_TRUE)//请求登录
				{
					if(secure_param.request_count<0xFFFFFFFF) secure_param.request_count++;
					if(secure_param.request_times==0) secure_param.request_times=3;//连续3次

					if(os_shell_compare(user01))
					{
						access_exist();
						secure_param.request_count=0;
						secure_param.bool_access=OS_TRUE;
						secure_param.bool_qflash=OS_TRUE;	
						secure_param.bool_super=OS_TRUE;
						//usart0_receive_clear();
						
						os_shell_printf("超级用户\r\n");
						os_shell_printf("正在接收数据..0%%\r\n");
						os_cmd_exit();
					}
					else if(os_shell_compare(user02))
					{
						access_exist();
						secure_param.request_count=0;
						secure_param.bool_access=OS_TRUE;
						os_shell_printf("普通用户\r\n");
					}
					else if(secure_param.request_times==1)
					{
						secure_param.bool_requesting=OS_FALSE;
					    //会有bug，在secure_param.cooltime累加到满后会从0开始
						secure_param.cooltime+=30000*secure_param.request_count;
						os_shell_printf("连续输入错误，请在%d秒后重试\r\n",secure_param.cooltime/1000);
					}
					else if(secure_param.request_times>1) 
					{
						secure_param.request_times--;
						os_shell_printf("用户不存在\r\n<剩余尝试次数:%d次>\r\n",secure_param.request_times);
					}
				}
			}
			
			//用户指令
			/*******************************************************************************************/
			//指令判断
			if(os_shell_compare((char*)cmd_system_iap))
			{
				if(secure_param.bool_access==OS_FALSE)
				{
					if(secure_param.cooltime==0) 
					{
						secure_param.bool_requesting=OS_TRUE;
						os_shell_printf("请输入用户名和密码(空格隔开)：\r\n");
					}
					else
					{
						os_shell_printf("连续输入错误，请在%d秒后重试\r\n",secure_param.cooltime/1000);
					}
				}
				else
				{
					access_exist();
					if(secure_param.bool_qflash==OS_TRUE) 
					{
						os_shell_printf("IAP正在进行..\r\n");
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
							
							os_shell_printf("超级用户\r\n");
							os_shell_printf("正在接收数据..0%%\r\n");
							os_cmd_exit();
						}
						else
						{
							os_shell_printf("普通用户>>暂无权限\r\n");
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
						os_shell_printf("请输入用户名和密码(空格隔开)：\r\n");
					}
					else
					{
						os_shell_printf("连续输入错误，请在%d秒后重试\r\n",secure_param.cooltime/1000);
					}
				}
				else
				{
					access_exist();
					if(secure_param.bool_qflash==OS_TRUE) 
					{
						os_shell_printf("无法读取，IAP正在进行..\r\n");
					}
					else 
					{
						os_shell_printf("正在读取..\r\n");
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
					os_shell_printf("登录成功！>>超级用户\r\n");
				}
				else if(os_shell_compares(sizeof(cmd_user_load)-1,user02))
				{
					access_exist();
					secure_param.bool_access=OS_TRUE;	
					secure_param.bool_super=OS_FALSE;					
					os_shell_printf("登录成功！>>普通用户\r\n");
				}
				else
				{
					secure_param.bool_access=OS_FALSE;	
					secure_param.bool_super=OS_FALSE;						
					os_shell_printf("登录失败！>>用户不存在\r\n");
				}
			}
			else if(os_shell_compare((char*)cmd_user_unload))
			{
				secure_param.bool_access=OS_FALSE;	
				secure_param.bool_super=OS_FALSE;						
				os_shell_printf("已退出登录！\r\n");
			}
			else if(os_shell_compare((char*)cmd_access))
			{
				if(secure_param.bool_access==OS_TRUE)
				{
					if(secure_param.bool_super==OS_TRUE)
					{
						os_shell_printf("已登录>>超级用户\r\n");
					}
					else
					{
						os_shell_printf("已登录>>普通用户\r\n");
					}
				}
				else
				{
					os_shell_printf("未登录>>暂无权限\r\n");
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
			//延时关机
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

			//倒计时立即关机
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
			else if(os_shell_compare_dig((char*)cmd_rtc_set,'=')) 
			{
				sys_rtc.std_timenow.year=os_shell_uint_get(0)-2000;
				sys_rtc.std_timenow.month=os_shell_uint_get(1);
				sys_rtc.std_timenow.day=os_shell_uint_get(2);
				sys_rtc.std_timenow.hour=os_shell_uint_get(3);
				sys_rtc.std_timenow.minute=os_shell_uint_get(4);
				sys_rtc.std_timenow.second=os_shell_uint_get(5);
				sys_rtc.std_timenow.week=os_shell_uint_get(6);
				sys_rtc_write(&sys_rtc.std_timenow);
			}
			else if(os_shell_compare((char*)cmd_rtc_read)) 
			{
				os_shell_printf("%04d/%02d/%02d %02d:%02d:%02d w%01d\r\n",\
				sys_rtc.std_timenow.year+2000,\
				sys_rtc.std_timenow.month,\
				sys_rtc.std_timenow.day,\
				sys_rtc.std_timenow.hour,\
				sys_rtc.std_timenow.minute,\
				sys_rtc.std_timenow.second,\
				sys_rtc.std_timenow.week);	
			}
			else
			{
				//os_shell_printf("\r\ncmd not found\r\n");
			}
			user_cmd_add();//用户指令对比和处理
		}
		
		//系统指令			
		if(os_shell_compare((char*)cmd_enter))  { os_cmd_enter();os_cmd_enter_tips();  }		
	
		os_shell_rx_buff_clean();
		os_shell_cmd.rx_counter=0;
		os_shell_cmd.bool_rx_ok=OS_FALSE;	
	}	
}	
/******************************************************/
//函数名: os_systick_handler_hook
//形参  ：无
//返回值：无
//作用  ：systick中断钩子函数(代替系统自带的同名弱函数)
//类型  ：伪系统函数
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
//函数名: os_app_shell_create
//形参  ：无 
//返回值：无
//作用  ：shell任务(代替系统自带的同
//        名弱函数)
//类型  ：任务函数
/***************************************/
os_astk APP_SHELL_STK[STK_SHELL_SIZE];
void os_app_shell_create(void)//shellAPP创建函数
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
	os_acb_list[os_app_shell_prio].delay_clock=1;//必须为1
	os_acb_list[os_app_shell_prio].state=APP_STATE_RUNNING;
	app_rdy_tbl_set(os_app_shell_prio); //APP就绪
}
/***************************************/
//函数名: app_ide
//形参  ：无 
//返回值：无
//作用  ：空闲任务(代替系统自带的同
//        名弱函数)
//类型  ：任务函数
/***************************************/
void app_shell(void)
{
	os_u8 i;
	os_lock();
	os_debug_printf("eos>>the highest prio app used to app_shell\r\n");
	os_shell_data.stk_ptr[0]=APP_IDLE_STK; //复制栈任务栈地址
	for(i=1;i<OS_APP_MAX;i++)
	{
		os_shell_data.stk_ptr[i]=os_acb_list[i].stk_ptr_bpk; //复制栈任务栈地址
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
//函数名: os_shell_input
//形参  : os_u8 rx_byte 串口接收字节数据
//返回值：无
//作用  ：eOS串口指令输入接口
//类型  ：数据流接口
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
//函数名: os_shell_inputs
//形参  : os_u8 rx_byte 串口接收数组数据
//返回值：os_bool OS_TRUE-是指令 OS_FALSE-非指令
//作用  ：eOS串口指令输入接口
//类型  ：数据流接口
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

/*---------------------------------------------字符串处理函数-----------------------------------------*/

/*****************************************/
//函数名: os_shell_string_move
//形参  : mlen 
//返回值：无
//作用  ：移动字符串
//类型  ：字符串处理函数
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
	if(dir==0) //左移
	{
		for(i=0;i<(slen-len);i++)
		{
			str[i]=str[i+len];
		}
	}
	else //右移
	{
		for(i=slen-1;i>(len-1);i--)
		{
			str[i]=str[i-len];
		}
	}
}
/****************************************/
//函数名: os_shell_string_len_get
//形参  ：char* p 字符串地址
//返回值：32bit整型长度值
//作用  ：获取字符串p的长度
//类型  ：字符串处理函数
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
//函数名: os_char_in_string_position_get
//形参  ：char* p字符串,char ch字符
//返回值：32bit整型长度值
//作用  ：获取字符ch在字符串p中的位置
//类型  ：字符串处理函数
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
//函数名: string_dig_convert
//形参  ：ptr_source 字符串输入，*outbuff 数字输出,len 转换数字个数
//返回值：无
//作用  ：将字符串型数字转换成double型数字
//类型  ：字符串处理函数
/********************************************/
void string_float_convert(char* ptr_source,double *outbuff,u32 len)
{
	u8 neg_flag=FALSE;
	u16 i=0;
	u16 dp_pos=0;
	char *p=NULL;

	p=ptr_source;
	
	//不能用memset(outbuff,0,len);替代下面的循环赋值0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//数字
		{
			if(*(p-1)=='-') neg_flag=TRUE;//判断数字前面的非数字符号是否是负号
			while((*p>='0')&&(*p<='9'))
			{
				outbuff[i]=outbuff[i]*10.0+(*p-'0');
				p++;
			}
			if(*p=='.') //判断数字后面的非数字符号是否是小数点符号
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
//函数名: string_int_convert
//形参  ：ptr_source 字符串输入，*outbuff 数字输出,len 转换数字个数
//返回值：无
//作用  ：将字符串型数字转换成int型数字
//类型  ：字符串处理函数
/********************************************/
void string_int_convert(char* ptr_source,int *outbuff,u32 len)
{
	u8 neg_flag=FALSE;
	u16 i=0;
	char *p=NULL;

	p=ptr_source;
	
	//不能用memset(outbuff,0,len);替代下面的循环赋值0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//数字
		{
			if(*(p-1)=='-') neg_flag=TRUE;//判断数字前面的非数字符号是否是负号
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
//函数名: string_uint_convert
//形参  ：ptr_source 字符串输入，*outbuff 数字输出,len 转换数字个数
//返回值：无
//作用  ：将字符串型数字转换成uint型数字
//类型  ：字符串处理函数
/********************************************/
void string_uint_convert(char* ptr_source,os_u32 *outbuff,u32 len)
{
	u16 i=0;
	char *p=NULL;

	p=ptr_source;
	
	//不能用memset(outbuff,0,len);替代下面的循环赋值0
	for(i=0;i<len;i++)
	{
		outbuff[i]=0;
	}
	i=0;
	while(*p!='\0') 
	{
		if((*p>='0')&&(*p<='9'))//数字
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
//函数名: string_hex_convert
//形参  ：ptr_source 字符串输入，*outbuff 数字输出,len 转换数字个数
//返回值：无
//作用  ：将字符串型数字转换成hex型数字
//类型  ：字符串处理函数
/********************************************/
void string_hex_convert(char* ptr_source,u8 *outbuff,u32 len)
{
	u16 i=0,j=0;
	char *p=NULL;

	p=ptr_source;
	
	//不能用memset(outbuff,0,len);替代下面的循环赋值0
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
			if((*p>='0')&&(*p<='9'))//16进制数字
			{
				outbuff[i]=outbuff[i]*16+(*p-'0');
			}
			else if((*p>='A')&&(*p<='F'))//16进制大写字母
			{
				outbuff[i]=outbuff[i]*16+(*p-'A'+10);
			}
			else if((*p>='a')&&(*p<='f'))//16进制小写字母
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
//函数名: string_hex32_convert
//形参  ：ptr_source 字符串输入，*outbuff 数字输出,len 转换数字个数
//返回值：无
//作用  ：将字符串型数字转换成hex32型数字
//类型  ：字符串处理函数
/********************************************/
void string_hex32_convert(char* ptr_source,u32 *outbuff,u32 len)
{
	u16 i=0,j=0;
	char *p=NULL;

	p=ptr_source;
	
	//不能用memset(outbuff,0,len);替代下面的循环赋值0
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
			if((*p>='0')&&(*p<='9'))//16进制数字
			{
				outbuff[i]=outbuff[i]*16+(*p-'0');
			}
			else if((*p>='A')&&(*p<='F'))//16进制大写字母
			{
				outbuff[i]=outbuff[i]*16+(*p-'A'+10);
			}
			else if((*p>='a')&&(*p<='f'))//16进制小写字母
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


