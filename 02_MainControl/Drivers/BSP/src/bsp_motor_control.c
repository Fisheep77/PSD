#include "main.h"

PID_TypeDef BM50_Vel_PID;            //底盘跟随角度pid
first_order_filter_type_t BM50_cmd_slow_set;  //使用一阶低通滤波减缓设定值

void BM50_MotorControlInit( void )
{
    //底盘速度环pid值
    const static float motor_speed_pid[3] = {BM50_MOTOR_SPEED_PID_KP, BM50_MOTOR_SPEED_PID_KI, BM50_MOTOR_SPEED_PID_KD};
    const static float chassis_x_order_filter[1] = {BM50_ACCEL_NUM};

    //初始化角度PID
    PID_init(&BM50_Vel_PID, PID_DELTA, motor_speed_pid, BM50_MOTOR_SPEED_PID_MAX_OUT, BM50_MOTOR_SPEED_PID_MAX_IOUT );

    //对设定速度一阶滤波，降低抖动
    first_order_filter_init(&BM50_cmd_slow_set, CONTROL_TIME, chassis_x_order_filter );
}

/*
*********************************************************************************************************
*   函 数 名: BM50_SetMotorDirection
*   功能说明: 设置电机方向
*   形    参: direction : 方向 (1:正转, -1:反转)
*   返 回 值: 无
*********************************************************************************************************
*/
static void BM50_SetMotorDirection( float _target )
{
    if( _target < 0 ) HAL_GPIO_WritePin( BM50_DIR_GPIO_Port, BM50_DIR_Pin, GPIO_PIN_RESET ); // 逆时针转
    else             HAL_GPIO_WritePin( BM50_DIR_GPIO_Port, BM50_DIR_Pin, GPIO_PIN_SET );  // 瞬时针转
}

/*
*********************************************************************************************************
*   函 数 名: BM50_SetPWMDuty
*   功能说明: 设置PWM占空比
*   形    参: _duty : PWM占空比 (0.0-1.0)
*   返 回 值: 无
*********************************************************************************************************
*/
void BM50_SetPWMDuty( float _duty )
{
    BM50_SetMotorDirection( _duty );
    set_pwm_duty( fabs( _duty ) );        //设置占空比
}

/*
*********************************************************************************************************
*   函 数 名: BM50_GetVel
*   功能说明: 获取当前转速并进行一阶滤波处理
*   形    参: 无
*   返 回 值: 滤波后的转速值(rad/s)
*********************************************************************************************************
*/
float BM50_GetVel( void )
{
    int16_t encoder_counter = get_encoder_counter();    //编码器值

    float phi_dot; // 飞轮角速度
    static float last_phi, last_phi_dot, phi; // 上一次飞轮的转角, 上一次飞轮的角速度, 飞轮的转角
    phi += ( encoder_counter / ENCODER_RES ) * _2PI;
    phi_dot = FILTER_ALPHA * ( phi - last_phi ) / CONTROL_TIME + ( 1 - FILTER_ALPHA ) * last_phi_dot; // 一阶低通滤波
    last_phi_dot = phi_dot;
    last_phi = phi;

    g_obs_encoder = encoder_counter;
    g_obs_phi = phi;
    return phi_dot;
}

/*
*********************************************************************************************************
*   函 数 名: BM50_VelControl
*   功能说明: 电机速度控制(PID算法)
*   形    参: _MotorVelRPM : 目标转速(rad/s)
*   返 回 值: 无
*********************************************************************************************************
*/
void BM50_VelControl( float _MotorVel )
{
    float set = _MotorVel;      //目标转速
    float ref = BM50_GetVel();  //实际转速
    g_obs_vel = ref;

    float out = PID_calc(&BM50_Vel_PID, ref, set ); //增量PID计算之后的输出
    g_obs_BM50_Vel_PID_out = out;

    out = set;
    float set_pwm_duty = out / MAX_RPM;  // 转换为PWM占空比
    set_pwm_duty = fp32_constrain( set_pwm_duty, -0.2, 0.2 ); // 限制输出范围 (0-60%)
    g_obs_set_pwm_duty = set_pwm_duty;

//    set_pwm_duty = 300 / MAX_RPM;
    if( fabs( set_pwm_duty ) < 0.01 ) BM50_SetPWMDuty( 0 ); // 死区限制
    else BM50_SetPWMDuty( set_pwm_duty ); // 更新PWM输出 (低电平有效)
}
