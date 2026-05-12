/*
 * flash_hal_cfg.c
 *
 *  Created on: 2026年4月18日
 *      Author: redmiX
 */

//******************************** Includes *********************************//
#include "flash_hal_cfg.h"
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/**
 * @brief   MCU 平台相关的 Flash 及启动配置宏定义
 * @note    根据 MCU_TYPE 宏切换，适配不同内核架构的物理特性差异
 *          主要差异点：扇区大小、擦除耗时、中断向量表结构、指令地址宽度
 */
#if (defined MCU_TYPE) && (MCU_TYPE == MCU_S32K14x)
    #define SECTOR_LEN                 (4096u)   /**< S32K144 P-Flash 扇区大小：4KB */
    #define MAX_ERASE_SECTOR_FLASH_MS  (130u)    /**< 擦除单扇区最大耗时：用于喂狗/超时判断 */
    
    /* --- ARM Cortex-M4 内核向量表结构 --- */
    #define APP_VECTOR_TABLE_OFFSET    (512u)    /**< APP向量表相对APP起始地址的偏移：
                                                     Cortex-M 规定向量表前 512 字节为系统异常 +
                                                     外设中断，APP代码需避开此区域 */
    #define RESET_HANDLE_OFFSET        (4u)      /**< 复位向量在向量表内的偏移：
                                                     向量表前4字节是初始栈顶(MSP)，第5字节起
                                                     才是 Reset_Handler 地址 */
    #define RESET_HANDLER_ADDR_LEN     (4u)      /**< 复位向量地址长度：
                                                     32位架构，函数指针占 4 字节 */

#elif (defined MCU_TYPE) && (MCU_TYPE == MCU_S12Z)
    #define SECTOR_LEN                 (512u)    /**< S12Z Flash 扇区大小：512 字节 */
    #define MAX_ERASE_SECTOR_FLASH_MS  (21u)     /**< 擦除单扇区最大耗时 */
    
    /* --- S12Z (MagniV) 内核向量表结构 --- */
    #define APP_VECTOR_TABLE_OFFSET    (512)     /**< APP向量表偏移 */
    #define RESET_HANDLE_OFFSET        (508u)    /**< 复位向量在向量表内的偏移：
                                                     S12Z 向量表布局与 ARM 完全不同，
                                                     复位向量固定存放在向量表区域的最后 3 个字节，
                                                     即 512 - 3 = 508 偏移处 */
    #define RESET_HANDLER_ADDR_LEN     (3u)      /**< 复位向量地址长度：
                                                     S12Z 采用 24 位地址总线，
                                                     函数指针/跳转地址占 3 字节 */

#else
    #error "Please config the MCU Type"          /**< 强制要求在编译选项中指定 MCU_TYPE */
#endif

/**
 * @brief   是否允许将复位向量写入 Flash（全局安全开关）
 * @details Bootloader 跳转 APP 前，常需修改 APP 的复位向量。
 *          在某些安全等级要求高的系统中，APP 区域默认是写保护的，
 *          禁止运行时修改复位向量以防被篡改（防木马注入）。
 *          - TRUE：允许 Bootloader 擦除/写入 APP 的复位向量区
 *          - FALSE：禁止写入，复位向量需在出厂前一次性固化或通过其他安全机制处理
 */
#define EN_WRITE_RESET_HANDLER_IN_FLASH (FALSE)

/**
 * @brief   多核应用信息结构体（仅当 CORE_NO >= 1 时使用）
 * @details 针对如 S32K144 这类多核异构芯片，不同核心对同一块物理 Flash 
 *          的寻址视图不同（通过 MSCM/SEM42 等硬件重映射）。
 *          此结构体用于管理 APP 镜像在不同核心视角下的地址映射关系。
 */
typedef struct 
{
    uint32_t imageAStartAddr;   /*!< APP_A 在 Flash 中的物理/逻辑起始地址（主核视角） */
    uint32_t imageBStartAddr;   /*!< APP_B 在 Flash 中的物理/逻辑起始地址（主核视角） */
    uint32_t imageAMirrorAddr;  /*!< APP_A 在从核地址空间的镜像访问地址（从核视角） */
    uint32_t imageBMirrorAddr;  /*!< APP_B 在从核地址空间的镜像访问地址（从核视角） */
    uint32_t remapAppAddr;      /*!< 硬件重映射目标地址：
                                    将 imageA/B 的物理地址映射到此统一地址，
                                    常用于多核无差异执行同一段代码的场景 */
} CoreInfo_t;
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Macros ***********************************//
//******************************** Macros ***********************************//
//---------------------------------------------------------------------------//
//******************************** 函数声明   *********************************//
//******************************** 函数声明   *********************************//
//---------------------------------------------------------------------------//
//******************************** Variables ********************************//
/**
 * @brief Flash 驱动自身存储区域配置   Flash 驱动程序，就是专门用来“擦除”和“写入”Flash 的一套底层函数库
 */
const BlockInfo_t gs_astFlashDriverBlock[] = {
    {0x1FFF8010u, 0x1FFF8810u},
};

