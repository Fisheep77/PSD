#include "main.h"

// I2C寄存器数组，模拟从设备寄存器空间
// PSD_REGISTERS_SIZE 应在头文件中定义，表示寄存器数量
uint8_t I2C_REGISTERS[PSD_REGISTERS_SIZE] = {0};

// 发送数据缓冲区（当前未使用，保留）
uint8_t txData[6] = {0};
// 发送字节计数器，记录已发送的字节数
volatile uint8_t txCount;
// 寄存器读取起始位置，主设备读取时指定起始寄存器地址
volatile uint8_t startPos = 0;

// 接收缓冲区大小定义
#define RxSize 11
// 接收数据缓冲区，存储主设备发送的数据
uint8_t RxData[RxSize];
// 接收字节计数器，记录已接收的字节数
volatile uint8_t rxCount;

// 调试统计变量：地址匹配事件计数
volatile int countAddr = 0;
// 调试统计变量：接收完成事件计数（当前未使用）
volatile int countRxCplt = 0;
// 调试统计变量：I2C错误事件计数
volatile int countError = 0;

/*
*********************************************************************************************************
*    函 数 名: bsp_i2c_Init
*    功能说明: 初始化i2c通信
*    形    参：无
*    返 回 值: 无
*********************************************************************************************************
*/
void bsp_i2c_Init(void)
{
    // 使能I2C从设备监听模式（中断方式）
    HAL_I2C_EnableListen_IT(&SLAVE_I2C);
    
    // 初始化寄存器默认值：
    // PSD_WHO_AM_I寄存器写入设备地址（右移1位转换为7位地址格式）
    I2C_REGISTERS[PSD_WHO_AM_I] = PSD_ADDRESS>>1;    //本机地址
    // 初始化光照阈值寄存器默认值
    I2C_REGISTERS[PSD_LIGHT_THRESHOLD] = 10;    //光照阈值默认为10
}

// 获取指定寄存器的值
// MemAddress: 寄存器地址
// 返回值: 寄存器的当前值
uint8_t I2C_Slave_GetRegister(uint16_t MemAddress)
{
    return I2C_REGISTERS[MemAddress];
}

// 设置指定寄存器的值
// MemAddress: 寄存器地址
// Data: 要写入的数据
void I2C_Slave_SetRegister(uint16_t MemAddress, uint8_t Data)
{
    I2C_REGISTERS[MemAddress] = Data;
}

// 处理接收到的数据：将接收缓冲区中的数据写入寄存器
// 约定：第一个接收字节为起始寄存器地址，后续字节为要写入的数据
void process_data(void)
{
    // 起始寄存器地址（接收的第一个字节）
    int startREG = RxData[0];
    // 实际接收的数据字节数（总字节数减1）
    int numREG = rxCount - 1;
    // 计算结束寄存器地址
    int endREG = startREG + numREG - 1;

    // 寄存器地址越界检查（假设最大寄存器号为9）
    if (endREG > 9) Error_Handler();

    // 数据缓冲区索引（从第二个字节开始）
    int index = 1;

    // 将接收数据写入连续寄存器
    for (int i = 0; i < numREG; i++)
    {
        I2C_REGISTERS[startREG++] = RxData[index++];
    }
}

// I2C监听完成回调函数（重新使能监听）
// hi2c: I2C句柄指针
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    // 重新进入监听模式，等待下一次通信
    HAL_I2C_EnableListen_IT(hi2c); // slave is ready again
}

// 地址匹配回调函数（检测到主设备寻址本从设备时触发）
// hi2c: I2C句柄指针
// TransferDirection: 传输方向（主设备读/写）
// AddrMatchCode: 匹配的地址值
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    // 主设备写操作（从设备接收数据）
    if (TransferDirection == I2C_DIRECTION_TRANSMIT)
    {
        rxCount = 0;        // 接收计数器清零
        countAddr++;        // 地址匹配计数增加（调试用）
        // 启动顺序接收：接收首字节（寄存器地址），使用I2C_FIRST_FRAME标志
        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData + rxCount, 1, I2C_FIRST_FRAME);
    }
    // 主设备读操作（从设备发送数据）
    else if (TransferDirection == I2C_DIRECTION_RECEIVE)
    {
        txCount = 0;        // 发送计数器清零
        // 使用之前接收的RxData[0]作为读取起始地址（约定）
        startPos = RxData[0];
        // 启动顺序发送：从startPos位置发送1字节，使用I2C_FIRST_FRAME标志
        HAL_I2C_Slave_Seq_Transmit_IT(hi2c, I2C_REGISTERS + startPos + txCount, 1, I2C_FIRST_FRAME);
    }
}

// 从设备发送完成回调函数（一字节发送完成）
// hi2c: I2C句柄指针
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    txCount++;  // 发送字节计数增加
    // 继续发送下一字节（自动递增寄存器地址）
    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, I2C_REGISTERS + startPos + txCount, 1, I2C_NEXT_FRAME);
}

// 从设备接收完成回调函数（一字节接收完成）
// hi2c: I2C句柄指针
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    rxCount++;  // 接收字节计数增加

    // 检查接收缓冲区是否未满
    if (rxCount < RxSize)
    {
        // 判断接收阶段：最后一帧还是中间帧
        if (rxCount == RxSize - 1)  // 即将接收最后一字节
        {
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData + rxCount, 1, I2C_LAST_FRAME);
        }
        else  // 中间数据帧
        {
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData + rxCount, 1, I2C_NEXT_FRAME);
        }
    }

    // 接收缓冲区满（达到RxSize）时处理数据
    if (rxCount == RxSize)
    {
        process_data();  // 将接收数据写入寄存器
    }
}

// I2C错误回调函数
// hi2c: I2C句柄指针
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    countError++;  // 错误计数增加
    uint32_t error_code = HAL_I2C_GetError(hi2c);  // 获取具体错误码

    // 处理应答失败错误（ACK Failure）
    if (error_code == 4) //AF error
    {
        // 清除应答错误标志
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

        // 根据传输方向处理错误
        if (txCount == 0) // 接收过程中出错
        {
            // 处理已接收的数据（主设备可能提前结束传输）
            process_data();
        }
        else              // 发送过程中出错
        {
            // 回退发送计数（准备重发当前字节？）
            txCount--;
        }
    }

    // 重新使能监听模式
    HAL_I2C_EnableListen_IT(hi2c); // slave is ready again
}