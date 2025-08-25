/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_CRC_H_
#define ELAB_CRC_H_

/* includes ----------------------------------------------------------------- */
#include <stdint.h>
#include "../common/elab_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* public functions --------------------------------------------------------- */
/**
  * @brief  Private function calculating CRC32 value of one memory.
  * @param  crc   The former CRC32 value.
  * @param  data  Data buffer to be calculated.
  * @param  size  Size of the buffer.
  * @retval CRC32 value.
  */
uint32_t elib_crc32(uint32_t crc, void *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif

/* ----------------------------- end of file -------------------------------- */
