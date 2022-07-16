#ifndef _OS_H_
#define _OS_H_
#include "main.h" 
//#include "dwg.h"

//�汾:V3.1.5
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

/********************************************OS��������*************************************************************/
#define OS_APP_USER_MAX       5			//�û�APP����(0-62,�ڴ�ʹ��ΪN*(10~12)) bytes)
#define OS_STACK_SIZE         1024*2	//�û�APPջ�ڴ�,��λ��word
#define OS_APP_SLICETIME      10		//APPʱ����Ƭ,��λ��tick(Ĭ��1ms,��eos_port2.c/systick_init()�޸�)(��Χ��1-200)  
#define STK_IDLE_SIZE         64		//����APPջ���� (�ڴ�ʹ��ΪN*4) bytes) >=32
#define STK_CORE_SIZE 		  128		//ϵͳ����ջ����	 (�ڴ�ʹ��ΪN*4) bytes) >=32	�����û�APP����+��ʱ�������йأ�	
#define STK_CORE_SLICETIME 	  100		//ϵͳ������Ӧʱ��,��λ��tick
#define STK_SHELL_SIZE        256		//APP_SHELLջ���� >150��ʹ��shell���ʱ������
#define OS_APP_NAME_SIZE      24		//APP�ַ�������		
	
#define STK_IDLE_TIMEOUT      10001    	//����APP��ʱ����ʱ��,��λ��tick
#define STK_CORE_TIMEOUT      1001    	//����APP��ʱ����ʱ��,��λ��tick
	
#define OS_TIMER_MAX	      6			//ϵͳ��ʱ������(ID����)
	
#define OS_FLAG_MAX		  	  0	 		//APPͬ����־������(ID����)  ��0-255
#define OS_VALUE_MAX	  	  0	 		//APPͬ����ֵ������(ID����)  ��0-255
#define OS_MUTEX_MAX	  	  0  		//APP����������(ID����)  ��	   0-255									    
#define OS_MBOX_MAX	          0			//APPͬ������/�ַ�������(ID����)  ��0-255
#define OS_MBOX_QUEUE_MAX	  0			//APP�첽����/�ַ�������(ID����)  ��0-255
	
#define OS_MAIL_SIZE		  32		//ϵͳ�ʼ�����
#define OS_QUEUE_SIZE		  16		//����������(1��queue_id�ܻ�����ʼ���)
	
#define _RAM_ALGO_            0			//0-rom,1-ram,�㷨���鱣��λ�á�
#define _OS_DEBUG_PRINTF_	  1			//0-��ֹos_printf��ӡ�����1-�����ӡ���
	
#define _USE_SHELL_           1			//0-������shell,1-����shell�����eos_plugin_shell�ļ�����Ч��
/****************************************-**OS ���Ź�����**********************************************************/

//�ṩ��OS��ι����������������˿��Ź�������������,δʹ�������ע�͵���
//#define	os_dwg_feed_api()	fwdgt_feed()//ι������ 

/************************************************************************************************************/
//���´���Ͳ��������޹�

//OS�ṩ�Ĵ�ӡ������� 
//#define os_printf(fmt, ...) { os_lock();printf( fmt, ##__VA_ARGS__);os_unlock();}//OS�ṩ�Ĵ�ӡ�������
#define os_printf(fmt, ...) { if(os_sys.print_sta==OS_TRUE){os_lock();if(os_sys.enter_shell==OS_FALSE){printf( fmt, ##__VA_ARGS__);}os_unlock();}}//OS�ṩ�Ĵ�ӡ�������
#define os_debug_printf(fmt, ...) {if(os_sys.debug_print_sta==OS_TRUE){ os_lock();if(os_sys.enter_shell==OS_FALSE){printf( fmt, ##__VA_ARGS__);}os_unlock();}}//OS�ṩ�Ĵ�ӡ�������
#define os_shell_printf(fmt, ...) { os_lock(); printf( fmt, ##__VA_ARGS__); os_unlock();}//OS SHELL�ṩ�Ĵ�ӡ�������



//APP״̬
typedef enum
{
	APP_STATE_RUNNING,//����̬
	APP_STATE_PENDING,//����̬
	APP_STATE_RESTART,//����̬
	APP_STATE_DELETED//ɾ��̬
}APP_STATE;

