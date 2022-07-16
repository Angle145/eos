#ifndef _OS_H_
#define _OS_H_
#include "main.h" 
//#include "dwg.h"

//版本:V3.1.5
#define EOS_VERSION "eOS_V3.1.5"
#define EOS_DATE 	"2022/07/15"

typedef signed char  os_s8;            
typedef signed short os_s16;           
typedef signed int   os_s32;     
typedef signed long  os_s64;  

typedef unsigned char os_u8;            
typedef unsigned short os_u16;           
typedef unsigned int    os_u32;     
typedef unsigned long  os_u64;  

typedef  void (*fun)(void);

typedef os_u32   os_stk; 
#define os_astk __align(8) os_stk

/********************************************OS参数配置*************************************************************/
#define OS_APP_USER_MAX       5			//用户APP数量(0-62,内存使用为N*(10~12)) bytes)
#define OS_STACK_SIZE         1024*2	//用户APP栈内存,单位：word
#define OS_APP_SLICETIME      10		//APP时间切片,单位：tick(默认1ms,在eos_port2.c/systick_init()修改)(范围：1-200)  
#define STK_IDLE_SIZE         64		//空闲APP栈长度 (内存使用为N*4) bytes) >=32
#define STK_CORE_SIZE 		  128		//系统核心栈长度	 (内存使用为N*4) bytes) >=32	（和用户APP数量+软定时器数量有关）	
#define STK_CORE_SLICETIME 	  100		//系统核心响应时间,单位：tick
#define STK_SHELL_SIZE        256		//APP_SHELL栈长度 >150，使用shell插件时有作用
#define OS_APP_NAME_SIZE      24		//APP字符串长度		
	
#define STK_IDLE_TIMEOUT      10001    	//空闲APP超时运行时间,单位：tick
#define STK_CORE_TIMEOUT      1001    	//空闲APP超时运行时间,单位：tick
	
#define OS_TIMER_MAX	      6			//系统软定时器数量(ID数量)
	
#define OS_FLAG_MAX		  	  0	 		//APP同步标志量数量(ID数量)  ，0-255
#define OS_VALUE_MAX	  	  0	 		//APP同步数值量数量(ID数量)  ，0-255
#define OS_MUTEX_MAX	  	  0  		//APP互斥量数量(ID数量)  ，	   0-255									    
#define OS_MBOX_MAX	          0			//APP同步数组/字符串数量(ID数量)  ，0-255
#define OS_MBOX_QUEUE_MAX	  0			//APP异步数组/字符串数量(ID数量)  ，0-255
	
#define OS_MAIL_SIZE		  32		//系统邮件长度
#define OS_QUEUE_SIZE		  16		//邮箱队列深度(1个queue_id能缓存的邮件数)
	
#define _RAM_ALGO_            0			//0-rom,1-ram,算法数组保存位置。
#define _OS_DEBUG_PRINTF_	  1			//0-禁止os_printf打印输出，1-允许打印输出
	
#define _USE_SHELL_           1			//0-不启用shell,1-启用shell（添加eos_plugin_shell文件后有效）
/****************************************-**OS 看门狗配置**********************************************************/

//提供给OS的喂狗函数（如果开启了看门狗则必须填入此项,未使用则必须注释掉）
//#define	os_dwg_feed_api()	fwdgt_feed()//喂狗函数 

/************************************************************************************************************/
//以下代码和参数配置无关

//OS提供的打印输出函数 
//#define os_printf(fmt, ...) { os_lock();printf( fmt, ##__VA_ARGS__);os_unlock();}//OS提供的打印输出函数
#define os_printf(fmt, ...) { if(os_sys.print_sta==OS_TRUE){os_lock();if(os_sys.enter_shell==OS_FALSE){printf( fmt, ##__VA_ARGS__);}os_unlock();}}//OS提供的打印输出函数
#define os_debug_printf(fmt, ...) {if(os_sys.debug_print_sta==OS_TRUE){ os_lock();if(os_sys.enter_shell==OS_FALSE){printf( fmt, ##__VA_ARGS__);}os_unlock();}}//OS提供的打印输出函数
#define os_shell_printf(fmt, ...) { os_lock(); printf( fmt, ##__VA_ARGS__); os_unlock();}//OS SHELL提供的打印输出函数



//APP状态
typedef enum
{
	APP_STATE_RUNNING,//运行态
	APP_STATE_PENDING,//挂起态
	APP_STATE_RESTART,//重启态
	APP_STATE_DELETED//删除态
}APP_STATE;

//系统总APP数量
#define OS_APP_MAX (OS_APP_USER_MAX+2+_USE_SHELL_)//增加1个空闲任务(最低先级)、1个系统核心任务(不使用shell情况下为最高优先级)
/*
作者认为还是要把最高优先级给到app_shell，这样shell才真正意义上拥用root权限。 
*/

