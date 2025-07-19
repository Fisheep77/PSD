#include "main.h"

volatile float g_obs_vel = 0;
volatile float g_obs_BM50_Vel_PID_out = 0;
volatile float g_obs_set_pwm_duty = 0;
volatile float g_obs_phi = 0;
volatile float g_obs_float_5 = 0;
volatile float g_obs_float_6 = 0;
volatile float g_obs_PSD_CoordinateX_0 = 0;
volatile float g_obs_PSD_CoordinateY_0 = 0;
volatile int16_t g_obs_encoder = 0;
volatile uint32_t g_obs_uint32 = 0;

void bsp_Init(void)
{
    init_cycle_counter(true);

    bsp_InitTimer();        /* 初始化定时器 */
    bsp_InitUart();
    
    bsp_InitI2C();          /* 初始化I2C */
    MPU6050_initialize();   /* MPU6050初始化 */
    DMP_Init();             /* 初始化DMP */
    bsp_InitPSD();          /* 初始化PSD */

    bsp_InitBlueTooth();
    BM50_MotorControlInit();
    Body_AngleControlInit();
//    bsp_InitFOC();
}