//ϵͳ��APP����
#define OS_APP_MAX (OS_APP_USER_MAX+2+_USE_SHELL_)//����1����������(����ȼ�)��1��ϵͳ��������(��ʹ��shell�����Ϊ������ȼ�)
/*
������Ϊ����Ҫ��������ȼ�����app_shell������shell������������ӵ��rootȨ�ޡ� 
*/

#if (OS_APP_USER_MAX>62) 
#error OS_APP_MAX must Less than 63!
#endif

extern os_astk OS_STACK[OS_STACK_SIZE];

//ϵͳ����ֵ
typedef enum 
{
	OS_FALSE=0x00, 
	OS_TRUE=0xFF,
}os_bool; 

//pend��ʽ
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

//OS�����ʱ��
#if ( OS_TIMER_MAX > 0 )
typedef struct 
{
	os_u32 timer_counter;
	os_u32 timer_reload;	
	void (*timer_count_handler)(void);
	void (*timer_handler)(void);
}OS_TIMER;
#endif

//��������ջ����
extern os_astk APP_IDLE_STK[STK_IDLE_SIZE];

//APPջ�ڴ������Ϣ
typedef struct 
{
	os_u32 offset;
}OS_STACK_INFO;
extern OS_STACK_INFO os_stack_info;

//APP���ƿ����Ͷ���
typedef struct 
{
	os_stk  *stk_ptr;  	//APPջָ�� (�����ǵ�һ��)  
	os_stk  *stk_ptr_bpk;//APPջָ��(ջ��ַ����)
	os_u32  *app_ptr;   //APP������ַ
	os_u32  stk_size;   //APPջ��С
	os_u32  delay_clock;//APP��ʱʱ��
	os_u32  timeout_clock;//APP��ʱʱ��
	os_u32  timeout_min;//APP��ʱʱ����ʷ��Сֵ
	os_u32  timeout_load;//APP��ʱʱ������ֵ
	os_u8   state;		//APP״̬
	os_u8   prio;     	//APP���ȼ�
#if OS_APP_NAME_SIZE>0
	char    name[OS_APP_NAME_SIZE+1];
#endif
}OS_ACB; 
extern OS_ACB os_acb_list[OS_APP_MAX]; //����APP��ACB�����鷽ʽ(���0-����APP�����1-app_01--���n-app_n)
extern OS_ACB *os_acb_cur;  //��ǰ�������е�APP
extern OS_ACB *os_acb_rdy; //�¸��������е�APP

