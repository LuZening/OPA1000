

/**********************************************************
  *
  *　 文件名：    kfifo.c
  *
  *　 文件描述：    该文件包含的kfifo的处理函数
  *
  *　 创建人：       GXP
  *
  *   创建日期:     2016年8月9日16:13:06
  *
  *　 版本号：    1.0
  *
  *　 修改记录：    无
    *
    *   本文参考博客地址: http://blog.csdn.net/linyt/article/details/5764312
    *                     http://blog.csdn.net/chen19870707/article/details/39899743
  *
***********************************************************/


#include "kfifo.h"
#include <string.h>
#ifdef KFIFO_DYNAMIC_ALLOCATE
#include "FreeRTOS.h"
#include "task.h"
#endif

#define min(a, b)                (((a) < (b)) ? (a) : (b))






void kfifo_static_init(KFIFO* p, uint8_t* buf, size_t size)
{
	p->in = 0;
	p->out = 0;
	// assert size == 2^n where n is integer
	p->size = size;
	p->buffer = buf;
}

#ifdef KFIFO_DYNAMIC_ALLOCATE
/*
 * 每次调用这个函数都会产生 两个内存块，一个内存块指向struct KFIFO，一个指向 KFIFO.buff
 * 因此 如果这两个内存块不在使用请释放掉！GXP,2016年8月17日12:55:54
 */

struct KFIFO *kfifo_alloc(unsigned int size)
{
    unsigned char *buffer;

    struct KFIFO *ret;

        ret=(struct KFIFO *) pvPortMalloc(sizeof (struct KFIFO));

    /*
     * round up to the next power of 2, since our 'let the indices
     * wrap' tachnique works only in this case.
         * 如果size 是2的 次幂圆整，则 size & (size - 1)  =0
     */

    if (size & (size - 1))
        {
            //        BUG_ON(size > 0x80000000);

            //如果你要申请的buffer 不是 2的 次幂圆整，就要把 size 变成 2的次幂圆整 ，方便下面计算
        size = roundup_pow_of_two(size);
    }

        //这里使用 FreeRTOS的 分配内存的 API
    buffer = (unsigned char*) pvPortMalloc(size);

    if (!buffer)   //如果返回的值为NULL，这说明分配内存失败
        return 0UL;

        //    ret = kfifo_init(buffer, size, lock);

        ret->buffer=buffer;
        ret->size  =size;
        ret->in  = 0;
        ret->out = 0;

    if (!ret) //如果ret的值为NULL，这说明分配内存失败
        vPortFree(buffer); //释放之前分配的 内存空间

    return ret;

}
#endif


unsigned int __kfifo_put(struct KFIFO *fifo, const unsigned char *buffer, unsigned int len)
{
    unsigned int L;

        //环形缓冲区的剩余容量为fifo->size - fifo->in + fifo->out，让写入的长度取len和剩余容量中较小的，避免写越界；
    len = min( len , fifo->size - fifo->in + fifo->out );

    /*
     * Ensure that we sample the fifo->out index -before- we
     * start putting bytes into the kfifo.
     */
                //多处理器 处理内存 的 屏障，STM32不需要这个
                //    smp_mb();

    /* first put the data starting from fifo->in to buffer end */
                /* 首先将数据从fifo.in 所在的位置开始写，写之前，首先要看一下fifo->in到 buffer 末尾的大小 是不是 比 len 大*/

                /*
                 * 前面讲到fifo->size已经2的次幂圆整，主要是方便这里计算，提升效率
             * 在对10进行求余的时候，我们发现，余数总是整数中的个位上的数字，而不用管其他位是什么；
                 * 所以,kfifo->in % kfifo->size 可以转化为 kfifo->in & (kfifo->size – 1)，效率会提升
                 * 所以fifo->size - (fifo->in & (fifo->size - L)) 即位 fifo->in 到 buffer末尾所剩余的长度，
                 * L取len和剩余长度的最小值，即为需要拷贝L 字节到fifo->buffer + fifo->in的位置上。
                 */
    L = min(len, fifo->size - (fifo->in & (fifo->size - 1)));

    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, L);

    /* then put the rest (if any) at the beginning of the buffer */

    memcpy(fifo->buffer, buffer + L, len - L);

    /*
     * Ensure that we add the bytes to the kfifo -before-
     * we update the fifo->in index.
     */

      // smp_wmb();   //多处理器 处理内存 的 屏障，STM32不需要这个

            /*
             * 注意这里 只是用了 fifo->in +=  len而未取模，
             * 这就是kfifo的设计精妙之处，这里用到了unsigned int的溢出性质，
             * 当in 持续增加到溢出时又会被置为0，这样就节省了每次in向前增加都要取模的性能，
             * 锱铢必较，精益求精，让人不得不佩服。
             */

    fifo->in += len;

      /*返回值 代表  写入数据的个数 ，这样 就可以根据返回值 判断缓冲区是否写满*/
    return len;
}


unsigned int __kfifo_get(struct KFIFO *fifo, unsigned char *buffer, unsigned int len)
{
    unsigned int L;

    len = min(len, fifo->in - fifo->out);

    /*
     * Ensure that we sample the fifo->in index -before- we
     * start removing bytes from the kfifo.
     */

    //smp_rmb();    //多处理器 处理内存 的 屏障，STM32不需要这个

    /* first get the data from fifo->out until the end of the buffer */
    L = min(len, fifo->size - (fifo->out & (fifo->size - 1)));
    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), L);

    /* then get the rest (if any) from the beginning of the buffer */
    memcpy(buffer + L, fifo->buffer, len - L);

    /*
     * Ensure that we remove the bytes from the kfifo -before-
     * we update the fifo->out index.
     */

    //smp_mb();   //多处理器 处理内存 的 屏障，STM32不需要这个

	/*
	 * 注意这里 只是用了 fifo->out +=  len 也未取模运算，
	 * 同样unsigned int的溢出性质，当out 持续增加到溢出时又会被置为0，
	 * 如果in先溢出，出现 in  < out 的情况，那么 in – out 为负数（又将溢出），
	 * in – out 的值还是为buffer中数据的长度。
	 */

    fifo->out += len;

    return len;
}
