#ifndef _LED_H_
#define _LED_H_
#include "eos.h"


typedef enum
{
	LED_STATE_OFF,
	LED_STATE_ON,
	LED_STATE_TOGGLE
}LED_STATE;


void led_init(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
void led_state_control(LED_STATE led_state);

#endif
