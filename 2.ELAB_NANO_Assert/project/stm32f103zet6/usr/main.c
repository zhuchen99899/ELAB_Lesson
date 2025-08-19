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
#include "../../../elab/common/elab_assert.h"

ELAB_TAG("main");


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
 BSP_Init();

  while (1)
  {
  HAL_Delay(2000); 

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
