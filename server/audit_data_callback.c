#include "tcp_cbna.h"
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <arpa/inet.h>
//#include <netinet/in.h>
#include "list.h"
#include "fbstat.h"



////HTTP AUDIT CALLBACK//
//void http_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
//{
//    if (a_tcp->nids_state == NIDS_JUST_EST)
//    {
//////        printf("NIDS_JUST_EST!\n");
//        if(a_tcp->addr.dest == 80)
//        {
//            a_tcp->client.collect++; // we want data received by a client
//            a_tcp->server.collect++; // and by a server, too
//        }
//    }
//    if (a_tcp->nids_state == NIDS_CLOSE)
//    {
//        printf("NIDS_CLOSE last time to callback;\n");
//    }
//    if (a_tcp->nids_state == NIDS_DATA)
//    {
//        //判断//
//        //printf("DATA FLOW\n");
//        //int save_flag=0;
//        struct half_stream *cs,*ss;
//        ss = &a_tcp->server;
//        cs = &a_tcp->client;
//        char *gg,*ssg;
//        if(cs->data)
//        {
//            if(a_tcp->filesize)  //已经知道了文件信息不用探寻
//            {
//                //这个判断条件回因为第二次下载而失效//
//                if(a_tcp->filesize==cs->count-a_tcp->filestart)
//                {
//                    printf("Here we have save all file info \n");
//                    printf("THIS PLACE ,We Will Start File Process\n");
//
//                    FILE *fp;
//                    char fnamebuff[1024];
//                    memset(fnamebuff,0,1024);
//                    strcat(fnamebuff,"attch/");
//                    strcat(fnamebuff,a_tcp->filename);
//                    fp=fopen(fnamebuff,"w+");
//                    if(!fp)
//                    {
//                        printf("Can't open file \n");
//                    }
//                    fwrite((a_tcp->client).data,(size_t)(a_tcp->filesize),
//                           1,fp);
//                    fclose(fp);
//                    memset(a_tcp->filename,0,sizeof(a_tcp->filename));
//                    free(a_tcp->filename);
//                    nids_discard(a_tcp,a_tcp->filesize);
//                    a_tcp->filename=NULL;
//                    a_tcp->filesize=0;
//                    return;
//                }
//                nids_discard(a_tcp,0);
//                return ;
//            }
//            //检查服务器流里面有没有关键字
//            if(a_tcp->filename==NULL)
//            {
//                gg=strstr(cs->data,"Content-Length: ");
//                if(gg)
//                {
//                    ssg=strstr(gg,": ");
//                    ssg=ssg+2;
//                    if(ssg)
//                        gg=strstr(ssg,"\x0d\x0a");
//                    //  printf("%d\n",gg-ssg);
//                    //把字符串数字转化为数据串//
//                    int ysb;
//                    int sum=0;
//                    int val=1;
//                    for(ysb=gg-ssg-1; ysb>=0; ysb--)
//                    {
//                        sum+=val* (*(ssg+ysb)-48);
//                        val*=10;
//                    }
//                    printf("SUM %d \n",sum);
//
//                    //保留数据段//
//                    a_tcp->filesize=sum;
//                    gg=strstr(ssg,"\x0d\x0a\x0d\x0a");
//                    gg=gg+4;
//                    //printf("报文结尾: %d \n",gg - cs->data);
//                    //printf("Count %d \n",cs->count);
//                    a_tcp->filestart=gg - cs->data;
//                    nids_discard(a_tcp,gg-cs->data);
//                }
//                else return;    //找不到长度就不用再找了//
//
//                gg=strstr(cs->data,"filename=\"");
//                if(gg)
//                {
//                    char *ssb1,*ssb2;
//                    ssb1=strstr(gg,"\"");
//                    ssb2=strstr(ssb1+1,"\"");
//                    a_tcp->filename = malloc(1024);
//                    memset(a_tcp->filename,0,1024);
//                    //printf("SSB  %d\n",ssb2-ssb1);
//                    int irt;
//                    for(irt=0; irt<ssb2-ssb1-1; irt++)
//                    {
//
//                        a_tcp->filename[irt]=*(ssb1+irt+1);
//                    }
//                    printf("----------CFILENAME:%s\n",a_tcp->filename);
//
//                }
//                else //找到了长度 找不到名字
//                {
//                    a_tcp->filesize=0;
//                    a_tcp->filestart=0;
//                }
//            }
//        }
//    }
//    if (a_tcp->nids_state == NIDS_RESET)
//    {
//        printf("RESET\n");
//    }
//    if (a_tcp->nids_state == NIDS_TIMED_OUT)
//    {
//        printf("NIDS_TIMED_OUT\n");
//    }
//    if (a_tcp->nids_state == NIDS_EXITING)
//    {
//        printf("NIDS_EXITING\n");
//    }
//    return ;
//}
/////////////////////////////////////////

