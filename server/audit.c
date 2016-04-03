#include "ringbuff.h"
#include "public.h"
#include "ip_fragment.h"
#include "tcp_cbna.h"
#include <pthread.h>
#include <glib.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

extern Ringbuff *cb_spk_aduit_a;
extern pthread_mutex_t *mutex_for_cb_spk_audit_a; // 用于同步  包捕获（a）线程 与审计线程//
extern pthread_cond_t *notFull_spk_audit_a, *notEmpty_spk_audit_a;// 用于同步  包捕获（a）线程 与审计线程//



////包的审计
//extern struct tcp_timeout *nids_tcp_timeouts;
//extern struct pcap_pkthdr * nids_last_pcap_header;

void spk_audit()
{
    struct sinfo  packet_tmp;
    int process_audit_flag=1;
    int need_free = 0;

    while(process_audit_flag)
    {
        pthread_mutex_lock(mutex_for_cb_spk_audit_a);
        while(ringbuff_isempty(cb_spk_aduit_a))
        {
            pthread_cond_wait(notEmpty_spk_audit_a, mutex_for_cb_spk_audit_a);
        }
        //memset(packet_tmp.pksave,0,1600);
        POP_RINGBUFF(cb_spk_aduit_a,Sinfo,packet_tmp);

        pthread_mutex_unlock(mutex_for_cb_spk_audit_a);
        pthread_cond_signal(notFull_spk_audit_a);



//        spk_get_ethernet_info(&packet_tmp);  //拿到以太网的信息//


//    if(packet_tmp.ether_type==0x0800)             //IP协议  == 0x0800//
        // {
        struct ip *iph= (struct ip *)(packet_tmp.pksave+ sizeof(struct ether_header));
//        //ip碎片的识别与重组
        switch (ip_defrag_stub(((struct ip *)(packet_tmp.pksave+ sizeof(struct ether_header))), &iph))//注意！传指针地址！指针已变！//
        {
        case IPF_ISF://3//
            printf("分片\n");
            continue;

        case IPF_NOTF://1//不是分片
            need_free = 0;
            //     printf("不是分片\n");
            break;

        case IPF_NEW://2//完整合并后的ip包
            need_free = 1;
            printf("ip碎片重组，完整合并后的ip包\n");
            break;

        default:
            ;
        }

//这里到tcp/udp重组审计处理//
        if(iph->ip_p==6)  //TCP//
        {

            process_tcp_cbna((u_char *)iph,packet_tmp.pklen - sizeof(struct ether_header));
        }

        else if(iph->ip_p==17)  //UDP//
        {

        }



        if (need_free)
            free(iph);

//    }

    }

    return;
}
