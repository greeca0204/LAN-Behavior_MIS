#include "ringbuff.h"
#include "public.h"
#include "IM.h"
#include <pcre.h>
#include <string.h>
#include <pthread.h>

//extern GHashTable *app_hash_tcp_pattern_list;
//extern GHashTable *app_hash_udp_pattern_list;

extern GHashTable *app_hash_white_list;
extern GHashTable *app_hash_person_list;
extern GHashTable *app_hash_whole_list;
extern GHashTable *app_hash_group_list;

extern GHashTable * app_tcp_hash_IPbuff_list;
extern GHashTable * app_udp_hash_IPbuff_list;

extern GHashTable *user_to_gid_hash_list;

extern char  hash_tcp_pattern[1000][300];
extern char  hash_tcp_pattern_name[1000][20];

extern char hash_udp_pattern[1000][300];
extern char hash_udp_pattern_name[1000][20];

char pattern_tcp_name[50]="0";

char pattern_udp_name[50]="0";

void app_hase_table_create()
{
//    app_hash_tcp_pattern_list=g_hash_table_new(g_str_hash,g_int_equal);
//    app_hash_udp_pattern_list=g_hash_table_new(g_str_hash,g_int_equal);
    app_hash_white_list=g_hash_table_new_full(g_str_hash,g_str_equal,
                                               free_data,free_data);
//    app_hash_black_list = g_hash_table_new(g_str_hash, g_str_equal);
    app_hash_person_list = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);
//    app_hash_group_list = g_hash_table_new(g_str_hash, g_str_equal);
    app_hash_group_list = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);
    app_hash_whole_list = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);

    app_tcp_hash_IPbuff_list = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);
    app_udp_hash_IPbuff_list = g_hash_table_new_full(g_str_hash, g_str_equal,
                                               free_data,free_data);
}


char * hash_tcp_pattern_process(char * packet,unsigned long len) //用hash_pattern查哈希表.得到i.再用编译好的正则匹配.
{
    int a;
    int r=0;//控制循环次数若连续有30个空hash_pattern[]，不再循环。避免空数据也继续循环太多。
//    int flag=0;
    memset(pattern_tcp_name,0,50);
//     for(a=1; a<10; a++)
//    {
//
//        printf("^^^^^^: %s\n",hash_tcp_pattern[a]);
//    }
    for(a=1; a<1000; a++)
    {

        if((strlen(hash_tcp_pattern[a]))>0&&hash_tcp_pattern[a]!=NULL)
        {
            if((pattern_tcp_pcre_exec(packet,hash_tcp_pattern[a],len,a)))
            {
                printf("Find TCP %s---%s\n",hash_tcp_pattern_name[a],hash_tcp_pattern[a]);
//                flag=1;
                strcpy(pattern_tcp_name,hash_tcp_pattern_name[a]);

                return pattern_tcp_name;
//                break;
            }
        }
        else
        {
            r++;
        }

        if(r==30)
        {
            return NULL;
        }

    }

    return NULL;
}


char * hash_udp_pattern_process(char * packet,unsigned long len) //用hash_pattern查哈希表.得到i.再用编译好的正则匹配.
{
    int a;
    int r=0;//控制循环次数若连续有30个空hash_pattern[]，不再循环。避免空数据也继续循环太多。
//    int flag=0;
    memset(pattern_udp_name,0,50);
//     for(a=1; a<10; a++)
//    {
//
//        printf("^^^^^^: %s\n",hash_udp_pattern[a]);
//    }
    //printf("UU %s\n",hash_udp_pattern[17]);


    for(a=1; a<1000; a++)
    {

        if((strlen(hash_udp_pattern[a]))>0&&hash_udp_pattern[a]!=NULL)
        {
            //printf("UU %d %s\n",a,hash_udp_pattern[a]);
            if((pattern_udp_pcre_exec(packet,hash_udp_pattern[a],len,a)))
            {
                printf("Find UDP %s---%s\n",hash_udp_pattern_name[a],hash_udp_pattern[a]);
//                flag=1;
                strcpy(pattern_udp_name,hash_udp_pattern_name[a]);

                return pattern_udp_name;
//                break;
            }
        }
        else
        {
            r++;
        }
        if(r==30)
        {
            return NULL;
        }


    }

    return NULL;
}





