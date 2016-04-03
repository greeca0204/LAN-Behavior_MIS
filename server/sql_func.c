#include "sql_func.h"
#include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcre.h>

extern MYSQL mysql;
extern char SUB_NET[16];
extern char NET_INTERFACE[5];
extern char GATEWAY[17];
extern u_int8_t FACK_GATEWAY_MAC[7];

extern pcre * tcpPcre[1000]; //先定义1000..以后不够.
extern pcre * udpPcre[1000];

extern char hash_tcp_pattern[1000][300];
extern char hash_tcp_pattern_name[1000][20];

extern char hash_udp_pattern[1000][300];
extern char hash_udp_pattern_name[1000][20];


int sql_start(MYSQL *mysql)
{

    mysql_init(mysql);
    if (!(mysql_real_connect(mysql,"localhost","root","","8-30",0,NULL,0)))
    {
        printf("can link mysql\n");
        fprintf(stderr,"Couldn't connect to engine!\n%s\n\n",mysql_error(mysql));
        perror("");
        sleep(2);
        exit(1);
    }
    else
        return 1;
}

/* system config*/
int sql_get_system_config(MYSQL *sock)
{
    MYSQL_ROW row;
    MYSQL_RES *res;
    //char *stop=NULL;
    //int i;
    //char temp_mac=NULL;
//    u_int i=0;
    memset(NET_INTERFACE,0,5);
    memset(SUB_NET,0,16);
    memset(GATEWAY,0,17);
    memset(FACK_GATEWAY_MAC,0,7);

//extern char SUB_NET[16];
//extern char NET_INTERFACE[5];
//extern char GATEWAY[17];
//extern char FACK_GATEWAY_MAC[6];
    //SUBNET
    if(mysql_query(sock,GET_SUB_NET_CONFIG))
    {
        printf("Couldn't get %s\n",GET_SUB_NET_CONFIG);
        return 0;
    }           //发送一条查询请求
    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",GET_SUB_NET_CONFIG);
        mysql_free_result(res);
        return 0;

    }     //检验是否有数据//
    while ((row = mysql_fetch_row(res)))
    {
        printf("SUB_NET:  %s\n",row[0]);
        if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
        {
            strncpy(SUB_NET,row[0],sizeof(SUB_NET));
            SUB_NET[15]='\0';
        }
    }
    mysql_free_result(res);

    //NET_INTERFACE
    if(mysql_query(sock,GET_NET_INTERFACE_CONFIG))
    {
        printf("Couldn't get %s\n",GET_NET_INTERFACE_CONFIG);
        return 0;
    }           //发送一条查询请求
    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",GET_NET_INTERFACE_CONFIG);
        mysql_free_result(res);
        return 0;

    }     //检验是否有数据//
    while ((row = mysql_fetch_row(res)))
    {

        if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
        {
            strncpy(NET_INTERFACE,row[0],sizeof(NET_INTERFACE));
            NET_INTERFACE[4]='\0';
            printf("NET_INTERFACE:  %s\n",NET_INTERFACE);
        }
    }
    mysql_free_result(res);

    //GATEWAY
    if(mysql_query(sock,GET_GATEWAY_CONFIG))
    {
        printf("Couldn't get %s\n",GET_GATEWAY_CONFIG);
        return 0;
    }           //发送一条查询请求
    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",GET_GATEWAY_CONFIG);
        mysql_free_result(res);
        return 0;

    }     //检验是否有数据//
    while ((row = mysql_fetch_row(res)))
    {

        if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
        {
            strncpy(GATEWAY,row[0],sizeof(GATEWAY));
            GATEWAY[16]='\0';
            printf("GATEWAY:  %s\n",GATEWAY);
        }
    }
    mysql_free_result(res);

    //NET_INTERFACE
    if(mysql_query(sock,GET_FACK_GATEWAY_MAC_CONFIG))
    {
        printf("Couldn't get %s\n",GET_FACK_GATEWAY_MAC_CONFIG);
        return 0;
    }           //发送一条查询请求
    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",GET_FACK_GATEWAY_MAC_CONFIG);
        mysql_free_result(res);
        return 0;

    }     //检验是否有数据//
    while ((row = mysql_fetch_row(res)))
    {

        if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
        {
            if((strlen(row[0]))>0&&row[0]!=NULL) //
            {

                //ok//printf("%i\n",FACK_GATEWAY_MAC[0]);
//        FACK_GATEWAY_MAC[0]=strtol(row[0],&stop,16);
//        FACK_GATEWAY_MAC[1]=strtol(row[1],&stop,16);
//        FACK_GATEWAY_MAC[2]=strtol(row[2],&stop,16);
//        FACK_GATEWAY_MAC[3]=strtol(row[3],&stop,16);
//        FACK_GATEWAY_MAC[4]=strtol(row[4],&stop,16);
//        FACK_GATEWAY_MAC[5]=strtol(row[5],&stop,16);
//        FACK_GATEWAY_MAC[6]='\0';
                //ok//
                //strncpy(FACK_GATEWAY_MAC,row[0],sizeof(FACK_GATEWAY_MAC));
                sscanf(row[0],"%x %x %x %x %x %x",(int*)(&FACK_GATEWAY_MAC[0]),(int*)(&FACK_GATEWAY_MAC[1]),
                       (int*)(&FACK_GATEWAY_MAC[2]),(int*)(&FACK_GATEWAY_MAC[3]),
                       (int*)(&FACK_GATEWAY_MAC[4]),(int*)(&FACK_GATEWAY_MAC[5]));
                FACK_GATEWAY_MAC[6]='\0';

                printf("FACK_GATEWAY_MAC: %x %x %x %x %x %x\n",FACK_GATEWAY_MAC[0],FACK_GATEWAY_MAC[1],
                       FACK_GATEWAY_MAC[2],FACK_GATEWAY_MAC[3],FACK_GATEWAY_MAC[4],FACK_GATEWAY_MAC[5]);
            }
        }

    }
    mysql_free_result(res);
    return 0;


}







