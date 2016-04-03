#include "usr_func.h"
#include "public.h"
#include <glib.h>
extern GHashTable *user_to_gid_hash_list;
extern GHashTable *user_to_uid_hash_list;
extern GHashTable *user_to_name_hash_list;

void user_hase_table_create()
{
    //哈希表初始化//
    user_to_gid_hash_list=  g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);//IP to gid //
    user_to_uid_hash_list=  g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);//IP to uid //
    user_to_name_hash_list=  g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);//IP to uid //
}
