//
// Created by redmiX on 2026/4/14.
//

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

//******************************** Includes *********************************//
#include <stdint.h>
//******************************** Includes *********************************//
//---------------------------------------------------------------------------//
//******************************** Defines **********************************//
#ifndef NULL
    /** 空指针常量 */
    #define NULL ((void *)0)
#endif

#ifndef NULL_PTR
    /** 空指针常量（别名） */
    #define NULL_PTR ((void *)0)
#endif

#ifndef FALSE
    /** 布尔假值 */
    #define FALSE 0U
#endif

#ifndef TRUE
    /** 布尔真值 */
    #define TRUE 1U
#endif

#ifndef LITTLE
    /** 小端标识 */
    #define LITTLE 1
#endif

#ifndef BIG
    /** 大端标识 */
    #define BIG 2
#endif

#ifndef MACHINE_ENDIAN
    /** 目标机字节序（默认为小端） */
    #define MACHINE_ENDIAN LITTLE
#endif

#if (LITTLE == MACHINE_ENDIAN)   /* 小端主机：需要转换 */

#ifndef ntohs
    /** 16位网络序转主机序（小端下执行字节交换） */
    #define ntohs(a) (((((uint16_t)(a)) >> 8) & 0x00ffU) | \
                      (((uint16_t)(((uint16_t)(a)) << 8)) & 0xff00U))
#endif

#ifndef ntohl
    /** 32位网络序转主机序 */
    #define ntohl(a) (((((uint32_t)(a)) >> 24) & 0x000000ffU) | \
                      ((((uint32_t)(a)) >> 8 ) & 0x0000ff00U) | \
                      ((uint32_t)(((uint32_t)(a)) << 8 ) & 0x00ff0000U) | \
                      ((uint32_t)(((uint32_t)(a)) << 24) & 0xff000000U))
#endif

#ifndef ntohll
    /** 64位网络序转主机序 */
    #define ntohll(a) (((((uint64_t)(a)) & 0xFF00000000000000ULL) >> 56U) | \
                       ((((uint64_t)(a)) & 0x00FF000000000000ULL) >> 40U) | \
                       ((((uint64_t)(a)) & 0x0000FF0000000000ULL) >> 24U) | \
                       ((((uint64_t)(a)) & 0x000000FF00000000ULL) >> 8U)  | \
                       ((((uint64_t)(a)) & 0x00000000FF000000ULL) << 8U)  | \
                       ((((uint64_t)(a)) & 0x0000000000FF0000ULL) << 24U) | \
                       ((((uint64_t)(a)) & 0x000000000000FF00ULL) << 40U) | \
                       ((((uint64_t)(a)) & 0x00000000000000FFULL) << 56U))
#endif

#ifndef htons
    /** 16位主机序转网络序（小端下同 ntohs） */
    #define htons(a) ntohs(a)
#endif

#ifndef htonl
    /** 32位主机序转网络序 */
    #define htonl(a) ntohl(a)
#endif

#ifndef htonll
    /** 64位主机序转网络序 */
    #define htonll(a) ntohll(a)
#endif

#elif (BIG == MACHINE_ENDIAN)    /* 大端主机：无需转换 */

#ifndef ntohs
    #define ntohs(a) (a)
#endif

#ifndef ntohl
    #define ntohl(a) (a)
#endif

#ifndef htons
    #define htons(a) (a)
#endif

#ifndef htonl
    #define htonl(a) (a)
#endif

#else
    #error "请定义 MACHINE_ENDIAN：BIG 或 LITTLE"
#endif
//******************************** Defines **********************************//
//---------------------------------------------------------------------------//
//******************************** Typedefs *********************************//
///* 有符号整数类型 */
//typedef signed char        sint8_t;
//typedef signed short       sint16_t;
//typedef signed int         sint32_t;
//
///** 布尔类型（无符号字符） */
//typedef unsigned char      boolean;
//
///** 64位无符号整数（用于时间戳） */
//typedef unsigned long long uint64;
//typedef signed long long   sint64_t;
//
//#if (!defined TYPEDEFS_H) && (!defined _STDINT) && (!defined _SYS__STDINT_H) && (!defined _EWL_CSTDINT)
//   typedef unsigned char       uint8_t;
//   typedef unsigned short      uint16_t;
//   typedef unsigned int        uint32_t;
//
//   typedef volatile unsigned char  vuint8_t;
//   typedef volatile unsigned short vuint16_t;
//   typedef volatile unsigned int   vuint32_t;
//   typedef volatile char           vint8_t;
//   typedef volatile short          vint16_t;
//   typedef volatile int            vint32_t;
//#endif
//
///* 旧类型定义（兼容 FECLLD） */
//typedef uint32_t uint32;
//typedef uint16_t uint16;
//typedef uint8_t  uint8;
//
//#if (!defined TYPEDEFS_H) && (!defined _STDINT) && (!defined _SYS__STDINT_H) && (!defined _EWL_CSTDINT)
//   typedef sint64_t int64_t;
//   typedef sint32_t int32_t;
//   typedef sint16_t int16_t;
//   typedef sint8_t  int8_t;
//#endif
//
//typedef sint8_t  sint8;
//typedef sint16_t sint16;
//typedef sint32_t sint32;
//
//#if (!defined TYPEDEFS_H) && (!defined _STDINT)
//   typedef unsigned long long uint64_t;
//#endif

/* 通用别名 */
typedef int          int_t;
typedef char         char_t;
typedef unsigned int uint_t;
typedef double       float64_t;
typedef unsigned int uaddr_t;
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

#endif /* COMMON_TYPES_H */
