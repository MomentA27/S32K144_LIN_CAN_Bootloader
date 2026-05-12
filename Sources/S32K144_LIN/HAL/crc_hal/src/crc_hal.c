//******************************** Includes *********************************//
/**
 * @file    crc_hal.c
 * @brief   CRC校验硬件抽象层实现
 * @details 支持软件和硬件两种CRC计算方式
 *          - 软件：基于查表法的CRC16-DNP实现
 *          - 硬件：基于MCU内置CRC模块
 */
#include "crc_hal.h"

#ifdef EN_CRC_HARDWARE
#include "crc1.h"
#endif
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Macros ***********************************//
//******************************** Macros ***********************************//
//---------------------------------------------------------------------------//
//******************************** 函数声明   *********************************//
#ifdef EN_CRC_SOFTWARE
static void UpadataSoftwareCrc16(const uint8 *i_pDataBuf, 
                                 const uint32 i_dataLen, 
                                       uint32 *m_pCurCrc);
#endif

#ifdef EN_CRC_HARDWARE
static void Crc_Init(void);
static void UpadataHardwareCrc16(const uint8_t *i_pucDataBuf, 
                                 const uint32_t i_ulDataLen,
                                       uint32_t *m_pCurCrc);
#endif
//******************************** 函数声明   *********************************//
//---------------------------------------------------------------------------//
//******************************** Variables ********************************//
#ifdef EN_CRC_SOFTWARE
/**
 * @brief CRC16-DNP查表法常量表
 * @details 参数配置：
 * 
 *          - 异或输出: 0xFFFF
 */
/*查表法里的那个 256 项数组，就是“0x00~0xFF 这 256 个字节，提前除以多项式得到的 256 个余数*/
static const uint16 g_dnpCrcTable[256u] = {
0x0000, 0x365e, 0x6cbc, 0x5ae2, 0xd978, 0xef26, 0xb5c4, 0x839a,
0xff89, 0xc9d7, 0x9335, 0xa56b, 0x26f1, 0x10af, 0x4a4d, 0x7c13,
0xb26b, 0x8435, 0xded7, 0xe889, 0x6b13, 0x5d4d, 0x07af, 0x31f1,
0x4de2, 0x7bbc, 0x215e, 0x1700, 0x949a, 0xa2c4, 0xf826, 0xce78,
0x29af, 0x1ff1, 0x4513, 0x734d, 0xf0d7, 0xc689, 0x9c6b, 0xaa35,
0xd626, 0xe078, 0xba9a, 0x8cc4, 0x0f5e, 0x3900, 0x63e2, 0x55bc,
0x9bc4, 0xad9a, 0xf778, 0xc126, 0x42bc, 0x74e2, 0x2e00, 0x185e,
0x644d, 0x5213, 0x08f1, 0x3eaf, 0xbd35, 0x8b6b, 0xd189, 0xe7d7,
0x535e, 0x6500, 0x3fe2, 0x09bc, 0x8a26, 0xbc78, 0xe69a, 0xd0c4,
0xacd7, 0x9a89, 0xc06b, 0xf635, 0x75af, 0x43f1, 0x1913, 0x2f4d,
0xe135, 0xd76b, 0x8d89, 0xbbd7, 0x384d, 0x0e13, 0x54f1, 0x62af,
0x1ebc, 0x28e2, 0x7200, 0x445e, 0xc7c4, 0xf19a, 0xab78, 0x9d26,
0x7af1, 0x4caf, 0x164d, 0x2013, 0xa389, 0x95d7, 0xcf35, 0xf96b,
0x8578, 0xb326, 0xe9c4, 0xdf9a, 0x5c00, 0x6a5e, 0x30bc, 0x06e2,
0xc89a, 0xfec4, 0xa426, 0x9278, 0x11e2, 0x27bc, 0x7d5e, 0x4b00,
0x3713, 0x014d, 0x5baf, 0x6df1, 0xee6b, 0xd835, 0x82d7, 0xb489,
0xa6bc, 0x90e2, 0xca00, 0xfc5e, 0x7fc4, 0x499a, 0x1378, 0x2526,
0x5935, 0x6f6b, 0x3589, 0x03d7, 0x804d, 0xb613, 0xecf1, 0xdaaf,
0x14d7, 0x2289, 0x786b, 0x4e35, 0xcdaf, 0xfbf1, 0xa113, 0x974d,
0xeb5e, 0xdd00, 0x87e2, 0xb1bc, 0x3226, 0x0478, 0x5e9a, 0x68c4,
0x8f13, 0xb94d, 0xe3af, 0xd5f1, 0x566b, 0x6035, 0x3ad7, 0x0c89,
0x709a, 0x46c4, 0x1c26, 0x2a78, 0xa9e2, 0x9fbc, 0xc55e, 0xf300,
0x3d78, 0x0b26, 0x51c4, 0x679a, 0xe400, 0xd25e, 0x88bc, 0xbee2,
0xc2f1, 0xf4af, 0xae4d, 0x9813, 0x1b89, 0x2dd7, 0x7735, 0x416b,
0xf5e2, 0xc3bc, 0x995e, 0xaf00, 0x2c9a, 0x1ac4, 0x4026, 0x7678,
0x0a6b, 0x3c35, 0x66d7, 0x5089, 0xd313, 0xe54d, 0xbfaf, 0x89f1,
0x4789, 0x71d7, 0x2b35, 0x1d6b, 0x9ef1, 0xa8af, 0xf24d, 0xc413,
0xb800, 0x8e5e, 0xd4bc, 0xe2e2, 0x6178, 0x5726, 0x0dc4, 0x3b9a,
0xdc4d, 0xea13, 0xb0f1, 0x86af, 0x0535, 0x336b, 0x6989, 0x5fd7,
0x23c4, 0x159a, 0x4f78, 0x7926, 0xfabc, 0xcce2, 0x9600, 0xa05e,
0x6e26, 0x5878, 0x029a, 0x34c4, 0xb75e, 0x8100, 0xdbe2, 0xedbc,
0x91af, 0xa7f1, 0xfd13, 0xcb4d, 0x48d7, 0x7e89, 0x246b, 0x1235,
}; 
#endif
//******************************** Variables ********************************//
//---------------------------------------------------------------------------//
//******************************** Functions ********************************//
/**
 * @brief 初始化CRC模块
 * @return TRUE 初始化成功
 */
