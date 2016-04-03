#include "spk.h"
#include "list.h"
#include "ip_fragment.h"
#include "tcp_cbna.h"
#include "config.h"
#include "public.h"
#include <unistd.h>
#include <string.h>
#include <net/bpf.h>
#include <pcap/pcap.h>
#include <pthread.h>

extern char NET_INTERFACE[5];
extern int pcap;
extern pthread_mutex_t *mutex_for_bpf;
extern pthread_cond_t *notUse_bpf;// *Use_bpf

extern Ringbuff *cb_pk_a;
extern pthread_mutex_t *mutex_for_cb_pk_a; // 用于同步  包捕获（a）线程 与解析线程//
extern pthread_cond_t *notFull_pk_a, *notEmpty_pk_a;// 用于同步  包捕获（a）线程 与解析线程//

extern Ringbuff *cb_udp_a;
extern pthread_mutex_t *mutex_for_cb_udp_a;
extern pthread_cond_t *notFull_udp_a, *notEmpty_udp_a;

extern Ringbuff *cb_spk_aduit_a;
extern pthread_mutex_t *mutex_for_cb_spk_audit_a; // 用于同步  包捕获（a）线程 与审计线程//
extern pthread_cond_t *notFull_spk_audit_a, *notEmpty_spk_audit_a;// 用于同步  包捕获（a）线程 与审计线程//

extern GThreadPool *threadpool_spk_process;
extern GThreadPool *threadpool_udp_process;
extern char* pooldata;

void spk_pcap()
{
    //printf("spk_pcap\n");
    int count =0;
    pcap_t *descr2 = NULL;

    struct bpf_program fpk2;

    char errbuf[1024];
    memset(errbuf,0,1024);
    descr2 = pcap_open_live(NET_INTERFACE,MAXBYTES2CAPTURE,1,10,errbuf);

    pthread_mutex_lock(mutex_for_bpf);
    while(pcap==1)
    {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
        pthread_cond_wait(notUse_bpf, mutex_for_bpf);

    }
//    else
//    {
    pcap=1;
    if(public_fun_bpf_config(descr2,"tcp and (not port 80) and (greater 64) and (tcp[13]!=0x14) and (tcp[13]!=0x04)",fpk2,0)==-1)// and (not port 80) or (ip[6:2] & 0x1fff != 0)
    {
        printf("bpf config failed!\n");
        sleep(2);
        exit(0);
    }
    else printf("normal BPF configure success! \n");
    pcap=0;
//    }
    pthread_mutex_unlock(mutex_for_bpf);
    pthread_cond_signal(notUse_bpf);


    pcap_loop(descr2,-1,spk_putin_buff,(u_char *)&count);//参数2，多少个包后就停止捕获//

//    while(1){sleep(5000);}

    pcap_close(descr2);
}


//UDP抓包
void udp_pcap()
{
    int count =0;
    pcap_t *descr3 = NULL;

    struct bpf_program fpk3;

    char errbuf[1024];
    memset(errbuf,0,1024);
    descr3 = pcap_open_live(NET_INTERFACE,MAXBYTES2CAPTURE,1,10,errbuf);

    pthread_mutex_lock(mutex_for_bpf);
    while(pcap==1)
    {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
        pthread_cond_wait(notUse_bpf, mutex_for_bpf);

    }
//    else
//    {
    pcap=1;

    if(public_fun_bpf_config(descr3,"udp and (greater 42)",fpk3,0)==-1)// or (ip[6:2] & 0x1fff != 0)
    {
        printf("bpf config failed!\n");
        sleep(2);
        exit(0);
    }
    else printf("udp BPF configure success! \n");
    pcap=0;
//    }
    pthread_mutex_unlock(mutex_for_bpf);
    pthread_cond_signal(notUse_bpf);

    pcap_loop(descr3,-1,udp_putin_buff,(u_char *)&count);//参数2，多少个包后就停止捕获//

//    while(1){sleep(5000);}

    pcap_close(descr3);
}


//AUDIT抓包
void audit_pcap()
{
    int count =0;
    pcap_t *descr4 = NULL;

    struct bpf_program fpk4;

    //这里开始TCP审计的初始化吧//
    tcp_exit();
    ip_frag_init(256);
    tcp_cbna_init(2048);
    nids_register_tcp(http_test_callback);
    nids_register_tcp(ftp_callback);
    nids_register_tcp(ftp_data_callback);
    initList();
    //--------------------------------------//
    char errbuf[1024];
    memset(errbuf,0,1024);
    descr4 = pcap_open_live(NET_INTERFACE,MAXBYTES2CAPTURE,1,10,errbuf);


    pthread_mutex_lock(mutex_for_bpf);
    while(pcap==1)
    {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
        pthread_cond_wait(notUse_bpf, mutex_for_bpf);

    }
//    else
//    {
    pcap=1;
    if(public_fun_bpf_config(descr4,"ip and (tcp[13]!=0x14) and (tcp[13]!=0x04)",fpk4,0)==-1)// or (ip[6:2] & 0x1fff != 0)
    {
        printf("bpf config failed!\n");
        sleep(2);
        exit(0);
    }
    else printf("audit BPF configure success! \n");
    pcap=0;
//    }
    pthread_mutex_unlock(mutex_for_bpf);
    pthread_cond_signal(notUse_bpf);
//    printf("audit pcap configure success! \n");

    pcap_loop(descr4,-1,audit_putin_buff,(u_char *)&count);//参数2，多少个包后就停止捕获//

//    while(1){sleep(5000);}

    //这里写TCP审计需要释放的结构体//
    tcp_exit();
    desylist();
    //--------------------------------------------//
    pcap_close(descr4);
}



