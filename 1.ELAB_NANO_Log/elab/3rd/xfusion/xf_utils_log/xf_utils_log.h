/**
 * @file xf_log.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-01
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_UTILS_LOG_H__
#define __XF_UTILS_LOG_H__

/* ==================== [Includes] ========================================== */

#include "xf_utils_log_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */


#if XF_LOG_DUMP_IS_ENABLE

#define XF_DUMP_HEAD_BIT        (0) // flags_mask 中表头的标志位
#define XF_DUMP_ASCII_BIT       (1) // flags_mask 中 ASCII 的标志位
#define XF_DUMP_ESCAPE_BIT      (2) // flags_mask 中带有转义字符的标志位
#define XF_DUMP_TAIL_BIT        (3) // flags_mask 中表尾的标志位

#define XF_DUMP_BIT(nr)         BIT(nr)

#define XF_DUMP_TABLE           (XF_DUMP_BIT(XF_DUMP_HEAD_BIT) | XF_DUMP_BIT(XF_DUMP_TAIL_BIT))

/* 只输出 16 进制格式数据 */
#define XF_DUMP_FLAG_HEX_ONLY           (XF_DUMP_TABLE)
/* 输出 16 进制格式数据并带有 ASCII 字符 */
#define XF_DUMP_FLAG_HEX_ASCII          (XF_DUMP_BIT(XF_DUMP_ASCII_BIT) | XF_DUMP_TABLE)
/* 输出 16 进制格式数据、 ASCII 字符、转义字符，其余输出 16 进制原始值 */
#define XF_DUMP_FLAG_HEX_ASCII_ESCAPE   (XF_DUMP_FLAG_HEX_ASCII | XF_DUMP_BIT(XF_DUMP_ESCAPE_BIT))

#endif

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

#if XF_LOG_DUMP_IS_ENABLE
/**
 * @brief 输出内存信息。
 *
 * @param addr 内存地址。
 * @param size 待输出的内存字节长度。
 * @param flags_mask 格式掩码，见 XF_DUMP_FLAG_*。
 * @return xf_err_t
 *      - XF_ERR_INVALID_ARG            参数错误
 *      - XF_OK                         成功
 */
xf_err_t xf_dump_mem(void *addr, size_t size, uint8_t flags_mask);
#endif

/* ==================== [Macros] ============================================ */

#if XF_LOG_DUMP_IS_ENABLE

#define XF_LOG_BUFFER_HEX(buffer, buffer_len)     xf_dump_mem(buffer, buffer_len, XF_DUMP_FLAG_HEX_ONLY)
#define XF_LOG_BUFFER_HEXDUMP(buffer, buffer_len)    xf_dump_mem(buffer, buffer_len, XF_DUMP_FLAG_HEX_ASCII)
#define XF_LOG_BUFFER_HEXDUMP_ESCAPE(buffer, buffer_len) xf_dump_mem(buffer, buffer_len, XF_DUMP_FLAG_HEX_ASCII_ESCAPE)

#else

#define XF_LOG_BUFFER_HEX(buffer, buffer_len)
#define XF_LOG_BUFFER_HEXDUMP(buffer, buffer_len)
#define XF_LOG_BUFFER_HEXDUMP_ESCAPE(buffer, buffer_len)

#endif // XF_LOG_DUMP_IS_ENABLE

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_UTILS_LOG_H__

