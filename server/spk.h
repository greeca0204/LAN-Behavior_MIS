#ifndef SPK_H_INCLUDED
#define SPK_H_INCLUDED
#include "ringbuff.h"
#include <pcap/pcap.h>

void spk_pcap();
void udp_pcap();
void audit_pcap();

void spk_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) ;
void udp_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet) ;
void audit_putin_buff(u_char *arg, const struct pcap_pkthdr* pkthdr,const u_char * packet);

void spk_process();
void tcp_deepprocess();
void tcp_rst_tcp();
int rst_tcp123(struct sinfo *my_packet,u_short src_port,
               u_short dst_port,unsigned long int seq,unsigned long int ack,
               unsigned long int win,int playlen );

void udp_process();
void udp_block();
int arp_spoofind(char * dst_ip_str ,u_char * dst_mac);

int tcp_prot_decete(char  * my_apphtr,int port_num);
int udp_prot_decete(char * my_apphtr,int port_num);


#endif // SPK_H_INCLUDED
