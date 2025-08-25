/**
 * @file ringbuf.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-31
 * 
 * @copyright Copyright (c) 2025
 * @note 修改自rtthread ringbuff 在在单生产/单消费的模式 中可以实现无锁即两个线程只单独操作读和写
 */
#include "ringbuf.h"

#include "../../elab/common/elab_assert.h"

ELAB_TAG("ringbuf");
static inline ringbuffer_status_t get_ringbuffer_status(ringbuf_t *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RINGBUFFER_EMPTY;
        else
            return RINGBUFFER_FULL;
    }
    return RINGBUFFER_HALFFULL;
}


/**
 * @brief  获取ringbuf长度
 * 
 * @param rb 
 * @return uint16_t buffersize
 */
uint16_t ringbuffer_data_len(ringbuf_t *rb)
{
    switch (get_ringbuffer_status(rb)) // 获取 ringbuff 的状态
    {
    case RINGBUFFER_EMPTY:
        return 0; // 空就返回 0
    case RINGBUFFER_FULL:
        return rb->buffer_size; // 满就返回缓冲区的大小
    case RINGBUFFER_HALFFULL:// 半满
    default:
        if (rb->write_index > rb->read_index) // 如果在同一镜像
            return rb->write_index - rb->read_index;// 返回下标差
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);// 如果不在同一镜像，通过计算获取数据的长度
    };
}





/**
 * @brief ringbuffer 初始化函数
 * 
 * @param rb ringbuffer
 * @param pool buffer poll
 * @param size  buffer size
 */
void ringbuffer_init(ringbuf_t *rb, uint8_t *pool, uint16_t size)
{

/*initialize the read and write index */
rb->write_mirror=rb->read_index=0;
rb->read_mirror=rb->write_index=0;

/*set buffer poll and size */
rb->buffer = pool;
rb->buffer_size =ELAB_SIZE_ALIGN_DOWN(size,4);//保证大小为按字节对齐


}

/**
 * @brief write data to ringbuffer 
 * 
 * @param rb ringbuffer
 * @param data the data to put in ringbuffer
 * @param length  the data length
 * @return uint16_t return the length of data put in ringbuffer
 */
