#include "eos.h"

//��ֲʱ���и�����ѡ��Ӳ����ʱ����ָ�������ϵͳ�ṩ��API_os_xxx()����


//��һ����
/*------------------------------------------------ϵͳ��ʱ��----------------------------------------------*/
//ϵͳ��ʱ��Ƶ��(STM32��Ϊϵͳ��Ƶ/8��Ƶ) -����mcu�޸���Ƶ(stm32����ƵΪSystemCoreClock)
#define SystickCoreClock 	SystemCoreClock //( SystemCoreClock / 8 )
/*��ʱ����ʹ��(�˴���Cortex-M�ں�ƽ̨��SysTick��ʱ������ϵeOSϵͳ��ʱ��)
*ARM-M:SysTick:CTRL�Ĵ�����bit0-��ʱ��ʹ�� bit1-�ж�ʹ�� bit2-ʱ��Դѡ��(=1ϵͳ��Ƶ��=0ϵͳ��Ƶ/8)  bit16-�������λ*/
void API_os_systick_init(void)//��ʱ����ʹ��
{ 	
	//��ֲ���
	char *Systick_priority = (char *)0xe000ed23;//Systick�ж����ȼ��Ĵ���
	SysTick->LOAD  = SystickCoreClock/1000-1; //Systick��ʱ����װ�ؼ���ֵ:1ms(�̶�ֵ)
	*Systick_priority = 0x00;               //Systick��ʱ���ж����ȼ�
	SysTick->VAL   = 0;                     //Systick��ʱ����������0
	SysTick->CTRL = (1<<2)|(1<<0);//Systick����ʹ���ⲿ����ʱ��,8��Ƶ  72MHz/8=9MHz  ����9000��=1ms  ����9��=1us
}
void API_os_systick_on(void)//��ʱ����
{
	//��ֲ���
	SysTick->CTRL |= 0x1;
}
void API_os_systick_off(void)//��ʱ���ر�
{
	//��ֲ���
	SysTick->CTRL &= ~(0x1);
}
os_u32 API_os_systick_1us_value_get(void)//��ȡ��ʱ��1us�����������
{
	//��ֲ���
	return (SystickCoreClock/1000000);//��ʱ����Ƶ/1000000=1us��������
}
os_u32 API_os_systick_value_get(void)//��ȡ��ʱ����ǰֵ
{
	//��ֲ���
	return (SysTick->VAL);
}
os_u32 API_os_systick_reload_get(void)//��ȡ��ʱ���Զ���װֵ
{
	//��ֲ���
	return (SysTick->LOAD);
}
void API_os_systick_irq_on(void)//��ʱ���ж�ʹ��
{
	//��ֲ���
	SysTick->CTRL |= 0x2;
}
void API_os_systick_irq_off(void)//��ʱ���ж�ʧ��
{
	//��ֲ���
	SysTick->CTRL &= ~(0x2);
}
//��ֲ����
void SysTick_Handler(void) //��ʱ���жϺ���(��ֲʱ�޸ĳɶ�Ӧ��Ӳ����ʱ���жϺ�����,����Systick��ʱ�����ø�)
{
	API_os_systick_handler(); //ϵͳ�ӿں���(�����޸�)
}


//�ڶ�����
/*-------------------------------------------------ϵͳ�л�-----------------------------------------------*/
//����жϳ�ʹ������������жϺ���(�ж�����APP�����л�����,�жϺ�����ASM�ļ�)
void API_os_svc_init(void)//PendSV��ʹ��
{
	//��ֲ���
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //���ȼ��Ĵ�����ַ
	*NVIC_SYSPRI14=0xFF;//����PendSV�ж����ȼ���� 
}
void API_os_sched_pulse(void)//����PendSV�ж�
{
	API_os_irq_disable();//ϵͳ�ӿں���(�����޸�)
	
	//��ֲ���
	/*--------------------------------------------------*/
	int* NVIC_INT_CTRL= (int *)0xE000ED04;  //�жϿ��ƼĴ�����ַ
	*NVIC_INT_CTRL=0x10000000; //��λ��������ж�
	/*--------------------------------------------------*/

	API_os_irq_enable();//ϵͳ�ӿں���(�����޸�)
}