extern struct ad_list * lshdr;

void ftp_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
    if (a_tcp->nids_state == NIDS_JUST_EST)
    {
        //printf("NIDS_JUST_EST!\n");
        if(a_tcp->addr.dest == 21)  //暂时先默认端口先
        {
            a_tcp->client.collect++; // we want data received by a client
            a_tcp->server.collect++; // and by a server, too
        }
    }
    if (a_tcp->nids_state == NIDS_RESET)
    {
        printf("RESET\n");
    }
    if (a_tcp->nids_state == NIDS_CLOSE)
    {
        printf("NIDS_CLOSE last time to callback;\n");
    }
    if (a_tcp->nids_state == NIDS_DATA)
    {
        //---------------//
//        struct in_addr faddr;
//        faddr.s_addr = (in_addr_t) a_tcp->addr.daddr;
//       // printf("%s\n",inet_ntoa(faddr))  ;
//        char fip[12];
//        memset(fip,0,12);
//        strcpy(fip,inet_ntoa(faddr));
//        //printf("FK %s\n",fip);
//        char *ptr= g_hash_table_lookup(audit_stream_table,fip);
//        if(ptr)//如果哈希表中有记录，则保存该流//
//        {
//            printf("sucessfully found\n");
////            a_tcp->client.collect++; // we want data received by a client
////            a_tcp->server.collect++; // and by a server, too
//        }
        //--------------//


        //printf("NIDS_DATA\n");

        struct half_stream *cs,*ss;
        ss = &a_tcp->server;
        cs = &a_tcp->client;

        char *ssb,*bbs;
        if(ss->data)
        {
            //printf("SS %s\n",ss->data);
            //PASV  表示要打开数据连接//
            ssb=strstr(ss->data,"PASV");
            if(ssb)
            {
                printf("PASV?\n");
                //先构造IP str//
                struct in_addr faddr;
                faddr.s_addr = (in_addr_t) a_tcp->addr.daddr;
                //printf("%s\n",inet_ntoa(faddr))  ;
                char fip[12];
                memset(fip,0,12);
                strcpy(fip,inet_ntoa(faddr));
                //开始写入哈希表//
                //有篇博文说 如果 值已有，则替换值，并不影响键的插入，因此直接这样使用就行了//
                Insert_ip_to_List(lshdr,fip);
                Insert_f_to_list(lshdr,fip,"DATA");
//                struct Node *sb;
//                sb = getElemNode(lshdr,fip);
//                if(sb)
//                {
//                    printf("sucess insert \n");
//                }
//                display(lshdr);
            }
            //size 是查询文件大小//
            ssb=strstr(ss->data,"SIZE ");
            if(ssb)
            {
                ssb+=5;
                bbs=strstr(ssb,"\x0d\x0a");
                if(!bbs)
                    return ;
                a_tcp->filename = malloc(64);
                memset(a_tcp->filename,0,64);
                int isb;
                for(isb=0; isb<bbs-ssb; isb++)
                    a_tcp->filename[isb] = *(ssb+isb);
                //printf("Fname %s \n",a_tcp->filename);
                //清调服务器里面的数据//
            }
            memset(ss->data,0,ss->count_new);
        }
        if(cs->data)
        {
            if(!a_tcp->filename)    //没找到名字，直接返回//
            {
                memset(cs->data,0,cs->count_new);
                return;
            }

            ssb=strstr(cs->data,"213 ");
            if(ssb)
            {
//                printf("%s\n",cs->data);
//                printf("CNT %d\n",cs->count_new);
                ssb+=4;
                bbs=strstr(ssb,"\x0d\x0a");
                if(!bbs)
                    return ;

                char sizebuff[30];
                int ist;
                for(ist=0; ist<bbs-ssb; ist++)
                    sizebuff[ist]=*(ssb+ist);
                sizebuff[ist]=0;
                //printf("size %s\n",sizebuff);
                char filebuff[100];
                memset(filebuff,0,100);
                strcat(filebuff,a_tcp->filename);
                strcat(filebuff,"\x0d\x0a");
                strcat(filebuff,sizebuff);
                strcat(filebuff,"\x0d\x0a");
                //printf("Filebuff %s\n",filebuff);

                //先构造IP str//
                struct in_addr faddr;
                faddr.s_addr = (in_addr_t) a_tcp->addr.daddr;
                //printf("%s\n",inet_ntoa(faddr))  ;
                char fip[12];
                memset(fip,0,sizeof(fip));
                strcpy(fip,inet_ntoa(faddr));

                //开始写入哈希表//
                //g_hash_table_insert(audit_stream_table,fip,filebuff);
                if((Insert_f_to_list(lshdr,fip,filebuff))==0)
                    printf("NO SUC found and insert \n");

//                //----//
//                char *fpk;
//                fpk = g_hash_table_lookup(audit_stream_table,fip);
//                printf("WHY? %s \n",fpk);
//                //----//
                memset(a_tcp->filename,0,64);
                free(a_tcp->filename);
                a_tcp->filename=NULL;

            }
            memset(cs->data,0,cs->count_new);
        }
    }
    if (a_tcp->nids_state == NIDS_TIMED_OUT)
    {
        printf("NIDS_TIMED_OUT\n");
    }
    if (a_tcp->nids_state == NIDS_EXITING)
    {
        printf("NIDS_EXITING\n");
    }
    return ;
}

