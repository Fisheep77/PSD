#pragma once

// Device Address
#define PSD_NUM        1             //PSD设备数量
#define PSD_ADDRESS_1  (0x01 << 1)
//#define PSD_ADDRESS_2  (0x02 << 1)

//寄存器总大小
#define PSD_REGISTERS_SIZE 256

//0x00 ~ 0x10 配置寄存器
#define PSD_SETTING   0x00                      //配置寄存器基地址
#define PSD_WHO_AM_I        (PSD_SETTING+1)     //本机地址寄存器
#define PSD_LIGHT_THRESHOLD (PSD_SETTING+2)     //光照阈值寄存器

//0x10 ~ 0x20 坐标寄存器
#define PSD_COORDINATE   0x10                   //坐标寄存器基地址
#define PSD_COORDINATE_X (PSD_COORDINATE+0)     //X坐标
#define PSD_COORDINATE_Y (PSD_COORDINATE+4)     //Y坐标

/*
*********************************************************************************************************
*                                          供外部调用的函数
*********************************************************************************************************
*/
void bsp_InitPSD( void );
void PSD_ReadCoordinate( void );
void PSD_SetLightThreshold( uint8_t _LightThreshold);
float PSD_GetLightAngle(void);

extern float LightAngle;
extern float PSD_CoordinateX[PSD_NUM];
extern float PSD_CoordinateY[PSD_NUM];
