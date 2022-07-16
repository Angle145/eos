;eOS for Cortex-M3

;第三部分
	import    os_acb_cur            ;向内函数声明(指明该函数体在外部)
    import    os_acb_rdy        
	
    export    API_os_irq_disable     ;向外函数声明(指明该函数体在此处)
    export    API_os_irq_enable    
	export	  API_os_psp_reset	
	export    PendSV_Handler		
     	
    preserve8                 
    area |.text|, code, readonly 
    thumb                     

API_os_irq_disable    ;关全局中断
    cpsid    I                
    bx    lr   
	
API_os_irq_enable     ;开全局中断   
    cpsie    I                
    bx    lr   
	
API_os_psp_reset     ;堆栈指针清0  
    cpsid    I                
    ldr r4,=0x0                        
    msr     psp, r4              
    cpsie    I                
    bx    lr  	
PendSV_Handler    ;软件触发中断函数(只要是能软件触发的中断都可以，中断触发中断:SV中断)  
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