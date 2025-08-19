/**
 * @file xf_DEF.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-01
 * 
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 * 
 */

#ifndef _XF_DEF_H
#define _XF_DEF_H


#if defined(__linux__)
#define STR_ENTER                                   "\n"
#else
#define STR_ENTER                                   "\r\n"
#endif

/**
 * @brief 分支预测，x 为真的可能性更大。
 */
#if defined(__GNUC__)
#   if !defined(likely)
#       
#   endif
#else
#   define likely(x) (x)
#endif

/**
 * @brief 分支预测，x 为假的可能性更大。
 */
#if defined(__GNUC__)
#   if !defined(unlikely)
#       define unlikely(x) __builtin_expect(!!(x), 0)
#   endif
#else
#   define unlikely(x) (x)
#endif





#ifndef ARRAY_SIZE
/**
 * @brief ARRAY_SIZE - 获取数组 arr 中的元素数量。
 *
 * @param arr 待求的数组。(必须是数组，此处不进行判断)
 *
 * @return 数组元素个数。
 */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifndef UNUSED
/**
 * @brief 未使用的变量通过 UNUSED 防止编译器警告。
 */
#define UNUSED(x)  \
    do {           \
        (void)(x); \
    } while (0)
#endif

#ifndef CONCAT
/**
 * @brief 拼接。
 */
#define CONCAT(a, b) a##b
#endif

#ifndef CONCAT3
/**
 * @brief 拼接 3 个参数。
 */
#define CONCAT3(a, b, c) a##b##c
#endif

#ifndef offsetof
/**
 * @brief offsetof - 返回结构成员相对于结构开头的字节偏移量。
 */
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif /* offsetof */

#ifndef container_of
/**
 * @brief container_of - 通过结构体成员变量地址获取结构体的地址.
 *
 * @param ptr 指向成员的指针。
 * @param type 结构体类型。
 * @param member 结构中成员的名称。
 *
 * @return 结构体的地址。
 *
 * @example
 * @code{c}
 * typedef struct {
 *     int member1;
 *     char member2;
 *     float member3;
 * } struct_test_t;
 *
 * int main()
 * {
 *     struct_test_t stru = {
 *         .member1 = 0x12345678,
 *         .member2 = 0xAB,
 *         .member3 = -123.456f,
 *     };
 *     char *p_member2 = &stru.member2;
 *     struct_test_t *p_stru = xf_container_of(p_member2, struct_test_t, member2);
 *     if ((p_stru->member1 != (int)0x12345678)
 *             || (p_stru->member2 != (char)0xAB)
 *             || ((p_stru->member3 - (-123.456f)) > 1e-6f)) {
 *         printf("container_of error!\n");
 *     } else {
 *         printf("container_of ok!\n");
 *     }
 * }
 * @endcode
 */
#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif

#if (!defined(BIT0))
#define BIT31 0x80000000
#define BIT30 0x40000000
#define BIT29 0x20000000
#define BIT28 0x10000000
#define BIT27 0x08000000
#define BIT26 0x04000000
#define BIT25 0x02000000
#define BIT24 0x01000000
#define BIT23 0x00800000
#define BIT22 0x00400000
#define BIT21 0x00200000
#define BIT20 0x00100000
#define BIT19 0x00080000
#define BIT18 0x00040000
#define BIT17 0x00020000
#define BIT16 0x00010000
#define BIT15 0x00008000
#define BIT14 0x00004000
#define BIT13 0x00002000
#define BIT12 0x00001000
#define BIT11 0x00000800
#define BIT10 0x00000400
#define BIT9  0x00000200
#define BIT8  0x00000100
#define BIT7  0x00000080
#define BIT6  0x00000040
#define BIT5  0x00000020
#define BIT4  0x00000010
#define BIT3  0x00000008
#define BIT2  0x00000004
#define BIT1  0x00000002
#define BIT0  0x00000001
#endif /* (!defined(BIT0)) */