bool CRC_HAL_Init(void)
{
    // 1. 硬件CRC初始化
#ifdef EN_CRC_HARDWARE
    CRC_DRV_Init(INST_CRC1, &crc1_InitConfig0);
#endif
    return TRUE;
}

/**
 * @brief 反初始化CRC模块
 */
void CRC_HAL_Deinit(void)
{
    // 1. 硬件资源释放（当前未实现）
}

/**
 * @brief 开始CRC计算
 * @param m_pCurCrc CRC值指针
 */
void CRC_HAL_StartCrc(uint32_t *m_pCurCrc)
{
    // 1. 硬件CRC初始化
#ifdef EN_CRC_HARDWARE
    if (NULL_PTR != m_pCurCrc)
    {
        // 1.1 初始化CRC值为0
        *m_pCurCrc = 0u;
        // 1.2 配置硬件CRC模块
        CRC_DRV_Configure(INST_CRC1, &crc1_InitConfig0);
    }
    // 2. 软件CRC初始化
#elif defined(EN_CRC_SOFTWARE)
    if (NULL_PTR != m_pCurCrc)
    {
        // 2.1 初始化CRC值为0
        *m_pCurCrc = 0u;
    }
#else
    #error "Non CRC module enabled!"
#endif
}

/**
 * @brief 更新CRC计算
 * @param i_pucDataBuf 数据缓冲区指针
 * @param i_ulDataLen 数据长度
 * @param m_pCurCrc CRC值指针
 */
