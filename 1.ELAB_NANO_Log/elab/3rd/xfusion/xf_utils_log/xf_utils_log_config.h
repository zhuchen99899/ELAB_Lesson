/**
 * @file xf_log_config.h
 * @author cangyu (sky.kirto@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-28
 *
 * @copyright Copyright (c) 2024, CorAL. All rights reserved.
 *
 */

#ifndef __XF_UTILS_LOG_CONFIG_H__
#define __XF_UTILS_LOG_CONFIG_H__

/* ==================== [Includes] ========================================== */
#include "../../../common/elab_std.h"
#include "../xf_common/xf_err.h"
#include "../xf_common/xf_def.h"
#ifdef __cplusplus
extern "C" {
#endif

/* ==================== [Defines] =========================================== */


#if XF_STDIO_IS_ENABLE
#   include <stdio.h>
#endif

#if !defined(XF_LOG_DUMP_ENABLE) || (XF_LOG_DUMP_ENABLE)
#   define XF_LOG_DUMP_IS_ENABLE (1)
#else
#   define XF_LOG_DUMP_IS_ENABLE (0)
#endif



// log 对接打印函数， 简化版对接。
#if !defined(xf_log_printf)
#   define xf_log_printf(format, ...) printf(format, ##__VA_ARGS__)
#endif

// log 对接打印二进制的后端，默认使用xf_log_printf打印
#if !defined(xf_log_dump_printf)
#   define xf_log_dump_printf(format, ...) xf_log_printf(format, ##__VA_ARGS__)
#endif




/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================= */

/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __XF_UTILS_LOG_CONFIG_H__