#if (!defined(BIT32))
#define BIT63 (0x80000000ULL << 32)
#define BIT62 (0x40000000ULL << 32)
#define BIT61 (0x20000000ULL << 32)
#define BIT60 (0x10000000ULL << 32)
#define BIT59 (0x08000000ULL << 32)
#define BIT58 (0x04000000ULL << 32)
#define BIT57 (0x02000000ULL << 32)
#define BIT56 (0x01000000ULL << 32)
#define BIT55 (0x00800000ULL << 32)
#define BIT54 (0x00400000ULL << 32)
#define BIT53 (0x00200000ULL << 32)
#define BIT52 (0x00100000ULL << 32)
#define BIT51 (0x00080000ULL << 32)
#define BIT50 (0x00040000ULL << 32)
#define BIT49 (0x00020000ULL << 32)
#define BIT48 (0x00010000ULL << 32)
#define BIT47 (0x00008000ULL << 32)
#define BIT46 (0x00004000ULL << 32)
#define BIT45 (0x00002000ULL << 32)
#define BIT44 (0x00001000ULL << 32)
#define BIT43 (0x00000800ULL << 32)
#define BIT42 (0x00000400ULL << 32)
#define BIT41 (0x00000200ULL << 32)
#define BIT40 (0x00000100ULL << 32)
#define BIT39 (0x00000080ULL << 32)
#define BIT38 (0x00000040ULL << 32)
#define BIT37 (0x00000020ULL << 32)
#define BIT36 (0x00000010ULL << 32)
#define BIT35 (0x00000008ULL << 32)
#define BIT34 (0x00000004ULL << 32)
#define BIT33 (0x00000002ULL << 32)
#define BIT32 (0x00000001ULL << 32)
#endif /* (!defined(BIT32)) */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifndef __ASSEMBLER__
#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif
#ifndef BIT64
#define BIT64(nr) (1ULL << (nr))
#endif
#else /* defined (__ASSEMBLER__) */
#ifndef BIT
#define BIT(nr) (1 << (nr))
#endif
#endif /* __ASSEMBLER__ */

/**
 * @brief 32 位位操作宏（一个位）。
 */
#ifndef BIT_SET0
/**
 * @brief 设置 32 位变量 var 的 bit_n 为 0。
 *
 * @param variable 被读写的变量。
 * @param n 比特下标，从 0 开始。
 */
#define BIT_SET0(var, n) ((var) &= ~BIT(n))
#endif

#ifndef BIT_SET1
/**
 * @brief 设置 32 位变量 var 的 bit_n 为 1。
 *
 * @param variable 被读写的变量。
 * @param n 比特下标，从 0 开始。
 */
#define BIT_SET1(var, n) ((var) |= BIT(n))
#endif

#ifndef BIT_SET
/**
 * @brief 设置 32 位变量 var 的 bit_n 为 value。
 *
 * @param variable 被读写的变量。
 * @param n 比特下标，从 0 开始。
 */
#define BIT_SET(var, n, value) \
    ((value) ? BIT_SET1((var), (n)) : BIT_SET0((var), (n)))
#endif

#ifndef BIT_FLIP
/**
 * @brief 翻转 32 位变量 var 的 bit_n。
 *
 * @param variable 被翻转的变量。
 * @param n 比特下标，从 0 开始。
 */
#define BIT_FLIP(var, n) ((var) ^= BIT(n))
#endif

#ifndef BIT_GET
/**
 * @brief 获取 32 位源 src 的 bit_n。
 *
 * @param variable 被读取的变量。
 * @param n 比特下标，从 0 开始。
 *
 * @return src 的 bit_n 上的值。
 */
#define BIT_GET(src, n) (((src) & BIT(n)) >> (n))
#endif

#ifndef BIT_GET_MODIFY0
/**
 * @brief 获取将 32 位源 src 的 bit_n 置 0 后的值。
 *
 * @param variable 被读取的变量。
 * @param n 比特下标，从 0 开始。
 *
 * @return 32 位 src 的 bit_n 置 0 后的值。
 */
#define BIT_GET_MODIFY0(src, n) ((src) & ~BIT(n))
#endif