#if (OS_APP_USER_MAX>62) 
#error OS_APP_MAX must Less than 63!
#endif

extern os_astk OS_STACK[OS_STACK_SIZE];

//系统布尔值
typedef enum 
{
	OS_FALSE=0x00, 
	OS_TRUE=0xFF,
}os_bool; 

//pend方式
#define	APP_BLOCK    0
#define APP_UNBLOCK  1


#if ( OS_APP_MAX < 16 )
#if ( _RAM_ALGO_ == 1 )
extern os_u8 BIT_MAP[16];
#else
extern os_u8 const BIT_MAP[16];
#endif
#else
#ifdef _RAM_ALGO_
extern os_u8 const BIT_MAP[256];
#else
extern os_u8 const BIT_MAP[256];
#endif
#endif
#if (( OS_MBOX_MAX > 0 )||( OS_MBOX_QUEUE_MAX > 0 ))
typedef struct 
{
  os_u32 data_len;
  os_u8 datum[OS_MAIL_SIZE];
}OS_MBOX;
#endif
#if ( OS_MBOX_QUEUE_MAX > 0 )
typedef struct 
{
  os_u32 queue_len;
  OS_MBOX mail[OS_QUEUE_SIZE];
}OS_MBOX_QUEUE;
#endif

//OS软件定时器
#if ( OS_TIMER_MAX > 0 )
typedef struct 
{
	os_u32 timer_counter;
	os_u32 timer_reload;	
	void (*timer_count_handler)(void);
	void (*timer_handler)(void);
}OS_TIMER;
#endif

//空闲任务栈声明
extern os_astk APP_IDLE_STK[STK_IDLE_SIZE];

//APP栈内存管理信息
typedef struct 
{
	os_u32 offset;
}OS_STACK_INFO;
extern OS_STACK_INFO os_stack_info;

//APP控制块类型定义
typedef struct 
{
	os_stk  *stk_ptr;  	//APP栈指针 (必须是第一个)  
	os_stk  *stk_ptr_bpk;//APP栈指针(栈地址备份)
	os_u32  *app_ptr;   //APP函数地址
	os_u32  stk_size;   //APP栈大小
	os_u32  delay_clock;//APP延时时间
	os_u32  timeout_clock;//APP超时时间
	os_u32  timeout_min;//APP超时时间历史最小值
	os_u32  timeout_load;//APP超时时间重载值
	os_u8   state;		//APP状态
	os_u8   prio;     	//APP优先级
#if OS_APP_NAME_SIZE>0
	char    name[OS_APP_NAME_SIZE+1];
#endif
}OS_ACB; 
extern OS_ACB os_acb_list[OS_APP_MAX]; //各个APP的ACB，数组方式(标号0-空闲APP，标号1-app_01--标号n-app_n)
extern OS_ACB *os_acb_cur;  //当前正在运行的APP
extern OS_ACB *os_acb_rdy; //下个即将运行的APP

//系统变量类型定义
typedef struct
{
	os_u32 clock_counter;      //系统时钟节拍累计变量
	os_u32 app_slicetime;
	os_u32 app_slicetime_counter;//APP时间切片(为系统时钟节拍倍数,APP持续运行的时间
	os_u32 lock_nesting;	//APP切换锁定层数统计变量 
	os_u32 int_nesting;		//APP中断层数统计变量
	os_u8 app_cur_num;  	//当前运行态APP的ACB组数标号
	os_bool print_sta;
	os_bool debug_print_sta;
	os_u8 run_sta;
#if (OS_APP_MAX	< 16 )
	os_u8 app_rdy_bit_tbl[4];
#else
  os_u8 app_rdy_bit_tbl[8];
#endif	
	os_u8 app_rdy_grp_bit_tbl;
	os_u8 enter_shell;
}OS_SYS;
extern OS_SYS os_sys;//系统变量