void ftp_data_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
    if (a_tcp->nids_state == NIDS_JUST_EST)
    {
        //printf("ftp_data_callback NIDS_JUST_EST!\n");
        //用IP来查找是否是ftp流//
        struct in_addr faddr;
        faddr.s_addr = (in_addr_t) a_tcp->addr.daddr;
        // printf("%s\n",inet_ntoa(faddr)) ;
        char fip[12];
        memset(fip,0,12);
        strcpy(fip,inet_ntoa(faddr));
        //printf("FK %s\n",fip);
//        char *ptr;
//        ptr = g_hash_table_lookup(audit_stream_table,fip);
        struct ANode * ptr ;
        ptr= getElemNode(lshdr,fip);
        if(ptr != NULL)//如果链表中有记录，则保存该流//
        {
            printf("FKKKKKKKKKKKKKKKK found\n%s \n",ptr->dip);
            a_tcp->client.collect++;
            a_tcp->server.collect++;
        }
    }
    if (a_tcp->nids_state == NIDS_CLOSE)
    {
        printf("NIDS_CLOSE last time to callback;\n");
    }
    if (a_tcp->nids_state == NIDS_DATA)
    {
        //printf("FTP DATA NIDS_DATA!\n");
        struct half_stream *cs,*ss;
        ss = &a_tcp->server;
        cs = &a_tcp->client;

        // 找文件名//
        struct in_addr faddr;
        faddr.s_addr = (in_addr_t) a_tcp->addr.daddr;
        // printf("%s\n",inet_ntoa(faddr))  ;
        char fip[16];
        memset(fip,0,16);
        strcpy(fip,inet_ntoa(faddr));


        if(a_tcp->filename==NULL)
        {


            //printf("FK %s\n",fip);
            struct ANode *ptr = getElemNode(lshdr,fip);
            char *aaa =ptr->data;
            if(strcmp(aaa,"DATA")==0)
                return ;                //还没有文件名//
            char *ssb,*bbs;
            ssb = strstr(aaa,"\x0d\x0a");
            a_tcp->filename = malloc(32);
            memset(a_tcp->filename,0,32);
            int i;
            for(i=0; i<ssb-aaa; i++)
                a_tcp->filename[i] = *(aaa+i);
            //printf("%s\n",a_tcp->filename);
            ssb+=2;
            bbs = strstr(ssb,"\x0d\x0a");

            int ysb;
            int sum=0;
            int val=1;
            for(ysb=bbs-ssb-1; ysb>=0; ysb--)
            {
                sum+=val* (*(ssb+ysb)-48);
                val*=10;
            }
            //printf("SUM %d \n",sum);
            a_tcp->filesize=sum;
            a_tcp->filestart=0;
        }
        else
        {
            if(cs->count == a_tcp->filesize)
            {
                FILE *fp;
                char fnamebuff[1024];
                memset(fnamebuff,0,1024);
                strcat(fnamebuff,"attch/");
                strcat(fnamebuff,a_tcp->filename);
                fp=fopen(fnamebuff,"w+");
                if(!fp)
                {
                    printf("Can't open file \n");
                    return ;
                }
                fwrite((a_tcp->client).data,(size_t)(a_tcp->filesize),
                       1,fp);
                fclose(fp);

                memset(a_tcp->filename,0,sizeof(a_tcp->filename));
                free(a_tcp->filename);
                nids_discard(a_tcp,a_tcp->filesize);

                //mysql
                audit_insert_down_attch(fip,fnamebuff);

                //删除链表节点//
                DelElemNode(lshdr,fip);

                a_tcp->filename=NULL;
                a_tcp->filesize=0;
                return ;
            }
        }

        nids_discard(a_tcp,0);
    }
    if (a_tcp->nids_state == NIDS_RESET)
    {
        printf("RESET\n");
    }
    if (a_tcp->nids_state == NIDS_TIMED_OUT)
    {
        printf("NIDS_TIMED_OUT\n");
    }
    if (a_tcp->nids_state == NIDS_EXITING)
    {
        printf("NIDS_EXITING\n");
    }
    return ;
}


