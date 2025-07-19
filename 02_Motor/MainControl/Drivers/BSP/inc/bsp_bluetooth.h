#pragma once

typedef enum
{
    Flag_Idle = 0,           //0
    Flag_Show,               //1
    Flag_WS2812,             //2
    Flag_MotorTarget,        //3
    Flag_MotorEnable,        //4
    Flag_MotorDisable,       //5
    Flag_MPU6050Stable,      //6
    Flag_SetLightThreshold,  //7
} FLAG_E;

typedef struct
{
    char paramName[32];  // 参数名
    int16_t paramValue;  // 参数值
} PARAMETER_T;


#define COM_BLUETOOTH	COM3		/* 选择串口 */

//#define BLUETOOTH_TO_COM1_EN

typedef void (*BLUETOOTH_HandleFrameCallback) (uint8_t *pucFrameData, uint8_t ucFrameLength);

/* 供外部调用的函数声明 */
void bsp_InitBLUETOOTH( BLUETOOTH_HandleFrameCallback _BLUETOOTH_HandleFrame );
uint8_t BLUETOOTH_ReadData( void );
void BLUETOOTH_SendData( uint8_t* _databuf, uint16_t _len );

void bsp_InitBlueTooth(void);
void BlueTooth_Read(void);
