/**
 * @file elab_def.h
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-17
 * 
 * 
 */

 
 #ifndef ELAB_DEF_H 
 #define ELAB_DEF_H 
 
 /* ==================== [Includes] ========================================== */
 
 
 
 #ifdef __cplusplus
 extern "C"{
 #endif
 
 /* ==================== [Defines] ========================================== */
 /**Complier */
#if defined(__CC_ARM) || defined(__CLANG_ARM)
    #include "stdarg.h"
    #define ELAB_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__) 
    #include "stdarg.h"
    #define ELAB_WEAK __weak
#elif defined(__GNUC__) 
    #include "stdarg.h"
    #define ELAB_WEAK __attribute__((weak))
#else
    #error "Complier not supported!"
#endif

 /* ==================== [Typedefs] ========================================== */
 
 /* ==================== [Global Prototypes] ================================== */
 
 /* ==================== [Macros] ============================================ */
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif	/* ELAB_DEF_H */
 
 