//url局部策略的URL哈希表插入  数据库中.局部策略表Tactics_type=url.detail可以写网站,网站种类,all。
int sql_get_website_to_part_hash(char *SELECT_QUERY,MYSQL *sock,int http_type,GHashTable * hash_table)
{
    MYSQL_ROW row;
    MYSQL_ROW row2;
    MYSQL_ROW row3;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    MYSQL_RES *res3;
    char buff[100];
//    char urls[100];
//    memset(buff,0,1024);
    char SELECT_QUERY2[100];////查有没有此类网站
    char GET_URL_eachkind_list[100];

//    printf("%s\n",SELECT_QUERY);

    if(mysql_query(sock,SELECT_QUERY))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        mysql_free_result(res);
        return 0;
    }

    //int iNum_rows = mysql_num_rows(res);
    //  int iNum_fields = mysql_num_fields(res);
    //  char sql_result_buf[mysql_num_fields(res)];

    //row[0] detail. row[1] user_ip
    while ((row = mysql_fetch_row(res)))
    {
        //printf("row[0] %s\n",row[0]);

        if(!strcmp(row[0],"all"))
        {
            //这里是对局部阻断所有url,all表示所有url

            if(mysql_query(sock,GET_URL_kinds_list))//从app_table表里拿到所有的URL种类
            {
                printf("app_table doesn't have Tactics_type = 'url' \n");
                continue;
            }
            res2=mysql_store_result(sock);
            while((row2=mysql_fetch_row(res2)))
            {
                //逐个种类里的url添加进哈希表.row2[0]是url种类
                //printf("局部URL种类:----%s\n",*row2);
                memset(GET_URL_eachkind_list,0,100);
                snprintf(GET_URL_eachkind_list,sizeof(GET_URL_eachkind_list), "select URL from %s",row2[0]);

                if(mysql_query(sock,GET_URL_eachkind_list)) //从每个种类中拿url
                {
                    printf("%s doesn't have url \n",row2[0]);
                    continue;
                }
                res3=mysql_store_result(sock);
                while((row3=mysql_fetch_row(res3)))
                {
                    //row3[0]是url

                    memset(buff,0,100);
                    strcat(buff,row3[0]);
                    strcat(buff,row[1]);
                    //printf("子URL :----%s\n",buff);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[1]));
                }
                mysql_free_result(res3);
                continue;

            }
            mysql_free_result(res2);
            continue;


        }
        else
        {
            //查有没有此类网站
            memset(SELECT_QUERY2,0,100);
            snprintf(SELECT_QUERY2, sizeof(SELECT_QUERY2),"select URL from %s",row[0]);

            if(mysql_query(sock,SELECT_QUERY2))
            {
                //没有，所以是row[0]供单条网站使用
                //printf("没有此网站大类,是单条网址 %s\n",row[0]);
                memset(buff,0,100);
                strcat(buff,row[0]);
                strcat(buff,row[1]);

                printf("单条网站   %s\n",buff);
                g_hash_table_insert(hash_table, g_strdup(buff), g_strdup(row[1]));
                continue;
            }
            else
            {
                //row[0]是网站大类
                //printf("网站大类  %s\n",row[0]);
                res2=mysql_store_result(sock);
                while((row2=mysql_fetch_row(res2)))
                {
                    memset(buff,0,100);
                    strcat(buff,row2[0]);
                    strcat(buff,row[1]);
                    //printf("子URL   %s\n",buff);

                    //printf("url:----%s\n",*row2);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[1]));

                }
                mysql_free_result(res2);
                continue;
            }

        }


    }

    mysql_free_result(res);
