/*
*********************************************************************************************************
*
*	模块名称 : NAND Flash 驱动模块
*	文件名称 : bsp_nand.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_NAND_H
#define __BSP_NAND_H

typedef struct
{
  uint8_t Maker_ID;
  uint8_t Device_ID;
  uint8_t Third_ID;
  uint8_t Fourth_ID;
}NAND_IDTypeDef;

typedef struct
{
  uint16_t Zone;
  uint16_t Block;
  uint16_t Page;
} NAND_ADDRESS_T;

/* NAND Flash 型号 */
typedef enum
{
	HY27UF081G2A = 0,
	K9F1G08U0A,
	K9F1G08U0B,
	NAND_UNKNOW
}NAND_TYPE_E;

#define NAND_TYPE	HY27UF081G2A

/*
	定义有效的 NAND ID
	HY27UF081G2A  	= 0xAD 0xF1 0x80 0x1D
	K9F1G08U0A		= 0xEC 0xF1 0x80 0x15
	K9F1G08U0B		= 0xEC 0xF1 0x00 0x95
*/
#define NAND_MAKER_ID	0xAD
#define NAND_DEVICE_ID	0xF1
#define NAND_THIRD_ID 	0x80
#define NAND_FOURTH_ID	0x1D

#define HY27UF081G2A	0xADF1801D
#define K9F1G08U0A		0xECF18015
#define K9F1G08U0B		0xECF10095

/* Exported constants --------------------------------------------------------*/
/* NAND Area definition  for STM3210E-EVAL Board RevD */
#define CMD_AREA                   (uint32_t)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (uint32_t)(1<<17)  /* A17 = ALE high */
#define DATA_AREA                  ((uint32_t)0x00000000)

/* FSMC NAND memory command */
#define	NAND_CMD_AREA_A            ((uint8_t)0x00)
#define	NAND_CMD_AREA_B            ((uint8_t)0x01)
#define NAND_CMD_AREA_C            ((uint8_t)0x50)
#define NAND_CMD_AREA_TRUE1        ((uint8_t)0x30)

#define NAND_CMD_WRITE0            ((uint8_t)0x80)
#define NAND_CMD_WRITE_TRUE1       ((uint8_t)0x10)

#define NAND_CMD_ERASE0            ((uint8_t)0x60)
#define NAND_CMD_ERASE1            ((uint8_t)0xD0)

#define NAND_CMD_READID            ((uint8_t)0x90)

#define NAND_CMD_LOCK_STATUS       ((uint8_t)0x7A)
#define NAND_CMD_RESET             ((uint8_t)0xFF)

/* NAND memory status */
#define NAND_BUSY                  ((uint8_t)0x00)
#define NAND_ERROR                 ((uint8_t)0x01)
#define NAND_READY                 ((uint8_t)0x40)
#define NAND_TIMEOUT_ERROR         ((uint8_t)0x80)

/* FSMC NAND memory parameters */
/* 用于HY27UF081G2A    K9F1G08 */
#if NAND_TYPE == HY27UF081G2A
	#define NAND_PAGE_SIZE             ((uint16_t)0x0800) /* 2 * 1024 bytes per page w/o Spare Area */
	#define NAND_BLOCK_SIZE            ((uint16_t)0x0040) /* 64 pages per block */
	#define NAND_ZONE_SIZE             ((uint16_t)0x0400) /* 1024 Block per zone */
	#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0040) /* last 64 bytes as spare area */
	#define NAND_MAX_ZONE              ((uint16_t)0x0001) /* 1 zones of 1024 block */

	/* 命令代码定义 */
	#define NAND_CMD_COPYBACK_A			((uint8_t)0x00)		/* PAGE COPY-BACK 命令序列 */
	#define NAND_CMD_COPYBACK_B			((uint8_t)0x35)
	#define NAND_CMD_COPYBACK_C			((uint8_t)0x85)
	#define NAND_CMD_COPYBACK_D			((uint8_t)0x10)

	#define NAND_CMD_STATUS				((uint8_t)0x70)		/* 读NAND Flash的状态字 */

	#define MAX_PHY_BLOCKS_PER_ZONE  1024	/* 每个区最大物理块号 */
	#define MAX_LOG_BLOCKS_PER_ZONE  1000	/* 每个区最大逻辑块号 */

	#define NAND_BLOCK_COUNT			1024 /* 块个数 */
	#define NAND_PAGE_TOTAL_SIZE		(NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE)	/* 页面总大小 */



#else
	#define NAND_PAGE_SIZE             ((uint16_t)0x0200) /* 512 bytes per page w/o Spare Area */
	#define NAND_BLOCK_SIZE            ((uint16_t)0x0020) /* 32x512 bytes pages per block */
	#define NAND_ZONE_SIZE             ((uint16_t)0x0400) /* 1024 Block per zone */
	#define NAND_SPARE_AREA_SIZE       ((uint16_t)0x0010) /* last 16 bytes as spare area */
	#define NAND_MAX_ZONE              ((uint16_t)0x0004) /* 4 zones of 1024 block */