/**
 * @brief 应用 A 可用的 Flash 块配置
 * @note  使用数组结构，是为了支持 APP_A 占用 **多段不连续的 Flash 区域**
 *        部分 MCU（如 S32K、车规芯片）的 Flash 地址非完全连续，存在空洞/保留区
 *        每个数组元素代表一段独立、连续的 Flash 块
 *        当前场景 APP_A 仅占用一段连续空间，因此数组只有 1 个元素
 */
const BlockInfo_t gs_astBlockNumA[] = {
    /**< 块 0：APP_A 占用的第一段 Flash 空间（地址 0x14000 ~ 0x80000） */
    {0x14000u, 0x80000u},    
};

/**
 * @brief 应用 A 包含的 Flash 块数量
 *        自动计算数组元素个数，无需手动修改，适配 1段 / 多段 Flash 场景
 */
const uint32_t gs_blockNumA = sizeof(gs_astBlockNumA) / sizeof(gs_astBlockNumA[0u]);

#ifdef EN_SUPPORT_APP_B
/**
 * @brief 应用 B 可用的 Flash 块配置
 */
const BlockInfo_t gs_astBlockNumB[] = {
    {0x14000u, 0x80000u}, 
};

/** 应用 B 块数量 */
const uint32 gs_blockNumB = sizeof(gs_astBlockNumB) / sizeof(gs_astBlockNumB[0u]);
#endif

#if (CORE_NO >= 1u)
/**
 * @brief 多核 Remap 信息表（静态配置）   让其他核
 */
const static CoreInfo_t gs_astMultiCoreAPPRemapInfo[CORE_NO] = {
    {
        .imageAStartAddr  = 0x1000000u, //固件在物理 Flash 中的真实存放地址（主核视角）
        .imageBStartAddr  = 0x1200000u, 
        .imageAMirrorAddr = 0xA000000u, //固件在从核地址空间中的只读镜像地址。
        .imageBMirrorAddr = 0xA200000u, 
        .remapAppAddr     = 0x2000000u  //从核执行代码的统一起跑线（硬件向量表基地址），通过配置将 MirrorAddr 搬移映射到此地址。
    },
};
#endif
//******************************** Variables ********************************//
//---------------------------------------------------------------------------//
//******************************** Functions ********************************//

/**
 * @brief   检查应用 A/B 配置的 Flash 块地址是否均按扇区对齐
 * @return  true  所有块起始/结束地址均对齐到扇区边界
 * @return  false 存在未对齐的块地址
 */
bool FLASH_HAL_APPAddrCheck(void)
{
    //1. 获取扇区大小掩码（例如 4096-1 = 0xFFF）
    const uint32_t flashAddrLowByte = (SECTOR_LEN) - 1u;   //大小4KB对齐
    BlockInfo_t *pBlockInfo = NULL_PTR;
    uint32_t item = 0;                                     //记录当前 APP（如 APP_A）所占用的 Flash 块的数量

    //2. 获取应用 A 的块配置
    if(TRUE == FLASH_HAL_GetFlashConfigInfo(APP_A_TYPE, &pBlockInfo, &item))
    {
        //2.1 遍历所有块，检查起止地址是否对齐
        while(item)
        {
            if((pBlockInfo->xBlockStartLogicalAddr & flashAddrLowByte) ||
               (pBlockInfo->xBlockEndLogicalAddr   & flashAddrLowByte))
            {
                //2.1.1 任一地址未对齐则返回失败
                return FALSE;
            }
            item--;
            pBlockInfo++;
        }
    }
#ifdef EN_SUPPORT_APP_B
    //3. 若支持应用 B，同样检查其块配置
    if (TRUE == FLASH_HAL_GetFlashConfigInfo(APP_B_TYPE, &pBlockInfo, &item))
    {
        while (item)
        {
            if((pBlockInfo->xBlockStartLogicalAddr & flashAddrLowByte) ||
               (pBlockInfo->xBlockEndLogicalAddr   & flashAddrLowByte))
            {
                return FALSE;
            }
            item--;
            pBlockInfo++;
        }
    }
#endif

    //4. 所有检查通过
    return TRUE;
}

/**
 * @brief   获取指定应用类型的 Flash 块配置数组
 * @param   i_appType     应用类型
 * @param   o_ppBlockInfo 输出：指向块信息数组的指针
 * @param   o_pItemLen    输出：数组元素个数
 * @return  true  成功获取
 * @return  false 应用类型无效
 */
bool FLASH_HAL_GetFlashConfigInfo(const tAPPType i_appType, 
                                  BlockInfo_t** o_ppBlockInfo, 
                                  uint32_t* o_pItemLen)
{
    bool ret = false;

    //1. 根据应用类型返回对应的全局配置数组
    if(APP_A_TYPE == i_appType)
    {
        *o_ppBlockInfo = (BlockInfo_t *)gs_astBlockNumA;
        *o_pItemLen = gs_blockNumA;
        ret = true;
    }

#ifdef EN_SUPPORT_APP_B   
    else if(APP_B_TYPE == i_appType)
    {
        *o_ppBlockInfo = (BlockInfo_t *)gs_astBlockNumB;
        *o_pItemLen = gs_blockNumB;
        ret = true;
    }
#endif

    return ret;
}

