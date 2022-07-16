#ifndef _EOS_PLUGIN_SHELL_H_
#define _EOS_PLUGIN_SHELL_H_
#include "eos.h"

#if (_USE_SHELL_ == 1)

#define os_app_shell_prio OS_APP_MAX-1

#define SHELL_RXBUFF_LEN     1024
#define SHELL_RXBUFF_DIG_MAX 8		//字符串转数字最大个数（可根据实际进行修改）

//安全参数
typedef struct
{
	os_u8 bool_super;//超级用户权限
	os_u8 bool_requesting;//正在请求权限
	os_u8 bool_access;//访问权限
	os_u8 bool_qflash;//外部flash
	os_u8 bool_iflash;//内部flash(ROM)
	os_u8 bool_hidecmd_enable;//超级用户权限
	os_u32 timeout;//超时时间
	os_u32 cooltime;//冷却时间(连续请求错误后的禁止请求时间)
	os_u32 request_times;//连续请求次数限制	
	os_u32 request_count;//连续请求次数计数	
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

//系统函数
void os_hard_reset(void);	

#endif

//接口函数(二选一，注意，空闲中断的指令前面要有起始字符串eos$:)
void os_shell_input(os_u8 rx_byte);//串口接口函数(放在串口接收中断里)
void os_shell_inputs(os_u8 *rx_bytes,os_u32 len);//串口接口函数(放在串口空闲接收中断里)


//用户可调用函数(字符串处理)
void os_shell_string_move(char *str,int mlen);
os_u32 os_shell_string_len_get(char* p);
os_u32 os_char_in_string_position_get(char* p,char ch);
void string_float_convert(char* ptr_source,double *outbuff,os_u32 len);
void string_int_convert(char* ptr_source,int *outbuff,os_u32 len);
void string_uint_convert(char* ptr_source,os_u32 *outbuff,os_u32 len);
void string_hex_convert(char* ptr_source,os_u8 *outbuff,os_u32 len);
void string_hex32_convert(char* ptr_source,os_u32 *outbuff,os_u32 len);

#endif

