

#ifndef KFIFO_H_
#define KFIFO_H_


//声明 一个 结构体 kfifo
#include <stdint.h>
#include <stddef.h>

struct KFIFO
{
    unsigned char *buffer;    /* the buffer holding the data */
    uint32_t size;            /* the size of the allocated buffer */
    unsigned int in;                /* data is added at offset (in % size) */
    unsigned int out;                /* data is extracted from off. (out % size) */
        /*STM32 只有一个核心，同一时刻只能写或者读，因此不需要*/
        //    volatile unsigned int *lock; /* protects concurrent modifications */
};
typedef struct KFIFO KFIFO;


#ifdef KFIFO_DYNAMIC_ALLOCATE
// initialize a KFIFO struct using static buffer
// size: must be round power of 2 (16, 32, 64, 128, 256 ...)
struct KFIFO *kfifo_alloc(unsigned int size);
#endif
void kfifo_static_init(KFIFO* p, uint8_t* buf, size_t size);

unsigned int __kfifo_put(struct KFIFO *fifo, const unsigned char *buffer, unsigned int len);

unsigned int __kfifo_get(struct KFIFO *fifo, unsigned char *buffer, unsigned int len);

#endif
