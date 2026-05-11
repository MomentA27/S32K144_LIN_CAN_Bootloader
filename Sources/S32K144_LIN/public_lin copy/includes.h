//
// Created by redmiX on 2026/4/14.
//

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

//******************************** Includes *********************************//
#include "Cpu.h"
#include "common_types.h"
#include "toolchain.h"
#include "autolibc.h"

/* 用户应用宏配置 */
#include "user_config.h"

#if defined (EN_ASSERT) || defined (EN_DEBUG_TIMER) || defined (EN_DEBUG_PRINT)
#include "bootloader_debug.h"
#endif
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/*** 断言与调试 IO/定时器 ***/
#ifdef EN_ASSERT
/* 断言：条件为真则死循环 */
#define ASSERT(xValue) \
    do { \
        if (xValue) { \
            while (1) {} \
        } \
    } while (0)

/* 断言打印字符串（不卡死） */
#define ASSERT_Printf(pString, xValue) \
    do { \
        if (FALSE != xValue) { \
            Debug_Printf(pString); \
        } \
    } while (0)

/* 断言打印字符串并死循环 */
#define ASSERT_DebugPrintf(pString, xValue) \
    do { \
        if (FALSE != xValue) { \
            Debug_Printf(pString); \
            while (1u) {} \
        } \
    } while (0)
#else
#define ASSERT(xValue)
#define ASSERT_Printf(pString, xValue)
#define ASSERT_DebugPrintf(pString, xValue)
#endif


/****** 调试模块开关 ******/
#ifdef EN_DEBUG_FLS_MODULE
#define FLS_DebugPrintf Debug_Printf
#else
#define FLS_DebugPrintf(fmt, args...)
#endif

#ifdef EN_UDS_DEBUG
#define UDS_DebugPrintf Debug_Printf
#else
#define UDS_DebugPrintf(fmt, args...)
#endif

#ifdef EN_TP_DEBUG
#define TP_DebugPrintf Debug_Printf
#else
#define TP_DebugPrintf(fmt, args...)
#endif

#ifdef EN_APP_DEBUG
#define APP_DebugPrintf Debug_Printf
#else
#define APP_DebugPrintf(fmt, args...)
#endif

#ifdef EN_DEBUG_FIFO
#define FIFO_DebugPrintf Debug_Printf
#else
#define FIFO_DebugPrintf(fmt, args...)
#endif


/****** 传输协议（TP）配置 ******/
/* 只能启用一种 TP，否则报错 */
#if defined EN_CAN_TP
    #if defined EN_LIN_TP || defined EN_ETHERNET_TP || defined EN_OTHERS_TP
        #error "只能启用一种传输协议（TP）"
    #endif
#elif defined EN_LIN_TP
    #if defined EN_CAN_TP || defined EN_ETHERNET_TP || defined EN_OTHERS_TP
        #error "只能启用一种传输协议（TP）"
    #endif
#elif defined EN_ETHERNET_TP
    #if defined EN_CAN_TP || defined EN_LIN_TP || defined EN_OTHERS_TP
        #error "只能启用一种传输协议（TP）"
    #endif
#elif defined EN_OTHERS_TP
    #if defined EN_CAN_TP || defined EN_LIN_TP || defined EN_ETHERNET_TP
        #error "只能启用一种传输协议（TP）"
    #endif
#else
    #error "请启用一种传输协议（EN_CAN_TP / EN_LIN_TP / EN_ETHERNET_TP / EN_OTHERS_TP）"
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
//******************************** 函数声明 ***********************************//

#endif /*__INCLUDES_H__*/

