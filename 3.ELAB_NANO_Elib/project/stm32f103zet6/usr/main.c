/**
 * @file main.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-14
 * 
 * 
 */
#include "bsp.h"
#include "../../../elab/elib/elib_queue.h"
#include <stdio.h>
elib_queue_t queue;
uint8_t queue_buffer[10];


uint8_t serial_buffer[4]={0xA0, 0xC0, 0xFF, 0xB2};
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  uint8_t get_buffer1;
  uint8_t get_buffer2;
  uint8_t get_buffer3;
  uint8_t get_buffer4;
 BSP_Init();

elib_queue_init (&queue, queue_buffer, sizeof(queue_buffer));
    /**模拟串口读取 */
  elib_queue_push(&queue, &serial_buffer[0], 1);
  elib_queue_push(&queue, &serial_buffer[1], 1);
  elib_queue_push(&queue, &serial_buffer[2], 1);
  elib_queue_push(&queue, &serial_buffer[3], 1);
  
  elib_queue_pull_pop(&queue, &get_buffer1, 1);
  elib_queue_pull_pop(&queue, &get_buffer2, 1);
  elib_queue_pull_pop(&queue, &get_buffer3, 1);
  elib_queue_pull_pop(&queue, &get_buffer4, 1);
  while (1)
  {

  printf("Get data: 0x%02X\r\n", get_buffer1);
  printf("Get data: 0x%02X\r\n", get_buffer2);
  printf("Get data: 0x%02X\r\n", get_buffer3);
  printf("Get data: 0x%02X\r\n", get_buffer4);
  HAL_Delay(1000); 

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
