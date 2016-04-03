#ifndef HTTP_HASH_FUNC_H_INCLUDED
#define HTTP_HASH_FUNC_H_INCLUDED
#include <glib.h>
#include "public.h"
#include "ringbuff.h"
void http_hase_table_create();

int http_hash_look_up(char *http_get_tmp,Sinfo *packet_tmp);
#endif // HTTP_HASH_FUNC_H_INCLUDED