/**
 * @brief   获取指定应用类型第一个块的信息（起始地址和总大小）
 * @param   i_appType           应用类型
 * @param   o_pAppInfoStartAddr 输出：应用起始逻辑地址
 * @param   o_pBlockSize        输出：应用占用总字节数
 * @return  true  成功获取
 * @return  false 应用类型无效
 */
bool FLASH_HAL_GetAPPInfo(const tAPPType i_appType, 
                          uint32_t* o_pAppInfoStartAddr, 
                          uint32_t* o_pBlockSize)
{
    bool ret = false;

    if(APP_A_TYPE == i_appType)
    {
        *o_pAppInfoStartAddr = gs_astBlockNumA[0].xBlockStartLogicalAddr;
        *o_pBlockSize = gs_astBlockNumA[0u].xBlockEndLogicalAddr 
            - gs_astBlockNumA[0u].xBlockStartLogicalAddr;
        ret = true;
    }

#ifdef EN_SUPPORT_APP_B  
    else if(APP_B_TYPE == i_appType)
    {
        *o_pAppInfoStartAddr = gs_astBlockNumB[0].xBlockStartLogicalAddr;
        *o_pBlockSize = gs_astBlockNumB[0u].xBlockEndLogicalAddr 
        - gs_astBlockNumB[0u].xBlockStartLogicalAddr;
        ret = true;
    }
#endif

    return ret;
}

/**
 * @brief   计算从指定地址开始、覆盖给定长度所需的扇区数量
 * @param   i_startFlashAddr 起始地址
 * @param   i_len            数据长度（字节）
 * @return  覆盖该范围最少需要的扇区个数
 * @note    考虑了起始地址非对齐和跨扇区边界的情况
 */
uint32_t FLASH_HAL_GetFlashLengthToSectors(const uint32_t i_startFlashAddr,
                                           const uint32_t i_len)
{
    const uint32_t mask = SECTOR_LEN - 1u;
    uint32_t aligned_start = i_startFlashAddr & ~mask;     //将低三位清零
    uint32_t end_addr = i_startFlashAddr + i_len;          //起始地址+数据长度
    uint32_t total_bytes = end_addr - aligned_start;
    return (total_bytes + mask) / SECTOR_LEN;
}

/**
 * @brief   获取 Flash 驱动所占内存范围
 * @param   o_pFlashDriverAddrStart 输出：驱动起始地址
 * @param   o_pFlashDriverEndAddr   输出：驱动结束地址（不包含）
 * @return  操作状态
 */
bool FLASH_HAL_GetFlashDriverInfo(uint32_t* o_pFlashDriverAddrStart,
                                  uint32_t* o_pFlashDriverEndAddr)
{
    //1. 参数有效性检验
    if(NULL_PTR == o_pFlashDriverAddrStart ||
       NULL_PTR == o_pFlashDriverEndAddr)
    {
        return false;
    }

    //2. 返回静态配置的驱动块信息
    *o_pFlashDriverAddrStart = gs_astFlashDriverBlock[0u].xBlockStartLogicalAddr;
    *o_pFlashDriverEndAddr   = gs_astFlashDriverBlock[0u].xBlockEndLogicalAddr;

    return TRUE;
}

/**
 * @brief   获取复位向量处理的配置参数  
 * @param   o_pIsEnableWriteResetHandlerInFlash 输出：是否允许写入复位向量
 * @param   o_pResetHanderOffset                输出：复位向量在向量表中的偏移
 * @param   o_pResetHandlerLength               输出：复位向量存储长度
 */
void FLASH_HAL_GetRestHanlderInfo(bool* o_pIsEnableWriteResetHandlerInFlash, 
                                  uint32_t* o_pResetHanderOffset, 
                                  uint32_t* o_pResetHandlerLength) 
{
    //1. 参数有效性检验
    if(NULL_PTR == o_pIsEnableWriteResetHandlerInFlash ||
       NULL_PTR == o_pResetHanderOffset                ||
       NULL_PTR == o_pResetHandlerLength)
    {
        return false;
    }

     //2. 返回宏定义配置
    *o_pIsEnableWriteResetHandlerInFlash = EN_WRITE_RESET_HANDLER_IN_FLASH;
    *o_pResetHanderOffset = APP_VECTOR_TABLE_OFFSET + RESET_HANDLE_OFFSET;
    *o_pResetHandlerLength = RESET_HANDLER_ADDR_LEN;
}

/**
 * @brief   获取复位向量在 Flash 中的存储偏移地址
 * @return  偏移量（字节，相对于应用起始地址）
 */
uint32_t FLASH_HAL_GetStorageRestHandlerAddr(void)
{
    return APP_VECTOR_TABLE_OFFSET + RESET_HANDLE_OFFSET;
}

/**
 * @brief   查询是否允许将复位向量写入 Flash
 * @return  true  允许
 * @return  false 禁止
 */
bool FLASH_HAL_IsEnableStorageResetHandlerInFlash(void)
{
    return EN_WRITE_RESET_HANDLER_IN_FLASH;
}