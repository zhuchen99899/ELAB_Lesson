/**
 * @file elab_log.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-17
 * 
 * 
 */

/* ==================== [Includes] ========================================== */
#include "elab_log.h"
#include "inttypes.h"
#include "stdio.h"
/* ==================== [Defines] ========================================== */

#define NONE "\033[0;0m"
#define LIGHT_RED "\033[1;31m"
#define YELLOW "\033[0;33m"
#define LIGHT_BLUE "\033[1;34m"
#define GREEN "\033[0;32m"


#define ELAB_LOG_BUFFER_SIZE 256
/* ==================== [Macros] ============================================ */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ========================================== */

/* ==================== [Static Variables] ========================================== */
#if (ELAB_COLOR_ENABLE != 0)
static const char *const elog_color_table[ELOG_LEVEL_MAX] = {
    NONE,
    LIGHT_RED,  // ELOG_LEVEL_ERROR
    YELLOW, // ELOG_LEVEL_WARNING
    LIGHT_BLUE, // ELOG_LEVEL_INFO
    GREEN// ELOG_LEVEL_DEBUG
};
#endif

static const char elog_level_label[ELOG_LEVEL_MAX] = {
    ' ',
    'E',  // ELOG_LEVEL_ERROR
    'W',  // ELOG_LEVEL_WARNNING
    'I',  // ELOG_LEVEL_INFO
    'D'   // ELOG_LEVEL_DEBUG
};
static char _buff[ELAB_LOG_BUFFER_SIZE];

/* ==================== [Static Functions] ================================== */

/* ==================== [Public Functions] ================================== */
void _elog_printf(const char *name ,uint32_t line,
                uint8_t level, const char *fmt, ...)
{
    printf("%s[%c/%s L:%"PRIu32",T:%"PRIu32"]",elog_color_table[level],elog_level_label[level],name,line,elab_time_ms());
    va_list args;
    va_start(args,fmt);
    int count= vsnprintf(_buff,ELAB_LOG_BUFFER_SIZE-1,fmt,args);
    va_end(args);
    _buff[count]=0;
    printf("%s",_buff);
    printf(NONE "\r\n");
}

