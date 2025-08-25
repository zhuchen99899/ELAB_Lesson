
#ifndef DEBUG_UART_H 
#define DEBUG_UART_H 

/* ==================== [Includes] ========================================== */
#include "stm32f1xx_hal.h"


#ifdef __cplusplus
extern "C"{
#endif

/* ==================== [Defines] ========================================== */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Global Prototypes] ================================== */
void elab_debug_uart_init(uint32_t baudrate);
/* ==================== [Macros] ============================================ */

#ifdef __cplusplus
}
#endif

#endif	/* DEBUG_UART_H */

