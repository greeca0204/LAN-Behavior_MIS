#include "public.h"
#include "ringbuff.h"
#include "spk.h"
#include <pthread.h>
#include <libnet.h>

//extern Ringbuff *cb_http_b;
//extern pthread_mutex_t *mutex_for_cb_http_b;
//extern pthread_cond_t *notFull_http_b, *notEmpty_http_b;

extern Ringbuff *cb_tcp_rst;
extern pthread_mutex_t *mutex_for_cb_rst;
extern pthread_cond_t *notFull_tcpcb_rst, *notEmpty_tcpcb_rst;



//TCP RST thread//

//int tcp_rst_flag =1;

void tcp_rst_tcp()
{
    struct tcpInfo packet_tmp;
    memset(&packet_tmp,0,sizeof(struct tcpInfo));

//    while(tcp_rst_flag)
//    {
    pthread_mutex_lock(mutex_for_cb_rst);
    while(ringbuff_isempty(cb_tcp_rst))
    {
        pthread_cond_wait(notEmpty_tcpcb_rst, mutex_for_cb_rst);
    }
    POP_RINGBUFF(cb_tcp_rst,TCPinfo,packet_tmp);//POP//
    pthread_mutex_unlock(mutex_for_cb_rst);
    pthread_cond_signal(notFull_tcpcb_rst);

    rst_tcp123(&(packet_tmp.srcpkt),packet_tmp.src_port,
               packet_tmp.dst_port,packet_tmp.seq,packet_tmp.ack,
               packet_tmp.win , packet_tmp.tcp_playlen);


//    }
}

int rst_tcp123(struct sinfo *my_packet,u_short src_port,
               u_short dst_port,unsigned long int seq,unsigned long int ack,
               unsigned long int win ,int playlen )
{
    int c;
    libnet_t *l;
    libnet_ptag_t t;
    u_long src_ip, dst_ip;
    char errbuf[LIBNET_ERRBUF_SIZE];
    //memset(errbuf,0,LIBNET_ERRBUF_SIZE);

//<html><body><p>The webpage you access has been banned.Please contact the administrator if necessary !</p></body></html>
    char * payload="The webpage you access has been banned.Please contact the administrator if necessary !";
    u_short payload_s;
    payload_s=strlen(payload);

    src_ip = libnet_name2addr4(l,my_packet->client_ip,LIBNET_DONT_RESOLVE);
    dst_ip = libnet_name2addr4(l,my_packet->server_ip,LIBNET_DONT_RESOLVE);


    l = libnet_init(LIBNET_LINK,"em0",errbuf);

    //send to src
//    l = libnet_init(LIBNET_LINK,"em0",errbuf);
    t = libnet_build_tcp(
            dst_port,
            src_port,
            ack,
            0,
            0x14,
            0,
            0,
            0,
            LIBNET_TCP_H,
            NULL,
            0,
            l,
            0);//ack after seq?0?

    t = libnet_build_ipv4(
            LIBNET_IPV4_H+LIBNET_TCP_H+0,
            0,
            242,
            0,
            64,
            IPPROTO_TCP,
            0,
            dst_ip,
            src_ip,
            NULL,
            0,
            l,
            0);

    t = libnet_build_ethernet(
            my_packet->src_mac,
            my_packet->dst_mac,
            ETHERTYPE_IP,
            NULL,
            0,
            l,
            0);

    c = libnet_write(l);



    //send to dst
    t = libnet_build_tcp(src_port,dst_port,(seq+playlen),0,TH_RST,win,0,0,LIBNET_TCP_H,NULL,0,l,0);
//    t = libnet_build_tcp(src_port,dst_port,seq,htonl(1),0x14,htons(4500) + rand()%1000,0,0,LIBNET_TCP_H,NULL,0,l,0);
    t = libnet_build_ipv4(LIBNET_IPV4_H+LIBNET_TCP_H+0,0,242,0,64,IPPROTO_TCP,0,src_ip,dst_ip,NULL,0,l,0);
    t = libnet_build_ethernet(my_packet->dst_mac,my_packet->src_mac,ETHERTYPE_IP,NULL,0,l,0);
    c = libnet_write(l);
    //printf("sent rcp %s   >>>>>>>>>>>   %s\n",my_packet->src_ip,my_packet->dst_ip);

    printf("packets RST sent: \n");



    //TCP send packet to src for abandon
    if(src_port==80||dst_port==80)
    {
        t = libnet_build_tcp(dst_port,src_port,ack,
                             (seq+playlen),TH_PUSH|TH_ACK|TH_FIN,
                             win,0,0,LIBNET_TCP_H+payload_s,
                             (u_char*)payload,payload_s,l,0);//ack after seq?0?
        t = libnet_build_ipv4(LIBNET_IPV4_H+LIBNET_TCP_H+payload_s,0,242,0,64,IPPROTO_TCP,0,dst_ip,src_ip,NULL,0,l,0);
        t = libnet_build_ethernet(my_packet->src_mac,my_packet->dst_mac,ETHERTYPE_IP,NULL,0,l,0);
        c = libnet_write(l);
    }


    libnet_destroy(l);
    return 0;


}