uint16_t ringbuffer_put(ringbuf_t *rb,const uint8_t *data, uint16_t length)
{


    uint16_t size = 0;
    /* whether has enough space */
    size = ringbuffer_space_len(rb);// 获取 ring_buff 中可用空间的大小
    /* if has no space  */
    if (size == 0)
    {
        return 0; // 没有空间就返回 0
    }

    /*drop some data */
    if(size < length){
        length = size; // 如果缓存区的控件不够保存这一次数据， 则把能够写入的这一部分数据写进去

    }

    /*enough space don't need next mirror */
    if(rb->buffer_size - rb->write_index >length){
        // 这里判断的是数据能够在第一个镜像写完，就直接写入当前的镜像，可以理解为在白纸上写数据，这个时候不需要翻页
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer[rb->write_index], data, length);

        /** this should not cause overflow because there is enough space for
        * length of data in current mirror */
        rb->write_index += length; // 更新写入下标
        return length;
        
    }

    /*need next mirror */
    //跨页写入
    memcpy(&rb->buffer[rb->write_index],// 把能够写入第一个镜像的数据线写入第一个镜像的缓存区
           &data[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer[0],// 前一个镜像写满了，就要从头写了，这里就体现了循环
           &data[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;// 完成 镜像 0 和 1 的翻转
    rb->write_index = length - (rb->buffer_size - rb->write_index);// 重新设置写数据的小标

    return length;
}

/**
 * @brief force write data to ringbuffer 
 * 
 * @param rb ringbuffer
 * @param data the data to put in ringbuffer
 * @param length  the data length
 * @return uint16_t return the length of data put in ringbuffer
 */

uint16_t ringbuffer_put_force(ringbuf_t *rb,
                            const uint8_t     *data,
                            uint16_t           length)
{

    uint16_t space_length;

    space_length = ringbuffer_space_len(rb); // 获取可用缓存区大小

    if (length > rb->buffer_size) // 如果长度超过了当前 ring_buff 的最大值
    {
        data = &data[length - rb->buffer_size];// 获取超出部分的地址
        length = rb->buffer_size;// lenght 设置为 ring_buff 的大小
    }

    if (rb->buffer_size - rb->write_index > length)// 如果足够存储这次将要写入的数据
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer[rb->write_index], data, length);// 拷贝数据
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;// 移动 write index

        if (length > space_length) // 如果长度大于可用缓存区大小
            rb->read_index = rb->write_index;// 因为数据已经被覆盖掉了，所以 read 的 index 必须往前移动

        return length; // 返回写入数据的长度
    }

    memcpy(&rb->buffer[rb->write_index],// 拷贝数据在当前镜像
           &data[0],
           rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer[0],// 拷贝数据到下一个镜像
           &data[rb->buffer_size - rb->write_index],
           length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;// 镜像翻转
    rb->write_index = length - (rb->buffer_size - rb->write_index); // 写完数据后移动 write index

    if (length > space_length) // 如果长度大于可用空间
    {
        rb->read_mirror = ~rb->read_mirror; // 翻转读镜像
        rb->read_index = rb->write_index; // 移动 read_index 到 write_index 
    }


    return length;

}

/**
 * @brief get data from ringbuffer
 * 
 * @param rb ring buffewr
 * @param ptr get data
 * @param length want to get the data length
 * @return uint16_t get the data length
 */
uint16_t ringbuffer_get(ringbuf_t *rb,
                        uint8_t *ptr,
                        uint16_t length)
{
    uint16_t size;

    /*whether has enough data */
    size= ringbuffer_data_len(rb);

    /*no data*/
    if(size == 0) return 0; //如果没有数据则直接返回
    

    /*less data */
    if (size < length) length =size;// 如果已经存在的数据小于想要获取的长度，那么就会把长度设置为 size 

    if (rb->buffer_size - rb->read_index > length)// 如果在一个镜像中就能获取到所有的数据
    {
        /* copy all of data */
        memcpy(ptr, &rb->buffer[rb->read_index], length);// 拷贝数据
        /**  this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length; // 移动读下标
        return length;// 返回读取到的长度
    }
    /* 能执行到这里说明，一个镜像内无法读取到所有的数据 */
    memcpy(&ptr[0],
           &rb->buffer[rb->read_index],
           rb->buffer_size - rb->read_index);// 拷贝当前镜像的数据
    memcpy(&ptr[rb->buffer_size - rb->read_index],
           &rb->buffer[0],
           length - (rb->buffer_size - rb->read_index));// 拷贝剩余的数据

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;// 翻转镜像
    rb->read_index = length - (rb->buffer_size - rb->read_index);// 移动读下标

    return length;
}


/**
 * @brief write a byte to ringbuffer
 * 
 * @param rb ringbuffer
 * @param data  the byte
 * @return uint16_t  success is 1 and fail is 0
 */
uint16_t ringbuffer_putbyte(ringbuf_t *rb, const uint8_t data) { 
    /*whether has enough space */
    if(!ringbuffer_space_len(rb)) return 0; // 没有空间就返回 0


    rb->buffer[rb->write_index] = data;// 拷贝数据

    /*flip mirror */
    if(rb->write_index == rb->buffer_size-1) // 如果在同一镜像
    {
        rb->write_mirror = ~rb->write_mirror;// 翻转镜像
        rb->write_index = 0;// 移动写下标
    }
    else
    {
        rb->write_index++;// 移动写下标

    }
    return 1;//写入一个字节
}


/**
 * @brief force write a byte to ringbuffer
 * 
 * @param rb ringbuffer
 * @param byte write byte
 * @return uint16_t  only 1 because force
 */
uint16_t ringbuffer_putbyte_force(ringbuf_t *rb, const uint8_t byte)
{
    ringbuffer_status_t old_state;

    old_state = get_ringbuffer_status(rb);// 获取状态

    rb->buffer[rb->write_index] = byte;// 写入数据

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1) // 检查当前镜像是不是满了
    {
        rb->write_mirror = ~rb->write_mirror; // 翻转写镜像
        rb->write_index = 0;// 翻转之后设置下标为 0
        if (old_state == RINGBUFFER_FULL) // 如果 ringbuff 的状态是满
        {
            rb->read_mirror = ~rb->read_mirror; // 翻转读镜像
            rb->read_index = rb->write_index; // 设置读下标和写下标一致
        }
    }
    else
    {
        rb->write_index++; // 写下标加1
        if (old_state == RINGBUFFER_FULL)
            rb->read_index = rb->write_index;// 如果满，设置读下标等于写下标
    }

    return 1; // 写入一个字符，返回1
}

/**
 * @brief read byte from ringbuffer
 * 
 * @param rb ringbuffer
 * @param byte read byte pointer
 * @return uint16_t 1 for success and 0 for fail
 */
uint16_t ringbuffer_getbyte(ringbuf_t *rb, uint8_t *byte)
{
    /* ringbuffer is empty */
    if (!ringbuffer_data_len(rb)) // 检查 ringbuff 是否为空
        return 0;

    /* put character */
    *byte = rb->buffer[rb->read_index];// 获取当前读下标的数据

    if (rb->read_index == rb->buffer_size-1)// 如果当前镜像满了
    {
        rb->read_mirror = ~rb->read_mirror;// 翻转镜像
        rb->read_index = 0; // 设置读数据的下标为0
    }
    else
    {
        rb->read_index++; // 下标加1
    }

    return 1;// 读取一个字节，返回1
}

/**
 * @brief Get the first readable byte of the ring buffer.
 *
 * @param rb        A pointer to the ringbuffer.
 * @param ptr       When this function return, *ptr is a pointer to the first readable byte of the ring buffer.
 *
 * @note It is recommended to read only one byte, otherwise it may cause buffer overflow.
 *
 * @return Return the size of the ring buffer.
 */
uint16_t ringbuffer_peek(ringbuf_t *rb, uint8_t **ptr)
{
    uint16_t size;

    assert(rb != NULL);

    *ptr = NULL;

    /* whether has enough data  */
    size = ringbuffer_data_len(rb);
    /* no data */
    if (size == 0)
        return 0;
    *ptr = &rb->buffer[rb->read_index];
    
    if ((uint8_t)(rb->buffer_size - rb->read_index) > size)
    {
        // rb->read_index += size;
        return size;
    }

    size = rb->buffer_size - rb->read_index;

    /* we are going into the other side of the mirror */
    // rb->read_mirror = ~rb->read_mirror;
    // rb->read_index = 0;

    return size;
}

void ringbuffer_reset(ringbuf_t *rb)
{
    rb->read_mirror = 0;
    rb->read_index = 0;
    rb->write_mirror = 0;
    rb->write_index = 0;
} // 所有的值都设置为 0

/**
 * @brief dynamic create ringbuffer
 * 
 * @param size 
 * @return ringbuf_t* 
 */
ringbuf_t* ringbuffer_create(uint16_t size)
{
    ringbuf_t *rb;
    uint8_t *pool;

    size = ELAB_SIZE_ALIGN_DOWN(size, 4);// 大小做字节对齐

    rb = (ringbuf_t *)malloc(sizeof(ringbuf_t));// 申请内存
    if (rb == NULL)
        goto exit;

    pool = (uint8_t *)malloc(size);// 申请数据缓冲区内存
    if (pool == NULL)
    {
        free(rb);
        rb = NULL;
        goto exit;
    }
    ringbuffer_init(rb, pool, size);// 初始化 ringff

exit:
    return rb;
}
