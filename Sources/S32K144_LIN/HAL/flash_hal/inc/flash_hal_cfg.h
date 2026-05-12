#ifndef S32K144_LIN_HAL_FLASH_HAL_INC_FLASH_HAL_CFG_H_
#define S32K144_LIN_HAL_FLASH_HAL_INC_FLASH_HAL_CFG_H_

#if defined (__cplusplus)
extern "C" {
#endif

//******************************** Includes *********************************//
#include "common_types.h"
#include "user_config.h"
#include <stdint.h>
#include <stdbool.h>
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
// Flash编程缓冲区最大字节数
#define MAX_FLASH_DATA_LEN (200u)
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Typedefs *********************************//
/**
 * @brief 逻辑地址类型，用于抽象 Flash 寻址
 */
typedef uint32_t tLogicAddr;

/**
 * @brief Flash 块信息结构体
 * @note  地址区间遵循 [start, end) 左闭右开规则
 *        - xBlockStartLogicalAddr：区间起始地址（包含，第一个可访问地址）
 *        - xBlockEndLogicalAddr：区间结束地址（不包含，第一个不可访问地址）
 *        实际有效地址范围：xBlockStartLogicalAddr ~ (xBlockEndLogicalAddr - 1)
 */
typedef struct
{
    tLogicAddr xBlockStartLogicalAddr;  /*!< 块起始逻辑地址（包含） */
    tLogicAddr xBlockEndLogicalAddr;    /*!< 块结束逻辑地址（不包含） */
} BlockInfo_t;
//******************************** Typedefs *********************************//
//---------------------------------------------------------------------------//
//**************************** Interface Structs ****************************//
//**************************** Interface Structs ****************************//
//---------------------------------------------------------------------------//
//******************************** Classes **********************************//
//******************************** Classes **********************************//
//---------------------------------------------------------------------------//
//**************************** extern  Variables ****************************//
//**************************** extern  Variables ****************************//
//---------------------------------------------------------------------------//
//******************************** 函数声明 ***********************************//
// 检查APP的Flash起始/结束地址是否按扇区对齐
extern bool FLASH_HAL_APPAddrCheck(void);
// 获取APP_A/APP_B的Flash分区信息（多段Flash）
extern bool FLASH_HAL_GetFlashConfigInfo(const tAPPType i_appType, 
                                         BlockInfo_t** o_ppBlockInfo, 
                                         uint32_t* o_pItemLen);
// 获取APP的起始地址和占用总大小
extern bool FLASH_HAL_GetAPPInfo(const tAPPType i_appType, 
                                 uint32_t* o_pAppInfoStartAddr, 
                                 uint32_t* o_pBlockSize);
// 一段数据从某地址开始，需要占用多少个扇区
extern uint32_t FLASH_HAL_GetFlashLengthToSectors(const uint32_t i_startFlashAddr,
                                                  const uint32_t i_len);
// 获取Flash驱动代码自身存放的地址
extern bool FLASH_HAL_GetFlashDriverInfo(uint32_t* o_pFlashDriverAddrStart,
                                         uint32_t* o_pFlashDriverEndAddr);
// 获取复位向量配置：是否允许写入、偏移、长度
extern void FLASH_HAL_GetRestHanlderInfo(bool* o_pIsEnableWriteResetHandlerInFlash, 
                                         uint32_t* o_pResetHanderOffset, 
                                         uint32_t* o_pResetHandlerLength);
// 获取复位向量相对于APP起始的偏移地址
extern uint32_t FLASH_HAL_GetStorageRestHandlerAddr(void);
// 是否允许把复位向量写入Flash
extern bool FLASH_HAL_IsEnableStorageResetHandlerInFlash(void);
// 获取复位向量地址长度（4字节S32K，3字节S12Z）
extern uint32_t FLASH_HAL_GetResetHandlerLen(void);
// 获取配置的CPU核心数量
extern uint32_t FLASH_HAL_GetConfigCoreNo(void);
// 获取多核下APP的镜像地址
extern bool FLASH_HAL_GetMultiCoreMirrorAddr(const tAPPType i_appType, 
                                             const uint32_t i_coreNo, 
                                             uint32_t* o_pMirrorAddr);
// 获取多核下APP的重映射地址
extern bool FLASH_HAL_GetMultiCoreRemapAddr(const tAPPType i_appType, 
                                            const uint32_t i_coreNo, 
                                            uint32_t* o_pReampAddr);
// 逻辑地址转物理Flash地址
extern uint32_t FLASH_HAL_LogicalToPhysicalAddr(const uint32_t i_logicalAddr);
// 物理Flash地址转逻辑地址
extern uint32_t FLASH_HAL_PhysicalToLogicalAddr(const uint32_t i_globalAddr);
// 获取擦除一个扇区的最大耗时（ms）
extern uint32_t FLASH_HAL_GetEraseFlashASectorMaxTimeMs(void);
// 获取APP总共占用多少个扇区
extern uint32_t FLASH_HAL_GetTotalSectors(const tAPPType i_appType);
// 输入第N个扇区，返回该扇区的起始Flash地址
extern bool FLASH_HAL_SectorNumberToFlashAddress(const tAPPType i_appType, 
                                                 const uint32_t i_secotrNo, 
                                                 uint32_t* o_pFlashAddr);
// 获取单个Flash扇区大小
extern uint32_t FLASH_HAL_Get1SectorBytes(void);
#if defined (__cplusplus)
}
#endif

#endif /* S32K144_LIN_HAL_FLASH_HAL_INC_FLASH_HAL_CFG_H_ */
