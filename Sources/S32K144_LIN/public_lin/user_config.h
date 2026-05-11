//
// Created by redmiX on 2026/4/14.
//

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

//******************************** Includes *********************************//
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/************ 核心配置 ************/
/* 核心数量，必须 >= 1 */
#define CORE_NO (1u)
#if (defined CORE_NO) && (CORE_NO < 1)
#undef CORE_NO
#define CORE_NO (1u)
#elif (!defined CORE_NO)
#define CORE_NO (1u)
#endif


/******* 断言与调试 IO/定时器 *******/
/* 启用调试 IO */
#define EN_DEBUG_IO

/* 启用调试定时器 */
#define EN_DEBUG_TIMER

/* 启用断言 */
#define EN_ASSERT

/* 调试模块开关 */
#define EN_DEBUG_FLS_MODULE   /* Flash 模块调试 */
//#define EN_UDS_DEBUG          /* UDS 调试 */
//#define EN_TP_DEBUG           /* 传输协议调试 */
//#define EN_APP_DEBUG          /* 应用层调试 */

/* 是否使用 CRC */
//#define DebugBootloader_NOTCRC

/* 启用 FIFO 调试 */
//#define EN_DEBUG_FIFO

/* 开启输出debug */
#if (defined EN_DEBUG_FLS_MODULE) || (defined EN_UDS_DEBUG) || (defined EN_TP_DEBUG) || (defined EN_APP_DEBUG) || (defined EN_DEBUG_FIFO)
#ifndef EN_DEBUG_PRINT
#define EN_DEBUG_PRINT
#endif
#else
//#define EN_DEBUG_PRINT
#endif


/******** UDS 算法访问配置 ********/
/* 启用软件算法访问 */
#define EN_ALG_SW

/* 启用硬件算法访问 */
#define EN_ALG_HW

/* AES 种子长度（字节）*/
#define AES_SEED_LEN (16u)


/******** 传输协议（TP）配置 ********/
/* 只能启用一种 TP，否则编译报错 */
//#define EN_CAN_TP
#define EN_LIN_TP
//#define EN_ETHERNET_TP
//#define EN_OTHERS_TP

#ifdef EN_CAN_TP
#define RX_FUN_ID (0x7FFu)   /* CAN TP 接收功能 ID */
#define RX_PHY_ID (0x784u)   /* CAN TP 接收物理 ID */
#define TX_ID      (0x7F0u)   /* CAN TP 发送 ID */
#define RX_ID_MASK (0x700u)   /* CAN TP 接收掩码 */
//#define EN_TX_CAN_FD          /* 启用 CAN FD 发送（单帧可超过 8 字节）*/
#endif

#ifdef EN_LIN_TP
#define RX_BOARD_ID (0x7Fu)   /* LIN TP 接收板级 ID（响应无关）*/
#define RX_FUN_ID   (0x7Eu)   /* LIN TP 接收功能 ID（仅支持单帧，无需响应）*/
#define RX_PHY_ID   (0x55u)   /* LIN TP 接收物理 ID */
#define TX_ID       (0x35u)   /* LIN TP 发送 ID（主节点 NAD）*/
#endif


/*********** CRC 配置 ***********/
#define EN_CRC_HARDWARE    /* 硬件 CRC */
//#define EN_CRC_SOFTWARE      /* 软件 CRC */

/* Flash 地址是否连续（否）*/
#define FALSH_ADDRESS_CONTINUE (FALSE)


/********** FIFO 配置 **********/
/* 总线接收 FIFO 标识 */
#define RX_BUS_FIFO ('r')

#ifdef EN_CAN_TP
#define RX_BUS_FIFO_LEN (300u)   /* CAN：接收 FIFO 长度 */
#elif (defined EN_LIN_TP)
#define RX_BUS_FIFO_LEN (50)     /* LIN：接收 FIFO 长度 */
#else
#define RX_BUS_FIFO_LEN (50u)    /* 默认接收 FIFO 长度 */
#endif

#ifdef EN_CAN_TP
#define TX_BUS_FIFO     ('t')    /* 总线发送 FIFO 标识 */
#define TX_BUS_FIFO_LEN (100u)   /* CAN：发送 FIFO 长度 */
#elif (defined EN_LIN_TP)
#define TX_BUS_FIFO     ('t')    /* 总线发送 FIFO 标识 */
#define TX_BUS_FIFO_LEN (50u)    /* LIN：发送 FIFO 长度 */
#endif


/********** FOTA A/B 分区配置 **********/
//#define EN_SUPPORT_APP_B       /* 支持 B 区应用 */

typedef enum
{
    APP_A_TYPE = 0u,            /* A 区应用 */
#ifdef EN_SUPPORT_APP_B
    APP_B_TYPE = 1u,            /* B 区应用 */
#endif
    APP_INVLID_TYPE = 0xFFu     /* 无效应用类型 */
} tAPPType;

#ifdef EN_SUPPORT_APP_B
/* 若最新应用无效，则回退至旧应用 */
#define EN_NEWEST_APP_INVALID_JUMP_OLD_APP
#endif


/************* 全局中断控制 *************/
#define DisableAllInterrupts()  INT_SYS_DisableIRQGlobal()
#define EnableAllInterrupts()   INT_SYS_EnableIRQGlobal()



/******** MCU 型号（影响扇区擦除时间） ********/
#define MCU_S12Z    (1)
#define MCU_S32K14x (2)

#define MCU_TYPE (MCU_S32K14x)   /* 当前 MCU 类型 */


/****** 跳转 APP 前的延时（未收到 UDS 消息时） ******/
//#define EN_DELAY_TIME
#define DELAY_MAX_TIME_MS (5000u)   /* 最大延时 5 秒 */
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

#endif /*__USER_CONFIG_H__*/

