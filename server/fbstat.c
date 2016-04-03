#include "fbstat.h"
//#include ""

#include <string.h>
#include <glib.h>
#include <mysql.h>
#include <unistd.h>
#include <time.h>

extern GHashTable *user_to_uid_hash_list;
extern GHashTable *user_to_name_hash_list;

extern MYSQL mysql;

//设计函数原型//
int stat_http_url(Sinfo *my_packet,char *my_url)
{
    //    得到已知的URL和IP信息//
    char str_cmp[1024];
    //printf("in stat before memset -- %s\n",str_cmp);
    memset(str_cmp,0,sizeof(str_cmp));
    char SELECT_QUERY[1024];

    strcpy(str_cmp,my_url);//获取URL
    //strcat(str_cmp,my_packet->src_ip);
    //printf("in stat after memset -- %s\n",str_cmp);
    //    对URL 进行分类 查找,(这个可以先不做考虑)//

    //    通过IP哈希查找获取NAME,UID//
    gpointer hsname,hsuid;
    if((hsname=g_hash_table_lookup(user_to_uid_hash_list, my_packet->src_ip)))
    {
        hsuid=g_hash_table_lookup(user_to_uid_hash_list, my_packet->src_ip);
        snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into audit_http(Name,Uid,IP,Http_type,Time,Detail,Protocol) values('%s','%s',%s','%s',FROM_UNIXTIME(%d),'%s','%s')",
                 (char *)hsname,
                 (char *)hsuid,
                 my_packet->src_ip,//,//
                 "",/*q->http_type,*/
                 0, //time 现在还没有处理//
                 str_cmp,
                 "80");
    }
    else
    {
        strcat(str_cmp,my_packet->src_ip);
        snprintf(SELECT_QUERY,sizeof(SELECT_QUERY), "insert into audit_http(Http_type,Time,Detail,Protocol) values('%s',FROM_UNIXTIME(%d),'%s','%s')",
                 "",/*q->http_type,*/
                 0, //time 现在还没有处理//
                 str_cmp,
                 "80");
    }
    printf("QUERY : %s\n",SELECT_QUERY);
    //    写入数据库//
    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
        sleep(5);
        exit(1);
    }
    return 1;
}

int audit_insert_BD_tieba(char *sip,char *title,char *content,char *host)  //url? host?
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);
    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }
//    printf("NAME %s \n",hsname);
//    printf("Uid %s \n",uid);
    char keyonly[225];
//    char urls[50];
    //key 格式   "时间 标题 IP HOST" //百度贴吧的先这个格式吧

    //时间的话，明晚再写//
//    char tme[40];
//    memset(tme,0,sizeof(tme));
    time_t ttt;
    ttt = time(NULL);
//
    struct tm  *stm = localtime(&ttt);

    memset(keyonly,0,sizeof(keyonly));
    strcat(keyonly,asctime(stm));
    strcat(keyonly,title);//没有做安全性检查//
    strcat(keyonly,sip);
    strcat(keyonly,host);

//    int i_uid=0;
//    mysql(&mysql,"SET NAMES 'utf-8'");
    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into audit_bbs(Key_only,Name,User_IP,Uid,Host,Time,Detail,Url,Title) \
             values('%s','%s','%s','%s','%s',FROM_UNIXTIME(%u),'%s','%s','%s')",
             keyonly,
             (char *)hsname,
             sip,
             (char *)uid,
             host ,//host//
             (uint32_t)time(NULL), //time 现在还没有处理//
             content ,  //detial
             host , //url
             title);  //title

//    printf("%s\n",SELECT_QUERY);
//    sleep(4);
    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }

    return 1;
}

int audit_insert_down_attch(char *sip,char *path)
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);
    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }

    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into attachment(IP,Name,Uid,Datetime,Position,UorD) \
             values('%s','%s','%s',FROM_UNIXTIME(%u),'%s',1)",
             sip,
             (char *)hsname,
             (char *)uid,
             (uint32_t)time(NULL), //time 现在还没有处理//
             path);  //title

    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }

    return 1;
}

