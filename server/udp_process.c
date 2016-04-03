#include "ringbuff.h"
#include "public.h"
#include "spk.h"
#include "IM.h"
#include "fbstat.h"

#include <string.h>
#include <pthread.h>
#include <glib.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

extern GThreadPool *threadpool_udp_block;

extern GHashTable *user_to_gid_hash_list;

//extern struct ip * ip_header;
extern Ringbuff *cb_udp_a;
extern pthread_mutex_t *mutex_for_cb_udp_a;
extern pthread_cond_t *notFull_udp_a, *notEmpty_udp_a;

extern Ringbuff *cb_udp_b;
extern pthread_mutex_t *mutex_for_cb_udp_b;
extern pthread_cond_t *notFull_udp_b, *notEmpty_udp_b;

extern char* pooldata;

extern GHashTable *app_udp_hash_IPbuff_list;
//int process_http_flag=1;

void udp_process()
{
    Sinfo packet_tmp;
    UDPinfo udppkt_tmp;
    int send_pkt_flag;

    char *app_packet;
    //unsigned long len=0;

    char *app_name=NULL;

    send_pkt_flag=0;   //0表示不必发包ARP阻断，1表示要//

    pthread_mutex_lock(mutex_for_cb_udp_a);
    while(ringbuff_isempty(cb_udp_a))
    {
        //printf("Waiting for udp a\n");
        pthread_cond_wait(notEmpty_udp_a, mutex_for_cb_udp_a);
    }
    POP_RINGBUFF(cb_udp_a,Sinfo,packet_tmp);//packet temp//
    pthread_mutex_unlock(mutex_for_cb_udp_a);
    pthread_cond_signal(notFull_udp_a);

    spk_get_ethernet_info(&packet_tmp);
    spk_get_ip_info(&packet_tmp);

//    int i;
//    printf("mac ");
//    for(i=0; i<6; i++)
//    {
//        printf(" %x",packet_tmp.src_mac[i]);
//    }
//    printf("\n");

    udppkt_tmp.srcpkt=packet_tmp;
    udppkt_get_info(&udppkt_tmp);



//    printf("playlen %d\n",udppkt_tmp.udp_playlen);
    if(udppkt_tmp.udp_playlen<=0) return;

      //IP查找哈希表看是否为记录软件ip//
    //send_pkt_flag=appip_hash_look_up(app_udp_hash_IPbuff_list,(udppkt_tmp.srcpkt).server_ip,(udppkt_tmp.srcpkt).client_ip);
//        printf("send_pkt_flag!!!!!! %d\n",send_pkt_flag);

    //下面是无分类的特征字分析//
    // fun to look key words//
    app_packet=((udppkt_tmp.srcpkt).pksave+(udppkt_tmp.srcpkt).pklen-(udppkt_tmp.udp_playlen));

    if(send_pkt_flag==0)
    {
//        printf("!!!!!!!!\n");
        app_name=hash_udp_pattern_process(app_packet,udppkt_tmp.udp_playlen);
        if(app_name==NULL) return;

        if(app_name!=NULL)
        {
            printf("%s\n",app_name);
            g_hash_table_insert(app_udp_hash_IPbuff_list,g_strdup((udppkt_tmp.srcpkt).server_ip), g_strdup(app_name));
                //这里还是要查黑白名单的!!//
            send_pkt_flag=appip_tactics_look_up(app_name , (udppkt_tmp.srcpkt).client_ip);

        }

    }


    if(send_pkt_flag)
    {
        pthread_mutex_lock(mutex_for_cb_udp_b);
        if(ringbuff_isfull(cb_udp_b))
        {
            //printf(">>here will fork the new thread  for process RST Pkg>>>>>>>>>>>>\n>>>>>>>>\n>>>>>>>>>\n");//for test
            printf("---UDP_queue b is full now ,waiting for arp process.----\n");
            pthread_cond_wait(notFull_udp_b, mutex_for_cb_udp_b);
        }
        else
        {
            PUSH_RINGBUFF(cb_udp_b,UDPinfo,udppkt_tmp);/////////////////////write~~~~~~
        }
        pthread_mutex_unlock(mutex_for_cb_udp_b);
        pthread_cond_signal(notEmpty_udp_b);
        g_thread_pool_push(threadpool_udp_block, (gpointer)pooldata ,  NULL);
    }

//    }
    return;
}
