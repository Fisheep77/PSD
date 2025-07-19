#pragma once

#define BM50_Enable()      HAL_GPIO_WritePin( BM50_BTAK_GPIO_Port, BM50_BTAK_Pin, GPIO_PIN_SET)
#define BM50_Disable()     HAL_GPIO_WritePin( BM50_BTAK_GPIO_Port, BM50_BTAK_Pin, GPIO_PIN_RESET)


// 系统参数
#define MAX_RPM        544.0f   // 最大转速 // = (4500*_2PI/60)                   
#define MIN_RPM        5.0f         // 最小有效转速
#define CONTROL_TIME   0.005f       // 控制周期(s)
#define CONTROL_TIME_MS 5           // 控制周期(ms)
#define ENCODER_RES    20.0f  // 每转20个脉冲(5PPR*4)

// 电机速度环PID参数
#define BM50_MOTOR_SPEED_PID_KP       5.0f
#define BM50_MOTOR_SPEED_PID_KI       1.0f
#define BM50_MOTOR_SPEED_PID_KD       0.1f
#define BM50_MOTOR_SPEED_PID_MAX_OUT  MAX_RPM
#define BM50_MOTOR_SPEED_PID_MAX_IOUT 0.5f
#define FILTER_ALPHA   0.2f    // 一阶滤波系数

#define BM50_ACCEL_NUM 0.5f

void BM50_MotorControlInit(void);
void BM50_VelControl(float _MotorVelRPM);

extern first_order_filter_type_t BM50_cmd_slow_set;
