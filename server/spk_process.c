#include "public.h"
#include "ringbuff.h"

#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <libnet.h>
extern Ringbuff  *cb_pk_a;  //ringbuff a   用于保存  捕获到（a）  数据包//
extern pthread_mutex_t *mutex_for_cb_pk_a; // 用于同步  包捕获（a）线程 与浅解析线程//
extern pthread_cond_t *notFull_pk_a, *notEmpty_pk_a;// 用于同步  包捕获（a）线程 与浅解析线程//

//send to tcp deep process//
extern Ringbuff  *cb_tcp_a;
extern pthread_mutex_t *mutex_for_cb_tcp_a;
extern pthread_cond_t *notFull_tcp_a, *notEmpty_tcp_a;

//send to udp deep process//
extern Ringbuff  *cb_udp_a;
extern pthread_mutex_t *mutex_for_cb_udp_a;
extern pthread_cond_t *notFull_udp_a, *notEmpty_udp_a;

extern GThreadPool *threadpool_tcp_deepprocess;
extern char* pooldata;

//int process_spk_flag=1;
void spk_process()
{
    Sinfo packet_tmp;
    TCPinfo tcppkt_tmp;
//
//    while(process_spk_flag)
//    {
        pthread_mutex_lock(mutex_for_cb_pk_a);
        while(ringbuff_isempty(cb_pk_a))
        {
            pthread_cond_wait(notEmpty_pk_a, mutex_for_cb_pk_a);
        }
        POP_RINGBUFF(cb_pk_a,Sinfo,packet_tmp);//POP//
        pthread_mutex_unlock(mutex_for_cb_pk_a);
        pthread_cond_signal(notFull_pk_a);

        spk_get_ethernet_info(&packet_tmp);  //拿到以太网的信息//

        if(packet_tmp.ether_type==0x0800)             //IP协议  == 0x0800//
        {
            spk_get_ip_info(&packet_tmp);//拿到IP的信息//

            if(packet_tmp.ip_type==6)  //TCP//
            {
                //printf("decete TCP \n");
                tcppkt_tmp.srcpkt=packet_tmp;   //先把Sinfo保存起来//
                tcppkt_get_info(&tcppkt_tmp);  //get tcp info//

                if(tcppkt_tmp.th_flags==0x14||tcppkt_tmp.th_flags==0x04)
                {
                    printf("Don't process RST!\n");
                    return;
                }                 //RST包 不处理 //

//                if(tcppkt_tmp.tcp_playlen<10)
//                {
//                    printf("playlen= %d\n",tcppkt_tmp.tcp_playlen);
//                }
                //printf("client ip : %s\n",packet_tmp.client_ip);

                //信号量处理,就是写入tcp_a //
                pthread_mutex_lock(mutex_for_cb_tcp_a);
                if(ringbuff_isfull(cb_tcp_a))
                {
                    //printf("Full cb_tcp_a!");
                    pthread_cond_wait(notFull_tcp_a,mutex_for_cb_tcp_a);
                }
                else
                {
                    PUSH_RINGBUFF(cb_tcp_a,TCPinfo,tcppkt_tmp);
                }
                pthread_mutex_unlock(mutex_for_cb_tcp_a);
                pthread_cond_signal(notEmpty_tcp_a);
                g_thread_pool_push(threadpool_tcp_deepprocess, (gpointer)pooldata ,  NULL);
            }

//            else ;
        }

//    }
    return;
}

