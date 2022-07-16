#include "eos.h"

/*----------------
����: eOS.c
�汾��V3.1.5
ʱ�䣺2022-07-15
-----------------*/

//�û�APPջ�ڴ��
os_astk OS_STACK[OS_STACK_SIZE]={0};
OS_STACK_INFO os_stack_info={0};

//ACB����
OS_ACB os_acb_list[OS_APP_MAX]={0}; 
OS_ACB *os_acb_cur; 
OS_ACB *os_acb_rdy;

//ϵͳ����
OS_SYS os_sys={0};

//����APPջ(������ʵ��)
os_astk APP_IDLE_STK[STK_IDLE_SIZE]={0};

//ϵͳ����APPջ(������ʵ��)
os_astk APP_CORE_STK[STK_CORE_SIZE]={0};

//os_flag\value\mutex\mbox\mbqueue
#if ( OS_FLAG_MAX > 0 )
#if ( OS_FLAG_MAX%8 == 0 )
os_u8 os_app_flag[OS_FLAG_MAX/8];
#else
os_u8 os_app_flag[OS_FLAG_MAX/8+1];
#endif
#endif
#if ( OS_VALUE_MAX > 0 )
double os_app_value[OS_VALUE_MAX];
#if ( OS_VALUE_MAX%8 == 0 )
os_u8 os_app_flag_value[OS_VALUE_MAX/8];
#else
os_u8 os_app_flag_value[OS_VALUE_MAX/8+1];
#endif
#endif
#if ( OS_MUTEX_MAX > 0 )
os_u8 os_app_mutex[OS_MUTEX_MAX];
#endif
#if ( OS_MBOX_MAX > 0 )
OS_MBOX os_mbox[OS_MBOX_MAX];
#endif
#if ( OS_MBOX_QUEUE_MAX > 0 )
OS_MBOX_QUEUE os_mbox_queue[OS_MBOX_QUEUE_MAX];
#endif
#if ( OS_TIMER_MAX > 0 )
OS_TIMER os_timer[OS_TIMER_MAX];
#endif


//������ȼ���������
#if ( OS_APP_MAX < 16 )
#if ( _RAM_ALGO_ == 1 )
os_u8 BIT_MAP[16]={
0u,0u,1u,1u,2u,2u,2u,2u,3u,3u,3u,3u,3u,3u,3u,3u,/*0x00to0x0F*/
};
#else
os_u8 const BIT_MAP[16]={
0u,0u,1u,1u,2u,2u,2u,2u,3u,3u,3u,3u,3u,3u,3u,3u,/*0x00to0x0F*/
};
#endif
#else
#if ( _RAM_ALGO_ == 1 )
os_u8 BIT_MAP[256]={
0u,0u,1u,1u,2u,2u,2u,2u,3u,3u,3u,3u,3u,3u,3u,3u,/*0x00to0x0F*/
4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,/*0x10to0x1F*/
5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,/*0x20to0x2F*/
5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,/*0x30to0x3F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x40to0x4F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x50to0x5F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x60to0x6F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x70to0x7F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0x80to0x8F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0x90to0x9F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xA0to0xAF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xB0to0xBF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xC0to0xCF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xD0to0xDF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xE0to0xEF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u /*0xF0to0xFF*/
};
#else
os_u8 const BIT_MAP[256]={
0u,0u,1u,1u,2u,2u,2u,2u,3u,3u,3u,3u,3u,3u,3u,3u,/*0x00to0x0F*/
4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,4u,/*0x10to0x1F*/
5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,/*0x20to0x2F*/
5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,5u,/*0x30to0x3F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x40to0x4F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x50to0x5F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x60to0x6F*/
6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,6u,/*0x70to0x7F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0x80to0x8F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0x90to0x9F*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xA0to0xAF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xB0to0xBF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xC0to0xCF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xD0to0xDF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,/*0xE0to0xEF*/
7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u,7u /*0xF0to0xFF*/
};
#endif
#endif

void os_app_create_idle(void);
void os_app_create_core(void);
void os_app_system_core_process(void);
void os_app_idle_core_process(void);



