#ifndef IP_FRAGMENT_H_INCLUDED
#define IP_FRAGMENT_H_INCLUDED

#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IP_CE		0x8000	/* Flag: "Congestion" */
#define IP_DF		0x4000	/* Flag: "Don't Fragment" 0表示分片 1表示不分片*/
#define IP_MF		0x2000	/* Flag: "More Fragments" 1表示更多的分片*/
#define IP_OFFSET	0x1FFF	/* "Fragment Offset" part *///便宜原始数据报的长度

#define IP_OFFSET	0x1FFF	/* "Fragment Offset" part *///便宜原始数据报的长度
#define IPF_NOTF 1
#define IPF_NEW  2
#define IPF_ISF  3

#define IPFRAG_HIGH_THRESH		(256*1024)
#define IPFRAG_LOW_THRESH		(192*1024)

#define IP_FRAG_TIME	(30 * 1000)	/* fragment lifetime */

#define UNUSED 314159
#define FREE_READ UNUSED
#define FREE_WRITE UNUSED
#define GFP_ATOMIC UNUSED
#define NETDEBUG(x)

//#define mknew(x)	(x *)malloc(sizeof(x))

int ip_frag_init(int n);

int ip_defrag_stub(struct ip *iph, struct ip **defrag);

//static char *ip_defrag(struct ip *iph, struct sk_buff *skb);

#endif // IP_FRAGMENT_H_INCLUDED
