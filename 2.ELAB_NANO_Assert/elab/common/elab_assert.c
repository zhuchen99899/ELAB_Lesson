/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

/* include ------------------------------------------------------------------ */
#include "elab_def.h"
#include "elab_assert.h"
#include <stdio.h>
#include "inttypes.h"
#ifdef __linux__
#include "assert.h"
#endif
#if (ELAB_RTOS_CMSIS_OS_EN != 0)
#include "../os/cmsis_os.h"
#endif


/* public function ---------------------------------------------------------- */
/**
  * @brief  eLab assert weak function. Users can defined the function in their 
  *         own way.
  * @retval None
  */
ELAB_WEAK void elab_assert_func(void)
{
#ifdef __linux__
    assert(0);
#else
    while (1)
    {
    }
#endif
}

/**
 * @brief  The internal assert function when assert fails.
 * @param  str_     The given string information, such as object name.
 * @param  id_      The given uint32_t information, such as object ID.
 * @param  tag      The file name setted by ELAG_TAG macro.
 * @param  location The assert location in file.
 */
void _assert(const char *str_, uint32_t id_, const char *tag, uint32_t location)
{
    printf("\033[1;31m" "Assert failure!\r\n");
    printf("Location: %s %"PRIu32".\r\n", tag, location);
    if (str_ != NULL)
        printf("Assert info: %s.\r\n", str_);
    else
        printf("Assert info: %"PRIu32".\r\n", id_);
    elab_assert_func();
}

/* ----------------------------- end of file -------------------------------- */
