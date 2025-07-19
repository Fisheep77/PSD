#include "main.h"

float LightAngle;
float LightThreshold;
float PSD_CoordinateX[PSD_NUM];
float PSD_CoordinateY[PSD_NUM];

// ����ַΪ_PSD_ID��̫�����ù�����ֵ
void PSD_SetLightThreshold(uint8_t _LightThreshold)
{
    uint8_t LightThreshold[1] = {_LightThreshold};
    I2C_Write(PSD_ADDRESS_1, PSD_LIGHT_THRESHOLD, 1, LightThreshold);
#ifdef PSD_ADDRESS_2
    I2C_Write(PSD_ADDRESS_2, PSD_LIGHT_THRESHOLD, 1, LightThreshold);
#endif
}

// ��ȡ��ַΪ_PSD_ID��̫�� ��ID�������ϵ��⴫����
uint8_t PSD_GetDeviceID(uint8_t _PSD_ID)
{
    uint8_t buffer[1];
    I2C_ReadBytes(_PSD_ID, PSD_WHO_AM_I, 1, buffer);
    return buffer[0];
}

// ���PSD�豸���������
uint8_t PSD_TestConnection(void)
{
    uint8_t ret = 0;
    uint8_t id = 0;

    id = PSD_GetDeviceID(PSD_ADDRESS_1);
    // ����һ����ַ��������ڣ�
    if (id == (PSD_ADDRESS_1 >> 1))
    {
        ret = 1;
    }

#ifdef PSD_ADDRESS_2 // ��������� PSD_ADDRESS_2���ż��ڶ�����ַ
    if (ret == 1)    // ֻ�е�һ����ַ��Чʱ�ż��ڶ���
    {
        if (PSD_GetDeviceID(PSD_ADDRESS_2) == (PSD_ADDRESS_2 >> 1))
        {
            ret = 2;
        }
    }
#endif

    return ret;
}

// PSD��ʼ��
void bsp_InitPSD(void)
{
    PSD_TestConnection(); // ����̫���������
}

// ��ȡ��ַΪ_PSD_ID�Ĺ������
void PSD_ReadOneCoordinate(uint8_t _PSD_ID)
{
    // ����PSD_IDѡ���������������
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

    // �� I2C ��ȡ 4 �ֽ�����
    I2C_Read(_PSD_ID, PSD_COORDINATE_X, 4, temp);
    RawData = ((uint32_t)temp[0] << 24) |
              ((uint32_t)temp[1] << 16) |
              ((uint32_t)temp[2] << 8)  |
              ((uint32_t)temp[3] << 0);

    // �� uint32_t �Ķ����Ʊ�ʾֱ��ת��Ϊ float
    memcpy(&PSD_CoordinateX[index], &RawData, sizeof(float));

    // �� I2C ��ȡ 4 �ֽ�����
    I2C_Read(_PSD_ID, PSD_COORDINATE_Y, 4, temp);
    RawData = ((uint32_t)temp[0] << 24) |
              ((uint32_t)temp[1] << 16) |
              ((uint32_t)temp[2] << 8)  |
              ((uint32_t)temp[3] << 0);

    // �� uint32_t �Ķ����Ʊ�ʾֱ��ת��Ϊ float
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

// ��������Ƕ�
float PSD_GetLightAngle(void)
{
    float radians;
    float degree;
    float CoordinateY = PSD_CoordinateY[0];

    // �������뷶Χ
    CoordinateY = fp32_constrain(CoordinateY, -2.0f, 2.0f);

    // ʹ��atan2����arctan(x/1)����Ч��atan(x)
    arm_atan2_f32(CoordinateY, 1.0f, &radians);

    // ����ת�Ƕ�
    degree = radians * 57.2957795f; // 180/�� �� 57.2957795
    return degree;
}
