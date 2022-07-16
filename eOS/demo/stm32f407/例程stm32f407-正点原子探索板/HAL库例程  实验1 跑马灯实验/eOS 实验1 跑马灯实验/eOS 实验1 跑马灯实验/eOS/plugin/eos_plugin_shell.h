#ifndef _EOS_PLUGIN_SHELL_H_
#define _EOS_PLUGIN_SHELL_H_
#include "eos.h"
#include "stdio.h" 
#include "string.h" 

#if (_USE_SHELL_ == 1)

#define os_app_shell_prio OS_APP_MAX-1

#define SHELL_RXBUFF_LEN     1024
#define SHELL_RXBUFF_DIG_MAX 8		//�ַ���ת�������������ɸ���ʵ�ʽ����޸ģ�

//��ȫ����
typedef struct
{
	os_u8 bool_super;//�����û�Ȩ��
	os_u8 bool_requesting;//��������Ȩ��
	os_u8 bool_access;//����Ȩ��
	os_u8 bool_qflash;//�ⲿflash
	os_u8 bool_iflash;//�ڲ�flash(ROM)
	os_u8 bool_hidecmd_enable;//�����û�Ȩ��
	os_u32 timeout;//��ʱʱ��
	os_u32 cooltime;//��ȴʱ��(������������Ľ�ֹ����ʱ��)
	os_u32 request_times;//���������������	
	os_u32 request_count;//���������������	
}SECURE_PARAM;
extern SECURE_PARAM secure_param;

typedef struct
{
	char rx_buff[SHELL_RXBUFF_LEN];
	double float_buff[SHELL_RXBUFF_DIG_MAX];
	int int_buff[SHELL_RXBUFF_DIG_MAX];
	os_u32 uint_buff[SHELL_RXBUFF_DIG_MAX];
	os_u32 hex32_buff[SHELL_RXBUFF_DIG_MAX];
	os_u8 hex_buff[SHELL_RXBUFF_DIG_MAX];
	os_u32 rx_counter;
	os_bool bool_rx_ok;
	os_bool bool_enter_sys;
}OS_SHELL_CMD;
extern OS_SHELL_CMD os_shell_cmd;

typedef struct
{	
	os_u32 runtime_year;
	os_u32 shutdowntime_year;
	
	os_u16 runtime_ms;
	
	os_u8  runtime_second;
	os_u8  runtime_minute;
	os_u8  runtime_hour;
	os_u8  runtime_day;
	os_u8  runtime_mouth;
	
	os_u8  shutdowntime_second;
	os_u8  shutdowntime_minute;
	os_u8  shutdowntime_hour;
	os_u8  shutdowntime_day;
	os_u8  shutdowntime_mouth;
}OS_SHELL_TIME;
extern OS_SHELL_TIME os_shell_time;

typedef struct
{	
	os_stk     *stk_ptr[OS_APP_MAX];
	os_u32     stk_size_used[OS_APP_MAX];
	os_u32     stk_size_free[OS_APP_MAX];
	os_u32     app_cpu_occrate_counter[OS_APP_MAX];
	os_u16     cpu_occrate_per1000;
	os_u16     cpu_occrate_per1000_max;
	os_u16     app_cpu_occrate_per1000[OS_APP_MAX];
}OS_SHELL_DATA;
extern OS_SHELL_DATA os_shell_data;

//ϵͳ����
void os_hard_reset(void);	

#endif

//�ӿں���(��ѡһ��ע�⣬�����жϵ�ָ��ǰ��Ҫ����ʼ�ַ���eos$:)
void os_shell_input(os_u8 rx_byte);//���ڽӿں���(���ڴ��ڽ����ж���)
void os_shell_inputs(os_u8 *rx_bytes,os_u32 len);//���ڽӿں���(���ڴ��ڿ��н����ж���)


//�û��ɵ��ú���(�ַ�������)
void os_shell_string_move(char *str,int mlen);
os_u32 os_shell_string_len_get(char* p);
os_u32 os_char_in_string_position_get(char* p,char ch);
void string_float_convert(char* ptr_source,double *outbuff,os_u32 len);
void string_int_convert(char* ptr_source,int *outbuff,os_u32 len);
void string_uint_convert(char* ptr_source,os_u32 *outbuff,os_u32 len);
void string_hex_convert(char* ptr_source,os_u8 *outbuff,os_u32 len);
void string_hex32_convert(char* ptr_source,os_u32 *outbuff,os_u32 len);

#endif

