/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELIB_QUEUE_H
#define ELIB_QUEUE_H

/* includes ----------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* public typedef ----------------------------------------------------------- */
typedef struct elib_queue
{
    uint8_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t capacity;
    uint16_t size_free;
} elib_queue_t;

/* public functions --------------------------------------------------------- */
void elib_queue_init(elib_queue_t * const me, void *buffer, uint16_t capacity);
int32_t elib_queue_push(elib_queue_t * const me, void *buffer, uint16_t size);
uint16_t elib_queue_pull(elib_queue_t * const me, void *buffer, uint16_t size);
uint16_t elib_queue_pop(elib_queue_t * const me, uint16_t size);
void elib_queue_clear(elib_queue_t * const me);
uint16_t elib_queue_pull_pop(elib_queue_t * const me, void *buffer, uint16_t size);
uint16_t elib_queue_free_size(elib_queue_t * const me);
bool elib_queue_is_empty(elib_queue_t * const me);
bool elib_queue_is_full(elib_queue_t * const me);

#ifdef __cplusplus
}
#endif

#endif

/* ----------------------------- end of file -------------------------------- */
