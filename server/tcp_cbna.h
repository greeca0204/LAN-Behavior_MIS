#ifndef TCP_CBNA_H_INCLUDED
#define TCP_CBNA_H_INCLUDED
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define mknew(x)	(x *)malloc(sizeof(x))
#define b_comp(x,y)	(!memcmp(&(x), &(y), sizeof(x)))
#define EXP_SEQ (snd->first_data_seq + rcv->count + rcv->urg_count)  //渴望得到的序列号

//定义Libnids状态
#define NIDS_JUST_EST 1
#define NIDS_DATA 2
#define NIDS_CLOSE 3
#define NIDS_RESET 4
#define NIDS_TIMED_OUT 5
#define NIDS_EXITING   6	//流连接结束时最后一次处理数据///* nids is exiting; last chance to get data */
//表示连接的逻辑状态,在回调函数的处理分类中非常重要//
//状态：NIDS_JUST_EST表示一个刚刚建立的连接，NIDS_DATA表示连接接收到新的数据，
//NIDS_CLOSE、NIDS_RESET 和NIDS_TIMEOUT，表示该连接已经关闭了

#define FIN_SENT 120
#define FIN_CONFIRMED 121
#define COLLECT_cc 1   //客户端有数据
#define COLLECT_sc 2   //服务器端有数据
#define COLLECT_ccu 4 //客户端有紧急数据
#define COLLECT_scu 8 //服务器端有紧急数?

struct tcp_timeout
{
  struct tcp_stream *a_tcp;
  struct timeval timeout;
  struct tcp_timeout *next;
  struct tcp_timeout *prev;
};

//回调函数的存放链表节点//
struct proc_node {
  void (*item)();
  struct proc_node *next;
};

//这个一直没看懂它跟回调函数之间的关系，先放着吧//
struct lurker_node {
  void (*item)();
  void *data;
  char whatto;//感兴趣则whatto不为0
  struct lurker_node *next;

};
//包的存放 是一个双向链表 ，乱序的包会先放在这里//
struct skbuff {
  struct skbuff *next;
  struct skbuff *prev;

  void *data;
  u_int len;
  u_int truesize;
  u_int urg_ptr;

  char fin;
  char urg;
  u_int seq;
  u_int ack;
};

//地址端口结构可以确定一个TCP连接
struct tuple4
{
  u_short source;
  u_short dest;
  u_int saddr;
  u_int daddr;
};

//TCP连接中一端的所有信息，可以是服务器或是客户端
struct half_stream
{
  char state;// socket 状态 (例如：TCP_ESTABLISHED // 连接的逻辑状态

  char collect;//标示有数据到达， if >0, 那么数据应该被存放到data缓冲区中。否则，
// 这个方向的数据流将被忽略

  char collect_urg; // 类似的，判断是否为紧急数据

  char *data;// 存储正常接收到的数据
  int offset;// 保存到"data"缓冲区中的第一个字节的偏移量
  int count;// 自连接建立以来已经有多少字节已经发送到data缓冲区中
  int count_new;// 有多少新的数据将要  存储到data中，如果为0，表示没有新数据到达
  int bufsize;//数据区最大长度
  int rmem_alloc;//该TCP连接所有数据的长度和，包括list队列里的

  int urg_count;// if != 0,新的紧急数据到达
  u_int acked;
  u_int seq;
  u_int ack_seq;
  u_int first_data_seq;
  u_char urgdata; //存储紧急数据
  u_char count_new_urg; //判断是否有紧急数据到达 如果非0，表示有新的紧急数据到达
  u_char urg_seen; //有新的URG数据，不是以前的重复数据
  u_int urg_ptr;//指向URG数据在流中的位置
  u_short window;//窗口大小
  u_char ts_on;
  u_char wscale_on;
  u_int curr_ts;
  u_int wscale;

  int count_gd;

  struct skbuff *list;
  //当收到的TCP包中SEQ比期望的SEQ大时，则将数据先保存到此双向链表中，
  //为访问方便，此链表按包中的SEQ从小到大排序，链尾为含最大的SEQ的TCP包
  struct skbuff *listtail;//ptr to tail of list
};

//表示一个TCP连接的所有信息
struct tcp_stream
{
  struct tuple4 addr;//tcp连接的四个重要信息
  char nids_state; //表示连接的逻辑状态,这个很重要，在回调函数中非常重要//
  //状态：NIDS_JUST_EST表示一个刚刚建立的连接，NIDS_DATA表示连接接收到新的数据，
  //NIDS_CLOSE、NIDS_RESET 和NIDS_TIMEOUT，表示该连接已经关闭了
  struct lurker_node *listeners;  //给TCP处理的回调函数使用，其中有一成员指向回调函数
  struct half_stream client;
  struct half_stream server;
  struct tcp_stream *next_node;
  struct tcp_stream *prev_node;
  int hash_index;
  struct tcp_stream *next_time;
  struct tcp_stream *prev_time;
  int read; //本次读进数据缓冲区的数据的长度
  struct tcp_stream *next_free;//指向下一个空闲tcp结构

  int filelen;
  int filetype;
  int sm_state;
  int tcp_num;
  char *filename;
  int filesize;
  int filestart;
};

int tcp_cbna_init(int size);
void nids_register_tcp(void (*x));
void process_tcp_cbna(const u_char * data, int skblen);
void http_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed);
void nids_discard(struct tcp_stream * a_tcp, int num);
void tcp_check_timeouts(struct timeval *now);
void tcp_exit(void);
void ftp_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed);
void ftp_data_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed);
//
//void http_search_callback(struct tcp_stream *a_tcp, void ** this_time_not_needed);
void http_test_callback(struct tcp_stream *a_tcp, void ** this_time_not_needed);


//void http_test2_callback(struct tcp_stream *a_tcp, void ** this_time_not_needed);

//void scan_init(void);
//void scan_exit(void);
//void detect_scan(struct ip *);
//int audit_init();
//void  audit_desy();
//函数名的还没加进来//
#endif // TCP_CBNA_H_INCLUDED
