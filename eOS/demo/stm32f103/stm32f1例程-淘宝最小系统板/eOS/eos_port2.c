#include "eos.h"

//移植时自行根据所选的硬件定时器的指令来填充系统提供的API_os_xxx()函数


//第一部分
/*------------------------------------------------系统定时器----------------------------------------------*/
//系统定时器频率(STM32中为系统主频/8分频) -根据mcu修改主频(stm32中主频为SystemCoreClock)
#define SystickCoreClock 	SystemCoreClock //( SystemCoreClock / 8 )
/*定时器初使化(此处是Cortex-M内核平台的SysTick定时器用作系eOS系统定时器)
*ARM-M:SysTick:CTRL寄存器：bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位*/
void API_os_systick_init(void)//定时器初使化
{ 	
	//移植填充
	char *Systick_priority = (char *)0xe000ed23;//Systick中断优先级寄存器
	SysTick->LOAD  = SystickCoreClock/1000-1; //Systick定时器重装载计数值:1ms(固定值)
	*Systick_priority = 0x00;               //Systick定时器中断优先级
	SysTick->VAL   = 0;                     //Systick定时器计数器清0
	SysTick->CTRL = (1<<2)|(1<<0);//Systick打开且使用外部晶振时钟,8分频  72MHz/8=9MHz  计数9000次=1ms  计数9次=1us
}
void API_os_systick_on(void)//定时器打开
{
	//移植填充
	SysTick->CTRL |= 0x1;
}
void API_os_systick_off(void)//定时器关闭
{
	//移植填充
	SysTick->CTRL &= ~(0x1);
}
os_u32 API_os_systick_1us_value_get(void)//获取定时器1us所需计数次数
{
	//移植填充
	return (SystickCoreClock/1000000);//定时器主频/1000000=1us计数次数
}
os_u32 API_os_systick_value_get(void)//获取定时器当前值
{
	//移植填充
	return (SysTick->VAL);
}
os_u32 API_os_systick_reload_get(void)//获取定时器自动重装值
{
	//移植填充
	return (SysTick->LOAD);
}
void API_os_systick_irq_on(void)//定时器中断使能
{
	//移植填充
	SysTick->CTRL |= 0x2;
}
void API_os_systick_irq_off(void)//定时器中断失能
{
	//移植填充
	SysTick->CTRL &= ~(0x2);
}
//移植改名
void SysTick_Handler(void) //定时器中断函数(移植时修改成对应的硬件定时器中断函数名,如是Systick定时器则不用改)
{
	API_os_systick_handler(); //系统接口函数(不需修改)
}


//第二部分
/*-------------------------------------------------系统切换-----------------------------------------------*/
//软件中断初使化及软件触发中断函数(中断里有APP环境切换代码,中断函数在ASM文件)
void API_os_svc_init(void)//PendSV初使化
{
	//移植填充
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //优先级寄存器地址
	*NVIC_SYSPRI14=0xFF;//设置PendSV中断优先级最低 
}
void API_os_sched_pulse(void)//触发PendSV中断
{
	API_os_irq_disable();//系统接口函数(不需修改)
	
	//移植填充
	/*--------------------------------------------------*/
	int* NVIC_INT_CTRL= (int *)0xE000ED04;  //中断控制寄存器地址
	*NVIC_INT_CTRL=0x10000000; //置位触发软件中断
	/*--------------------------------------------------*/

	API_os_irq_enable();//系统接口函数(不需修改)
}

