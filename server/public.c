//#include <glib.h>
#include "public.h"
#include "ringbuff.h"
#include <string.h>
#include <net/bpf.h>
#include <string.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcre.h>

//extern GHashTable *app_hash_tcp_pattern_list;
//extern GHashTable *app_hash_udp_pattern_list;

extern pcre * tcpPcre[1000]; //先定义1000..以后不够.
extern pcre * udpPcre[1000];

extern char SUB_NET[16];
char http_temp_info[50];

int spk_get_ethernet_info(Sinfo *packet_tmp)   //此函数已经默认拿以太网协议的包,如果网络上有非以太网的包，可能会出问题//
{
    int i;
    struct ether_header *eptr=NULL;
    eptr=(struct ether_header *)(packet_tmp->pksave);
    packet_tmp->ether_type=ntohs(eptr->ether_type);
    for(i=0; i<6; i++)
    {
        packet_tmp->src_mac[i]=eptr->ether_shost[i];
        packet_tmp->dst_mac[i]=eptr->ether_dhost[i];
    }

    return 1;
}

int spk_get_ip_info(Sinfo *packet_tmp)
{
    int i;
    char *ip_local=NULL;
    struct ip *ip_header =NULL;
    ip_header= (struct ip *)(packet_tmp->pksave+ sizeof(struct ether_header));
    strcpy(packet_tmp->dst_ip,inet_ntoa(ip_header->ip_dst));
    strcpy(packet_tmp->src_ip,inet_ntoa(ip_header->ip_src));
    packet_tmp->dst_ip[16]='\0';
    packet_tmp->src_ip[16]='\0';
    //printf("dst_ip!!!!!!!! %s\n",packet_tmp->dst_ip);
    packet_tmp->ip_type = ip_header->ip_p;

    ip_local=strstr(packet_tmp->src_ip,SUB_NET);//之后要改为从数据库拿值（监听网段）
    if(ip_local!=NULL)
    {
        //dst is server
        strcpy(packet_tmp->client_ip,packet_tmp->src_ip);
        packet_tmp->client_ip[16]='\0'; //可防止后面libnet_name2addr4调用时读越界.写‘\0’终结符.
        strcpy(packet_tmp->server_ip,packet_tmp->dst_ip);
        packet_tmp->server_ip[16]='\0';

        for(i=0; i<6; i++)
        {
            packet_tmp->client_mac[i]=packet_tmp->src_mac[i];
            packet_tmp->server_mac[i]=packet_tmp->dst_mac[i];
        }
    }
    else
    {
        //src is server ip. dst is client.
        strcpy(packet_tmp->client_ip,packet_tmp->dst_ip);
        packet_tmp->client_ip[16]='\0'; //可防止后面libnet_name2addr4调用时读越界.写‘\0’终结符.
        strcpy(packet_tmp->server_ip,packet_tmp->src_ip);
        packet_tmp->server_ip[16]='\0';

        for(i=0; i<6; i++)
        {
            packet_tmp->client_mac[i]=packet_tmp->dst_mac[i];
            packet_tmp->server_mac[i]=packet_tmp->src_mac[i];
        }

    }

    return 1;
}

int tcppkt_get_info(TCPinfo *tcppkt_tmp)
{
    struct ip *this_iphdr=NULL;
    this_iphdr= (struct ip *)((tcppkt_tmp->srcpkt).pksave+ sizeof(struct ether_header));
    struct tcphdr *tcphtr=NULL;
    tcphtr=(struct tcphdr *)( (tcppkt_tmp->srcpkt).pksave  + sizeof(struct ether_header) + sizeof(struct ip));
    tcppkt_tmp->th_flags=tcphtr->th_flags;
    tcppkt_tmp->src_port=ntohs(tcphtr->th_sport);////
    tcppkt_tmp->dst_port=ntohs(tcphtr->th_dport);////
    tcppkt_tmp->tcp_playlen=ntohs(this_iphdr->ip_len)- 4 * this_iphdr->ip_hl - 4 * tcphtr->th_off; //
    tcppkt_tmp->seq=ntohl(tcphtr->th_seq);
    tcppkt_tmp->ack=ntohl(tcphtr->th_ack);
    tcppkt_tmp->win=ntohs(tcphtr->th_win);

    return 1;
}

int udppkt_get_info(UDPinfo *udppkt_tmp)
{
    struct ip *this_iphdr=NULL;
    this_iphdr = (struct ip *)((udppkt_tmp->srcpkt).pksave+ sizeof(struct ether_header));
    struct udphdr *udphtr=NULL;
    udphtr=(struct udphdr *)((udppkt_tmp->srcpkt).pksave  + sizeof(struct ether_header) + sizeof(struct ip));
    udppkt_tmp->src_port=ntohs(udphtr->uh_sport);////
    udppkt_tmp->dst_port=ntohs(udphtr->uh_dport);////
    udppkt_tmp->udp_playlen=ntohs(this_iphdr->ip_len)-4 * this_iphdr->ip_hl-8;//ntohs(this_iphdr->ip_len)-4 * this_iphdr->ip_hl-8;

    return 1;
}


/*
*  1，配置BPF的过滤规则
*  2，参数 handle是打开网卡后的句柄，filter_exp是BPF表达式，fp 是BPF表达式在编译成二进制后的指令的指针，net 是网卡掩码
*  3，出错返回-1
*/

int public_fun_bpf_config(pcap_t *handle,char *filter_exp,struct bpf_program fp,bpf_u_int32 net)
{
    //这里有BUG！！！！！！！！
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));

        return -1;
    }
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return -1;
    }
    //else printf("BPF configure success! \n");
    return 0;
}


