#include "http.h"
#include "ringbuff.h"
#include "public.h"
#include "http_hash_func.h"
#include "fbstat.h"

#include <string.h>
#include <pthread.h>
#include <glib.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>


extern GHashTable *http_hash_white_list;
extern GHashTable *http_hash_person_list;
extern GHashTable *http_hash_group_list;
extern GHashTable *http_hash_whole_list;
extern GHashTable *user_to_gid_hash_list;

//extern struct ip * ip_header;
extern Ringbuff *cb_http_a;
extern pthread_mutex_t *mutex_for_cb_http_a;
extern pthread_cond_t *notFull_http_a, *notEmpty_http_a;

extern Ringbuff *cb_tcp_rst;
extern pthread_mutex_t *mutex_for_cb_rst;
extern pthread_cond_t *notFull_tcpcb_rst, *notEmpty_tcpcb_rst;

extern GThreadPool *threadpool_tcp_rst;
extern char pooldata[15];


//int process_http_flag=1;

void process_http()
{

    struct sinfo  spkt_tmp;
    memset(&spkt_tmp,0,sizeof( struct sinfo));
    struct tcpInfo tcppkt_tmp;
    memset(&tcppkt_tmp,0,sizeof( struct tcpInfo));
    int start_len=0;
    int send_rst_flag=0;
    char *p=NULL,*p2=NULL,*gg=NULL;
    char http_referer[50];
    char http_host[50];
    //char pooldata[15]="http_rst_tcp";


//    while(process_http_flag)
//    {
        pthread_mutex_lock(mutex_for_cb_http_a);
        while(ringbuff_isempty(cb_http_a))
        {
            pthread_cond_wait(notEmpty_http_a, mutex_for_cb_http_a);
        }
        POP_RINGBUFF(cb_http_a,Sinfo,spkt_tmp);//packet temp~~~//

        pthread_mutex_unlock(mutex_for_cb_http_a);
        pthread_cond_signal(notFull_http_a);

//        return;
        spk_get_ethernet_info(&spkt_tmp);
        spk_get_ip_info(&spkt_tmp);

        tcppkt_tmp.srcpkt=spkt_tmp;
        tcppkt_get_info(&tcppkt_tmp);
        //printf("IP--DST--%s---SRC--%s\n",spkt_tmp.dst_ip,spkt_tmp.src_ip);


        start_len=(int)(spkt_tmp.pklen-tcppkt_tmp.tcp_playlen);  //tcp_playlen 这个正确赋值了???//
//        printf("start_len %d\n",start_len);
        char *http =NULL;
        http=(spkt_tmp.pksave+start_len);

        if(http) //需要吗？？？？？？？？？？？？？？？？？？？//
        {

//            send_rst_flag=0;     //这个就用来作为host 和 referer 二选一的标记 //

            //确认GET包
            gg=strstr(http,"GET");
            if(gg==NULL)
            {
                //printf("NOT GETS\n");
                return;
//                return;
            }

            //printf("playlen!!!!!!! %d\n",tcppkt_tmp.tcp_playlen);

            //查找referer//
            p=http_get_info(http,"Referer",16);
            if(p!=NULL)
            {
                memset(http_referer,0,50);
                strncpy(http_referer,p,sizeof(http_referer));
                //printf("Referer %s\n",p);
                //没写的统计函数//
                if(http_hash_look_up(http_referer,&spkt_tmp))
                {
                    send_rst_flag=1;  //signal//
                }
            }

            //查找host//
            p2=http_get_info(http,"Host",6);
            if(!(send_rst_flag))          //已经有了信号就不用再查询了//
            {
                if(p2!=NULL)
                {
                    memset(http_host,0,50);
                    strncpy(http_host,p2,sizeof(http_host));
                    //没写的统计函数//
//                    printf("Host: %s\n",p2);
                    //hash look up func
                    if(http_hash_look_up(http_host,&spkt_tmp))
                    {
                        send_rst_flag=1;  //signal//
                    }
                }
            }

            //以后面对其他应用使用80端口的情况，也可以通过令send_rst_flag=1即可//

            if(send_rst_flag)
            {
//                tcppkt_tmp.srcpkt=spkt_tmp;
//                tcppkt_get_info(&tcppkt_tmp);
                pthread_mutex_lock(mutex_for_cb_rst);
                if(ringbuff_isfull(cb_tcp_rst))
                {
                    printf("---http_queue b is full now ,waiting for RST process.----\n");
                    pthread_cond_wait(notFull_tcpcb_rst, mutex_for_cb_rst);
                }
                else
                {
                    PUSH_RINGBUFF(cb_tcp_rst,TCPinfo,tcppkt_tmp);/////////////////////write~~~~~~
                }
                pthread_mutex_unlock(mutex_for_cb_rst);
                pthread_cond_signal(notEmpty_tcpcb_rst);
                //thread process//
                g_thread_pool_push(threadpool_tcp_rst, (gpointer)pooldata , NULL);
            }
        }
//    }
}






