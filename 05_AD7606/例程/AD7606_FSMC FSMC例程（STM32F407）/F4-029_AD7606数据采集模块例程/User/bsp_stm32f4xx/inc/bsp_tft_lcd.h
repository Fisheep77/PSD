/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_tft_lcd.h
*	版    本 : V2.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库版本 V3.5.0版本。
*		v2.0    2011-10-16 armfly  ST固件库版本 V3.5.0版本。规范函数接口，优化内部结构
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _BSP_TFT_LCD_H
#define _BSP_TFT_LCD_H

#include "stm32f4xx.h"


/* 定义LCD显示区域的分辨率 */
#define LCD_30_HEIGHT	240		/* 3.0寸宽屏 高度，单位：像素 */
#define LCD_30_WIDTH	400		/* 3.0寸宽屏 宽度，单位：像素 */

#define LCD_43_HEIGHT	272		/* 4.3寸宽屏 高度，单位：像素 */
#define LCD_43_WIDTH	480		/* 4.3寸宽屏 宽度，单位：像素 */

#define LCD_70_HEIGHT	480		/* 7.0寸宽屏 高度，单位：像素 */
#define LCD_70_WIDTH	800		/* 7.0寸宽屏 宽度，单位：像素 */

/* 支持的驱动芯片ID */
enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075
};

#define CHIP_STR_5420	"SPFD5420A"
#define CHIP_STR_4001	"OTM4001A"
#define CHIP_STR_61509	"R61509V"
#define CHIP_STR_8875	"RA8875"

/* LCD 颜色代码，CL_是Color的简写 */
enum
{
	CL_WHITE        = 0xFFFF,	/* 白色 */
	CL_BLACK        = 0x0000,	/* 黑色 */
	CL_GREY         = 0xF7DE,	/* 灰色 */
	CL_BLUE         = 0x001F,	/* 蓝色 */
	CL_BLUE2        = 0x051F,	/* 浅蓝色 */
	CL_RED          = 0xF800,	/* 红色 */
	CL_MAGENTA      = 0xF81F,	/* 红紫色，洋红色 */
	CL_GREEN        = 0x07E0,	/* 绿色 */
	CL_CYAN         = 0x7FFF,	/* 蓝绿色，青色 */
	CL_YELLOW       = 0xFFE0,	/* 黄色 */
	CL_MASK			= 0x9999	/* 颜色掩码，用于文字背景透明 */
};

/* 字体代码 */

/* 字体代码 */
enum
{
	FC_ST_16 = 0,		/* 宋体15x16点阵 （宽x高） */
	FC_ST_12 = 1		/* 宋体12x12点阵 （宽x高） */
};

/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* 字体代码 0 表示16点阵 */
	uint16_t usTextColor;	/* 字体颜色 */
	uint16_t usBackColor;	/* 文字背景颜色，透明 */
	uint16_t usSpace;		/* 文字间距，单位 = 像素 */
}FONT_T;

/* 背景光控制 */
#define BRIGHT_MAX		255
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5

/* 可供外部模块调用的函数 */
void LCD_InitHard(void);
void LCD_GetChipDescribe(char *_str);
uint16_t LCD_GetHeight(void);
uint16_t LCD_GetWidth(void);
void LCD_DispOn(void);
void LCD_DispOff(void);
void LCD_ClrScr(uint16_t _usColor);
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY);
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_DrawIcon(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_DrawIconActive(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_SetBackLight(uint8_t _bright);
uint8_t LCD_GetBackLight(void);

/* 下面3个变量，主要用于使程序同时支持不同的屏 */
extern uint16_t g_ChipID;			/* 驱动芯片ID */
extern uint16_t g_LcdHeight;		/* 显示屏分辨率-高度 */
extern uint16_t g_LcdWidth;			/* 显示屏分辨率-宽度 */

#endif


