#include "led.h"

static u8 led_state=0;

//LED  -  PC13
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	led_state=0;
}
void led_on(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	led_state=1;
}
void led_off(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	led_state=0;
}
void led_toggle(void)
{
	if(led_state) led_off();
	else led_on();
}
void led_state_control(LED_STATE led_state)
{
	switch(led_state)
	{
		case LED_STATE_OFF: led_off();break;
		case LED_STATE_ON: led_on();break;
		case LED_STATE_TOGGLE: led_toggle();break;	
	}
}
