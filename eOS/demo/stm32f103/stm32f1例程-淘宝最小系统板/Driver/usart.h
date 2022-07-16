#ifndef _USART_
#define _USART_
#include "eos.h"


void usart1_init(u32 baudRate);
void usart1_send_bytes(u8 *p,u32 len);

#endif