#endif

#define NAND_BAD_BLOCK_FLAG			0x00	/* 块内第1个page备用区的第1个字节写入非0xFF数据表示坏块 */
#define NAND_USED_BLOCK_FLAG		0xF0	/* 块内第1个page备用区的第2个字节写入非0xFF数据表示已使用的块 */

#define BI_OFFSET				0		/* 块内第1个page备用区的第1个字节是坏块标志 */
#define USED_OFFSET				1		/* 块内第1个page备用区的第1个字节是已用标志 */
#define LBN0_OFFSET				2		/* 块内第1个page备用区的第3个字节表示逻辑块号低8bit */
#define LBN1_OFFSET				3		/* 块内第1个page备用区的第4个字节表示逻辑块号高8bit */
#define VALID_SPARE_SIZE		4		/* 实际使用的备用区大小,用于函数内部声明数据缓冲区大小 */

/* FSMC NAND memory address computation */
#define ADDR_1st_CYCLE(ADDR)       (uint8_t)((ADDR)& 0xFF)               /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF00) >> 8)      /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF0000) >> 16)   /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (uint8_t)(((ADDR)& 0xFF000000) >> 24) /* 4th addressing cycle */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#define NAND_OK   0
#define NAND_FAIL 1

#define FREE_BLOCK  (1 << 12 )
#define BAD_BLOCK   (1 << 13 )
#define VALID_BLOCK (1 << 14 )
#define USED_BLOCK  (1 << 15 )

/*
		LUT[]的格式：
		uint16_t usGoodBlockFirst;				 // 第1个好块
		uint16_t usDataBlockCount;	             // 可用于数据存储的块个数, 从第2个好块开始
		uint16_t usBakBlockStart;				 // 备份块起始块号
		uint32_t usPhyBlockNo[ulDataBlockCount]; // 物理块号数组。低字节在前，高字节在后。
*/
#define DATA_BLOCK_PERCENT		98	/* 数据块占总有效块数的百分比 */
#define LUT_FIRST_GOOD_BLOCK	0	/* LUT[] 第1个单元用于存储第1个有效块号 */
#define LUT_DATA_BLOCK_COUNT	1	/* LUT[] 第2个单元用于存储第有效块号个数 */
#define LUT_BAK_BLOCK_START		2	/* LUT[] 第3个单元用于备份区起始块号 */
#define LUT_GOOD_BLOCK_START	3	/* LUT[] 第4个单元用于数据区起始块号 */


/* Private Structures---------------------------------------------------------*/
typedef struct __SPARE_AREA {
	uint16_t LogicalIndex;
	uint16_t DataStatus;
	uint16_t BlockStatus;
} SPARE_AREA;

typedef enum {
  WRITE_IDLE = 0,
  POST_WRITE,
  PRE_WRITE,
  WRITE_CLEANUP,
  WRITE_ONGOING
}WRITE_STATE;

typedef enum {
  OLD_BLOCK = 0,
  UNUSED_BLOCK
}BLOCK_STATE;

/* Private macro --------------------------------------------------------------*/
//#define WEAR_LEVELLING_SUPPORT		磨损平衡支持
#define WEAR_DEPTH         10			/* 磨损深度 */
#define PAGE_TO_WRITE      (Transfer_Length/512)

#define BAD_BALOK_TEST_CYCLE	5		/* 判别坏块算法的重复擦写次数  */

/* Private variables ----------------------------------------------------------*/
/* Private function prototypes ------------------------------------------------*/
/* exported functions ---------------------------------------------------------*/
uint8_t NAND_Init(void);
uint8_t NAND_Write(uint32_t _ulMemAddr, uint32_t *_pWriteBuf, uint16_t _usSize);
uint8_t NAND_Read(uint32_t _ulMemAddr, uint32_t *_pReadBuf, uint16_t _usSize);
uint8_t NAND_Format(void);
void NAND_DispBadBlockInfo(void);
uint8_t NAND_ScanBlock(uint32_t _ulPageNo);
uint32_t NAND_FormatCapacity(void);
uint32_t NAND_ReadID(void);

void NAND_DispPhyPageData(uint32_t _uiPhyPageNo);
void NAND_DispLogicPageData(uint32_t _uiLogicPageNo);

uint8_t NAND_WriteMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount);
uint8_t NAND_ReadMultiSectors(uint8_t *_pBuf, uint32_t _SectorNo, uint16_t _SectorSize, uint32_t _SectorCount);

#endif /* __FSMC_NAND_H */

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