//    mysql_free_result(res2);
//    mysql_free_result(res3);
    return 0;   //
}


//url全局策略的哈希表插入  数据库中.Tactics_global全局策略表Tactics_type=url.detail可以写网站,网站种类，写all。
int sql_get_website_to_whole_hash(char *SELECT_QUERY,MYSQL *sock,int http_type,GHashTable * hash_table)
{
    MYSQL_ROW row;
    MYSQL_ROW row2;
    MYSQL_ROW row3;

    MYSQL_RES *res;
    MYSQL_RES *res2;
    MYSQL_RES *res3;

    char value[3]="ALL";

    char GET_URL_eachkind_list[100];

//    printf("%s\n",SELECT_QUERY);

    if(mysql_query(sock,SELECT_QUERY))
    {

        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;

    }

    //int iNum_rows = mysql_num_rows(res);
    //  int iNum_fields = mysql_num_fields(res);
    //  char sql_result_buf[mysql_num_fields(res)];

    while ((row = mysql_fetch_row(res)))
    {
        if(!strcmp(row[0],"all"))
        {
            //这里是对全局阻断所有url,all表示所有url

            if(mysql_query(sock,GET_URL_kinds_list))//从app_table表里拿到所有的URL种类
            {
                printf("app_table doesn't have Tactics_type = 'url' \n");
                continue;
            }
            res2=mysql_store_result(sock);
            while((row2=mysql_fetch_row(res2)))
            {
                //逐个种类里的url添加进哈希表
                //printf("全局阻断URL种类:----%s\n",*row2);
                memset(GET_URL_eachkind_list,0,100);
                snprintf(GET_URL_eachkind_list, sizeof(GET_URL_eachkind_list),"select URL from %s",row2[0]);
                //printf("!!!!!!!!!!!!! %s\n",GET_URL_eachkind_list);
                if(mysql_query(sock,GET_URL_eachkind_list)) //从每个种类中拿url
                {
                    printf("%s doesn't have url \n",row2[0]);
                    continue;
                }
                res3=mysql_store_result(sock);
                while((row3=mysql_fetch_row(res3)))
                {
                    // printf("子URL :----%s\n",*row3);
                    g_hash_table_insert(hash_table,g_strdup(row3[0]),g_strdup(value));
                }
                mysql_free_result(res3);
                continue;

            }
            mysql_free_result(res2);
            continue;
        }
        else //将单条或者一类url设为全局阻断
        {
            memset(GET_URL_eachkind_list,0,100);
            snprintf(GET_URL_eachkind_list,sizeof(GET_URL_eachkind_list), "select URL from %s",row[0]);
            if(mysql_query(sock,GET_URL_eachkind_list))
            {
                //这里说明是单条的url
                //printf("单条网站  %s\n",*row);
                g_hash_table_insert(hash_table, g_strdup(row[0]), g_strdup(value));

                continue;
            }
            else
            {
                //这里说明是一类的url
                //printf("全局黑名单网站大类  %s\n",row[0]);
                res2=mysql_store_result(sock);
                while((row2=mysql_fetch_row(res2)))
                {
                    //printf("url   %s\n",*row2);
                    g_hash_table_insert(hash_table,g_strdup(row2[0]),g_strdup(value));
                }

                mysql_free_result(res2);
                continue;

            }



            continue;

        }



    }

    mysql_free_result(res);
//    mysql_free_result(res2);
//    mysql_free_result(res3);

    return 0;   //


}


int sql_get_user_to_hash(char *SELECT_QUERY,MYSQL *sock,GHashTable * hash_table)
{
    MYSQL_ROW row;
    MYSQL_RES *res;

    if(mysql_query(sock,SELECT_QUERY))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);

        return 0;
    }

    while ((row = mysql_fetch_row(res)))
    {
        printf("USer:--%s--GET--%s \n",row[0],row[1]);
        g_hash_table_insert(hash_table, g_strdup(row[0]), g_strdup(row[1]));
    }

    mysql_free_result(res);
    return 0;   //
}

