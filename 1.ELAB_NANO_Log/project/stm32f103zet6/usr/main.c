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
#include "../../../elab/common/elab_log.h"
#include "../../../elab/3rd/xfusion/xf_utils.h"
ELAB_TAG("main");

uint8_t data[]={0xaa,0xbc,0xff,0x12,0xef};
uint8_t string[]="hello world";
uint8_t string2[]="hello world\r\n";
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  BSP_Init();
  while (1)
  {
    /* USER CODE END WHILE */

    elog_debug("debug hello world");
    elog_info("info hello world");
    elog_warn("warn hello world");
    elog_error("error hello world");


    XF_LOG_BUFFER_HEX(data,sizeof(data));
    XF_LOG_BUFFER_HEXDUMP(string,sizeof(string));
    XF_LOG_BUFFER_HEXDUMP_ESCAPE(string2,sizeof(string2));
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
