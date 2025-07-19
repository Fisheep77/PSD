#include "main.h"

void AD7606_Reset(void);
void AD7606_StartConv(void);

static int16_t s_adc_now[8];
int8_t g_obs = -1;

/**
  * @brief  Init AD7606
  * @param  None
  * @retval None
  */
void bsp_spi_AD7606_Init(void)
{
    AD7606_Reset();              /* Reset AD7606 */
    AD_CONVST_HIGH();            /* Set CONVST Pin to High level */
}

/**
  * @brief  Reset AD7606
  * @param  None
  * @retval None
  */
void AD7606_Reset(void)
{
    AD_RESET_LOW();

    AD_RESET_HIGH();
    HAL_Delay(1);

    AD_RESET_LOW();
}

/**
  * @brief  start AD7606 convert
  * @param  None
  * @retval None
  */
void AD7606_StartConv(void)
{
    /* 上升沿开始转换，低电平持续时间至少25ns  */
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();    /* 连续执行2次，低电平约50ns */

    AD_CONVST_HIGH();
}

/**
  * @brief  get AD7606 value
  * @param  None
  * @retval status
  */
uint8_t AD7606_Scan(void)         /* 此函数代码按照时序编写 */
{
    uint8_t i;
    uint8_t status = 0;

    /* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */
    if (AD_BUSY_IS_LOW())
    {
        for (i = 0; i < CH_NUM; i++)
        {
            uint8_t rx[2] = {0};
            g_obs = HAL_SPI_Receive(&hspi1, rx, 2, HAL_MAX_DELAY);

            s_adc_now[i] = (rx[0] << 8) | (rx[1]);
        }

        AD7606_StartConv();    /* 给开始信号 */

        status = 1;
    }

    return status;
}

/**
  * @brief  return AD7606 value
  * @param  _ch channel
  * @retval value
  */
int16_t AD7606_ReadAdc(uint8_t _ch)
{
    int16_t sAdc;

    sAdc = s_adc_now[_ch];

    return sAdc;
}
