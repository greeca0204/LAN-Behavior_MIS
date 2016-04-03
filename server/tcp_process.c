#include "ringbuff.h"
#include "spk.h"
#include "IM.h"
#include "public.h"
#include "fbstat.h"

#include <pthread.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
//#include <netinet/udp.h>
//此文件的函数将声明在spk.h//
//tcp_a is used for sending to tcp deep process//
extern Ringbuff  *cb_tcp_a;
extern pthread_mutex_t *mutex_for_cb_tcp_a;
extern pthread_cond_t *notFull_tcp_a, *notEmpty_tcp_a;

//tcp_rst  is used for blocking //
extern Ringbuff  *cb_tcp_rst;
extern pthread_mutex_t *mutex_for_cb_rst;
extern pthread_cond_t *notFull_tcpcb_rst, *notEmpty_tcpcb_rst;

extern GThreadPool *threadpool_tcp_rst;
extern char* pooldata;

extern GHashTable *app_tcp_hash_IPbuff_list;

//int deepprocess_tcp_flag=1;
void tcp_deepprocess()
{
    struct tcpInfo packet_tmp;
    int send_pkt_flag;

    char *app_packet;
//    unsigned long len=0;

    char *app_name=NULL;
//    char *ip_local=NULL;
//    char server_ip[16];
//    char client_ip[16];

//    while(deepprocess_tcp_flag)
//    {
    send_pkt_flag=0;   //0表示不必发包阻断，1表示要//

    pthread_mutex_lock(mutex_for_cb_tcp_a);
    while(ringbuff_isempty(cb_tcp_a))
    {
        //printf("Waiting for tcp a\n");
        pthread_cond_wait(notEmpty_tcp_a, mutex_for_cb_tcp_a);
    }
    POP_RINGBUFF(cb_tcp_a,TCPinfo,packet_tmp);//packet temp//
    pthread_mutex_unlock(mutex_for_cb_tcp_a);
    pthread_cond_signal(notFull_tcp_a);

//    ip_local=strstr(packet_tmp.srcpkt.src_ip,"192.168");//之后要改为从数据库拿值（监听网段）
//    if(ip_local!=NULL)
//    {
//        //dst is server
//        strcpy(client_ip,packet_tmp.srcpkt.src_ip);
//        strcpy(server_ip,packet_tmp.srcpkt.dst_ip);
//    }
//    else
//    {
//        //src is server ip. dst is client.
//        strcpy(client_ip,packet_tmp.srcpkt.dst_ip);
//        strcpy(server_ip,packet_tmp.srcpkt.src_ip);
//
//    }

    //  IP查找哈希表看是否为记录软件ip//
    send_pkt_flag=appip_hash_look_up(app_tcp_hash_IPbuff_list,(packet_tmp.srcpkt).server_ip , (packet_tmp.srcpkt).client_ip);
//        printf("send_pkt_flag!!!!!! %d\n",send_pkt_flag);
    //深度分析函数//
//        printf("size: %d\n",sizeof(packet_tmp));
//       printf("et : %d\n ip : %d\n tcp : %d\n",sizeof(struct ether_header),sizeof(struct ip),sizeof(struct tcphdr));



    //接下来是根据端口分类以后再进行的深度分析//
    //源端口要不要处理，暂时不管先了。//
//        if(1)
//        send_pkt_flag=tcp_prot_decete(app,packet_tmp.dst_port);

    //下面是无分类的特征字分析//
    // fun to look key words//
    app_packet=((packet_tmp.srcpkt).pksave+(packet_tmp.srcpkt).pklen-(packet_tmp.tcp_playlen));
//    len=((packet_tmp.srcpkt).pklen-sizeof(struct ether_header)
//         - sizeof(struct ip) - sizeof(struct tcphdr));
//         printf("len2 %d\n",len);
// len-->tcp_playlen

    if(send_pkt_flag==0)
    {
        app_name=hash_tcp_pattern_process(app_packet,packet_tmp.tcp_playlen);
        if(app_name==NULL) return;

        if(app_name!=NULL)
        {
            //printf("*********** %s\n",packet_tmp.srcpkt.src_ip);
//            ip_local=strstr(packet_tmp.srcpkt.src_ip,"192.168");//之后要改为从数据库拿值（监听网段）
//            if(ip_local!=NULL)
//            {
                //src is serever ip.
                //printf("send packet service ip -- %s\n",packet_tmp.srcpkt.dst_ip);
                //插入新ip及名词进hash表//
                g_hash_table_insert(app_tcp_hash_IPbuff_list,g_strdup((packet_tmp.srcpkt).server_ip), g_strdup(app_name));
                //这里还是要查黑白名单的!!//
                send_pkt_flag=appip_tactics_look_up(app_name , (packet_tmp.srcpkt).client_ip);
//            }
//            else
//            {
//                //dst is server ip.
//                //printf("!!!receive packet sever ip --%s\n",packet_tmp.srcpkt.src_ip);
//                g_hash_table_insert(app_hash_IPbuff_list,g_strdup(packet_tmp.srcpkt.src_ip), g_strdup(app_name));
//                send_pkt_flag=appip_tactics_look_up(app_name,packet_tmp.srcpkt.dst_ip);
//            }
            //          printf("send_pkt_flag: %d\n",send_pkt_flag);
            /* //测试输出哈希表app_hash_IPbuff_list
             void iterator(gpointer key, gpointer value, gpointer user_data)
             {
                 g_print(user_data, key, value);
             }

             g_hash_table_foreach(app_hash_IPbuff_list,(GHFunc)iterator,"The hash of %s is %s \n");
             */
        }

    }


    if(send_pkt_flag)
    {
        pthread_mutex_lock(mutex_for_cb_rst);
        if(ringbuff_isfull(cb_tcp_rst))
        {
            //printf(">>here will fork the new thread  for process RST Pkg>>>>>>>>>>>>\n>>>>>>>>\n>>>>>>>>>\n");//for test
            printf("---TCP_queue b is full now ,waiting for RST process.----\n");
            pthread_cond_wait(notFull_tcpcb_rst, mutex_for_cb_rst);
        }
        else
        {
            PUSH_RINGBUFF(cb_tcp_rst,TCPinfo,packet_tmp);/////////////////////write~~~~~~
        }
        pthread_mutex_unlock(mutex_for_cb_rst);
        pthread_cond_signal(notEmpty_tcpcb_rst);
        g_thread_pool_push(threadpool_tcp_rst, (gpointer)pooldata ,  NULL);
    }

//    }
    return;
}