void CRC_HAL_UpadataCrc(const uint8_t *i_pucDataBuf, 
                        const uint32_t i_ulDataLen, 
                              uint32_t *m_pCurCrc)
{
    // 1. 根据配置选择CRC计算方式
#ifdef EN_CRC_HARDWARE
    UpadataHardwareCrc16(i_pucDataBuf, i_ulDataLen, m_pCurCrc);
#elif defined(EN_CRC_SOFTWARE)
    UpadataSoftwareCrc16(i_pucDataBuf, i_ulDataLen, m_pCurCrc);
#else
    #error "Non CRC module enabled!"
#endif
}

/**
 * @brief 结束CRC计算
 * @param m_pCurCrc CRC值指针
 */
void CRC_HAL_EndCrc(uint32_t *m_pCurCrc)
{
    // 1. 硬件CRC结束处理
#ifdef EN_CRC_HARDWARE
    if (NULL_PTR != m_pCurCrc)
    {
        // 1.1 获取硬件CRC结果
        uint32_t result = CRC_DRV_GetCrcResult(INST_CRC1);
        // 1.2 取反并截取低16位
        *m_pCurCrc = (~result) & 0xFFFFu;
    }
    // 2. 软件CRC结束处理
#elif defined(EN_CRC_SOFTWARE)
    if (NULL_PTR != m_pCurCrc)
    {
        // 2.1 取反并截取低16位
        uint32 crc = *m_pCurCrc;
        *m_pCurCrc = (uint32)((~crc) & 0xFFFFu);
    }
#else
    #error "Non CRC module enabled!"
#endif
}

/**
 * @brief 一次性完成CRC计算
 * @param i_pucDataBuf 数据缓冲区指针
 * @param i_ulDataLen 数据长度
 * @param m_pCurCrc CRC值指针
 */
void CRC_HAL_CalculateCRCOnce(const uint8_t *i_pucDataBuf, 
                              const uint32_t i_ulDataLen, 
                              uint32_t *m_pCurCrc)
{
    // 1. 参数校验
    if (NULL_PTR == i_pucDataBuf || 
        0u == i_ulDataLen        || 
        NULL_PTR == m_pCurCrc)
    {
        return;
    }
    
    // 2. 执行CRC计算流程
    // 2.1 初始化CRC
    CRC_HAL_StartCrc(m_pCurCrc);
    // 2.2 更新CRC计算
    CRC_HAL_UpadataCrc(i_pucDataBuf, i_ulDataLen, m_pCurCrc);
    // 2.3 结束CRC计算
    CRC_HAL_EndCrc(m_pCurCrc);
}

#ifdef EN_CRC_SOFTWARE
/**
 * @brief 软件 CRC16 查表计算
 * @param i_pDataBuf 数据缓冲区指针
 * @param i_dataLen 数据长度
 * @param m_pCurCrc CRC值指针
 */
static void UpadataSoftwareCrc16(const uint8 *i_pDataBuf, 
                                 const uint32 i_dataLen, 
                                       uint32 *m_pCurCrc)
{
    // 1. 初始化CRC值
    uint16 crc = (uint16)*m_pCurCrc;
    uint32 index;

    // 2. 查表法计算CRC
    for (index = 0u; index < i_dataLen; index++)
    {
        // 2.1 标准查表算法：crc = (crc >> 8) ^ table[(crc ^ data_byte) & 0xFF]
        crc = (crc >> 8) ^ g_dnpCrcTable[(crc ^ i_pDataBuf[index]) & 0x00FF];
    }

    // 3. 保存结果
    *m_pCurCrc = crc;
}
#endif

#ifdef EN_CRC_HARDWARE
/**
 * @brief 硬件 CRC16 计算
 * @param i_pucDataBuf 数据缓冲区指针
 * @param i_ulDataLen 数据长度
 * @param m_pCurCrc CRC值指针（未使用）
 */
static void UpadataHardwareCrc16(const uint8_t *i_pucDataBuf,
                                 const uint32_t i_ulDataLen,
                                       uint32_t *m_pCurCrc)
{
    // 1. 参数校验
    (void)m_pCurCrc;
    if (i_pucDataBuf == NULL || i_ulDataLen == 0) return;

    // 2. 写入数据到硬件CRC模块
    CRC_DRV_WriteData(INST_CRC1, i_pucDataBuf, i_ulDataLen);
}
#endif
//******************************** Functions ********************************//