__weak void os_dwg_feed(void)
{
#ifdef os_dwg_feed_api
	os_dwg_feed_api();
#endif
}
__weak void os_systick_handler_hook(void) //ϵͳʱ���жϹ��Ӻ���
{

}
void os_timer_handler(void)
{
#if ( OS_TIMER_MAX > 0 )
	os_u8 i;
	 for(i=0;i<OS_TIMER_MAX;i++)
	 {
		 if(os_timer[i].timer_counter==1)
		 {
			 if(os_timer[i].timer_handler!=(void*)0) os_timer[i].timer_handler();
			 os_timer[i].timer_counter=os_timer[i].timer_reload;    
		 }
	 }
#endif		
}
void os_app_rdy_num_update(void)//����������ȼ�APP�㷨
{
	os_u8 grp;
	grp=BIT_MAP[os_sys.app_rdy_grp_bit_tbl];
#if ( OS_APP_MAX <= 16 )
	os_sys.app_cur_num = (grp<<2) + BIT_MAP[os_sys.app_rdy_bit_tbl[grp]]; //����2�������4
#else	
	os_sys.app_cur_num = (grp<<3) + BIT_MAP[os_sys.app_rdy_bit_tbl[grp]];	//����3�������8
#endif
}
os_bool app_rdy_tbl_state_get(os_u8 prio) //��ȡprio����״̬
{
	os_u8 tprio;
#if ( OS_APP_MAX < 16 )
	tprio=prio>>2;
	if((os_sys.app_rdy_grp_bit_tbl&(1<<tprio))&&(os_sys.app_rdy_bit_tbl[tprio]&(1<<(prio&0x03)))){
		return OS_TRUE;
	}			
#else
	tprio=prio>>3; 
	if(os_sys.app_rdy_grp_bit_tbl&(1<<tprio)&&(os_sys.app_rdy_bit_tbl[tprio]&(1<<(prio&0x07)))){
		return OS_TRUE;
	}		
#endif		
  return OS_FALSE;				
}
void app_rdy_tbl_set(os_u8 prio) //�����б� <��λ>��ʾ �Ѿ���
{
	os_u8 tprio;

#if ( OS_APP_MAX < 16 )
	tprio=prio>>2;
	os_sys.app_rdy_grp_bit_tbl|=1<<tprio;
	os_sys.app_rdy_bit_tbl[tprio]|=1<<(prio&0x03);
#else
	tprio=prio>>3;
	os_sys.app_rdy_grp_bit_tbl|=1<<tprio;
	os_sys.app_rdy_bit_tbl[tprio]|=1<<(prio&0x07);
#endif			
}
void app_rdy_tbl_reset(os_u8 prio) //�����б� <��λ>��ʾ δ����
{
	os_u8 tprio;
#if ( OS_APP_MAX < 16 )
	tprio=prio>>2;
	os_sys.app_rdy_grp_bit_tbl&=~(1<<tprio);
	os_sys.app_rdy_bit_tbl[tprio]&=~(1<<(prio&0x03));
#else
	tprio=prio>>3;
	os_sys.app_rdy_grp_bit_tbl&=~(1<<tprio);
	os_sys.app_rdy_bit_tbl[tprio]&=~(1<<(prio&0x07));
#endif	
}
void API_os_systick_handler(void)//ϵͳ��ʱ���ж� (���˺�������Ӳ����ʱ�����жϺ�����)
{
	os_u8 i;
	for(i=0;i<OS_APP_MAX;i++)
	{
		if(os_acb_list[i].state==APP_STATE_RUNNING)
		{
			if(os_acb_list[i].delay_clock>1) os_acb_list[i].delay_clock--;
			else if(os_acb_list[i].delay_clock==1) app_rdy_tbl_set(os_acb_list[i].prio);
		}
	}
	 os_sys.clock_counter++;  //ϵͳʱ�ӽ����ۼ�
	 os_systick_handler_hook();
#if ( OS_TIMER_MAX > 0 )
	 for(i=0;i<OS_TIMER_MAX;i++)
	 {
		 if(os_timer[i].timer_counter>1) os_timer[i].timer_counter--;
		 if(os_timer[i].timer_counter==1)
		 {
			 if(os_timer[i].timer_count_handler!=(void*)0) os_timer[i].timer_count_handler();
			 os_timer[i].timer_counter=os_timer[i].timer_reload;    
		 }
	 }
#endif	
	 if((--os_sys.app_slicetime_counter) == 0)  
	 {
		 os_sys.app_slicetime_counter = os_sys.app_slicetime;//����ʱ��Ƭ��ֵ
		 
		 for(i=1;i<OS_APP_MAX;i++)//��������ɨ�裬���ȱ�֤������ȼ���APP�ܹ���ʱ����(Ϊ�˿˷���ռʽ��ȱ��)
		 {
			if(os_acb_list[i].state==APP_STATE_RUNNING)
			{
				if(os_acb_cur==&os_acb_list[i])
				{
					os_acb_list[i].timeout_clock=os_acb_list[i].timeout_load;
					os_acb_list[i].timeout_min=os_acb_list[i].timeout_load;
					continue;
				}
				if(os_acb_list[i].timeout_clock>1) 
				{
					os_acb_list[i].timeout_clock--;
					if(os_acb_list[i].timeout_min>os_acb_list[i].timeout_clock)
					{
						os_acb_list[i].timeout_min=os_acb_list[i].timeout_clock;
					}
				}
				else if(os_acb_list[i].timeout_clock==1) 
				{
					os_debug_printf("\r\neos>>%s runing timeout..\r\n",os_acb_list[i].name);
					
					os_acb_list[i].timeout_clock=os_acb_list[i].timeout_load;	
					os_acb_rdy = &os_acb_list[i];
					break;		
				}
			}
		 }
		if(i<OS_APP_MAX) os_switch();	//ǿ���л�APP		
		else os_sched_and_switch();//û�г�ʱAPP�򰴹������������ȼ�APP���л�APP
	 } 
}
void os_switch(void)  //APP�л�
{	
	if(os_sys.lock_nesting == 0) //APP�л�������⣬>0˵��������==0����
	{
		if(os_acb_rdy!=os_acb_cur) API_os_sched_pulse(); //APP�����л�����os_acb_rdy�л���os_acb_cur����	
	}
}
void os_scheduler(void)  //APP������(�����¸�Ҫ���е�APP��OS����)
{	
	os_app_rdy_num_update();//�����㷨��ȡ������ȼ�APP
	if(os_acb_list[os_sys.app_cur_num].delay_clock==1) os_acb_list[os_sys.app_cur_num].delay_clock=0;
	os_acb_rdy = &os_acb_list[os_sys.app_cur_num];//����os_acb_rdy	
	os_acb_rdy->timeout_clock=os_acb_rdy->timeout_load;
}
void os_sched_and_switch(void)//APP���Ȳ��л�����
{
	if(os_sys.lock_nesting == 0) //APP�л�������⣬>0˵��������==0����
	{
		os_scheduler();   //�ȵ��ȣ�os_acb_rdy��ȡ����Ҫ���е�APP
		if(os_acb_rdy!=os_acb_cur) API_os_sched_pulse(); //APP�����л�����os_acb_rdy�л���os_acb_cur����	
	}
}
void os_sched_and_switch_force(void)//APP���Ȳ�ǿ���л�����
{
	if(os_sys.lock_nesting == 0) //APP�л�������⣬>0˵��������==0����
	{
		os_scheduler();   //�ȵ��ȣ�os_acb_rdy��ȡ����Ҫ���е�APP
		API_os_sched_pulse(); //APP�����л�����os_acb_rdy�л���os_acb_cur����	
	}
}
//APP��ջ��ʹ��
os_stk* os_app_stk_init(void* app,os_stk *stk_ptr)
{
    os_stk *stk;
    stk = stk_ptr;
#if (__FPU_PRESENT == 1)
    stk  -= 18;                        // fpu stk  :unknow register + FPSCR + S15-S00
#endif		
    *(  stk)  = (os_stk)0x01000000L;   // xPSR bit[24]������1�������ϵ��ֱ�ӽ���Fault�ж� 
    *(--stk)  = (os_stk)app;           // R15 (PC)             
    *(--stk)  = (os_stk)0xFFFFFFFEL;   // R14 (LR)��ʹ��Ϊ���4λΪE����һ���Ƿ�ֵ����ҪĿ���ǲ���ʹ��R14���������ǲ��ܷ��ص�         
    stk  -= 4;                         //(R12��R3-R1)
	*(--stk)  = (os_stk)0;             // R0(����ջ��ʹ��R0������0,SP��R0�ָ�)
#if (__FPU_PRESENT == 1)		
	stk  -= 16;                        // fpu stk :S31-S16
#endif	
	stk  -=  8;                        // R11-R4
    return stk;
}
void os_init(void)//ϵͳ��ʹ��(����main������)
{
    os_u8 i;
    for(i = 0; i < OS_APP_MAX; i++) {
		os_acb_list[i].stk_ptr = (os_stk*)0;
		os_acb_list[i].delay_clock=1;//����Ϊ1 (��ƣ�Ϊ0ʱΪ������)
		os_acb_list[i].state=APP_STATE_DELETED;
		os_acb_list[i].prio=i;
		os_acb_list[i].stk_size=0;
		app_rdy_tbl_reset(i); //APPδ����
    }
	os_stack_info.offset=0;
	os_sys.clock_counter=0;
	os_sys.app_slicetime = OS_APP_SLICETIME;
	os_sys.app_slicetime_counter = os_sys.app_slicetime;
	os_sys.print_sta=OS_TRUE;
	os_sys.run_sta=OS_TRUE;
	os_app_create_idle();
    os_app_create_core();
    os_acb_cur = &os_acb_list[0];
    os_acb_rdy = &os_acb_list[0];	
	os_sys.debug_print_sta=(os_bool)_OS_DEBUG_PRINTF_;
}
void os_startup(void) //ϵͳ����(����os_main������)
{
	os_app_shell_create();
	API_os_systick_init();  //��ʹ����ʱ��(δʹ���ж�):�����û���ʹ������(delay_us/ms������ʼ��Ч)
	os_user_init();			//�û���ʹ�������ڴ˴�������
	API_os_systick_init();  //���³�ʹ����ʱ����Ϊ�����ö�ʱֵ
	API_os_systick_irq_on();//�򿪶�ʱ���ж�
	API_os_psp_reset();		//��ջָ����0
	API_os_svc_init();		//��ʹ������ж�(��������ж����ȼ�)
	API_os_sched_pulse();	//�������һ���ж�
	while(1);//�ȴ���������ж�(��ʡ��) (��������жϺ�PCָ��ᱻ�޸ĳɿ�������ĵ�ַ���˳��жϺ��ֱ����������APP����)
}
os_stk* os_app_stack_malloc(os_u32 stk_size)
{
	os_stk* re=(os_stk*)&OS_STACK[os_stack_info.offset];
	while(((os_u32)re&0x00000007)!=0x00000000) 
	{
		os_stack_info.offset++;
		re=(os_stk*)&OS_STACK[os_stack_info.offset];
	}
	os_stack_info.offset+=stk_size;
	if((os_stack_info.offset*4)>=sizeof(OS_STACK))
	{
		os_debug_printf("\r\neos>>error:os_stack small\r\n");
	}
	return re;
}
os_bool os_app_create(os_u8 prio,char *name_ptr,fun app,os_u32 stk_size,os_u32 timeout,os_u8 state)//APP��������(����main������)
{
	os_stk *stk_ptr;
    if((os_acb_list[prio].stk_ptr != (os_stk*)0)){
      if(os_acb_list[prio].state!=APP_STATE_DELETED) return OS_FALSE;
	}	
#if OS_APP_NAME_SIZE>0
	strcpy(os_acb_list[prio].name,name_ptr);
#endif	
	stk_ptr=os_app_stack_malloc(stk_size);
	os_acb_list[prio].stk_ptr_bpk=stk_ptr;
	os_acb_list[prio].stk_ptr=os_app_stk_init(app,&stk_ptr[stk_size-1]);
	os_acb_list[prio].app_ptr=(os_u32*)app;
	os_acb_list[prio].stk_size=stk_size;
	os_acb_list[prio].prio=prio;  
	os_acb_list[prio].delay_clock=1;//����Ϊ1
	if(timeout>0)os_acb_list[prio].timeout_load=timeout+1;
	else os_acb_list[prio].timeout_load=timeout;
	os_acb_list[prio].timeout_clock=os_acb_list[prio].timeout_load;
	os_acb_list[prio].state=state;
	if(state==APP_STATE_RUNNING)app_rdy_tbl_set(prio); //APP����
	else app_rdy_tbl_reset(prio); //APPδ����
	
	return OS_TRUE;
}
void app_idle(void) //����APP�����ڷ�ֹ�û�APP��������ͣ������Ϊ0ʱ����
{
	while(1) //����������뽫����APP��ջ(>>eos.h)���󣬷�������
	{
		os_dwg_feed();
		os_app_idle_core_process();
	}
}
void app_system_core(void) //ϵͳ����APP��������Ӧ��ʱ�������жϺ�������Ӧϵͳ���������
{
	while(1) //����������뽫����APP��ջ(>>eos.h)���󣬷�������
	{
		os_dwg_feed();
		os_app_system_core_process();
		os_app_sleep(STK_CORE_SLICETIME);
	}
}
__weak void os_app_shell_create(void)//shellAPP��������
{
	
}
void os_app_create_idle(void)//����APP��������
{
#if OS_APP_NAME_SIZE>0
	strcpy(os_acb_list[0].name,"idle");
#endif
	os_acb_list[0].timeout_load=STK_IDLE_TIMEOUT;
	os_acb_list[0].timeout_clock=STK_IDLE_TIMEOUT;
	os_acb_list[0].app_ptr=(os_u32*)app_idle;
	os_acb_list[0].stk_ptr_bpk=APP_IDLE_STK;
    os_acb_list[0].stk_ptr = os_app_stk_init(app_idle,(os_stk*)&APP_IDLE_STK[STK_IDLE_SIZE - 1]);
	os_acb_list[0].stk_size=STK_IDLE_SIZE;
	os_acb_list[0].prio=0;
	os_acb_list[0].state=APP_STATE_RUNNING;
	app_rdy_tbl_set(0); //APP����
}
void os_app_create_core(void)//����APPϵͳ���ĺ���
{
#if (_USE_SHELL_ == 0)
#if OS_APP_NAME_SIZE>0
	strcpy(os_acb_list[OS_APP_MAX-1].name,"core");
#endif
	os_acb_list[OS_APP_MAX-1].timeout_load=STK_CORE_TIMEOUT;
	os_acb_list[OS_APP_MAX-1].timeout_clock=STK_CORE_TIMEOUT;
	os_acb_list[OS_APP_MAX-1].app_ptr=(os_u32*)app_system_core;
	os_acb_list[OS_APP_MAX-1].stk_ptr_bpk=APP_CORE_STK;
    os_acb_list[OS_APP_MAX-1].stk_ptr = os_app_stk_init(app_system_core,(os_stk*)&APP_CORE_STK[STK_CORE_SIZE - 1]);
	os_acb_list[OS_APP_MAX-1].stk_size=STK_CORE_SIZE;
	os_acb_list[OS_APP_MAX-1].prio=OS_APP_MAX-1;
	os_acb_list[OS_APP_MAX-1].state=APP_STATE_RUNNING;
	app_rdy_tbl_set(OS_APP_MAX-1); //APP����
#else
#if OS_APP_NAME_SIZE>0
	strcpy(os_acb_list[OS_APP_MAX-2].name,"core");
#endif
	os_acb_list[OS_APP_MAX-2].timeout_load=STK_CORE_TIMEOUT;
	os_acb_list[OS_APP_MAX-2].timeout_clock=STK_CORE_TIMEOUT;
	os_acb_list[OS_APP_MAX-2].app_ptr=(os_u32*)app_system_core;
	os_acb_list[OS_APP_MAX-2].stk_ptr_bpk=APP_CORE_STK;
    os_acb_list[OS_APP_MAX-2].stk_ptr = os_app_stk_init(app_system_core,(os_stk*)&APP_CORE_STK[STK_CORE_SIZE - 1]);
	os_acb_list[OS_APP_MAX-2].stk_size=STK_CORE_SIZE;
	os_acb_list[OS_APP_MAX-2].prio=OS_APP_MAX-2;
	os_acb_list[OS_APP_MAX-2].state=APP_STATE_RUNNING;
	app_rdy_tbl_set(OS_APP_MAX-2); //APP����
#endif
}
void os_app_restart_process(void)
{
	os_u8 i;
	for(i=1;i<OS_APP_MAX;i++)
	{
		if(os_acb_list[i].state==APP_STATE_RESTART)
		{
			os_app_restart_other((fun)os_acb_list[i].app_ptr);
		}
	}	
}
__weak void os_idle_handler_hook(void)//�˹��Ӻ�����ֹ����os_app_slee(),����delay_ms/us()
{
	
}
void os_app_idle_core_process(void)
{
	os_idle_handler_hook();
	os_sched_and_switch();
}
void os_app_system_core_process(void)
{
	os_app_restart_process();
	os_timer_handler();
	os_sched_and_switch();
}
void os_lock(void) //APP�л�����  (os_lock�����os_unlock�ɶԳ���)
{  
	os_sys.lock_nesting++;	
}
void os_unlock(void) //APP�л����� 
{ 
	os_sys.lock_nesting--; 
} 
//APP����(APP��������Ȩ)
void os_app_sleep(os_u32 ms)  //��������CPU����Ȩ,ms=0 ��������,ms>0����һ��ʱ���ָ�(��ʱ)
{
	if(ms>0) ms=ms+1;
	os_acb_cur->delay_clock=ms;
	os_sys.app_slicetime_counter = os_sys.app_slicetime;//����ʱ��Ƭ��ֵ
	app_rdy_tbl_reset(os_acb_cur->prio); //APPδ����
	os_sched_and_switch();
}
os_u8 os_app_prio_from_app(fun app)//��ȡapp�����ȼ�(ֻ���û�app��Ч)
{
	os_u8 i;
	for(i=1;i<OS_APP_MAX;i++)//��������app
	{
		if(os_acb_list[i].app_ptr==(os_u32*)app)
		{
			return i;
		}
	}
	return 0; //���û�app��������
}
void os_app_suspend(fun app)//����APP(�κ�)
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		os_acb_list[prio].state=APP_STATE_PENDING;
		app_rdy_tbl_reset(prio);	
	}
	os_unlock();
	os_sched_and_switch_force();
}
void os_app_resume(fun app)//�ָ�APP(������APP)
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		os_acb_list[prio].state=APP_STATE_RUNNING;
		app_rdy_tbl_set(prio);
	}
	os_unlock();
}
void os_app_restart_other(fun app)//��������APP(������APP)
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		os_acb_list[prio].stk_ptr=os_app_stk_init(app,&os_acb_list[prio].stk_ptr_bpk[os_acb_list[prio].stk_size-1]);
		os_acb_list[prio].prio=prio;
		os_acb_list[prio].delay_clock=1;//����Ϊ1
		os_acb_list[prio].state=APP_STATE_RUNNING;//����״̬
		app_rdy_tbl_reset(prio); //APPδ����
	}
	os_unlock();
}
void os_app_restart(fun app)//��������APP(�κ�)
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		os_acb_list[prio].state=APP_STATE_RESTART;//����״̬
	}
	os_unlock();
}
void os_app_delete(fun app)//ɾ��APP(�κ�)
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		os_acb_list[prio].state=APP_STATE_DELETED;
		app_rdy_tbl_reset(prio);
	}
	os_unlock();
	os_sched_and_switch_force();
}
void os_app_prio_set(fun app,os_u8 new_prio)//����APP�����ȼ�Ϊnew_prio
{
	os_u8 prio;
	os_lock();
	prio=os_app_prio_from_app(app);
	if(prio>0) 
	{
		if(os_acb_list[new_prio].state==APP_STATE_DELETED)
		{
			os_acb_list[new_prio]=os_acb_list[prio];
			os_acb_list[new_prio].prio=new_prio;
			os_acb_list[prio].state=APP_STATE_DELETED;
			app_rdy_tbl_reset(prio);
			if(app_rdy_tbl_state_get(prio)==OS_TRUE) app_rdy_tbl_set(new_prio);
			else app_rdy_tbl_reset(new_prio);
		}
	}
	os_unlock();
	os_sched_and_switch();
}
void os_app_slicetime_set(os_u32 ms)//����APPʱ����Ƭ
{
	os_sys.app_slicetime=ms;
}
os_u32 os_clock_get(void)
{
    return os_sys.clock_counter;  //ϵͳʱ�ӽ����ۼƱ���
}
void os_debug_print_enable(void)  //ʹ��debug��ӡ���
{
	os_sys.debug_print_sta=OS_TRUE;
}
void os_debug_print_disable(void)//��ֹdebug��ӡ���
{
	os_sys.debug_print_sta=OS_FALSE;
}
#if ( OS_TIMER_MAX > 0 ) /*-------------------------------------------------------------*/
void os_timer_count_interrupt_set(os_u8 id,fun function)//����ϵͳ�����ʱ���жϺ���
{
	os_timer[id].timer_count_handler=function;
}
void os_timer_interrupt_set(os_u8 id,fun function)//����ϵͳ�����ʱ���жϺ���
{
	os_timer[id].timer_handler=function;
}
void os_timer_reload_set(os_u8 id,os_u32 tick)//����ϵͳ�����ʱ�����ؼ���ֵ(���ô˺�������Զ����ؼ���)
{
	os_lock();
	if(tick>0) tick+=1;
    os_timer[id].timer_counter=tick;
	os_timer[id].timer_reload=tick;
	os_unlock();
}
void os_timer_value_set(os_u8 id,os_u32 tick)//����ϵͳ�����ʱ�����¼���ֵ(���ô˺������ȡ���Զ����ؼ�������)
{
	os_lock();
	if(tick>0) tick+=1;
	os_timer[id].timer_counter=tick;
	os_timer[id].timer_reload=0;
	os_unlock();
}
os_u32 os_timer_value_get(os_u8 id)//��ȡ�����ʱ����ǰ����ֵ
{
  return os_timer[id].timer_counter;
}
#endif /*------------------------------------------------------------------------------------*/

