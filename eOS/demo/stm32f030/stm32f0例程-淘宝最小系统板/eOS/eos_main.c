#include "eos.h"
#include "led.h"
#include "usart.h"

void app_led(void);
void app_rtc(void);
void app_temp(void);

void os_user_init(void) //�û������ʹ������(delay_us/ms()�ڴ���Ч,os_delay()��ֹ)
{
	  LED_Init();
	  USART1_Init(115200); //����1��ʹ��
	  printf("-@eOS_V3.1.4 Inside\r\n\r\n");
}

void os_main(void)
{   
	os_init();
	
	/*         APP_ID  �������ַ���     ������ 		ջ��С   ��ʱʱ�� 	APP״̬     */
	os_app_create(1, "app_led", app_led, 64,  500,	APP_STATE_RUNNING);
	os_app_create(2, "app_rtc", app_rtc, 64,  500,	APP_STATE_RUNNING);
	os_app_create(3, "app_temp", app_temp, 64,  500,	APP_STATE_RUNNING);
	
    os_startup();
}

void app_led(void)  //APP
{  	
	while(1) 
	 {	
		  LED_ON();
			os_printf("led on\r\n");
		  os_app_sleep(500);
		  LED_OFF();
		 	os_printf("led off\r\n");
		  os_app_sleep(500);
   } 
}
void app_rtc(void)  //APP
{   
	os_u8 hour=6,munite=0,second=0;
	while(1) 
	 {	 		 
		 if(second<59) second++; 
		 else 
		 {
			 second=0;
			 munite++;
			 if(munite==60) 
			 {
			   second=0;
				 munite=0;
				 hour++;
				 if(hour==24)
				 {
				 	 second=0;
				   munite=0;
					 hour=0;
				 }
			 }
		 }
		 os_printf("real time : 1949.10.01  %02d:%02d:%02d\r\n",hour,munite,second);
		 os_app_sleep(1000);
   } 
}

void app_temp(void)  //APP
{  
	while(1) 
	 {	
		  os_lock();
			os_printf("temp read\r\n");
		  os_unlock();
		  os_app_sleep(3000);
   } 
}
