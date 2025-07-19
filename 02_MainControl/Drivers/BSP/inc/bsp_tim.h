#pragma once

#define TIM_Encoder htim4
#define TIM_PWM     htim2

void bsp_InitTimer(void);      

int16_t get_encoder_counter(void);
void set_pwm_duty(float duty);
