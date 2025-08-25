/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

/* includes ----------------------------------------------------------------- */
#include "elib_queue.h"
#include "../common/elab_def.h"

/* public functions --------------------------------------------------------- */
/**
  * @brief  Queue initialization.
  * @param  me          this pointer
  * @param  buffer      Queue's buffer memory.
  * @param  capacity    Queue's capacity.
  * @retval None
  */
void elib_queue_init(elib_queue_t * const me, void *buffer, uint16_t capacity)
{
    me->buffer = (uint8_t *)buffer;
    me->capacity = capacity;
    me->size_free = capacity;
    me->head = 0;
    me->tail = 0;
}

/**
  * @brief  Push data into queue.
  * @param  me      this pointer
  * @param  buffer  data buffer memory.
  * @param  size    data's size.
  * @retval if > 0, the data size; if < 0, error id.
  */
int32_t elib_queue_push(elib_queue_t * const me, void *buffer, uint16_t size)
{
    int32_t ret = (int32_t)size;

    if (size > me->size_free)
    {
        ret = ELAB_ERR_NOT_ENOUGH;
    }
    else
    {
        for (uint16_t i = 0; i < size; i ++)
        {
            me->buffer[me->head] = ((uint8_t *)buffer)[i];
            me->head = (me->head + 1) % me->capacity;
            me->size_free --;
        }
    }

    return ret;
}

/**
  * @brief  Pull data from queue, but the data is not poped.
  * @param  me      this pointer
  * @param  buffer  data buffer memory.
  * @param  size    data's size.
  * @retval Actual pulled data size.
  */
uint16_t elib_queue_pull(elib_queue_t * const me, void *buffer, uint16_t size)
{
    uint16_t size_data = me->capacity - me->size_free;
    uint16_t size_pull = size > size_data ? size_data : size;

    for (uint16_t i = 0; i < size_pull; i ++)
    {
        ((uint8_t *)buffer)[i] = me->buffer[(me->tail + i) % me->capacity];
    }

    return size_pull;
}

/**
  * @brief  Pop data from queue.
  * @param  me      this pointer
  * @param  size    data's size.
  * @retval Actual poped data size.
  */
uint16_t elib_queue_pop(elib_queue_t * const me, uint16_t size)
{
    uint16_t size_data = me->capacity - me->size_free;
    uint16_t size_pop = size > size_data ? size_data : size;

    me->tail = (me->tail + size_pop) % me->capacity;
    me->size_free += size_pop;

    return size_pop;
}

/**
  * @brief  Pull and pop data from queue, and the data is poped.
  * @param  me      this pointer
  * @param  buffer  data buffer memory.
  * @param  size    data's size.
  * @retval Actual pulled & poped data size.
  */
uint16_t elib_queue_pull_pop(elib_queue_t * const me, void *buffer, uint16_t size)
{
    uint16_t size_data = me->capacity - me->size_free;
    uint16_t size_pop = size > size_data ? size_data : size;

    for (uint16_t i = 0; i < size_pop; i ++)
    {
        ((uint8_t *)buffer)[i] = me->buffer[(me->tail + i) % me->capacity];
    }
    me->tail = (me->tail + size_pop) % me->capacity;
    me->size_free += size_pop;

    return size_pop;
}

/**
  * @brief  Clear all the data of the queue.
  * @param  me          this pointer
  * @retval Free size.
  */
void elib_queue_clear(elib_queue_t * const me)
{
    me->size_free = me->capacity;
    me->head = 0;
    me->tail = 0;
}

/**
  * @brief  Get the free size of the queue.
  * @param  me          this pointer
  * @retval Free size.
  */
uint16_t elib_queue_free_size(elib_queue_t * const me)
{
    return me->size_free;
}

/**
  * @brief  Check the queue is empty or not.
  * @param  me          this pointer
  * @retval Empty or not.
  */
bool elib_queue_is_empty(elib_queue_t * const me)
{
    return me->size_free == me->capacity ? true : false;
}

/**
  * @brief  Check the queue is full or not.
  * @param  me          this pointer
  * @retval Full or not.
  */
bool elib_queue_is_full(elib_queue_t * const me)
{
    return me->size_free == 0 ? true : false;
}

/* ----------------------------- end of file -------------------------------- */
