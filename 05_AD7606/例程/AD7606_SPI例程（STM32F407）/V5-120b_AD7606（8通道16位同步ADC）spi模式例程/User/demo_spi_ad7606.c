/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 主程序
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2015-11-06 armfly  首发
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"		/* 如果要用ST的固件库，必须包含这个文件 */
#include "demo_spi_ad7606.h"

/* 仅允许本文件内调用的函数声明 */
static void DispMenu(void);

static void AD7606_Mak(void);
static void AD7606_Disp(void);

static int16_t s_volt[8];
static int16_t s_dat[8];
/*
*********************************************************************************************************
*	函 数 名: Demo_spi_AD7606
*	功能说明: 串行AD7606读写例程
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Demo_spi_AD7606(void)
{
	uint8_t cmd;

	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。
	*/
	g_tAD7606.Range = 1;	/* 10V */

	bsp_spi_InitAD7606();	/* 配置AD7606所用的GPIO */

	bsp_StartAutoTimer(0, 500);	/* 启动1个500ms的自动重装的定时器 */

	DispMenu();			/* 显示操作菜单 */	
	while (1)
	{
		bsp_Idle();		/* 空闲时执行的函数,比如喂狗. 在bsp.c中 */

		if (bsp_CheckTimer(0))
		{	
			/* 每隔500ms 进来一次. 由软件启动转换 */
			AD7606_Scan();
		
			/* 处理数据 */
			AD7606_Mak();
										 
			/* 打印ADC采样结果 */
			AD7606_Disp();		
		}
		
		if (comGetChar(COM1, &cmd))	/* 从串口读入一个字符(非阻塞方式) */
		{
			switch (cmd)
			{
				case '1':
					if (g_tAD7606.Range == 0)
					{
						g_tAD7606.Range = 1;
					}
					else
					{
						g_tAD7606.Range = 0;
					}
					AD7606_SetInputRange(g_tAD7606.Range);
					break;

				default:
					DispMenu();	/* 无效命令，重新打印命令提示 */
					break;

			}
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: AD7606_Mak
*	功能说明: 处理采样后的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Mak(void)
{
	uint8_t i;
	int16_t adc;

	for (i = 0;i < CH_NUM; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		32767 = 5V , 这是理论值，实际可以根据5V基准的实际值进行公式矫正 
		volt[i] = ((int16_t)dat[i] * 5000) / 32767;	计算实际电压值（近似估算的），如需准确，请进行校准            
		volt[i] = dat[i] * 0.3051850947599719
	*/
		
		adc = s_dat[i];
		if (g_tAD7606.Range == 0)
		{
			s_volt[i] = (adc * 5000) / 32767;
		}
		else
		{
			s_volt[i] = (adc * 10000) / 32767;
		}
	}
}
 
/*
*********************************************************************************************************
*	函 数 名: AD7606_Disp
*	功能说明: 显示采样后的数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AD7606_Disp(void)
{
	int16_t i;	
	int16_t iTemp;

	/* 打印采集数据 */
	for (i = 0; i < CH_NUM; i++)
	{                
   		iTemp = s_volt[i];	/* uV  */
		
		if (s_dat[i] < 0)
		{
			iTemp = -iTemp;
            printf(" CH%d = %6d,0x%04X (-%d.%d%d%d V) \r\n", i+1, s_dat[i], (uint16_t)s_dat[i], iTemp /1000, (iTemp%1000)/100, (iTemp%100)/10,iTemp%10);
		}
		else
		{
         	printf(" CH%d = %6d,0x%04X ( %d.%d%d%d V) \r\n", i+1, s_dat[i], s_dat[i] , iTemp /1000, (iTemp%1000)/100, (iTemp%100)/10,iTemp%10);                    
		}
	}
	printf("\33[%dA", (int)CH_NUM);  /* 光标上移n行 */		
}



/*
*********************************************************************************************************
*	函 数 名: DispMenu
*	功能说明: 显示操作提示菜单
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispMenu(void)
{
	printf("\r\n------------------------------------------------\r\n");
	printf("请选择操作命令:\r\n");
	printf("1 - 切换量程(5V或10V)\r\n");
	printf("\r\n");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
