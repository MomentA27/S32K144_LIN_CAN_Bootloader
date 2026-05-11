//
// Created by redmiX on 2026/4/14.
//

#ifndef BOOTLOADER_DEBUG_H
#define BOOTLOADER_DEBUG_H

#if defined (__cplusplus)
extern "C" {
#endif

//******************************** Includes *********************************//
#include "includes.h"

#ifdef EN_DEBUG_IO
#include "debug_IO.h"
#endif /*#ifdef EN_DEBUG_IO*/

#ifdef EN_DEBUG_TIMER
#include "debug_timer.h"
#endif /*#ifdef EN_DEBUG_TIMER*/
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/* 调试功能统一宏定义：通过宏开关裁剪调试代码 */
/* 调试GPIO控制宏 */
#ifdef  EN_DEBUG_IO
#define InitDebugIO()            DEBUG_IO_Init()            /* 初始化调试GPIO */
#define SetDebugIOHigh()         DEBUG_IO_SetDebugIOHigh()  /* 调试GPIO置高 */
#define SetDebugIOLow()          DEBUG_IO_SetDebugIOLow()   /* 调试GPIO置低 */
#define ToggleDebugIO()          DEBUG_IO_ToggleDebugIO()   /* 调试GPIO翻转 */
#else
#define InitDebugIO()
#define SetDebugIOHigh()
#define SetDebugIOLow()
#define ToggleDebugIO()
#endif

/* 调试定时器宏 */
#ifdef EN_DEBUG_TIMER
#define InitDebugTimer()         DEBUG_TIMER_Init()           /* 初始化调试定时器 */
#define GetDegbuTimerValue()     DEBUG_TIMER_GetTimerValue()  /* 获取定时器计数值 */
#else
#define InitDebugTimer()
#define GetDegbuTimerValue()
#endif

/* 调试打印（串口）宏 */
#ifdef EN_DEBUG_PRINT
#define DEBUG_LOG_BUF_SIZE       (200u)                              /* 调试日志缓冲区大小 */
extern void Bootloader_DebugPrintInit(void);
extern void Bootloader_DebugPrint(const char *fmt, ...);
#define InitDebugPrint()           Bootloader_DebugPrintInit()        /* 初始化调试打印 */
#define Debug_Printf(fmt, args...) Bootloader_DebugPrint(fmt, ##args) /* 格式化打印（类printf） */
#else
#define InitDebugPrint()
#define Debug_Printf(fmt, args...)
#endif
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Typedefs *********************************//
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
extern void BOOTLOADER_DEBUG_Init(void);
//******************************** 函数声明 ***********************************//

#if defined (__cplusplus)
}
#endif

#endif /* BOOTLOADER_DEBUG_H */
