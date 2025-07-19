#include "main.h"

void bsp_InitTimer( void )
{
    //启动编码器
    HAL_TIM_Encoder_Start(&TIM_Encoder, TIM_CHANNEL_ALL );

    //启动PWM输出波
    HAL_TIM_PWM_Start(&TIM_PWM, TIM_CHANNEL_1 );
}

/**
 * @brief 读取编码器计数（单位时间）
 * @param none
 * @return uint32_t 编码器计数值
 */
int16_t get_encoder_counter( void )
{
    static uint8_t once = 0;
    if( once == 0 )
    {
        __HAL_TIM_SET_COUNTER(&TIM_Encoder, 0 );
        once = 1;
    }

    int16_t encoder_counter = __HAL_TIM_GET_COUNTER(&TIM_Encoder ); // 读取当前值（16位有符号）
//    g_obs_encoder = encoder_counter;
    __HAL_TIM_SET_COUNTER(&TIM_Encoder, 0 );                    // 清零计数器
    return encoder_counter;
}

void set_pwm_duty( float duty )
{
    __HAL_TIM_SET_COMPARE(&TIM_PWM, TIM_CHANNEL_1, duty * TIM_PWM.Instance->ARR );
}