//  IP查找哈希表看是否为记录软件ip//
int appip_hash_look_up(GHashTable *hash_IPbuff_list, char *server_ip,char *client_ip)
{
    char str_cmp[100];
    char *app_name;
//    char name_ip[100];

    memset(str_cmp,0,100);
    strcpy(str_cmp,server_ip);//获取IP(一定server)


    app_name=(char*)(g_hash_table_lookup(hash_IPbuff_list, str_cmp));
    //strlen(app_name)
//    printf("app_name@@@@!!!!    %s\n",app_name);
//    memset(name_ip,0,100);

    if(app_name!=NULL)//Ip hash 里有记录！//
    {
        //查黑白名单
        if(appip_tactics_look_up(app_name,client_ip))
            return 1;
    }

    return 0;
}

//查找个人白名单 个人黑名单 组黑名单 全局黑名单  需要发包则返回 true
int appip_tactics_look_up(char *app_name,char *client_ip)
{
    char name_ip[100];
    memset(name_ip,0,100);
    strcpy(name_ip,app_name);
    strcat(name_ip,client_ip);
    char gid[32];
//        printf("name_ip!!!! %s\n",name_ip);

    if(g_hash_table_lookup(app_hash_white_list, name_ip))  return 0;  //白名单
    else
    {
        if(g_hash_table_lookup(app_hash_person_list, name_ip))
        {
            //printf("person  list \n\n"); //黑名单（个人）
            return 1;
        }
        else
        {
            memset(name_ip,0,100);
            strcpy(name_ip,app_name);//重新进行查找字符串的匹配

            if(g_hash_table_lookup(user_to_gid_hash_list,client_ip))
            {
                memset(gid,0,32);
                strcpy(gid,g_hash_table_lookup(user_to_gid_hash_list,client_ip));
                strcat(name_ip,gid);
            }

            if(g_hash_table_lookup(app_hash_group_list, name_ip))
            {
                //printf("group list \n\n");   //黑名单（组）
                return 1;
            }
            else
            {
                //printf("Whole::::%s\n",http_get_tmp);
                if(g_hash_table_lookup(app_hash_whole_list, app_name))
                {
                    //printf("whole list %s\n",(char *)g_hash_table_lookup(http_hash_whole_list, http_get_tmp)); //黑名单（全局）
                    return 1;
                }
            }
        }

    }

    return 0;

}








//int msn(char * packet,unsigned long len)
//{
//    int flag=0;
//    char *msn="VER.*MSNP";
//
//    if((pattern_pcre_exec(packet,msn,len)))
//    {
//        printf("success to find msn \n");
//        flag=1;
//    }
//
//    return flag;//
//}
//
//int YY(char * packet,unsigned long len)
//{
//    int flag=0;
//    char *YY1="^\\x4e\\x00\\x00\\x00\\x01\\x08";//\\\\x4f\\\\x00\\\\x00\\\\x00\\\\x04\\\\x11
////    char *YY2="^\\x4c\\x00\\x00\\x00\\x02";//2，4，5貌似没多大用
//    char *YY3="^\\x4f\\x00\\x00\\x00\\x04\\x11";
////    char *YY4="^\\x06\\x00\\x00\\x00\\x04\\x12";
////    char *YY5="^\\x01\\x02\\x00\\x04\\x00\\x00";
//
//    if((pattern_pcre_exec(packet,YY1,len)))
//    {
//        printf("find YY1\n");
//        flag=1;
//    }
//
////    if((pattern_pcre_exec(packet,YY2,len)))
////    {
////        printf("find YY2\n");
////        flag=1;
////    }
//
//    if((pattern_pcre_exec(packet,YY3,len)))
//    {
//        printf("find YY3\n");
//        flag=1;
//    }
//
////        if((pattern_pcre_exec(packet,YY4,len)))
////    {
////        printf("find YY4\n");
////        flag=1;
////    }
////
////        if((pattern_pcre_exec(packet,YY5,len)))
////    {
////        printf("find YY5\n");
////        flag=1;
////    }
//
//
//    return flag;// ^\x4f\x00\x00\x00\x04\x11
//}
//
//int qq(char * packet,unsigned long len)
//{
//    int flag=0;
//    char *qq="^.?.?\\x02.+\\x03$";
////    printf("qq--:%s\n",qq);
//
//    if((pattern_pcre_exec(packet,qq,len)))
//    {
//        printf("success to find qq \n");
//        flag=1;
//    }
//
//    return flag;//
//}
