#ifndef PUBLIC_H_INCLUDED
#define PUBLIC_H_INCLUDED
#include "ringbuff.h"
#include <pthread.h>
#include <mysql.h>
#include <glib.h>
#include <pcap/pcap.h>

int public_fun_bpf_config(pcap_t *handle,char *filter_exp,
                                 struct bpf_program fp,bpf_u_int32 net);

int spk_get_ethernet_info(Sinfo *packet_tmp);
int spk_get_ip_info(Sinfo *packet_tmp);

int tcppkt_get_info(TCPinfo *tcppkt_tmp);
int udppkt_get_info(UDPinfo *udppkt_tmp);

char * http_get_info(char *packet,char * str_cmp,int len);
//int public_fun_get_packet_info(const u_char * packet,struct sinfo  *my_packet);

int pattern_tcp_pcre_exec(char * packet,char *str_cmp,unsigned long len,int a);
int pattern_udp_pcre_exec(char * packet,char *str_cmp,unsigned long len,int a);
int spk_check_patten(char *packet,char * str_cmp);

void free_data(gpointer data);

#endif // PUBLIC_H_INCLUDED