//把tcp's pattern存进hash表.在使用时.用pattern[]数组查到hash表内对应值.用值提取相关内容。
//这里还做了正则式编译
int sql_get_pattern_to_hash(char *SELECT_QUERY,MYSQL *sock,GHashTable * hash_table, char*type)
{
    MYSQL_ROW row;
    MYSQL_RES *res;
    //int perl[500]={0,1,2};

    if(mysql_query(sock,SELECT_QUERY))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }           //发送一条查询请求

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;

    }     //检验是否有数据//


    while ((row = mysql_fetch_row(res)))
    {
        int i=atoi(row[1]);//str transmit int
        const char *error;
        int erroroffset=0;

        const char *error1;
        int erroroffset1=0;

//        printf("i:  %s\n",row[1]);
        printf("pattern Name:  %s\n",row[2]);
        printf("APP_pattern:  %s\n",row[0]);
        printf("pattern perl:  %d\n",i);


        //g_hash_table_insert(hash_table,g_strdup(row[0]), (gpointer)i);
//        g_hash_table_insert(hash_table,g_strdup(row[0]), GINT_TO_POINTER(i)); //new fix

//        void iterator(gpointer key, gpointer value, gpointer user_data)
//        {
//            g_print(user_data, key, value);
//        }
//
//        g_hash_table_foreach(hash_table,(GHFunc)iterator,"The hash of %s is %d \n");

//        printf("hash_table %s\n",type);
        //tcp's pcre compile
        if(!strcmp(type,"TCP"))
        {
            if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
            {
                strcpy(hash_tcp_pattern[i],row[0]); //将pattern另外存进pattern[],供hash表使用.
                strcpy(hash_tcp_pattern_name[i],row[2]);
                tcpPcre[i]=pcre_compile(row[0],0, &error, &erroroffset,NULL);//编译正则
            }

            if(tcpPcre[i]==NULL)
            {
                printf("PCRE tcp compile failed\n");
                mysql_free_result(res);
                return 1;
            }
            else
            {
                printf("PCRE tcp compile success\n");
            }

        }

        //udp's pcre compile
        if(!strcmp(type,"UDP"))
        {
            if((strlen(row[0]))>0&&row[0]!=NULL) //这里是不是应该改为检验长度?
            {
                strcpy(hash_udp_pattern[i],row[0]); //将pattern另外存进pattern[],供hash表使用.
                strcpy(hash_udp_pattern_name[i],row[2]);
                udpPcre[i]=pcre_compile(row[0],0, &error1, &erroroffset1,NULL);//编译正则
            }
            if(udpPcre[i]==NULL)
            {
                printf("PCRE udp compile failed\n");
                mysql_free_result(res);
                return 1;
            }
            else
            {
                printf("PCRE udp compile success\n");
            }

        }


    }       //这个函数.强制转换那里可能有点毛病..ok

    mysql_free_result(res);

    return 0;   //
}