/*
*  获取HTTP包的有效信息
*  参数packet，原始包；my_packet，我们自己的包
*
*/
//int public_fun_get_packet_info(const u_char * packet,struct sinfo  *my_packet)
//{
//    //struct packetinfo my_packet;
//    int i=0;
//    struct ether_header *eth_header=(struct ether_header*)packet;  //get ether header ,include s_mac,d_mac,ether_type
//    memset(my_packet->src_mac, 0, sizeof(my_packet->src_mac));
//    for(i=0; i<6; i++)
//    {
//        my_packet->src_mac[i]=eth_header->ether_shost[i];
//        my_packet->dst_mac[i]=eth_header->ether_dhost[i];
//    }
//    if(ntohs(eth_header->ether_type)==ETHERTYPE_IP)
//    {
//        /**************************IP info **************************/
//        struct ip *ip_header = (struct ip *)(packet + sizeof(struct ether_header));
//        strcpy(my_packet->dst_ip,inet_ntoa(ip_header->ip_dst));
//        strcpy(my_packet->src_ip,inet_ntoa(ip_header->ip_src));
//        if(ip_header->ip_p==6)
//        {
//            /***************************tCP  info  **************************/
//            struct tcphdr *tcp_header= (struct tcphdr *)(packet+ sizeof(struct ether_header) + sizeof(struct ip));
//            if(tcp_header->th_flags==0x14||tcp_header->th_flags==0x04)
//            {
//                return 0;
//            }
//            my_packet->dst_port=ntohs(tcp_header->th_dport);
//            my_packet->src_port=ntohs(tcp_header->th_sport);
//
//            my_packet->seq=ntohl(tcp_header->th_seq);
//            my_packet->ack=ntohl(tcp_header->th_ack);
//            my_packet->win=ntohs(tcp_header->th_win);
//        }
//    }
//    return 1;
//}


/*
*  获取HTTP包的要匹配信息
*  参数packet，原始包；str_cmp，要匹配的信息；len 长度；
*
*/
char * http_get_info(char *packet,char * str_cmp,int len)
{
    char *p=NULL;
    char *p2=NULL;

    p=strstr(packet,str_cmp);
    if(p==NULL)
    {
        //printf("can't find the str\n");
        return 0;
    }

    if(str_cmp[0]=='R')
    {
        p2=strchr(p+len,'/');
//        if(p2==NULL)
        if(p2==NULL)
        {
            p2=strstr(p,"\r\n");
            if(p2==NULL) return 0;

        }
//        if(p2==NULL) return 0;
    }
    else
    {
        p2=strstr(p,"\r\n");
        if(p2==NULL) return 0;
    }

    memset(http_temp_info,0,50);
    //printf("p %d ,p2 %d \n",p,(p2-p)-len);
    strncpy(http_temp_info,p+len,(p2-p)-len);   //安全性检查还不够//
    http_temp_info[49]='\0';
    //printf("http_get_info()>>>>>>>>>>>>>>  %s \n",http_temp_info);
    return http_temp_info;

}


int spk_check_patten(char *packet,char * str_cmp)
{
    char * r=NULL;
    r=strstr(packet,str_cmp);
    if(r==NULL)
    {
//        printf("can't find the patten\n");
        return 0;
    }
    else
    {
//        printf("success find the patten\n");
        return 1;
    }


}

//tcp's pattern check by pcre_exec
int pattern_tcp_pcre_exec(char * packet,char *str_cmp,unsigned long len,int a)
{
    int offset=-1,find=0; //偏移量，是否找到，pattern id ,i=0
    //i=(gint)(glong)(g_hash_table_lookup(app_hash_tcp_pattern_list,str_cmp));

    if(a)
    {
//        printf("i=---%d\n",i);
        offset=pcre_exec(tcpPcre[a], NULL, packet, len, 0, 0, NULL, 0);
 //check hash
//哈希表输出函数
//        void iterator(gpointer key, gpointer value, gpointer user_data)
//        {
//            g_print(user_data, key, value);
//        }
//
//        g_hash_table_foreach(app_hash_pattern_list,(GHFunc)iterator,"The hash of %s is %d \n");

//        int d;
//        for(d=0;d<70;d++)
//        {
//            printf("%x",packet[d]);
//            if(packet[0]=='o') printf("22222222222222222222222222222222222\n");
//        }//输出整个包
//    printf("%x   ||%x   ||%x   ||%x   ||%x   ||\n",packet[0],packet[1],packet[2],packet[3],
//           packet[4]);//输出前五个
//    printf("\n\n");
//        printf("\n\n");
//        printf("len : %d\n",strlen(packet));


        //返回匹配串的偏移位置
//        printf("r== %d\n",r);
        if(offset>=0)
        {
            find=1;
        }

    }else printf("tcp hash lookup failue \n");//hash lookup failue

    return find;
}

//udp check pattern by pcre_exec
int pattern_udp_pcre_exec(char * packet,char *str_cmp,unsigned long len,int a)
{
    int offset=-1,find=0; //偏移量，是否找到，pattern id ,i=0
    //i=(gint)(glong)(g_hash_table_lookup(app_hash_udp_pattern_list,str_cmp));
    //printf("i %d\n",i);
    if(a)
    {
//        printf("i=---%d\n",i);
        offset=pcre_exec(udpPcre[a], NULL, packet, len, 0, 0, NULL, 0);

        if(offset>=0)
        {
            find=1;
        }

    }else printf("udp hash lookup failue \n");//hash lookup failue

    return find;
}

void free_data(gpointer data)
{

        g_free(data);
        data = NULL;

}



