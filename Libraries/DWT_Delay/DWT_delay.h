//
// Created by redmiX on 2026/4/16.
//

#ifndef DWT_DELAY_H
#define DWT_DELAY_H

#if defined (__cplusplus)
extern "C" {
#endif

//******************************** Includes *********************************//
#include <stdint.h>
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/**
 * @brief 系统核心时钟频率定义
 * @note  需根据实际硬件配置修改 CPU_FREQ_MHZ 的值。
 *        对于 S32K144，常见值为 48MHz、80MHz、112MHz 等。
 */
#define CPU_FREQ_MHZ     48UL   // CPU主频，单位 MHz
#define CPU_FREQ_HZ      (CPU_FREQ_MHZ * 1000000UL)  // CPU主频，单位 Hz

/**
 * @brief ARM Cortex-M 内核调试组件基地址
 *
 * DWT_BASE    : Data Watchpoint and Trace 单元基址（包含周期计数器 CYCCNT）
 * CoreDebug_BASE : CoreDebug 寄存器组基址（包含调试使能位 TRCENA）
 */
#define DWT_BASE         (0xE0001000UL)    /* DWT 单元基地址 */
#define CoreDebug_BASE   (0xE000EDF0UL)    /* CoreDebug 基地址 */

/**
 * @brief 相关寄存器位掩码定义
 *
 * CoreDebug_DEMCR_TRCENA_Msk : DEMCR 寄存器的 TRCENA 位，置1使能 DWT 跟踪功能
 * DWT_CTRL_CYCCNTENA_Msk     : DWT 控制寄存器的 CYCCNTENA 位，置1启动周期计数器
 */
#define CoreDebug_DEMCR_TRCENA_Msk   (1UL << 24)  /* 全局跟踪使能 */
#define DWT_CTRL_CYCCNTENA_Msk       (1UL << 0)   /* 周期计数器使能 */
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Typedefs *********************************//
/**
 * @brief DWT (Data Watchpoint and Trace) 单元寄存器映射结构体
 *
 * 该结构体描述了 DWT 模块的寄存器布局，用于直接访问硬件寄存器。
 * 仅列出本次延时功能所需的寄存器，后续比较器寄存器省略不影响 CYCCNT 使用。
 */
typedef struct
{
    volatile uint32_t CTRL;      /* 控制寄存器 */
    volatile uint32_t CYCCNT;    /* 周期计数器（CPU周期计数） */
    volatile uint32_t CPICNT;    /* CPI 计数器 */
    volatile uint32_t EXCCNT;    /* 异常开销计数器 */
    volatile uint32_t SLEEPCNT;  /* 睡眠计数器 */
    volatile uint32_t LSUCNT;    /* LSU 计数器 */
    volatile uint32_t FOLDCNT;   /* 折叠指令计数器 */
    volatile uint32_t PCSR;      /* 程序计数器采样寄存器 */
    /* 后续比较器寄存器等，此处省略，不影响 CYCCNT 使用 */
} DWT_Type;

/**
 * @brief CoreDebug 寄存器映射结构体
 *
 * 包含调试异常和监视控制寄存器（DEMCR），用于使能 DWT 等跟踪单元。
 */
typedef struct {
    volatile uint32_t DHCSR;	/* 调试暂停控制和状态寄存器 */
    volatile uint32_t DCRSR;	/* 调试核心寄存器选择寄存器 */
    volatile uint32_t DCRDR;	/* 调试核心寄存器数据寄存器 */
    volatile uint32_t DEMCR;	/* 调试异常和监视控制寄存器 */
} CoreDebug_Type;

/**
 * @brief 将结构体指针映射到硬件实际地址
 *
 * 通过强制类型转换，使得可以通过结构体成员方式访问硬件寄存器。
 * 例如：DWT->CYCCNT 即可读取周期计数器当前值。
 */
#define DWT              ((DWT_Type *) DWT_BASE)
#define CoreDebug        ((CoreDebug_Type *) CoreDebug_BASE)
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
void DWT_Delay_Init(void);
void DWT_Delay_us(uint32_t us);
void DWT_Delay_ms(uint32_t ms);

// 安全的DWT循环计时宏
#define DWT_TIMEOUT_CHECK(start, timeout_us)              \
(((DWT->CYCCNT) >= (start)) ?                             \
((DWT->CYCCNT) - (start) > (timeout_us) * CPU_FREQ_MHZ) : \
((UINT32_MAX - (start) + (DWT->CYCCNT) + 1) > (timeout_us) * CPU_FREQ_MHZ))
//******************************** 函数声明 ***********************************//

#if defined (__cplusplus)
}
#endif

#endif //DWT_DELAY_H
