#include "http.h"
#include "config.h"
#include "public.h"
#include <pthread.h>
#include <pcap/pcap.h>
#include <string.h>
#include <net/bpf.h>
#include <string.h>

extern char NET_INTERFACE[5];
extern int pcap;
extern pthread_mutex_t *mutex_for_bpf;
extern pthread_cond_t *notUse_bpf;// *Use_bpf

extern Ringbuff  *cb_http_a;  //ringbuff a   用于保存  捕获到（a）  数据包
extern pthread_mutex_t *mutex_for_cb_http_a; // 用于同步  http包捕获（a）线程 与http解析线程
extern pthread_cond_t *notFull_http_a, *notEmpty_http_a;// 用于同步  http包捕获（a）线程 与http解析线程

extern GThreadPool *threadpool_http_process;
extern char pooldata[15];

void http_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) //callback funtion to process packet
{
    if(pkthdr->len<80) return ;  //3次握手的包，里面没有HTTP的信息，直接放弃

    Sinfo  my_packet;
    memset(&my_packet,0,sizeof(Sinfo));
    //char pooldata[20]="http_process";

    pthread_mutex_lock(mutex_for_cb_http_a);
    if(ringbuff_isfull(cb_http_a))
    {
        /*new a thread is not being here*/
//        printf(">>>>>>>>>>here will fork the new thread to process the  packet>>>>>>>>>>>>\n>>>>>>>>>>>>\n>>>>>>>>\n>>>>>>>>>\n");//for test
        printf("---http_queue a is full now ,waiting for http_process process.----\n");
        pthread_cond_wait(notFull_http_a, mutex_for_cb_http_a);
    }
    else
    {
        // printf("len   is    $$$$$$$$$$$    %d\n",pkthdr->len);
        memmove(my_packet.pksave,packet,(size_t )pkthdr->len);
        my_packet.pklen=pkthdr->len;
        //printf(" %s\n",my_packet.http);
        PUSH_RINGBUFF(cb_http_a,Sinfo,my_packet);/////////////////////write~~~~~~
    }
    pthread_cond_signal(notEmpty_http_a);
    pthread_mutex_unlock(mutex_for_cb_http_a);


    g_thread_pool_push(threadpool_http_process, (gpointer)pooldata ,  NULL);
    //printf("http packet  counter: %d \n",*counter);
//    return;
}


void  http_pcap()
{
    int count =0;
    pcap_t *descr = NULL;
    //const char *device="em0";

    struct bpf_program fpk;
    memset(&fpk,0,sizeof(struct bpf_program));

    char errbuf[50];
    memset(errbuf,0,50);
    descr = pcap_open_live(NET_INTERFACE,MAXBYTES2CAPTURE,1,10,errbuf);

    pthread_mutex_lock(mutex_for_bpf);
    while(pcap==1)
    {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
        pthread_cond_wait(notUse_bpf, mutex_for_bpf);

    }
//    else
//    {
        pcap=1;
        if(public_fun_bpf_config(descr,"port 80 and (greater 64)",fpk,0)==-1)//tcp port 80
        {
            printf("bpf config failed!\n");
            exit(0);
        }
        else printf("http BPF configure success! \n");
        pcap=0;
//    }
    pthread_mutex_unlock(mutex_for_bpf);
    pthread_cond_signal(notUse_bpf);



    pcap_loop(descr,-1,http_putin_buff,(u_char *)&count);//参数2，多少个包后就停止捕获
    pcap_close(descr);
//    while(1)
//    {
//        sleep(5000);
//    }
}
