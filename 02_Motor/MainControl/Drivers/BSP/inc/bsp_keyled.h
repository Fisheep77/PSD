#pragma once

#include "main.h"

#ifdef LED_Pin
    #define LED_ON()      HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET)
    #define LED_OFF()     HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET)
    #define LED_TOGGLE()  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)
#endif /*LED_Pin*/
