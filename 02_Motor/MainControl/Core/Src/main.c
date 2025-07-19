/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float target_vel_set = 0;
float target_vel = 0;
float target_angle = 0;
uint8_t ucStable_Flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    HAL_Delay(500);
    bsp_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    printf("\r\n\r\n\r\n*******\r\nHello\r\n*******\r\n\r\n\r\n");
    uint8_t angle_control_flag = 0;
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        if (perfc_is_time_out_ms(5)) // 陀螺仪角度读取  周期5ms
        {
            // 读取MPU6050的Yaw角
            MPU6050_Read_DMP(); //===读取倾角
            if (ucStable_Flag == 1)
            {
                YawOffset = Yaw;
                ucStable_Flag = 0;
                angle_control_flag = 1;
            }
            Yaw -= YawOffset;
        }

        if (perfc_is_time_out_ms(10)) // 太阳光角度读取  周期10ms
        {
            // 读取各PSD的坐标点
            PSD_ReadCoordinate();
            LightAngle = PSD_GetLightAngle();
            target_angle = LightAngle;
            target_angle = 0;
        }

        if (perfc_is_time_out_ms(BODY_ANGLE_CONTROL_TIME_MS)) // 角度控制  周期50ms
        {
            if (angle_control_flag == 1)
            {
                target_vel_set += Body_AngleControl(target_angle);
            }
        }

        if (perfc_is_time_out_ms(CONTROL_TIME_MS)) // 速度控制  周期5ms
        {
            first_order_filter_cali(&BM50_cmd_slow_set, target_vel_set); // 将设定速度进行一阶滤波
            target_vel = BM50_cmd_slow_set.out;                      // 使用一阶滤波后的速度作为电机的目标值
            target_vel = target_vel_set;                      // 使用一阶滤波后的速度作为电机的目标值
            BM50_VelControl(target_vel);
        }

        if (perfc_is_time_out_ms(50)) // 串口打印,读取  周期50ms
        {
            /* 串口读取 */
            BlueTooth_Read();

            
            /* 串口打印 */
            //BM50电机动态参数
            PRINT(encoder, "%d", g_obs_encoder);
            PRINT(phi, "%.4f", g_obs_phi);
            PRINT(rpm, "%.4f,%.4f,%.4f", target_vel_set, target_vel, g_obs_vel);
//            PRINT(out, "%.4f,%.4f", g_obs_BM50_Vel_PID_out, g_obs_set_pwm_duty);
            PRINT(pwm_duty, "%.4f", g_obs_set_pwm_duty);

            //光斑坐标
            PRINT(Coordinate, "%.4f,%.4f", g_obs_PSD_CoordinateX_0, g_obs_PSD_CoordinateY_0);

            //角度
            PRINT(Yaw, "%.4f,%.4f,%.4f", target_angle, Yaw, LightAngle);
        }

        if (perfc_is_time_out_ms(500)) // LED闪烁，判断是否卡死  周期500ms
        {
            LED_TOGGLE();
        }
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