//APP����uS��ʱ(��ͨ����ʱ����)
void delay_us(os_u32 us)
{       
    os_u32 ticks;
    os_u32 told,tnow,tcnt=0;
    os_u32 reload=API_os_systick_reload_get();       
    ticks=us*API_os_systick_1us_value_get();   
    tcnt=0;
    told=API_os_systick_value_get();
    while(1)
    {
		tnow=API_os_systick_value_get();  
		if(tnow!=told)
		{       
			if(tnow<told) tcnt += told-tnow;
			else tcnt += reload-tnow+told;      
			told = tnow;
			if(tcnt>=ticks) break;
		} 					
    }
}
//APP����mS��ʱ(��ͨ����ʱ����)
void delay_ms(os_u32 ms)
{   
	delay_us(ms*1000);			
}

//OS_APP_FLAG  ϵͳȫ�ֱ�־��
#if ( OS_FLAG_MAX > 0 ) /*-------------------------------------------------------------*/
os_bool os_app_flag_pend(os_u8 mod,os_u8 id) //mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
{
	if(mod) {
		if(!(os_app_flag[id>>3]&(1<<(id&0x07)))) return OS_FALSE;
	}
	else while(!(os_app_flag[id>>3]&(1<<(id&0x07)))) 
	{
		os_dwg_feed();
		os_app_sleep(1);
	}
	os_app_flag[id>>3]&=~(1<<(id&0x07));
	return OS_TRUE;
}
os_bool os_app_flag_post(os_u8 id)
{
	os_app_flag[id>>3]|=1<<(id&0x07);
	return OS_TRUE;
}
#endif /*------------------------------------------------------------------------------------*/
//OS_APP_VALUE  ϵͳȫ����ֵ��
#if ( OS_VALUE_MAX > 0 ) /*-------------------------------------------------------------*/
os_bool os_app_value_pend(os_u8 mod,os_u8 id,double *value_out) //mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
{	
	if(mod) {
		if(!(os_app_flag_value[id>>3]&(1<<(id&0x07)))) return OS_FALSE;
	}
	else while(!(os_app_flag_value[id>>3]&(1<<(id&0x07)))) 
	{
		os_dwg_feed();
		os_app_sleep(1);
	}
	os_app_flag_value[id>>3]&=~(1<<(id&0x07));
	*value_out=os_app_value[id];
	return OS_TRUE;
}
os_bool os_app_value_post(os_u8 id,double *value_in)
{
	os_app_value[id]=*value_in;
	os_app_flag_value[id>>3]|=1<<(id&0x07);
	return OS_TRUE;
}
#endif /*------------------------------------------------------------------------------------*/
//OS_APP_MUTEX  ϵͳȫ�ֻ�����
#if ( OS_MUTEX_MAX > 0 ) /*-------------------------------------------------------------*/
os_bool os_app_mutex_pend(os_u8 mod,os_u8 id) //mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
{
	if(mod){
		if(os_app_mutex[id]!=0) return OS_FALSE;
	}
	else while(os_app_mutex[id]!=0) 
	{
		os_dwg_feed();
		os_app_sleep(1);
	}
	os_app_mutex[id]=os_acb_cur->prio;
	return OS_TRUE;	
}
os_bool os_app_mutex_post(os_u8 id)
{
	if(os_app_mutex[id]==os_acb_cur->prio){
		os_app_mutex[id]=0;
		return OS_FALSE;
	}
	return OS_TRUE;
}
#endif /*------------------------------------------------------------------------------------*/
//OS_APP_MBOX  ϵͳȫ������(ֻ��һ�����䣬���ڿɴ�Ŷ���ʼ���ÿ���ʼ���Ӧһ��ID)
#if ( OS_MBOX_MAX > 0 ) /*-------------------------------------------------------------*/
os_bool os_mbox_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len)//�����ʼ�(�������ʼ�ID���ʼ����ʼ�����)
{
	if(mail_len>OS_MAIL_SIZE) return OS_FALSE;
	os_mbox[id].data_len=mail_len;
	memcpy(os_mbox[id].datum,send_mail,os_mbox[id].data_len);
	return OS_TRUE;
}
os_bool os_mbox_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len)//mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
{
	if(mod) {
		if(os_mbox[id].data_len==0) return OS_FALSE;
	}
	else while(os_mbox[id].data_len==0) 
	{
		os_dwg_feed();
		os_app_sleep(1);
	}
	memcpy(read_mail,os_mbox[id].datum,os_mbox[id].data_len);
	*mail_len=os_mbox[id].data_len;
	os_mbox[id].data_len=0;
	return OS_TRUE;
}
#endif /*------------------------------------------------------------------------------------*/	
//OS_APP_MBOX_QUEUE  ϵͳȫ�ֶ�������(�ж�����䣬ÿ�������Ӧһ��ID��ÿ�����ڿɴ�Ŷ���ʼ��������ʼ������д��)
#if ( OS_MBOX_QUEUE_MAX > 0 ) /*-------------------------------------------------------------*/
os_bool os_mbox_queue_post(os_u8 id,os_u8 *send_mail,os_u32 mail_len)//���Ͷ����ʼ�(��������������ID���ʼ����ʼ�����)
{
	if(os_mbox_queue[id].queue_len>=OS_QUEUE_SIZE) return OS_FALSE;
	if(mail_len>=OS_MAIL_SIZE) mail_len=OS_MAIL_SIZE;
	os_mbox_queue[id].mail[os_mbox_queue[id].queue_len].data_len=mail_len;	
	memcpy(os_mbox_queue[id].mail[os_mbox_queue[id].queue_len].datum,send_mail,mail_len);
	os_mbox_queue[id].queue_len++;
	return OS_TRUE;
}
os_bool os_mbox_queue_pend(os_u8 mod,os_u8 id,os_u8 *read_mail,os_u32 *mail_len)//mod=0������=1������(����OS_TRUE��ʾ�յ�POST)
{
	os_u8 i;
	if(mod) {
		if(os_mbox_queue[id].queue_len==0) return OS_FALSE;//�ն���
	}
	else while(os_mbox_queue[id].queue_len==0) 
	{
		os_dwg_feed();
		os_app_sleep(1);
	}
	*mail_len=os_mbox_queue[id].mail[0].data_len;
	memcpy(read_mail,os_mbox_queue[id].mail[0].datum,os_mbox_queue[id].mail[0].data_len);
	for(i=0;i<os_mbox_queue[id].queue_len-1;i++)
	{
		if(os_mbox_queue[id].mail[i+1].data_len>0)
		{
			os_mbox_queue[id].mail[i].data_len=os_mbox_queue[id].mail[i+1].data_len;
			memcpy(os_mbox_queue[id].mail[i].datum,os_mbox_queue[id].mail[i+1].datum,os_mbox_queue[id].mail[i+1].data_len);
		}
		else break;
	}
	os_mbox_queue[id].mail[os_mbox_queue[id].queue_len-1].data_len=0;
	os_mbox_queue[id].queue_len--;
	return OS_TRUE;
}
#endif /*------------------------------------------------------------------------------------*/


//�����ָʾ������Ҫ�뽫ע��ȥ��
/* void HardFault_Handler(void)
{
	os_sys.enter_shell=OS_FALSE;
	os_shell_printf("-----$$ HardFault $$-----\r\n");
    while(1){
    }
} */


