//
// Created by redmiX on 2026/4/14.
//

//******************************** Includes *********************************//
#include "bootloader_debug.h"

#ifdef EN_DEBUG_PRINT
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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
//******************************** 函数声明   *********************************//
//---------------------------------------------------------------------------//
//******************************** Variables ********************************//
//******************************** Variables ********************************//
//---------------------------------------------------------------------------//
//******************************** Functions ********************************//

/**
 * @brief   初始化 Bootloader 调试模块。
 * @details 该函数用于初始化本模块。
 * @implements BOOTLOADER_DEBUG_Init_Activity
 */
void BOOTLOADER_DEBUG_Init(void)
{
	InitDebugIO();
	InitDebugTimer();
	InitDebugPrint();

}

#ifdef EN_DEBUG_PRINT
/**
 * @brief   初始化 Bootloader 调试打印所使用的 LPUART 外设。
 * @details 该函数调用 LPUART 驱动初始化接口，配置 LPUART1 通道，
 *          使能后续通过 Bootloader_DebugPrint 输出调试信息。
 * @note    需确保 lpuart1_State 和 lpuart1_InitConfig0 已预先定义并正确配置。
 */
void Bootloader_DebugPrintInit(void)
{
    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);
}

/**
 * @brief   通过 LPUART1 输出格式化的调试日志信息。
 * @details 该函数接收 printf 风格的格式化字符串及可变参数，
 *          将格式化后的结果存入静态缓冲区，再通过阻塞式 LPUART 发送接口输出。
 * @param   fmt  格式化字符串，与 printf 用法相同。
 * @param   ...  对应格式化字符串的可变参数列表。
 * @note    缓冲区大小由 DEBUG_LOG_BUF_SIZE 宏定义决定，超出部分会被截断。
 * @note    发送超时时间为 1000 个 tick，若发送失败函数会阻塞等待。
 */
void Bootloader_DebugPrint(const char *fmt, ...)
{
    static char buffer [DEBUG_LOG_BUF_SIZE];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buffer, DEBUG_LOG_BUF_SIZE, fmt, ap);
    va_end(ap);

    LPUART_DRV_SendDataBlocking(INST_LPUART1, (uint8_t *)buffer, strlen(buffer), 1000);
}
#endif
