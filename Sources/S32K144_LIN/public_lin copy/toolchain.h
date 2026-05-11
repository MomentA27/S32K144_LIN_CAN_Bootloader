//
// Created by redmiX on 2026/4/14.
//

#ifndef TOOLCHAIN_H
#define TOOLCHAIN_H

//******************************** Includes *********************************//
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
/*-------------------------- GCC / GHS（Green Hills）-------------------------*/
#if defined __ghs__ || defined (__GNUC__)
    /** 内嵌汇编关键字 */
    #define ASM_KEYWORD         __asm
    /** 中断函数修饰符 */
    #define INTERRUPT_FUNC      __interrupt
    /** 内联函数关键字 */
    #define INLINE              inline
    /** 打包结构体开始（GCC 无需额外标记） */
    #define PACKED_STRUCT_BEGIN
    /** 打包结构体字段标记 */
    #define PACKED_STRUCT_FIELD(x) x __attribute__((packed))
    /** 打包结构体结束标记 */
    #define PACKED_STRUCT_END   __attribute__((packed))
    /** 指定对齐字节数 */
    #define ALIGNEDXB(x)        __attribute__((aligned(x)))

/*-------------------------- Diab Data Compiler (DCC) ------------------------*/
#elif defined __DCC__
    #define ASM_KEYWORD         __asm volatile
    #define INTERRUPT_FUNC      __interrupt__
    #define INLINE              __inline__
    #define PACKED_STRUCT_BEGIN __packed__
    #define PACKED_STRUCT_FIELD(x) x
    #define PACKED_STRUCT_END
    #define ALIGNEDXB(x)        __attribute__((aligned(x)))

/*-------------------------- Metrowerks CodeWarrior --------------------------*/
#elif defined __MWERKS__
    #define INLINE              inline
    // 其他宏若需支持请在此补充

/*-------------------------- ARM Compiler (DS-5/Keil) ------------------------*/
#elif defined __CC_ARM
    #define ASM_KEYWORD         __asm
    #define INTERRUPT_FUNC      __irq
    /* 注：DS-5 中模块间的内联可能失效，故此处强制使用 __inline */
    #if 1
        #define INLINE          __inline
    #else
        #define INLINE
    #endif
    #define PACKED_STRUCT_BEGIN
    #define PACKED_STRUCT_FIELD(x) x __attribute__((packed))
    #define PACKED_STRUCT_END   __attribute__((packed))
    #define ALIGNEDXB(x)        __attribute__((aligned(x)))

/*-------------------------- IAR Embedded Workbench --------------------------*/
#elif defined __IAR_SYSTEMS_ICC__
    #define ASM_KEYWORD         __asm volatile
    #define INTERRUPT_FUNC      __interrupt
    #define INLINE              inline
    #if 1   /* IAR 7.50.3 及更高版本适用 */
        #define STRINGIZE(X)    #X
        #define PACKED_STRUCT_BEGIN     __packed
        #define PACKED_STRUCT_FIELD(x)  x
        #define PACKED_STRUCT_END
        #define ALIGNEDXB(n)            _Pragma(STRINGIZE(data_alignment=n))
    #else   /* IAR 8.30.2 及更高版本适用 */
        #define PACKED_STRUCT_BEGIN
        #define PACKED_STRUCT_FIELD(x)  x
        #define PACKED_STRUCT_END       __attribute__((packed))
        #define ALIGNEDXB(n)            __attribute__((aligned(n)))
    #endif

/*-------------------------- 未知工具链 --------------------------------------*/
#else
    #error "Unknown toolchain"
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

#endif /* TOOLCHAIN_H */
