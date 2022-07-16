;eOS for Cortex-M0

;第三部分
	import    os_acb_cur        ;向内函数声明(指明该函数体在外部)
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
PendSV_Handler    ;软件触发中断函数  
    cpsid    I                
    mrs     r0, psp              	
    cmp     R0 , #0x00
	beq     os_reset_enter  	                        
    
    subs    R0 , R0 , #0x10  	
    str     R4 , [R0]        ;save r4-r7 to (sp-0x10) -> sp       
    adds    R0 , R0 , #0x4           
    str     R5 , [R0]               
    adds    R0 , R0,  #0x4           
    str     R6 , [R0]               
    adds    R0 , R0 , #0x4           
    str     R7 , [R0]               
    adds    R0 , R0 , #0x4  	
	;M0 is disable to operate R8-R15 ,so move R8-R15 to r0-r7 to operate
	subs    R0 , R0 , #0x20  ;save r8-r11 to (sp-0x20) -> (sp-0x10) 
	mov     R4 , R8
    str     R4 , [R0]               
    adds    R0 , R0 , #0x4           
	mov     R5 , R9
    str     R5 , [R0]                
    adds    R0 , R0 , #0x4           
	mov     R6 , R10
    str     R6 , [R0]                
    adds    R0 , R0 , #0x4           
    mov     R7 , R11	
    str     R7 , [R0]                
    adds    R0 , R0 , #0x4  
 	
    subs    R0 , R0 , #0x10  ;r0 = r0 -0x20 , (r0 save sp value)

    ldr     r1, =os_acb_cur   							  
    ldr     r1, [r1]          
    str     r0, [r1] 
os_reset_enter                  
    ldr     r0, =os_acb_cur   
    ldr     r1, =os_acb_rdy 
    ldr     r2 , [r1]         
    str     r2 , [r0]         
    ldr     r0 , [r2]      ;r0 = (sp-0x20)   

    ;M0 is disable to operate R8-R15 ,so move R8-R15 to r0-r7 to operate
    ldr     R4 , [R0]             
    adds    R0 , R0 , #0x4         
	mov     R8 , R4        ;recover r8-r11 form (sp-0x20) -> (sp-0x10)  
    ldr     R5 , [R0]             
    adds    R0 , R0 , #0x4         
	mov     R9 , R5
    ldr     R6 , [R0]             
    adds    R0 , R0 , #0x4         
	mov     R10, R6 
    ldr     R7 , [R0]             
    adds    R0 , R0 , #0x4         
	mov     R11, R7 
	
    ldr     R4 , [R0]      ;recover r4-r7 form (sp-0x20) -> (sp-0x10)          
    adds    R0 , R0 , #0x4         
    ldr     R5 , [R0]              
    adds    R0 , R0 , #0x4         
    ldr     R6 , [R0]              
    adds    R0 , R0 , #0x4         
    ldr     R7 , [R0]     
	
    adds    R0 , R0 , #0x4 ;r0 = sp , (r0 save sp value)
    	
    msr     PSP, R0    
    movs    R1 , #0x04
	mov     R2 , LR
    orrs    R2 , R2 , R1  
	mov     LR , R2 
	
    cpsie     I                
    bx    lr   
	
    align 4                    
    end                        
    ;by Angle_145    	