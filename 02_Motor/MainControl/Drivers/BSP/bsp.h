#pragma once

#define PRINT(window, fmt, args...) printf("{"#window"}"fmt"\n", ##args);
//PRINT(plotter, "%d,%d", current, voltage);

/*
*********************************************************************************************************
*                                         CubeMX
*********************************************************************************************************
*/
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "arm_math.h"
#include "perf_counter.h"

/*
*********************************************************************************************************
*                                         BSP / APP
*********************************************************************************************************
*/
#include "bsp_pid.h"
#include "bsp_user_lib.h"
#include "bsp_keyled.h"
#include "bsp_uart_dma.h"
#include "bsp_bluetooth.h"
#include "bsp_i2c_master.h"
#include "bsp_tim.h"
#include "bsp_filter.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "bsp_mpu6050.h"
#include "bsp_psd.h"

#include "bsp_motor_control.h"
#include "bsp_body_angle_control.h"

/*
*********************************************************************************************************
*                                          供外部调用的函数
*********************************************************************************************************
*/
void bsp_Init( void );


extern volatile float g_obs;
extern volatile float g_obs_vel;
extern volatile float g_obs_BM50_Vel_PID_out;
extern volatile float g_obs_set_pwm_duty;
extern volatile float g_obs_phi;
extern volatile float g_obs_float_5;
extern volatile float g_obs_float_6;
extern volatile float g_obs_PSD_CoordinateX_0;
extern volatile float g_obs_PSD_CoordinateY_0;
extern volatile int16_t g_obs_encoder;
extern volatile uint32_t g_obs_uint32;
extern float target_vel_set;
extern float target_angle;
extern uint8_t ucStable_Flag;

