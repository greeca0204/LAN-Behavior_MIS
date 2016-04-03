#ifndef SQL_FUNC_H_INCLUDED
#define SQL_FUNC_H_INCLUDED
#include <glib.h>
#include <mysql.h>
int sql_start(MYSQL *mysql);

int sql_get_system_config(MYSQL *sock);

int sql_get_website_to_part_hash(char *SELECT_QUERY,
                            MYSQL *sock,int http_type,GHashTable * hash_table);
int sql_get_website_to_whole_hash(char *SELECT_QUERY,
                            MYSQL *sock,int http_type,GHashTable * hash_table);
int sql_get_app_to_hash(char *SELECT_QUERY,
                            MYSQL *sock,int http_type,GHashTable * hash_table);
int sql_get_app_to_whole_hash (char *SELECT_QUERY,
                            MYSQL *sock,int http_type,GHashTable * hash_table);
int sql_get_user_to_hash(char *SELECT_QUERY,MYSQL *sock,
                         GHashTable *hash_table);
int sql_get_pattern_to_hash(char *SELECT_QUERY,MYSQL *sock,
                            GHashTable * hash_table, char*type);
#endif // SQL_FUNC_H_INCLUDED

