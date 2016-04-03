#ifndef RINGBUFF_H_INCLUDED
#define RINGBUFF_H_INCLUDED
#include <sys/types.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
//spk struct //
typedef struct sinfo
{
    //u_int8_t  raw_packet[64];
    char pksave[1600];
    int pklen;  //包总长
//    int playlen; //负载长度
    struct timeval cap_time;

    int packet_id;
    //BEHAVIOR_TYPE behavior_type;
    u_char dst_mac[6];
    u_char src_mac[6];

    char src_ip[17];
    char dst_ip[17];

    u_char ip_type;
    u_short ether_type;

    char server_ip[17];
    char client_ip[17];

    u_char server_mac[6];
    u_char client_mac[6];


} Sinfo;            //改了新名字，不喜欢别喷//

typedef struct tcpInfo
{
    struct sinfo srcpkt;
    //TCP 特有//
    int tcp_playlen; //负载长度
    u_char	th_flags;
    unsigned long int win;
    unsigned long int seq;
    unsigned long int ack;

    u_short src_port;
    u_short dst_port;
} TCPinfo;              //TCP info//
typedef struct udpInfo
{
    struct sinfo srcpkt;
    int udp_playlen;
    u_short	uh_ulen;		/* udp length */
    u_short	uh_sum;			/* udp checksum */
    u_short src_port;
    u_short dst_port;

} UDPinfo;            //UDP info //

//----------//
//Ringbuff//
typedef  void*   rfpointer;
typedef struct ring_queue
{
    int start;
    int end;
    int size;
    int count;
    rfpointer *buff;
} Ringbuff;

//funtion//
int free_ringbuff_queue(Ringbuff *queue);

int ringbuff_isempty(Ringbuff *queue);

int ringbuff_isfull(Ringbuff *queue);

#define   INIT_RINGBUFF(rb,BUFFTYPE,SIZE )        \
               rb=(Ringbuff *)malloc(sizeof(Ringbuff))  \
               ,rb->start=0     \
               ,rb->end=0       \
               ,rb->count=0     \
               ,rb->size=SIZE\
               ,rb->buff=( rfpointer )malloc(sizeof(BUFFTYPE)*SIZE)

#define   PUSH_RINGBUFF(rb,BUFFTYPE,REAL)                                \
               if(rb==NULL)                                                                  \
           {                                                           \
                    printf(#rb" do not exist!\n");                                     \
                    exit(0);                                                                       \
            }                                                           \
                if(rb->count==rb->size-1)                                             \
           {                                                            \
                    printf(#rb" buff has been full.\n");                           \
            }                                                           \
                *((BUFFTYPE*)rb->buff+rb->end)=REAL;                     \
                rb->end=(rb->end+1)%rb->size;                               \
                rb->count=rb->count+1



#define POP_RINGBUFF(rb,BUFFTYPE,REAL)                               \
             if(rb==NULL)                                                               \
         {                                                            \
                    printf(#rb" do not exist!\n");                                     \
                    exit(0);                                                                    \
         }                                                           \
             if(rb->count>0)                                                              \
         {                                                           \
                    REAL=*((BUFFTYPE*)rb->buff+rb->start);              \
                    rb->count=rb->count-1;                                         \
                    rb->start=(rb->start+1)%rb->size;                           \
          }                                                            \
             else                                                                                  \
             printf("No element in "#rb"\n")




#endif // RINGBUFF_H_INCLUDED
