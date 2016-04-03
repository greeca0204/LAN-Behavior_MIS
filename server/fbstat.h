#ifndef FBSTAT_H_INCLUDED
#define FBSTAT_H_INCLUDED
#include "ringbuff.h"

int stat_http_url(Sinfo *my_packet,char *my_url);

int audit_insert_BD_tieba(char *sip,char *title,char *content,char *host);  //url? host?

int audit_insert_down_attch(char *sip,char *path);

int audit_insert_qm_up_attch(char *sip,char *path);

int audit_insert_BD_sech(char *sip,char *mubiao);

int audit_insert_qm_gzip_browse(char *sip,char *path);

int audit_insert_http_url(char *sip,char *url);
#endif // FBSTAT_H_INCLUDED