#ifndef BIT_GET_MODIFY1
/**
 * @brief 获取将 32 位源 src 的 bit_n 置 1 后的值。
 *
 * @param variable 被读取的变量。
 * @param n 比特下标，从 0 开始。
 *
 * @return 32 位 src 的 bit_n 置 1 后的值。
 */
#define BIT_GET_MODIFY1(src, n) ((src) | BIT(n))
#endif

#ifndef BIT_GET_MODIFY
/**
 * @brief 获取将 32 位源 src 的 bit_n 置 value 后的值。
 *
 * @param variable 被读取的变量。
 * @param n 比特下标，从 0 开始。
 * @param value 0 或 1。
 *
 * @return 32 位 src 的 bit_n 置 value 后的值。
 */
#define BIT_GET_MODIFY(src, n, value) \
    ((value) ? BIT_GET_MODIFY1((src), (n)) : BIT_GET_MODIFY0((src), (n)))
#endif

#ifndef BIT_GET_MODIFY_FLIP
/**
 * @brief 获取翻转 32 位源 src 的 bit_n 后的值。
 *
 * @param variable 被读取的变量。
 * @param n 比特下标，从 0 开始。
 *
 * @return 翻转 32 位 src 的 bit_n 后的值。
 */
#define BIT_GET_MODIFY_FLIP(src, n) ((src) ^ BIT(n))
#endif

/**
 * @brief 32 位位操作宏（多位）。
 */
#ifndef BITS_MASK
/**
 * @brief 获取低 n 位为 1 的位掩码。
 *
 * @param n 小于等于 32 的数。
 *
 * @return 低 n 位为 1 的位掩码。
 *
 * @example
 * BITS_MASK(6)  == 0x003f ==          0b00111111
 * BITS_MASK(13) == 0x1fff == 0b00011111 11111111
 */
#define BITS_MASK(n) (((n) < 32) ? (BIT(n) - 1) : (~0UL))
#endif

#ifndef BITS_SET0
/**
 * @brief 设置 32 位变量 var 的对应位掩码 bits_mask 为 1 的地方为 0。
 *
 * @param var 被设置的变量。
 * @param bits_mask 位掩码。注意！为 1 的位表示需要被设为 0 的位。
 *
 * @note
 * 1. 位掩码为 1 的位表示需要被设为 0 的位。
 *
 * @example
 * uint32_t var = 0xffff;
 * BITS_SET0(var, 0b0011 0110 0101 1010) == 0b11001001 10100101
 * var = 0xffff;
 * BITS_SET0(var, BITS_MASK(4) << 3)     == 0b11111111 10000111
 */
#define BITS_SET0(var, bits_mask) ((var) &= ~(bits_mask))
#endif

#ifndef BITS_SET1
/**
 * @brief 设置 32 位变量 var 的对应位掩码 bits_mask 为 1 的地方为 1。
 *
 * @param var 被设置的变量。
 * @param bits_mask 位掩码。注意！为 1 的位表示需要被设为 1 的位。
 *
 * @note
 * 1. 位掩码为 1 的位表示需要被设为 1 的位。
 *
 * @example
 * uint32_t var = 0x0000;
 * BITS_SET1(var, 0b0011 0110 0101 1010) == 0b00110110 01011010
 * var = 0x0000;
 * BITS_SET1(var, BITS_MASK(4) << 3)     == 0b00000000 01111000
 */
#define BITS_SET1(var, bits_mask) ((var) |= (bits_mask))
#endif

#ifndef BITS_FLIP
/**
 * @brief 翻转 32 位变量 var 的 bits_mask 为 1 的位。
 *
 * @param var 被设置的变量。
 * @param bits_mask 位掩码。注意！为 1 的位表示需要被翻转的位。
 *
 * @note
 * 1. 位掩码为 1 的位表示需要被翻转的位。
 */
#define BITS_FLIP(var, bits_mask) ((var) ^= (bits_mask))
#endif