void os_main(void);									   //系统主函数(主要用来创建APP)
void API_os_irq_disable(void);                         //关闭全局中断函数(在汇编中)            
void API_os_irq_enable(void);                          //打开全局中断函数(在汇编中) 
void os_init(void);									   //系统初始化函数
void os_startup(void);                                 //系统启动函数
void API_os_systick_init(void);                        //系统定时器初始化函数 
void API_os_systick_handler(void);					   //系统定时器中断函数(把此函数放在硬件定时器中断函数中)
void API_os_systick_on(void);                          //系统定时器打开
void API_os_systick_off(void);                         //系统定时器关闭
os_u32 API_os_systick_1us_value_get(void);			   //获取定时器1us所需计数次数
os_u32 API_os_systick_value_get(void);				   //获取定时器当前值
os_u32 API_os_systick_reload_get(void);				   //获取定时器自动重装值
void API_os_systick_irq_on(void);                      //系统定时器中断打开
void API_os_systick_irq_off(void);                     //系统定时器中断关闭
void API_os_svc_init(void);                            //软件触发切换初始化函数
void os_systick_handler_hook(void);                    //systcik中断钩子函数
void os_user_init(void);							   //用户代码初始化函数(用户的初使化代码放在此函数中)
void API_os_psp_reset(void);                           //PSP指针初使化
void os_switch(void);								   //APP切换
void os_scheduler(void);                               //APP调度器
void API_os_sched_pulse(void);                         //APP环境切换函数 
void os_sched_and_switch(void);                        //APP调度并切换函数    
void app_rdy_tbl_set(os_u8 prio);                      //就绪列表置位表示 已就绪
void app_rdy_tbl_reset(os_u8 prio);                    //就绪列表复位表示 未就绪
void os_app_rdy_num_update(void);                      //更新下个运行的APP
os_stk* os_app_stk_init(void* app,os_stk *topstkptr);  //APP堆栈初使化函数
void os_app_shell_create(void);						   //shellAPP创建函数
void app_idle(void);                                   //空闲APP函数体

//用户可用API 
os_bool os_app_create(os_u8 prio,char *name_ptr,fun app,os_u32 stk_size,os_u32 timeout,os_u8 state);//APP创建函数
void os_app_suspend(fun app);				 	//挂起APP(任何)
void os_app_resume(fun app);					//恢复APP(对其他APP,非自己)
void os_app_prio_set(fun app,os_u8 new_prio); 	//设置APP的优先级为new_prio(对外)
void os_app_restart(fun app);				 	//重新启动APP(任何)
void os_app_restart_other(fun app);				//重新启动APP(对其他APP,非自己)
void os_app_delete(fun app);               		//删除APP(任何)
void os_lock(void);                              //APP切换上锁函数
void os_unlock(void);                            //APP切换解锁函数
void os_app_slicetime_set(os_u32 ms);            //设置APP时间切片
os_u32 os_clock_get(void);                       //获取系统时钟节拍数函数
void os_timer_count_interrupt_set(os_u8 id,fun function);//设置系统软件定时器计数中断函数(只能在里面作变量计数累加)
void os_timer_interrupt_set(os_u8 id,fun function);//设置系统软件定时器中断函数(能在里面调用其他函数)
void os_timer_reload_set(os_u8 id,os_u32 tick);  //设置系统软件定时器重载计数值(调用此函数后会自动重载计数)
void os_timer_value_set(os_u8 id,os_u32 tick);   //设置系统软件定时器重新计数值(调用此函数后会取消自动重载计数功能)
os_u32 os_timer_value_get(os_u8 id);             //获取软件定时器当前计数值

void os_app_sleep(os_u32 ms);                    //APP休眠函数(ms)
void delay_us(os_u32 us);                        //微秒延时函数(us)
void delay_ms(os_u32 ms);	                     //毫秒延时函数(ms)

void os_debug_print_enable(void);  				 //使能debug打印输出
void os_debug_print_disable(void);				 //禁止debug打印输出

os_bool os_app_flag_pend(os_u8 mod,os_u8 id);    //等待标志量，mod=0或APP_BLOCK阻塞，=1或APP_UNBLOCK非阻塞(返回OS_TRUE表示收到POST)
os_bool os_app_flag_post(os_u8 id);              //发送标志量
os_bool os_app_value_pend(os_u8 mod,os_u8 id,double *value_out);   //等待数值量，mod=0或APP_BLOCK阻塞，=1或APP_UNBLOCK非阻塞(返回OS_TRUE表示收到POST)
os_bool os_app_value_post(os_u8 id,double *value_in);             //发送数值量
os_bool os_app_mutex_pend(os_u8 mod,os_u8 id);   //等待进入互斥状态，mod=0阻塞，=1非阻塞(返回OS_TRUE表示收到POST)
os_bool os_app_mutex_post(os_u8 id);             //退出互斥状态
os_bool os_mbox_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len);//等待邮件，mod=0阻塞，=1非阻塞(返回OS_TRUE表示成功取出邮件)
os_bool os_mbox_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len);//发送邮件(参数：邮箱ID，数据地址，数据长度)
os_bool os_mbox_queue_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len);//发送邮箱队列邮件
os_bool os_mbox_queue_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len);//等待邮箱队列邮件mod=0阻塞，=1非阻塞(返回OS_TRUE表示队列成功取出队头邮件)
#endif
