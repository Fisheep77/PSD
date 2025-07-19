#pragma once

//从机i2c句柄
#define SLAVE_I2C hi2c1

//从机地址
#define PSD_ADDRESS (0x01<<1)

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

/* 供外部调用的函数声明 */
void bsp_i2c_Init(void);
uint8_t I2C_Slave_GetRegister(uint16_t MemAddress);
void I2C_Slave_SetRegister(uint16_t MemAddress, uint8_t Data);
