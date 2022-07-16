#ifndef  __LCD_RGB_H
#define	__LCD_RGB_H

#include "stm32h7xx_hal.h"
#include "usart.h"
#include "lcd_fonts.h"
#include <stdio.h>


/*-------------------------------------------------------- LCD相关参数 -------------------------------------------------------*/

#define HBP  80	// 根据屏幕的手册进行设置
#define VBP  40
#define HSW  1
#define VSW  1
#define HFP  200
#define VFP  22

#define LCD_Width     	800				//	LCD的像素长度
#define LCD_Height    	480				//	LCD的像素宽度
#define LCD_MemoryAdd   0x24000000 		//	显存的起始地址  

// layer0 的显存地址，本例程使用7B0内部的AXI SRAM作为显存，起始地址0x24000000，大小为1M
// layer0 显存大小等于 = LCD_Width * LCD_Width * BytesPerPixel_0（每个像素所占字节大小）
// 因为 AXI SRAM 的大小只有1M，用户设置的区域一定不能超过这个值！	
#define LCD_MemoryAdd   	0x24000000 				//	显存的起始地址  

// layer0 每个像素所占字节
#define 	BytesPerPixel_0		2		//16位色模式每个像素占2字节

#define	ColorMode_0   			LTDC_PIXEL_FORMAT_RGB565   		//定义 layer0 的颜色格式


/*---------------------------------------------------------- 函数声明 -------------------------------------------------------*/
	
void MX_LTDC_Init(void);					// LTDC以及层初始化 

void  LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t *color);	// 复制缓冲区到显示区


/*-------------------------------------------------------- LCD背光引脚 -------------------------------------------------------*/

#define  LCD_Backlight_PIN								GPIO_PIN_15								
#define	LCD_Backlight_PORT							GPIOD									
#define 	GPIO_LDC_Backlight_CLK_ENABLE        	__HAL_RCC_GPIOD_CLK_ENABLE()	 	

#define	LCD_Backlight_OFF		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);	// 关闭背光
#define 	LCD_Backlight_ON		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);		// 开启背光


#endif //__LCD_RGB_H

