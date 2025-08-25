/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_ASSERT_H
#define ELAB_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "elab_std.h"
#include "elab_log.h"

/* Exported functions --------------------------------------------------------*/
void elab_assert_func(void);

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  The legacy assert function.
  * @param  test_   The given condition.
  */
#define elab_assert(test_)                  do                                 \
    {                                                                          \
        if (!(test_))                                                          \
            _assert(#test_, 0, TAG, __LINE__);                                 \
    } while (0)
    
#define assert(test_)                       elab_assert(test_)

/**
 * @brief  Assert function with ID.
 * @param  test_   The given condition.
 * @param  id_     The given id.
 */
#define assert_id(test_, id_)               do                                 \
    {                                                                          \
        if (!(test_))                                                          \
            _assert(#test_, id_, NULL, __LINE__);                              \
    } while (0)

#define elab_assert_id                      assert_id

/**
 * @brief  Assert function with name.
 * @param  test_   The given condition.
 * @param  name_   The given name.
 */
#define assert_name(test_, name_)           do                                 \
    {                                                                          \
        if (!(test_))                                                          \
            _assert(name_, 0, TAG, __LINE__);                                  \
    } while (0)

#define elab_assert_name                    assert_name

/**
 * @brief  Assert function for checking the given pointer is not NULL.
 * @param  ptr_    The given data pointer.
 */
#define assert_not_null(ptr_)               assert_name((ptr_ != NULL), #ptr_)

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  The internal assert function when assert fails.
 * @param  str_     The given string information, such as object name.
 * @param  id_      The given uint32_t information, such as object ID.
 * @param  tag      The file name setted by ELAG_TAG macro.
 * @param  location The assert location in file.
 */
void _assert(const char *str_, uint32_t id_, const char *tag, uint32_t location);

#ifdef __cplusplus
}
#endif

#endif

/* ----------------------------- end of file -------------------------------- */
