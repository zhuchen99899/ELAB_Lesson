#ifndef __RING_BUF_
#define __RING_BUF_
#include "../../elab/common/elab_def.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
    RINGBUFFER_EMPTY=0,
    RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RINGBUFFER_HALFFULL,
}ringbuffer_status_t;


    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     */
typedef struct{
    uint8_t *buffer;        // Buffer pointer
    uint16_t read_mirror:1; //page 0 or 1, used to indicate which page is being read
    uint16_t read_index:14;  // Read index, used to indicate the current read position in the buffer 
    uint16_t write_mirror:1; //page 0 or 1
    uint16_t write_index:14; // Write index

    uint16_t buffer_size;   // Size of the buffer
}ringbuf_t;


void ringbuffer_init(ringbuf_t *rb, uint8_t *pool, uint16_t size);
void ringbuffer_reset(ringbuf_t *rb);
uint16_t ringbuffer_put(ringbuf_t *rb,const uint8_t *data, uint16_t length);
uint16_t ringbuffer_put_force(ringbuf_t *rb,const uint8_t *data,uint16_t length);
uint16_t ringbuffer_putbyte(ringbuf_t *rb, const uint8_t data);
uint16_t ringbuffer_putbyte_force(ringbuf_t *rb, const uint8_t data);
uint16_t ringbuffer_get(ringbuf_t *rb ,uint8_t *ptr ,uint16_t length);
uint16_t ringbuffer_peek(ringbuf_t *rb, uint8_t **ptr);
uint16_t ringbuffer_getbyte(ringbuf_t *rb, uint8_t *byte);
uint16_t ringbuffer_data_len(ringbuf_t *rb);


/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))




#ifdef __cplusplus
}
#endif

#endif //__RING_BUF_



