#include "spk.h"

#include <pthread.h>
#include <libnet.h>

extern Ringbuff * cb_udp_b;
extern pthread_mutex_t * mutex_for_cb_udp_b;
extern pthread_cond_t *notFull_udp_b, *notEmpty_udp_b;

void udp_block()
{
    struct udpInfo packet_tmp;

    pthread_mutex_lock(mutex_for_cb_udp_b);
    while(ringbuff_isempty(cb_udp_b))
    {
        pthread_cond_wait(notEmpty_udp_b, mutex_for_cb_udp_b);
    }
    POP_RINGBUFF(cb_udp_b,UDPinfo,packet_tmp);//POP//
    pthread_mutex_unlock(mutex_for_cb_udp_b);
    pthread_cond_signal(notFull_udp_b);

    arp_spoofind((packet_tmp.srcpkt).client_ip ,(packet_tmp.srcpkt).client_mac);

}


