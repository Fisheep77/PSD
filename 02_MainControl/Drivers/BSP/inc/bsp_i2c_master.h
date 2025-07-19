#pragma once

#define MASTER_I2C hi2c1

void bsp_InitI2C(void);
uint8_t I2C_Write( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t *pData );
uint8_t I2C_Read( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t *pData );
uint8_t I2C_ReadOneByte( uint16_t DevAddress, uint16_t MemAddress );
uint16_t I2C_ReadBytes( uint16_t DevAddress, uint16_t MemAddress, uint16_t Size, uint8_t *pData );
uint8_t I2C_WriteBit( uint16_t DevAddress, uint16_t MemAddress, uint8_t bitNum, uint8_t data );
uint8_t I2C_WriteBits( uint16_t DevAddress, uint16_t MemAddress, uint8_t bitStart, uint8_t length, uint8_t data );


