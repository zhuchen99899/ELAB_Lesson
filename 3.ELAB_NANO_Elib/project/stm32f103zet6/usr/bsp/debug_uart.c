/**
 * @file debug_uart.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-17
 * 
 * 
 */
/* ==================== [Includes] ========================================== */
#include "debug_uart.h"
#include "stdint.h"
#include "stdio.h"
#include "../../../elab/elib/elib_queue.h"
/* ==================== [Defines] ========================================== */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_PORT                      GPIOA
#define USARTx_IRQn                      USART1_IRQn

#define ELAB_DEBUG_UART_BUFFER_TX               (512)
#define ELAB_DEBUG_UART_BUFFER_RX               (16)
/* ==================== [Macros] ============================================ */

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ========================================== */
UART_HandleTypeDef huart1;
static elib_queue_t queue_rx;
static uint8_t buffer_rx[ELAB_DEBUG_UART_BUFFER_RX];
static elib_queue_t queue_tx;
static uint8_t buffer_tx[ELAB_DEBUG_UART_BUFFER_TX];
static uint8_t byte_recv;
/* ==================== [Static Variables] ========================================== */

/* ==================== [Static Functions] ================================== */
/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USARTx)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    USARTx_CLK_ENABLE();

    USARTx_GPIO_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = USARTx_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */

  }

}

/* private functions -------------------------------------------------------- */
/**
  * @brief  The weak UART tx callback function in HAL library.
  * @param  uartHandle  UART handle.
  * @retval None.
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    uint8_t byte = 0;
    
    if (UartHandle->Instance == USARTx)
    {
        elib_queue_pop(&queue_tx, 1);
        if (elib_queue_pull(&queue_tx, &byte, 1))
        {
            HAL_UART_Transmit_IT(&huart1, &byte, 1);
        }
    }
}

/**
  * @brief  The weak UART rx callback function in HAL library.
  * @param  uartHandle  UART handle.
  * @retval None.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    if (UartHandle->Instance == USARTx)
    {
        HAL_UART_Receive_IT(&huart1, &byte_recv, 1);
        elib_queue_push(&queue_rx, &byte_recv, 1);
    }
}

/**
  * @brief This function handles USART3 and USART4 interrupts.
  */
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}


/* ==================== [Public Functions] ================================== */



/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void elab_debug_uart_init(uint32_t baudrate)    
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USARTx;
  huart1.Init.BaudRate = baudrate;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
  elib_queue_init(&queue_rx, buffer_rx, ELAB_DEBUG_UART_BUFFER_RX);
  elib_queue_init(&queue_tx, buffer_tx, ELAB_DEBUG_UART_BUFFER_TX);
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */
}


/**
  * @brief  Send data to the debug uart.
  * @param  buffer  this pointer
  * @retval Free size.
  */
int16_t elab_debug_uart_send(void *buffer, uint16_t size)
{
    int16_t ret = 0;
    uint8_t byte = 0;

    HAL_NVIC_DisableIRQ(USARTx_IRQn);
    if (elib_queue_is_empty(&queue_tx))
    {
        ret = elib_queue_push(&queue_tx, buffer, size);
        if (elib_queue_pull(&queue_tx, &byte, 1) == 1)
        {
            HAL_UART_Transmit_IT(&huart1, &byte, 1);
        }
    }
    else
    {
        ret = elib_queue_push(&queue_tx, buffer, size);
    }
    HAL_NVIC_EnableIRQ(USARTx_IRQn);

    return ret;
}

/**
  * @brief  Initialize the elab debug uart.
  * @param  buffer  this pointer
  * @retval Free size.
  */
int16_t elab_debug_uart_receive(void *buffer, uint16_t size)
{
    int16_t ret = 0;

    HAL_NVIC_DisableIRQ(USARTx_IRQn);
    ret = elib_queue_pull_pop(&queue_rx, buffer, size);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);

    return ret;
}

/**
  * @brief  Clear buffer of the elab debug uart.
  * @param  buffer  this pointer
  * @retval Free size.
  */
void elab_debug_uart_buffer_clear(void)
{
    HAL_NVIC_DisableIRQ(USARTx_IRQn);

    elib_queue_clear(&queue_rx);
    elib_queue_clear(&queue_tx);

    HAL_NVIC_EnableIRQ(USARTx_IRQn);
}



//printf 重定向

#ifdef __ARMCC_VERSION //  ARM Compiler
    #pragma import __use_no_semihosting_swi
    void _sys_exit(int x)
    {
        (void)x;
    }
    struct __FILE { int handle; };
    FILE __stdout;

    int fputc(int ch, FILE *f)
    {
        (void)f;
        HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
        return ch;
    }
#elif defined(__GNUC__) // GCC Compiler
    int __io_putchar(int ch)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
        return ch;
    }

    int _write(int file, char *ptr, int len)
    {
        (void)file;
        for(int i=0;i<len;i++)
        {
           __io_putchar(*ptr++); 
        }
        return len;

    }
#endif
