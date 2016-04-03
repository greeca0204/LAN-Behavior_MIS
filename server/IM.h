#ifndef IM_H_INCLUDED
#define IM_H_INCLUDED

#include <glib.h>
//int YY(char * packet,unsigned long len);
//int msn(char * packet,unsigned long len);
//int qq(char * packet,unsigned long len);
void app_hase_table_create();
char * hash_tcp_pattern_process(char * packet,unsigned long len);
char * hash_udp_pattern_process(char * packet,unsigned long len);
int appip_hash_look_up(GHashTable *hash_IPbuff_list, char *server_ip,char *client_ip);
int appip_tactics_look_up(char *app_name,char *src_ip);


#endif // IM_H_INCLUDED
