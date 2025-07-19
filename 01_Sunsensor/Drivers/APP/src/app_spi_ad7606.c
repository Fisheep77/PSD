#include "main.h"

static void AD7606_Mak(void);
static void AD7606_Disp(void);

static int16_t s_dat[8];
static float s_volt[8];

//params about PSD
static float PSD_L = 4.5;       //PSD length
static float coordinate_x = 0;
static float coordinate_y = 0;
static float s_theta = 0;

// 通用低通滤波器结构体
typedef struct {
    float alpha;    // 滤波系数 (0 < alpha < 1)
    float prev;     // 上一次滤波后的值
} LPF_Filter;

// 初始化滤波器
void LPF_Init(LPF_Filter *filter, float alpha, float init_val) {
    filter->alpha = alpha;
    filter->prev = init_val;
}

// 通用低通滤波函数
float LPF_Update(LPF_Filter *filter, float input) {
    float output = filter->alpha * filter->prev + (1 - filter->alpha) * input;
    filter->prev = output;  // 更新历史值
    return output;
}

// 定义滤波器实例（存储历史状态）
static LPF_Filter lpf_x = { .alpha = 0.9f, .prev = 0 };  // X坐标滤波器
static LPF_Filter lpf_y = { .alpha = 0.9f, .prev = 0 };  // Y坐标滤波器

/**
  * @brief  AD7606 demo
  * @param  None
  * @retval None
  */
void Demo_spi_AD7606(void)
{    
    uint8_t ucData;
    while (1)
    {
        //        bsp_Idle();
        if( comGetChar( COM2, &ucData ) )
        {
            HAL_UART_Transmit(&huart2, (uint8_t*)&ucData, 1, HAL_MAX_DELAY);
        }
        
        if (perfc_is_time_out_ms(5))  //2ms任务周期
        {
            AD7606_Scan();     //进行AD7606扫描转换，采集数据
            AD7606_Mak();      //对采集到的数据进行处理
        }
        
        if (perfc_is_time_out_ms(100))  // 100ms任务周期
        {
            AD7606_Disp();     //打印传输数据
            LED_TOGGLE();      //灯翻转
        }
    }
}


void I2C_Slave_SetCoordinateRegister(uint16_t adr, float val)
{
    uint32_t val_bits;
    memcpy(&val_bits, &val, sizeof(float));  // 复制 float 的二进制表示到 uint32_t

    if (adr == PSD_COORDINATE_X)
    {
        I2C_Slave_SetRegister(PSD_COORDINATE_X+0, (uint8_t)(val_bits >> 24));
        I2C_Slave_SetRegister(PSD_COORDINATE_X+1, (uint8_t)(val_bits >> 16));
        I2C_Slave_SetRegister(PSD_COORDINATE_X+2, (uint8_t)(val_bits >> 8 ));
        I2C_Slave_SetRegister(PSD_COORDINATE_X+3, (uint8_t)(val_bits >> 0 ));
    }
    else if (adr == PSD_COORDINATE_Y)
    {
        I2C_Slave_SetRegister(PSD_COORDINATE_Y+0, (uint8_t)(val_bits >> 24));
        I2C_Slave_SetRegister(PSD_COORDINATE_Y+1, (uint8_t)(val_bits >> 16));
        I2C_Slave_SetRegister(PSD_COORDINATE_Y+2, (uint8_t)(val_bits >> 8 ));
        I2C_Slave_SetRegister(PSD_COORDINATE_Y+3, (uint8_t)(val_bits >> 0 ));
    }
}

/**
  * @brief  read values and calculate
  * @param  None
  * @retval None
  */
void AD7606_Mak(void)
{
    for (uint8_t i = 0; i < CH_NUM; i++)
    {
        s_dat[i] = AD7606_ReadAdc(i);        
    }
    
    uint8_t LightThreshold = I2C_Slave_GetRegister(PSD_LIGHT_THRESHOLD); //设置光照阈值
    if ((abs(s_dat[0]) < LightThreshold) && 
        (abs(s_dat[1]) < LightThreshold) && 
        (abs(s_dat[2]) < LightThreshold) && 
        (abs(s_dat[3]) < LightThreshold))     //如果各通道的光电流都未达到阈值，说明入射光无效，将数据置0
    {
        lpf_x.prev = 0;
        lpf_y.prev = 0;
        coordinate_x = 0;  // 更新X坐标滤波
        coordinate_y = 0;  // 更新Y坐标滤波
        I2C_Slave_SetCoordinateRegister(PSD_COORDINATE_X, coordinate_x);  //将float型的坐标X转为4字节，存入寄存器PSD_COORDINATE_X
        I2C_Slave_SetCoordinateRegister(PSD_COORDINATE_Y, coordinate_y);  //将float型的坐标Y转为4字节，存入寄存器PSD_COORDINATE_Y
        return;
    }
    
    //根据S5990手册公式，将四通道光电流转换为坐标
    int32_t sum = s_dat[0] + s_dat[1] + s_dat[2] + s_dat[3];   

    int32_t temp1 = s_dat[1] + s_dat[2] - (s_dat[0] + s_dat[3]);
    int32_t temp2 = s_dat[1] + s_dat[3] - (s_dat[0] + s_dat[2]);

    //原始数据
    float raw_x = temp1 * PSD_L / 2.0f / sum;
    float raw_y = temp2 * PSD_L / 2.0f / sum;
    
    //滤波数据
    coordinate_x = LPF_Update(&lpf_x, raw_x);  // 更新X坐标滤波
    coordinate_y = LPF_Update(&lpf_y, raw_y);  // 更新Y坐标滤波
    
    I2C_Slave_SetCoordinateRegister(PSD_COORDINATE_X, coordinate_x); //将float型的坐标X转为4字节，存入寄存器PSD_COORDINATE_X
    I2C_Slave_SetCoordinateRegister(PSD_COORDINATE_Y, coordinate_y); //将float型的坐标Y转为4字节，存入寄存器PSD_COORDINATE_Y
}

/**
  * @brief  show results
  * @param  None
  * @retval None
  */
void AD7606_Disp(void)
{
#if 1
    PRINT(raw_data, "%d,%d,%d,%d", s_dat[0], s_dat[1], s_dat[2], s_dat[3]);
    PRINT(coordinate, "%.3f,%.3f", coordinate_x, coordinate_y);
#else

#endif
}