//ϵͳ�������Ͷ���
typedef struct
{
	os_u32 clock_counter;      //ϵͳʱ�ӽ����ۼƱ���
	os_u32 app_slicetime;
	os_u32 app_slicetime_counter;//APPʱ����Ƭ(Ϊϵͳʱ�ӽ��ı���,APP�������е�ʱ��
	os_u32 lock_nesting;	//APP�л���������ͳ�Ʊ��� 
	os_u32 int_nesting;		//APP�жϲ���ͳ�Ʊ���
	os_u8 app_cur_num;  	//��ǰ����̬APP��ACB�������
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
extern OS_SYS os_sys;//ϵͳ����


void os_main(void);									   //ϵͳ������(��Ҫ��������APP)
void API_os_irq_disable(void);                         //�ر�ȫ���жϺ���(�ڻ����)            
void API_os_irq_enable(void);                          //��ȫ���жϺ���(�ڻ����) 
void os_init(void);									   //ϵͳ��ʼ������
void os_startup(void);                                 //ϵͳ��������
void API_os_systick_init(void);                        //ϵͳ��ʱ����ʼ������ 
void API_os_systick_handler(void);					   //ϵͳ��ʱ���жϺ���(�Ѵ˺�������Ӳ����ʱ���жϺ�����)
void API_os_systick_on(void);                          //ϵͳ��ʱ����
void API_os_systick_off(void);                         //ϵͳ��ʱ���ر�
os_u32 API_os_systick_1us_value_get(void);			   //��ȡ��ʱ��1us�����������
os_u32 API_os_systick_value_get(void);				   //��ȡ��ʱ����ǰֵ
os_u32 API_os_systick_reload_get(void);				   //��ȡ��ʱ���Զ���װֵ
void API_os_systick_irq_on(void);                      //ϵͳ��ʱ���жϴ�
void API_os_systick_irq_off(void);                     //ϵͳ��ʱ���жϹر�
void API_os_svc_init(void);                            //��������л���ʼ������
void os_systick_handler_hook(void);                    //systcik�жϹ��Ӻ���
void os_user_init(void);							   //�û������ʼ������(�û��ĳ�ʹ��������ڴ˺�����)
void API_os_psp_reset(void);                           //PSPָ���ʹ��
void os_switch(void);								   //APP�л�
void os_scheduler(void);                               //APP������
void API_os_sched_pulse(void);                         //APP�����л����� 
void os_sched_and_switch(void);                        //APP���Ȳ��л�����    
void app_rdy_tbl_set(os_u8 prio);                      //�����б���λ��ʾ �Ѿ���
void app_rdy_tbl_reset(os_u8 prio);                    //�����б�λ��ʾ δ����
void os_app_rdy_num_update(void);                      //�����¸����е�APP
os_stk* os_app_stk_init(void* app,os_stk *topstkptr);  //APP��ջ��ʹ������
void os_app_shell_create(void);						   //shellAPP��������
void app_idle(void);                                   //����APP������

//�û�����API 
os_bool os_app_create(os_u8 prio,char *name_ptr,fun app,os_u32 stk_size,os_u32 timeout,os_u8 state);//APP��������
void os_app_suspend(fun app);				 	//����APP(�κ�)
void os_app_resume(fun app);					//�ָ�APP(������APP,���Լ�)
void os_app_prio_set(fun app,os_u8 new_prio); 	//����APP�����ȼ�Ϊnew_prio(����)
void os_app_restart(fun app);				 	//��������APP(�κ�)
void os_app_restart_other(fun app);				//��������APP(������APP,���Լ�)
void os_app_delete(fun app);               		//ɾ��APP(�κ�)
void os_lock(void);                              //APP�л���������
void os_unlock(void);                            //APP�л���������
void os_app_slicetime_set(os_u32 ms);            //����APPʱ����Ƭ
os_u32 os_clock_get(void);                       //��ȡϵͳʱ�ӽ���������
void os_timer_count_interrupt_set(os_u8 id,fun function);//����ϵͳ�����ʱ�������жϺ���(ֻ�������������������ۼ�)
void os_timer_interrupt_set(os_u8 id,fun function);//����ϵͳ�����ʱ���жϺ���(�������������������)
void os_timer_reload_set(os_u8 id,os_u32 tick);  //����ϵͳ�����ʱ�����ؼ���ֵ(���ô˺�������Զ����ؼ���)
void os_timer_value_set(os_u8 id,os_u32 tick);   //����ϵͳ�����ʱ�����¼���ֵ(���ô˺������ȡ���Զ����ؼ�������)
os_u32 os_timer_value_get(os_u8 id);             //��ȡ�����ʱ����ǰ����ֵ

void os_app_sleep(os_u32 ms);                    //APP���ߺ���(ms)
void delay_us(os_u32 us);                        //΢����ʱ����(us)
void delay_ms(os_u32 ms);	                     //������ʱ����(ms)

void os_debug_print_enable(void);  				 //ʹ��debug��ӡ���
void os_debug_print_disable(void);				 //��ֹdebug��ӡ���

os_bool os_app_flag_pend(os_u8 mod,os_u8 id);    //�ȴ���־����mod=0��APP_BLOCK������=1��APP_UNBLOCK������(����OS_TRUE��ʾ�յ�POST)
os_bool os_app_flag_post(os_u8 id);              //���ͱ�־��
os_bool os_app_value_pend(os_u8 mod,os_u8 id,double *value_out);   //�ȴ���ֵ����mod=0��APP_BLOCK������=1��APP_UNBLOCK������(����OS_TRUE��ʾ�յ�POST)
os_bool os_app_value_post(os_u8 id,double *value_in);             //������ֵ��
os_bool os_app_mutex_pend(os_u8 mod,os_u8 id);   //�ȴ����뻥��״̬��mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
os_bool os_app_mutex_post(os_u8 id);             //�˳�����״̬
os_bool os_mbox_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len);//�ȴ��ʼ���mod=0������=1������(����OS_TRUE��ʾ�ɹ�ȡ���ʼ�)
os_bool os_mbox_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len);//�����ʼ�(����������ID�����ݵ�ַ�����ݳ���)
os_bool os_mbox_queue_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len);//������������ʼ�
os_bool os_mbox_queue_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len);//�ȴ���������ʼ�mod=0������=1������(����OS_TRUE��ʾ���гɹ�ȡ����ͷ�ʼ�)
#endif
