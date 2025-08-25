/**
 * @file elab_common.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-17
 * 
 * 
 */
/* ==================== [Includes] ========================================== */
#include "elab_common.h"
/* ==================== [Defines] ========================================== */

/* ==================== [Macros] ============================================ */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ========================================== */

/* ==================== [Static Variables] ========================================== */

/* ==================== [Static Functions] ================================== */

/* ==================== [Public Functions] ================================== */


ELAB_WEAK uint32_t elab_time_ms(void)
{
#if !defined(__linux__) //win32

    #if(ELAB_RTOS_CMSIS_OS_EN!= 0)
    //osKernelGetTickCount();
    #else
    return 0;
    #endif
#else //linux
    //return osKernelGetTickCount();
#endif

}




#if !defined(__linux__) && !defined(_WIN32)
ELAB_WEAK
#endif
void *elab_malloc(uint32_t size)
{
    return malloc(size);
}

#if !defined(__linux__) && !defined(_WIN32)
ELAB_WEAK
#endif
void elab_free(void *memory)
{
    if (memory != NULL)
    {
        free(memory);
    }
}


