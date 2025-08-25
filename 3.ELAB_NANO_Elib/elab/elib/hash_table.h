/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* includes ----------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "../common/elab_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/* public typedef ----------------------------------------------------------- */
#define HASH_SEEK_TIMES_MAX                (32)

/* public typedef ----------------------------------------------------------- */
typedef struct hash_table_data
{
    uint32_t hash_time33;
    uint32_t hash_elf;
    uint32_t hash_bkdr;
    void *data;
} hash_table_data_t;

typedef struct hash_table
{
    uint32_t capacity;
    uint32_t prime_max;
    hash_table_data_t *table;
} hash_table_t;

/* public functions --------------------------------------------------------- */
hash_table_t *hash_table_new(uint32_t capacity);
void hash_table_destroy(hash_table_t * const me);
void hash_table_init(hash_table_t * const me,
                            hash_table_data_t *table,
                            uint32_t capacity);
elab_err_t hash_table_add(hash_table_t * const me, char *name, void *data);
elab_err_t hash_table_remove(hash_table_t * const me, char *name);
void *hash_table_get(hash_table_t * const me, char *name);
bool hash_table_existent(hash_table_t * const me, char *name);
int32_t hash_table_index(hash_table_t * const me, char *name);

#ifdef __cplusplus
}
#endif

#endif  /* HASH_TABLE_H */

/* ----------------------------- end of file -------------------------------- */
