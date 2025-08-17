/**
 * @file elab_log.h
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-17
 * 
 * 
 */


 
 #ifndef ELAB_LOG_H 
 #define ELAB_LOG_H 
 
 /* ==================== [Includes] ========================================== */
 #include "elab_common.h"
 
 
 #ifdef __cplusplus
 extern "C"{
 #endif
 
 /* ==================== [Defines] ========================================== */
#define ELAB_COLOR_ENABLE (1)

#define ELAB_TAG(_tag)   static const char *TAG = _tag

#ifndef ELOG_LEVEL_CURRENT
#define ELOG_LEVEL_CURRENT ELOG_LEVEL_DEBUG
#endif
 /* ==================== [Typedefs] ========================================== */
 enum elog_level_enum
{
    ELOG_LEVEL_ERROR = 1 ,
    ELOG_LEVEL_WARNING  = 2 ,
    ELOG_LEVEL_INFO  = 3 ,
    ELOG_LEVEL_DEBUG = 4 ,
    
    ELOG_LEVEL_MAX=5,
};
 /* ==================== [Global Prototypes] ================================== */
 void _elog_printf(const char *name ,uint32_t line,
                uint8_t level, const char *fmt, ...);
 /* ==================== [Macros] ============================================ */
 


/* Enable error level debug message */
#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_ERROR
#define elog_error(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define elog_error(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_WARNING
#define elog_warn(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_WARNING, __VA_ARGS__)
#else
#define elog_warn(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_INFO
#define elog_info(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_INFO, __VA_ARGS__)
#else
#define elog_info(...)
#endif

#if ELOG_LEVEL_CURRENT >= ELOG_LEVEL_DEBUG
#define elog_debug(...) _elog_printf(TAG, __LINE__, ELOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define elog_debug(...)
#endif




 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* ELAB_LOG_H */
 
 