#ifndef BITS_GET
/**
 * @brief 获取 32 位 src 内，从 offset 位起，共 n 位数据。
 *
 * @param src 被读取的源。
 * @param n 需要读取 n 位。
 * @param offset 偏移量。从 bit0 起计算。
 *
 * @return 32 位源 src 内，从 offset 位起，共 n 位数据。
 *
 * @example
 * BITS_GET(0b0011 1010 1111 0010, 7, 5) == 0x57 == 0b01010111
 *
 * bit:                54 3210  : bit == 5
 * src:  0b0011 1010 1111 0010  : src == 0x3af2
 *              ↓      ↓
 * num:         7654 321        : n   == 7
 * ————————————————————————————————————————————
 * ret:     0b0 1010 111        : ret == 0x57
 */
#define BITS_GET(src, n, offset) (((src) & (BITS_MASK(n) << (offset))) >> (offset))
#endif

#ifndef BITS_CHECK
/**
 * @brief 检查变量 var 在 bits_mask 的位置上是否存在 1。
 *
 * @param var 待检查的变量。
 * @param bits_mask 位掩码。注意！为 1 的位表示需要检查的位。
 *
 * @note
 * 1. 位掩码为 1 的位表示需要检查的位。
 */
#define BITS_CHECK(src, bits_mask) (!!((src) & (bits_mask)))
#endif

#ifndef BITS_GET_MODIFY
/**
 * @brief 将 32 位 src 的 offset 位起 n 位修改为 value 并返回（不修改 src）。
 *
 * @param src 被读取的源。
 * @param n 需要修改的位数。
 * @param offset 偏移量。从 bit0 起计算。
 * @param value src 的 offset 位起 n 位源需要修改到的目标值。
 *
 * @return 32 位 src 的 offset 位起 n 位修改为 value 的值。
 *
 * @example
 *  BITS_GET_MODIFY(src       , n, offset, value     )
 *  BITS_GET_MODIFY(0x4c5ca6d2, 8, 13    , 0b10101111)
 *
 *  src:    0100 1100 0101 1100 1010 0110 1101 0010 == 0x4c5ca6d2
 *                       1 0101 111                 == (value << 13)
 *                       ↓ ↓↓↓↓ ↓↓↓
 *  ret:    0100 1100 0101 0101 1110 0110 1101 0010 == 0x4c55e6d2
 *
 * @details
 *          0100 1100 0101 1100 1010 0110 1101 0010   <=  src
 *       ^  0000 0000 0001 0101 1110 0000 0000 0000   <=  ((BITS_MASK(n) & (value)) << (offset))
 *      ————————————————————————————————————————————————
 *          0100 1100 0100 1001 0100 0110 1101 0010
 *
 *          0100 1100 0100 1001 0100 0110 1101 0010       ans
 *       &  0000 0000 0001 1111 1110 0000 0000 0000   <=  (BITS_MASK(n) << 13)
 *      ————————————————————————————————————————————————
 *          0000 0000 0000 1001 0100 0000 0000 0000
 *
 *          0000 0000 0000 1001 0100 0000 0000 0000       ans
 *       ^  0100 1100 0101 1100 1010 0110 1101 0010   <=  src
 *      ————————————————————————————————————————————————
 *          0100 1100 0101 0101 1110 0110 1101 0010   =>  ret
 *
 * @note
 * 1. n + offset 不要大于 32 位。
 */
#define BITS_GET_MODIFY(src, n, offset, value) \
    ((((src) ^ ((BITS_MASK(n) & (value)) << (offset))) & (BITS_MASK(n) << (offset))) ^ (src))
#endif

#ifndef BITS_SET
/**
 * @brief 设置 32 位 src 的 offset 位起 n 位为 value。
 *
 * @see BITS_GET_MODIFY @ref BITS_GET_MODIFY
 * @note
 * 1. n + offset 不要大于 32 位。
 */
#define BITS_SET(src, n, offset, value) \
    ((src) = BITS_GET_MODIFY((src), (n), (offset), (value)))
#endif

#endif /* _XF_DEF_H */