void spk_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) //callback funtion to process packet//
{
    struct sinfo  my_packet;

    //给tcp分析和拦截用//
    if(pkthdr->len>=80)//
    {
        //et14 ip20 tcp20.
        pthread_mutex_lock(mutex_for_cb_pk_a);
        if(ringbuff_isfull(cb_pk_a))
        {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
            pthread_cond_wait(notFull_pk_a, mutex_for_cb_pk_a);
        }
        else
        {
            // printf("len   is    $$$$$$$$$$$    %d\n",pkthdr->len);
            memmove(my_packet.pksave,packet,(size_t )pkthdr->len);
            my_packet.pklen=pkthdr->len;
            //my_packet.playlen=pkthdr->len-54;
//            if(pkthdr->len-54<10)
//            printf("len %d\n",pkthdr->len-54);
            PUSH_RINGBUFF(cb_pk_a,Sinfo,my_packet);//write~~~~~~//
        }
        pthread_mutex_unlock(mutex_for_cb_pk_a);
        pthread_cond_signal(notEmpty_pk_a);
        g_thread_pool_push(threadpool_spk_process, (gpointer)pooldata ,  NULL);
        //printf("http packet  counter: %d \n",*counter);

    }

    return;
}



void udp_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) //callback funtion to process packet//
{
    struct sinfo  my_packet;

    //udp分析和拦截用
    if(pkthdr->len>=80)
    {
        //et14 ip20 tcp20.
        pthread_mutex_lock(mutex_for_cb_udp_a);
        if(ringbuff_isfull(cb_udp_a))
        {
//        printf(">>>>>>>>>>here will fork shouldthe new thread to process the  spk packet>>>>>\n");//for test
            pthread_cond_wait(notFull_udp_a, mutex_for_cb_udp_a);
        }
        else
        {
            // printf("len   is    $$$$$$$$$$$    %d\n",pkthdr->len);
            memmove(my_packet.pksave,packet,(size_t )pkthdr->len);
            my_packet.pklen=pkthdr->len;
            //printf("no \n len %d\n",pkthdr->len-42);
            //my_packet.playlen=pkthdr->len-42;
            //printf(" %s\n",my_packet.http);
            PUSH_RINGBUFF(cb_udp_a,Sinfo,my_packet);//write~~~~~~//
        }
        pthread_mutex_unlock(mutex_for_cb_udp_a);
        pthread_cond_signal(notEmpty_udp_a);
        g_thread_pool_push(threadpool_udp_process, (gpointer)pooldata ,  NULL);
        //printf("http packet  counter: %d \n",*counter);

    }


    return;
}


//目的是将超时的TCP连接释放，暂时先部署在这，如果异步处理对其有影响，到时再改//
extern struct tcp_timeout *nids_tcp_timeouts;
extern struct pcap_pkthdr * nids_last_pcap_header;


void audit_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) //callback funtion to process packet//
{
    struct sinfo  my_packet;


//    //检查 TCP流超时//
    if (NULL != nids_tcp_timeouts)
        tcp_check_timeouts((struct timeval *)(&pkthdr->ts));

    nids_last_pcap_header =(struct pcap_pkthdr*) pkthdr;
    //--------------------//


    //debug//  //测试结果说明 ringbuff 出了点问题//
//    struct ip *iph = (struct ip *)(packet + sizeof(struct ether_header));
//    if(iph->ip_p==6)  //TCP//
//    {
//
//        process_tcp_cbna((u_char *)iph,pkthdr->len - sizeof(struct ether_header));
//    }
    //---------//

    //给UDP审计用放入和TCP审计同一个ringbuff?cb_spk_aduit_a?????//
    pthread_mutex_lock(mutex_for_cb_spk_audit_a);
    if(ringbuff_isfull(cb_spk_aduit_a))
    {
        pthread_cond_wait(notFull_spk_audit_a, mutex_for_cb_spk_audit_a);
    }
    else
    {
        // printf("len   is    $$$$$$$$$$$    %d\n",pkthdr->len);
        //memset(my_packet.pksave,0,1600);
        memmove(my_packet.pksave,packet,(size_t )pkthdr->len);
        my_packet.pklen=(int )pkthdr->len;
        //my_packet.playlen=pkthdr->len-54;       //不一定是54吧//
        //printf(" %s\n",my_packet.http);
        PUSH_RINGBUFF(cb_spk_aduit_a,Sinfo,my_packet);//write~~~~~~//
    }
    pthread_mutex_unlock(mutex_for_cb_spk_audit_a);
    pthread_cond_signal(notEmpty_spk_audit_a);



    return;
}
