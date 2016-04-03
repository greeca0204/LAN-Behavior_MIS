#include "ringbuff.h"
#include <stdio.h>
//主要就是init ,pop, push 要写成宏定义函数



// free//
int free_ringbuff_queue(Ringbuff *queue)
{
    if(queue==NULL)
    {
        printf("No this_queue!\n");
        return 0;
    }
    free(queue->buff); /* OK if null */
    free(queue);
    queue=NULL;
    return 1;
}

//  empty  //
int ringbuff_isempty(Ringbuff *queue)
{
    if(queue==NULL)
    {
        printf("No this queue!\n");
        return 0;
    }
    if(queue->count==0)
    {
        return 1;
    }
    return 0;
}
//  full  //
int ringbuff_isfull(Ringbuff *queue)
{
    if(queue==NULL)
    {
        printf("No this queue!\n");
        return 0;
    }
    if(queue->count==queue->size-1)
    {
        return 1;
    }
    return 0;
}
