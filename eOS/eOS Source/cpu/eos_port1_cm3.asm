;eOS for Cortex-M3

;��������
	import    os_acb_cur            ;���ں�������(ָ���ú��������ⲿ)
    import    os_acb_rdy        
	
    export    API_os_irq_disable     ;���⺯������(ָ���ú������ڴ˴�)
    export    API_os_irq_enable    
	export	  API_os_psp_reset	
	export    PendSV_Handler		
     	
    preserve8                 
    area |.text|, code, readonly 
    thumb                     

API_os_irq_disable    ;��ȫ���ж�
    cpsid    I                
    bx    lr   
	
API_os_irq_enable     ;��ȫ���ж�   
    cpsie    I                
    bx    lr   
	
API_os_psp_reset     ;��ջָ����0  
    cpsid    I                
    ldr r4,=0x0                        
    msr     psp, r4              
    cpsie    I                
    bx    lr  	
PendSV_Handler    ;��������жϺ���(ֻҪ��������������ж϶����ԣ��жϴ����ж�:SV�ж�)  
    cpsid    I                
    mrs     r0, psp              	
    cbz     r0, os_reset_enter 	
	
    sub     r0, r0, #0x20  	  
    stm     r0, {r4-r11}      
    ldr     r1, =os_acb_cur   							  
    ldr     r1, [r1]          
    str     r0, [r1] 
os_reset_enter                  
    ldr     r0, =os_acb_cur   
    ldr     r1, =os_acb_rdy   
    ldr     r2 , [r1]         
    str     r2 , [r0]         
    ldr     r0 , [r2]         
	ldm     r0, {r4-r11}       
	add     r0, r0, #0x20 	
	
    msr     psp, r0            
    orr     lr, lr, #0x04      
    cpsie     I                
    bx    	lr   
	
    align 4                    
    end                        
    ;by Angle_145    	