//给非全局的阻断用的哈希表插入函数
//数据库中.非全局策略表Tactics_type=app_xxx .detail可以写网站,网站种类，不能写all。
int sql_get_app_to_hash(char *SELECT_QUERY,MYSQL *sock,int http_type,GHashTable * hash_table)
{
    MYSQL_ROW row;
    MYSQL_ROW row2;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    char buff[100];

    if(mysql_query(sock,SELECT_QUERY))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;

    }

    //int iNum_rows = mysql_num_rows(res);
    //  int iNum_fields = mysql_num_fields(res);
    //  char sql_result_buf[mysql_num_fields(res)];

    //row[0]是app_xxx种类，row[1]是detail--all,或具体app。row[2]是用户ip。
    while ((row = mysql_fetch_row(res)))
    {
        //这里插入的一类的app+user_ip//
        if(!strcmp(row[1],"all"))
        {
            if(!strcmp(row[0],"app_im"))
            {
                if(mysql_query(sock,GET_APP_IM_list))
                {
                    printf("Couldn't get %s\n",GET_APP_IM_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                //row2[0]是app的name//
                while((row2=mysql_fetch_row(res2)))
                {
                    memset(buff,0,100);
                    strcat(buff,row2[0]);
                    strcat(buff,row[2]);
                    printf("IM:----%s\n",buff);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[2]));
                }
                mysql_free_result(res2);
                continue;
            }
            if(!strcmp(row[0],"app_p2p"))
            {
                if(mysql_query(sock,GET_APP_P2P_list))
                {
                    printf("Couldn't get %s\n",GET_APP_P2P_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                while((row2=mysql_fetch_row(res2)))
                {
                    memset(buff,0,100);
                    strcat(buff,row2[0]);
                    strcat(buff,row[2]);
                    printf("p2p:----%s\n",buff);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[2]));
                }
                mysql_free_result(res2);
                continue;
            }
            if(!strcmp(row[0],"app_game"))
            {
                if(mysql_query(sock,GET_APP_GAME_list))
                {
                    printf("Couldn't get %s\n",GET_APP_GAME_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                while((row2=mysql_fetch_row(res2)))
                {
                    memset(buff,0,100);
                    strcat(buff,row2[0]);
                    strcat(buff,row[2]);
                    printf("game:----%s\n",buff);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[2]));
                }
                mysql_free_result(res2);
                continue;
            }
            if(!strcmp(row[0],"app_other"))
            {
                if(mysql_query(sock,GET_APP_OTHER_list))
                {
                    printf("Couldn't get %s\n",GET_APP_OTHER_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                while((row2=mysql_fetch_row(res2)))
                {
                    memset(buff,0,100);
                    strcat(buff,row2[0]);
                    strcat(buff,row[2]);
                    printf("other:----%s\n",buff);
                    g_hash_table_insert(hash_table,g_strdup(buff),g_strdup(row[2]));
                }
                mysql_free_result(res2);
                continue;
            }



        }
        else //
        {
            //这里是插入一个app+user_ip//
            //g_char * str;
            //str=g_strconcat(row[0],row[1]);
            memset(buff,0,100);
            strcat(buff,row[1]);
            strcat(buff,row[2]);

            printf("app  %s\n",buff);

            g_hash_table_insert(hash_table, g_strdup(buff), g_strdup(row[2]));


        }

    }

    mysql_free_result(res);
//    mysql_free_result(res2);
    return 0;   //
}

//全局的app阻断哈希表插入函数
int sql_get_app_to_whole_hash(char *SELECT_QUERY,MYSQL *sock,int http_type,GHashTable * hash_table)
{
    MYSQL_ROW row;
    MYSQL_RES *res;
    MYSQL_RES *res2;
    char value[3]="ALL";


    if(mysql_query(sock,SELECT_QUERY))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;
    }

    if (!(res=mysql_store_result(sock)))
    {
        printf("Couldn't get %s\n",SELECT_QUERY);
        return 0;

    }


    //row[0]是app_xxx种类.row[1]是all,或网站.
    while ((row = mysql_fetch_row(res)))
    {
        if(!strcmp(row[1],"all"))
        {
            //把一类的app全部添加进哈希表，all代表一类的全部
            if(!strcmp(row[0],"app_im"))
            {
                if(mysql_query(sock,GET_APP_IM_list))
                {
                    printf("Couldn't get %s\n",GET_APP_IM_list);
                    continue;
                }
                res2=mysql_store_result(sock);

                //row[0]变为网站
                while((row=mysql_fetch_row(res2)))
                {
                    printf("全局阻断IM:----%s\n",*row);
                    g_hash_table_insert(hash_table,g_strdup(row[0]),g_strdup(value));
                }
                mysql_free_result(res2);
                continue;
            }

            if(!strcmp(row[0],"app_p2p"))
            {
                if(mysql_query(sock,GET_APP_P2P_list))
                {
                    printf("Couldn't get %s\n",GET_APP_P2P_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                //row[0]变为网站
                while((row=mysql_fetch_row(res2)))
                {
                    printf("全局阻断P2P:----%s\n",*row);
                    g_hash_table_insert(hash_table,g_strdup(row[0]),g_strdup(value));
                }
                mysql_free_result(res2);
                continue;
            }

            if(!strcmp(row[0],"app_game"))
            {
                if(mysql_query(sock,GET_APP_GAME_list))
                {
                    printf("Couldn't get %s\n",GET_APP_GAME_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                //row[0]变为网站
                while((row=mysql_fetch_row(res2)))
                {
                    printf("全局阻断GAME:----%s\n",*row);
                    g_hash_table_insert(hash_table,g_strdup(row[0]),g_strdup(value));
                }
                mysql_free_result(res2);
                continue;
            }

            if(!strcmp(row[0],"app_other"))
            {
                if(mysql_query(sock,GET_APP_OTHER_list))
                {
                    printf("Couldn't get %s\n",GET_APP_OTHER_list);
                    continue;
                }
                res2=mysql_store_result(sock);
                //row[0]变为网站
                while((row=mysql_fetch_row(res2)))
                {
                    printf("全局阻断other app:----%s\n",*row);
                    g_hash_table_insert(hash_table,g_strdup(row[0]),g_strdup(value));
                }
                mysql_free_result(res2);
                continue;
            }


        }
        else
        {
            //单个app
            printf("全局阻断app  %s\n",row[1]);
            g_hash_table_insert(hash_table, g_strdup(row[1]), g_strdup(value));
        }

    }
    mysql_free_result(res);
//    mysql_free_result(res2);
    return 0;   //
}
