#pragma once

#include "main.h"

#ifdef USER_LED_Pin
    #define LED_ON()      HAL_GPIO_WritePin( USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET)
    #define LED_OFF()     HAL_GPIO_WritePin( USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET)
    #define LED_TOGGLE()  HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin)
#endif /*LED_Pin*/
