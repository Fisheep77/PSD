#pragma once

#define CH_NUM    4

#define AD_RESET_LOW()                     HAL_GPIO_WritePin(AD7606_RESET_GPIO_Port, AD7606_RESET_Pin, GPIO_PIN_RESET)
#define AD_RESET_HIGH()                    HAL_GPIO_WritePin(AD7606_RESET_GPIO_Port, AD7606_RESET_Pin, GPIO_PIN_SET)

#define AD_CONVST_LOW()                    HAL_GPIO_WritePin(AD7606_CONVST_GPIO_Port, AD7606_CONVST_Pin, GPIO_PIN_RESET)
#define AD_CONVST_HIGH()                   HAL_GPIO_WritePin(AD7606_CONVST_GPIO_Port, AD7606_CONVST_Pin, GPIO_PIN_SET)

#define AD_BUSY_IS_LOW()                   (HAL_GPIO_ReadPin(AD7606_BUSY_GPIO_Port, AD7606_BUSY_Pin) == GPIO_PIN_RESET)

void bsp_spi_AD7606_Init(void);
uint8_t AD7606_Scan(void);
int16_t AD7606_ReadAdc(uint8_t _ch);
