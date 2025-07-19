#include "main.h"

void bsp_InitI2C( void )
{
}

uint8_t I2C_Write( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t* pData )
{
    if( HAL_I2C_Mem_Write(&MASTER_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY ) != HAL_OK )
    {
        return 1;
    }
    return 0;
}

uint8_t I2C_Read( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t* pData )
{
    if( HAL_I2C_Mem_Read(&MASTER_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY ) != HAL_OK )
    {
        return 1;
    }
    return 0;
}

uint8_t I2C_ReadOneByte( uint16_t DevAddress, uint16_t MemAddress )
{
    uint8_t data;
    if( HAL_I2C_Mem_Read(&MASTER_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY ) != HAL_OK )
    {
        return 0;
    }
    return data;
}

uint16_t I2C_ReadBytes( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t* pData )
{
    if( HAL_I2C_Mem_Read(&MASTER_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, HAL_MAX_DELAY ) != HAL_OK )
    {
        return 0;
    }
    return Size;
}

/**************************实现函数********************************************
*函数原型:      uint8_t I2C_ReadByte(uint8_t dev, uint8_t reg, uint8_t *data)
*功　　能:      读取指定设备 指定寄存器的一个值
输入    dev  目标设备地址
        reg    寄存器地址
        *data  读出的数据将要存放的地址
返回   1
*******************************************************************************/
uint8_t I2C_ReadByte( uint16_t DevAddress, uint16_t MemAddress, uint8_t* pData )
{
    *pData = I2C_ReadOneByte( DevAddress, MemAddress );
    return 1;
}

/**************************实现函数********************************************
*函数原型:      unsigned char I2C_WriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*功　　能:      写入指定设备 指定寄存器一个字节
输入    dev  目标设备地址
        reg    寄存器地址
        data  将要写入的字节
返回   1
*******************************************************************************/
uint8_t I2C_WriteByte( uint16_t DevAddress, uint16_t MemAddress, uint8_t data )
{
    if( HAL_I2C_Mem_Write(&MASTER_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY ) != HAL_OK )
    {
        return 0;
    }
    return 1;
}

/**************************实现函数********************************************
*函数原型:      uint8_t I2C_WriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data)
*功　　能:      读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入    dev  目标设备地址
        reg    寄存器地址
        bitStart  目标字节的起始位
        length   位长度
        data    存放改变目标字节位的值
返回   成功 为1
        失败为0
*******************************************************************************/
uint8_t I2C_WriteBits( uint16_t DevAddress, uint16_t MemAddress, uint8_t bitStart, uint8_t length, uint8_t data )
{
    uint8_t b;
    if( I2C_ReadByte( DevAddress, MemAddress, &b ) != 0 )
    {
        uint8_t mask = ( 0xFF << ( bitStart + 1 ) ) | 0xFF >> (( 8 - bitStart ) + length - 1 );
        data <<= ( 8 - length );
        data >>= ( 7 - bitStart );
        b &= mask;
        b |= data;
        return I2C_WriteByte( DevAddress, MemAddress, b );
    }
    else
    {
        return 0;
    }
}

/**************************实现函数********************************************
*函数原型:      uint8_t I2C_WriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
*功　　能:      读 修改 写 指定设备 指定寄存器一个字节 中的1个位
输入    dev  目标设备地址
        reg    寄存器地址
        bitNum  要修改目标字节的bitNum位
        data  为0 时，目标位将被清0 否则将被置位
返回   成功 为1
        失败为0
*******************************************************************************/
uint8_t I2C_WriteBit( uint16_t DevAddress, uint16_t MemAddress, uint8_t bitNum, uint8_t data )
{
    uint8_t b;
    I2C_ReadByte( DevAddress, MemAddress, &b );
    b = ( data != 0 ) ? ( b | ( 1 << bitNum ) ) : ( b & ~( 1 << bitNum ) );
    return I2C_WriteByte( DevAddress, MemAddress, b );
}

//------------------End of File----------------------------
