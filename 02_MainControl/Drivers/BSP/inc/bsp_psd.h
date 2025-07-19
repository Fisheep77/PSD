#pragma once

// Device Address
#define PSD_NUM        1             //PSD�豸����
#define PSD_ADDRESS_1  (0x01 << 1)
//#define PSD_ADDRESS_2  (0x02 << 1)

//�Ĵ����ܴ�С
#define PSD_REGISTERS_SIZE 256

//0x00 ~ 0x10 ���üĴ���
#define PSD_SETTING   0x00                      //���üĴ�������ַ
#define PSD_WHO_AM_I        (PSD_SETTING+1)     //������ַ�Ĵ���
#define PSD_LIGHT_THRESHOLD (PSD_SETTING+2)     //������ֵ�Ĵ���

//0x10 ~ 0x20 ����Ĵ���
#define PSD_COORDINATE   0x10                   //����Ĵ�������ַ
#define PSD_COORDINATE_X (PSD_COORDINATE+0)     //X����
#define PSD_COORDINATE_Y (PSD_COORDINATE+4)     //Y����

/*
*********************************************************************************************************
*                                          ���ⲿ���õĺ���
*********************************************************************************************************
*/
void bsp_InitPSD( void );
void PSD_ReadCoordinate( void );
void PSD_SetLightThreshold( uint8_t _LightThreshold);
float PSD_GetLightAngle(void);

extern float LightAngle;
extern float PSD_CoordinateX[PSD_NUM];
extern float PSD_CoordinateY[PSD_NUM];
