/**
 * @file xf_log_mem.c
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-26
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_utils_log_config.h"
#include "stdio.h"
#if XF_LOG_DUMP_IS_ENABLE
#include "xf_utils_log.h"

/* ==================== [Defines] =========================================== */

/* 只输出 16 进制格式数据时，每行实际会输出的字节数 */
#define XF_DUMP_HEX_BYTES_PER_LINE                  (53)
/* 输出 16 进制格式数据和 ASCII 字符时，每行实际会输出的字节数 */
#define XF_DUMP_HEX_ASCII_BYTES_PER_LINE            (72)
/* 输出 16 进制格式数据、 ASCII 字符、转义字符时，每行实际会输出的字节数 */
#define XF_DUMP_HEX_ASCII_ESCAPE_BYTES_PER_LINE     (104)
/* 表头字节数，可能会多几个字节 */
#define XF_DUMP_TABLE_HEADER_BYTES                  (332)

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

/* ==================== [Static Variables] ================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

xf_err_t xf_dump_mem(void *addr, size_t size, uint8_t flags_mask)
{
    if ((!addr) || (size == 0)) {
        printf("Invalid memory block\n");
        return XF_ERR_INVALID_ARG;
    }

    // 每行输出的内存字节数，不等于实际输出字节
    const uint8_t bytes_mem_out_per_line = 16;

    // 每行实际输出字节数
    uint8_t bytes_actually_out_per_line = 0;
    if (!BIT_GET(flags_mask, XF_DUMP_ASCII_BIT)) {
        bytes_actually_out_per_line = XF_DUMP_HEX_BYTES_PER_LINE;
    } else if (BIT_GET(flags_mask, XF_DUMP_ASCII_BIT)
               && !BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
        bytes_actually_out_per_line = XF_DUMP_HEX_ASCII_BYTES_PER_LINE;
    } else if (BIT_GET(flags_mask, XF_DUMP_ASCII_BIT)
               && BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
        bytes_actually_out_per_line = XF_DUMP_HEX_ASCII_ESCAPE_BYTES_PER_LINE;
    }

    // 打印内存块的起始地址
    printf("MEMORY START ADDRESS: %p, OUTPUT %d BYTES."STR_ENTER, addr, (int)size);

    // 打印表头
    if (BIT_GET(flags_mask, XF_DUMP_HEAD_BIT)) {
        for (uint8_t i = 0; i < bytes_actually_out_per_line; i++) {
            printf("-");
        }
        printf(STR_ENTER);
        printf(" OFS  "); // 偏移 offset
        for (uint8_t i = 0; i < bytes_mem_out_per_line; i++) {
            printf("%2X ", i);
        }
        if (BIT_GET(flags_mask, XF_DUMP_ASCII_BIT)) {
            printf("| ASCII");
        }
        printf(STR_ENTER);
        for (uint8_t i = 0; i < bytes_actually_out_per_line; i++) {
            printf("-");
        }
        printf(STR_ENTER);
    }

    uint8_t byte = 0; // 当前地址的字节的值
    // 遍历内存块中
    for (size_t i = 0; i < size; i++) {
        byte = *((uint8_t *)addr + i);

        if (i % bytes_mem_out_per_line == 0) {
            // 每行的开头打印行号
            printf("%04lu: ", (unsigned long)(i / bytes_mem_out_per_line));
        }

        printf("%02X ", byte); // 打印当前地址的字节的值的十六进制

        // 如果达到每行显示的字节数，
        // 或者已经是最后一个字节，就打印 ASCII 字符串
        if (((i + 1) % bytes_mem_out_per_line == 0) || (i == size - 1)) {
            // 如果不是每行显示的字节数，就补齐空格
            if ((i + 1) % bytes_mem_out_per_line != 0) {
                for (size_t j = 0;
                        j < (bytes_mem_out_per_line - (i + 1) % bytes_mem_out_per_line) * 3;
                        j++) {
                     printf(" ");
                }
            }
            // 遍历输出 ASCII
            if (BIT_GET(flags_mask, XF_DUMP_ASCII_BIT)) {
                printf("| ");
                for (size_t j = i - i % bytes_mem_out_per_line; j <= i; j++) {
                    uint8_t b = *((uint8_t *)addr + j);
                    if (b >= ' ' && b <= '~') { // 可见字符
                        if (BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
                            printf(" ");
                        }
                        printf("%c", b);
                        if (BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
                            printf(" ");
                        }
                    } else if (BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
                        // 显示转义字符
                        switch (b) {
                        case '\0': printf("\\0 "); break; // 空字符
                        case '\a': printf("\\a "); break; // 响铃符
                        case '\b': printf("\\b "); break; // 退格符
                        case '\t': printf("\\t "); break; // 水平制表符
                        case '\n': printf("\\n "); break; // 换行符
                        case '\v': printf("\\v "); break; // 垂直制表符
                        case '\f': printf("\\f "); break; // 换页符
                        case '\r': printf("\\r "); break; // 回车符
                        // 其他不可打印字符，用十六进制表示
                        default: printf("%02x ", b); break;
                        }
                    } else {
                        // 转义字符外的不可见字符
                        printf(".");
                        if (BIT_GET(flags_mask, XF_DUMP_ESCAPE_BIT)) {
                            printf("  ");
                        }
                    } /* else: 非转义字符 */
                } /* for: 输出 ASCII */
            } /* if: 需要输出 ASCII */
            printf(STR_ENTER);
        } /* if: 需要输出 ASCII 或空格 */
    } /* for: 0 ~ size */
    /* 表尾 */
    if (BIT_GET(flags_mask, XF_DUMP_TAIL_BIT)) {
        for (uint8_t i = 0; i < bytes_actually_out_per_line; i++) {
            printf("-");
        }
        printf(STR_ENTER);
    }
    return XF_OK;
}

#endif // XF_LOG_DUMP_IS_ENABLE

/* ==================== [Static Functions] ================================== */

