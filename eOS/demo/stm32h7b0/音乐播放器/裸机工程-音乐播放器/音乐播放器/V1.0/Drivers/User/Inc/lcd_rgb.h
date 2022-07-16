#ifndef  __LCD_RGB_H
#define	__LCD_RGB_H

#include "stm32h7xx_hal.h"
#include "usart.h"
#include "lcd_fonts.h"
#include <stdio.h>


/*-------------------------------------------------------- LCD��ز��� -------------------------------------------------------*/

#define HBP  80	// ������Ļ���ֲ��������
#define VBP  40
#define HSW  1
#define VSW  1
#define HFP  200
#define VFP  22

#define LCD_Width     	800				//	LCD�����س���
#define LCD_Height    	480				//	LCD�����ؿ��
#define LCD_MemoryAdd   0x24000000 		//	�Դ����ʼ��ַ  

// layer0 ���Դ��ַ��������ʹ��7B0�ڲ���AXI SRAM��Ϊ�Դ棬��ʼ��ַ0x24000000����СΪ1M
// layer0 �Դ��С���� = LCD_Width * LCD_Width * BytesPerPixel_0��ÿ��������ռ�ֽڴ�С��
// ��Ϊ AXI SRAM �Ĵ�Сֻ��1M���û����õ�����һ�����ܳ������ֵ��	
#define LCD_MemoryAdd   	0x24000000 				//	�Դ����ʼ��ַ  

// layer0 ÿ��������ռ�ֽ�
#define 	BytesPerPixel_0		2		//16λɫģʽÿ������ռ2�ֽ�

#define	ColorMode_0   			LTDC_PIXEL_FORMAT_RGB565   		//���� layer0 ����ɫ��ʽ


/*---------------------------------------------------------- �������� -------------------------------------------------------*/
	
void MX_LTDC_Init(void);					// LTDC�Լ����ʼ�� 

void  LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height,uint32_t *color);	// ���ƻ���������ʾ��


/*-------------------------------------------------------- LCD�������� -------------------------------------------------------*/

#define  LCD_Backlight_PIN								GPIO_PIN_15								
#define	LCD_Backlight_PORT							GPIOD									
#define 	GPIO_LDC_Backlight_CLK_ENABLE        	__HAL_RCC_GPIOD_CLK_ENABLE()	 	

#define	LCD_Backlight_OFF		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_RESET);	// �رձ���
#define 	LCD_Backlight_ON		HAL_GPIO_WritePin(LCD_Backlight_PORT, LCD_Backlight_PIN, GPIO_PIN_SET);		// ��������


#endif //__LCD_RGB_H