int audit_insert_qm_up_attch(char *sip,char *path)
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);
    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }

    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into attachment(IP,Name,Uid,Datetime,Position,UorD) \
             values('%s','%s','%s',FROM_UNIXTIME(%u),'%s',0)",
             sip,
             (char *)hsname,
             (char *)uid,
             (uint32_t)time(NULL), //time 现在还没有处理//
             path);  //title

    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }
    return 1;
}

int audit_insert_BD_sech(char *sip,char *mubiao)
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);
    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }
    char keyonly[225];

    //key 格式   "IP 标题  HOST 时间" //百度搜的先这个格式吧

    time_t ttt;
    ttt = time(NULL);
    char host[30]= {"www.baidu.com"};
    struct tm  *stm = localtime(&ttt);

    memset(keyonly,0,sizeof(keyonly));
    strcat(keyonly,sip);
    strcat(keyonly,mubiao);//没有做安全性检查//
    strcat(keyonly,host);
    strcat(keyonly,asctime(stm));

    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into audit_search(Key_only,Name,User_IP,Uid,Http_website,Time,Detail) \
             values('%s','%s','%s','%s','%s',FROM_UNIXTIME(%u),'%s')",
             keyonly,
             (char *)hsname,
             sip,
             (char *)uid,
             host ,//host//
             (uint32_t)time(NULL), //time 现在还没有处理//
             mubiao //detial
            );  //title

//    printf("%s\n",SELECT_QUERY);
//    sleep(4);
    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }

    return 1;
}


int audit_insert_qm_gzip_browse(char *sip,char *path)
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);
    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }

    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into audit_email_read(IP,Name,Uid,Datetime,file_name) \
             values('%s','%s','%s',FROM_UNIXTIME(%u),'%s')",
             sip,
             (char *)hsname,
             (char *)uid,
             (uint32_t)time(NULL), //time 现在还没有处理//
             path);  //title
    //printf("%s\n",snprintf);
    //
    if(mysql_query(&mysql,SELECT_QUERY) != 0)
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }
    return 1;
}




int audit_insert_http_url(char *sip,char *url)
{
    char SELECT_QUERY[1024];
    gpointer hsname=NULL,uid=NULL;
    hsname=g_hash_table_lookup(user_to_name_hash_list, sip);

    char keyonly[225];

    if(!hsname)
    {
        printf("NO hsname\n");
        return 0;
    }
    uid=g_hash_table_lookup(user_to_uid_hash_list, sip);
    if(!uid)
    {
        printf("NO Uid\n");
        return 0;
    }

    time_t ttt;
    ttt = time(NULL);
    struct tm  *stm = localtime(&ttt);

    char vtme[40];
    memset(vtme,0,sizeof(vtme));

    strftime(vtme,sizeof(vtme),"%F %R",stm);

    memset(keyonly,0,sizeof(keyonly));
    //strcat(keyonly,asctime(stm));
    //strcat(keyonly,title);//没有做安全性检查//
    strcat(keyonly,sip);
    strcat(keyonly,url);
    strcat(keyonly,vtme);

    memset(SELECT_QUERY,0,1024);
    snprintf(SELECT_QUERY, sizeof(SELECT_QUERY),"insert into audit_http(Name,User_IP,Uid,Http_type,Time,Detail,Protocol,Key_only) \
             values('%s','%s','%s','',FROM_UNIXTIME(%u),'%s','','%s')",
             (char *)hsname,
             sip,
             (char *)uid,
//             host ,//host//
             (uint32_t)time(NULL), //time 现在还没有处理//
             url,  //detial
//             host , //url
             keyonly);  //title

    printf("keyonly %s\n",keyonly);

    if(mysql_query(&mysql,SELECT_QUERY))
    {
        fprintf(stderr,"Query failed (%s)\n",mysql_error(&mysql));
//        sleep(5);
//        exit(1);
    }

    return 1;

}
