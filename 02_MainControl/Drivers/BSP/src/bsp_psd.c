#include "main.h"

float LightAngle;
float LightThreshold;
float PSD_CoordinateX[PSD_NUM];
float PSD_CoordinateY[PSD_NUM];

// 给地址为_PSD_ID的太敏设置光照阈值
void PSD_SetLightThreshold(uint8_t _LightThreshold)
{
    uint8_t LightThreshold[1] = {_LightThreshold};
    I2C_Write(PSD_ADDRESS_1, PSD_LIGHT_THRESHOLD, 1, LightThreshold);
#ifdef PSD_ADDRESS_2
    I2C_Write(PSD_ADDRESS_2, PSD_LIGHT_THRESHOLD, 1, LightThreshold);
#endif
}

// 获取地址为_PSD_ID的太敏 的ID，用于上电检测传感器
uint8_t PSD_GetDeviceID(uint8_t _PSD_ID)
{
    uint8_t buffer[1];
    I2C_ReadBytes(_PSD_ID, PSD_WHO_AM_I, 1, buffer);
    return buffer[0];
}

// 检测PSD设备的连接情况
uint8_t PSD_TestConnection(void)
{
    uint8_t ret = 0;
    uint8_t id = 0;

    id = PSD_GetDeviceID(PSD_ADDRESS_1);
    // 检查第一个地址（必须存在）
    if (id == (PSD_ADDRESS_1 >> 1))
    {
        ret = 1;
    }

#ifdef PSD_ADDRESS_2 // 如果定义了 PSD_ADDRESS_2，才检查第二个地址
    if (ret == 1)    // 只有第一个地址有效时才检查第二个
    {
        if (PSD_GetDeviceID(PSD_ADDRESS_2) == (PSD_ADDRESS_2 >> 1))
        {
            ret = 2;
        }
    }
#endif

    return ret;
}

// PSD初始化
void bsp_InitPSD(void)
{
    PSD_TestConnection(); // 检查各太敏连接情况
}

// 读取地址为_PSD_ID的光斑坐标
void PSD_ReadOneCoordinate(uint8_t _PSD_ID)
{
    // 根据PSD_ID选择坐标数组的索引
    uint8_t index = 0;
    switch (_PSD_ID)
    {
    case PSD_ADDRESS_1:
        index = 0;
        break;
#ifdef PSD_ADDRESS_2
    case PSD_ADDRESS_2:
        index = 1;
        break;
#endif
    default:
        break;
    }

    uint32_t RawData;
    uint8_t temp[4];

    // 从 I2C 读取 4 字节数据
    I2C_Read(_PSD_ID, PSD_COORDINATE_X, 4, temp);
    RawData = ((uint32_t)temp[0] << 24) |
              ((uint32_t)temp[1] << 16) |
              ((uint32_t)temp[2] << 8)  |
              ((uint32_t)temp[3] << 0);

    // 将 uint32_t 的二进制表示直接转换为 float
    memcpy(&PSD_CoordinateX[index], &RawData, sizeof(float));

    // 从 I2C 读取 4 字节数据
    I2C_Read(_PSD_ID, PSD_COORDINATE_Y, 4, temp);
    RawData = ((uint32_t)temp[0] << 24) |
              ((uint32_t)temp[1] << 16) |
              ((uint32_t)temp[2] << 8)  |
              ((uint32_t)temp[3] << 0);

    // 将 uint32_t 的二进制表示直接转换为 float
    memcpy(&PSD_CoordinateY[index], &RawData, sizeof(float));

    g_obs_PSD_CoordinateX_0 = PSD_CoordinateX[index];
    g_obs_PSD_CoordinateY_0 = PSD_CoordinateY[index];
}

void PSD_ReadCoordinate(void)
{
    PSD_ReadOneCoordinate(PSD_ADDRESS_1);

#ifdef PSD_ADDRESS_2
    PSD_ReadOneCoordinate(PSD_ADDRESS_2);
#endif
}

// 获得入射光角度
float PSD_GetLightAngle(void)
{
    float radians;
    float degree;
    float CoordinateY = PSD_CoordinateY[0];

    // 限制输入范围
    CoordinateY = fp32_constrain(CoordinateY, -2.0f, 2.0f);

    // 使用atan2计算arctan(x/1)，等效于atan(x)
    arm_atan2_f32(CoordinateY, 1.0f, &radians);

    // 弧度转角度
    degree = radians * 57.2957795f; // 180/π ≈ 57.2957795
    return degree;
}
