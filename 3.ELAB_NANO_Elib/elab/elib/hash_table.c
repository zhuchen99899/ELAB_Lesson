/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

/* Includes ----------------------------------------------------------------- */
#include <stdlib.h>
#include "hash_table.h"
#include "../common/elab_assert.h"
#include "../common/elab_common.h"

ELAB_TAG("HashTable");

/* Private function prototypes ---------------------------------------------- */
static uint32_t _get_prime_max(uint32_t capacity);
static uint32_t _hash_time33(char *str);
static uint32_t _hash_bkdr(char *str);
static uint32_t _hash_elf(char *str);

/* Exported functions ------------------------------------------------------- */
/**
  * @brief  Newly create one hash table in the given capacity.
  * @param  size    The given capacity size.
  * @retval The hash table handle.
  */
hash_table_t *hash_table_new(uint32_t capacity)
{
    hash_table_t *me = NULL;
    hash_table_data_t *table = NULL;

    me = elab_malloc(sizeof(hash_table_t));
    elab_assert(me != NULL);

    table = elab_malloc(sizeof(hash_table_data_t) * capacity);
    elab_assert(table != NULL);

    hash_table_init(me, table, capacity);

    return me;
}

/**
  * @brief  Destroy the hash table which is generate by the funciton hash_table_new.
  *         All the related resources are freed in this function.
  * @param  me  The hash table handle.
  * @retval None.
  */
void hash_table_destroy(hash_table_t * const me)
{
    elab_free(me->table);
    me->table = NULL;
    elab_free(me);
}

/**
  * @brief  Initialize one hash table in the static mode.
  * @param  me          The hash table handle.
  * @param  data        The hash table data.
  * @param  capacity    The hash table capacity.
  * @retval None.
  */
void hash_table_init(hash_table_t * const me,
                        hash_table_data_t *data,
                        uint32_t capacity)
{
    me->table = data;
    me->capacity = capacity;

    for (uint32_t i = 0; i < capacity; i ++)
    {
        me->table[i].hash_time33 = UINT32_MAX;
        me->table[i].hash_elf = UINT32_MAX;
        me->table[i].hash_bkdr = UINT32_MAX;
        me->table[i].data = NULL;
    }

    me->prime_max = _get_prime_max(capacity);
}

/**
  * @brief  Add one data block into hash table by the given name .
  * @param  me          The hash table handle.
  * @param  name        The given key name.
  * @param  data        The data block.
  * @retval See elab_err_t.
  */
elab_err_t hash_table_add(hash_table_t * const me, char *name, void *data)
{
    elab_err_t ret = ELAB_ERR_FULL;

    uint32_t hash_time33 = _hash_time33(name);
    uint32_t hash_elf = _hash_elf(name);
    uint32_t hash_bkdr = _hash_bkdr(name);

    uint32_t index_start = hash_time33 % me->prime_max;
    uint32_t index;
    uint32_t times_count = 0;
    for (uint32_t i = 0; i < me->capacity; i ++)
    {
        index = (index_start + i) % me->capacity;
        if (me->table[index].data == NULL)
        {
            me->table[index].data = data;
            me->table[index].hash_time33 = hash_time33;
            me->table[index].hash_elf = hash_elf;
            me->table[index].hash_bkdr = hash_bkdr;
            ret = ELAB_OK;
            break;
        }
        else
        {
            times_count ++;
            if (times_count > HASH_SEEK_TIMES_MAX)
            {
                break;
            }
        }
    }

    return ret;
}

/**
  * @brief  Remove one data block from hash table by the given name .
  * @param  me          The hash table handle.
  * @param  name        The given key name.
  * @retval See elab_err_t.
  */
elab_err_t hash_table_remove(hash_table_t * const me, char *name)
{
    int32_t ret = hash_table_index(me, name);

    if (ret != ELAB_ERROR)
    {
        me->table[ret].data = NULL;
        return ELAB_OK;
    }

    return ELAB_ERROR;
}

/**
  * @brief  Get the data block from hash table by the given name .
  * @param  me          The hash table handle.
  * @param  name        The given key name.
  * @retval The data block pointer.
  */
void *hash_table_get(hash_table_t * const me, char *name)
{
    int32_t ret = hash_table_index(me, name);

    return ret == ELAB_ERROR ? NULL : me->table[ret].data;
}

/**
  * @brief  Check the data block is existent or not in hash table by the given
  *         name .
  * @param  me          The hash table handle.
  * @param  name        The given key name.
  * @retval True or false.
  */
bool hash_table_existent(hash_table_t * const me, char *name)
{
    return hash_table_index(me, name) == ELAB_ERROR ? false : true;
}

/**
  * @brief  Get the data block's index from hash table by the given name.
  * @param  me          The hash table handle.
  * @param  name        The given key name.
  * @retval The data block pointer.
  */
int32_t hash_table_index(hash_table_t * const me, char *name)
{
    int32_t ret = ELAB_ERROR;
    uint32_t hash_time33 = _hash_time33(name);
    uint32_t hash_elf = _hash_elf(name);
    uint32_t hash_bkdr = _hash_bkdr(name);

    uint32_t index_start = hash_time33 % me->prime_max;
    uint32_t index;
    uint32_t times_count = 0;
    for (uint32_t i = 0; i < me->capacity; i ++)
    {
        index = (index_start + i) % me->capacity;
        if (me->table[index].data == NULL)
        {
            continue;
        }
        else 
        if (me->table[index].hash_time33 == hash_time33 &&
                    me->table[index].hash_elf == hash_elf &&
                    me->table[index].hash_bkdr == hash_bkdr)
        {
            ret = index;
            break;
        }
        else
        {
            times_count ++;
            if (times_count > HASH_SEEK_TIMES_MAX)
            {
                break;
            }
        }
    }

    return ret;
}

/* Private functions -------------------------------------------------------- */
/**
  * @brief  Get the maximum prime number in the specific range.
  * @param  capacity  The given range size.
  * @retval The maximum prime number.
  */
static uint32_t _get_prime_max(uint32_t capacity)
{
    uint32_t prime_max = 0;

    for (int64_t i = capacity; i > 0; i --)
    {
        bool is_prime = true;
        for (uint32_t j = 2; j < capacity; j++)
        {
            if (i <= j)
            {
                break;
            }
            if ((i % j) == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            prime_max = i;
            break;
        }
    }

    return prime_max;
}

/**
  * @brief  Calculate the time33 hash value of one string.
  * @param  str  The input string.
  * @retval The time33 hash value.
  */
static uint32_t _hash_time33(char *str)
{  
    uint32_t hash = 5381;

    while (*str)
    {
        /* hash *= 33 */
        hash = (*str++) + (hash << 5) + hash;
    }

    return hash;
}

/**
  * @brief  Calculate the ELF hash value of one string.
  * @param  str  The input string.
  * @retval The ELF hash value.
  */
uint32_t _hash_elf(char *str)
{
    uint32_t hash = 0;
    uint32_t x	= 0;
 
    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }
 
    return (hash & 0x7FFFFFFF);
}

/**
  * @brief  Calculate the BKDR hash value of one string.
  * @param  str  The input string.
  * @retval The BKDR hash value.
  */
uint32_t _hash_bkdr(char *str)
{
    uint32_t seed = 131; /* 31 131 1313 13131 131313 etc.. */
    uint32_t hash = 0;
 
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
 
    return (hash & 0x7FFFFFFF);
}

/* ----------------------------- end of file -------------------------------- */
