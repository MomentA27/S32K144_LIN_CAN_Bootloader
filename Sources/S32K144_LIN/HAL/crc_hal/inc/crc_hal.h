#ifndef S32K144_LIN_HAL_CRC_HAL_INC_CRC_HAL_H_
#define S32K144_LIN_HAL_CRC_HAL_INC_CRC_HAL_H_

#if defined (__cplusplus)
extern "C" {
#endif

//******************************** Includes *********************************//
#include "includes.h"
#include <stdint.h>
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Typedefs *********************************//
// CRC 值类型定义（实际使用低16位）
typedef uint32_t tCrc;
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
/**
 * @brief 初始化CRC模块
 * @return TRUE 初始化成功
 * @note   需要在使用CRC功能前调用
 */
extern bool CRC_HAL_Init  (void);

/**
 * @brief 反初始化CRC模块
 * @note   释放CRC模块占用的资源
 */
extern void CRC_HAL_Deinit(void);

/**
 * @brief 开始CRC计算
 * @param m_pCurCrc CRC值指针，用于存储初始值
 * @note   初始化CRC值为0，并配置CRC模块
 */
extern void CRC_HAL_StartCrc  (uint32_t *m_pCurCrc);

/**
 * @brief 更新CRC计算
 * @param i_pucDataBuf 数据缓冲区指针
 * @param i_ulDataLen 数据长度
 * @param m_pCurCrc CRC值指针，用于存储中间结果
 * @note   可多次调用以分块计算CRC
 */
extern void CRC_HAL_UpadataCrc(const uint8_t *i_pucDataBuf, 
                               const uint32_t i_ulDataLen, 
                                     uint32_t *m_pCurCrc);

/**
 * @brief 结束CRC计算
 * @param m_pCurCrc CRC值指针，用于存储最终结果
 * @note   对CRC结果进行取反处理并截取低16位
 */
extern void CRC_HAL_EndCrc    (uint32_t *m_pCurCrc);

/**
 * @brief 一次性完成CRC计算
 * @param i_pucDataBuf 数据缓冲区指针
 * @param i_ulDataLen 数据长度
 * @param m_pCurCrc CRC值指针，用于存储计算结果
 * @note   封装了StartCrc、UpadataCrc和EndCrc的完整流程
 */
extern void CRC_HAL_CalculateCRCOnce(const uint8_t *i_pucDataBuf, 
                                     const uint32_t i_ulDataLen, 
                                           uint32_t *m_pCurCrc);
//******************************** 函数声明 ***********************************//

#if defined (__cplusplus)
}
#endif

#endif /* S32K144_LIN_HAL_CRC_HAL_INC_CRC_HAL_H_ */