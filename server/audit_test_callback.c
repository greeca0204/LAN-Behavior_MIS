#include <arpa/inet.h>
#include <zlib.h>
#include "fbstat.h"
#include "tcp_cbna.h"
#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#include <iconv.h>
#include <ctype.h>
#include "public.h"
int hexToInt (char c)
{
    if (c == 'A' || c == 'a')
        return 10;
    else if (c == 'B' || c == 'b')
        return 11;
    else if (c == 'C' || c == 'c')
        return 12;
    else if (c == 'D' || c == 'd')
        return 13;
    else if (c == 'E' || c == 'e')
        return 14;
    else if (c == 'F' || c == 'f')
        return 15;
    return (c-'0');
}
int email_content_iconv (char *args)
{
    if (strchr(args, '%') == NULL    )
    {
        return 1;
    }

    char buf[1024*128] = {'\0'};
    char *p = args;
    int i = 0;
L1:
    while (*p &&( *p != '%'  ))
        *(buf + i++) = *p++;
    if (! *p)
        goto L2;
    p++;
    *(buf + i++) = (char)(hexToInt(*p++) * 16 + hexToInt(*p++));
    goto L1;
L2:
    *(buf + i) = '\0';

    strcpy(args,buf);

    return 1;

}


int code_convert_email(char *keyword)
{
    int len_start,i;
    char hex_file_name[1024*100];
    memset(hex_file_name,0,1024*100);
    len_start=0;
    for(  i=0; i<strlen(keyword); i++  )//01name.jpg
    {

        if(isprint(keyword[i]))
        {
            hex_file_name[len_start]=keyword[i];
            len_start++;
        }
        else
        {
            char a[3];
            sprintf(a,"%%%X",(unsigned char)keyword[i]);
            strcat(hex_file_name,a);
            len_start=len_start+3;
        }
    }

    // printf(" %d  #################    %s\n",strlen(keyword),hex_file_name);

    memset(keyword,0,strlen(keyword));
    strcpy(keyword,hex_file_name);

    email_content_iconv(keyword);
    return 1;
}



int num_tcp=1;
//char fsbname[100]= {"attch/aaa"};

Byte zip_destbuff[100000];
const unsigned long iDLen = 100000;
int UnzipHttpFile(Byte *s,int iSLen,unsigned long *knlen/*,Byte *d,unsigned long * iDLen*/)
{
    int err ;
    z_stream d_stream;

    memset(&d_stream,0,sizeof(z_stream));
    memset(zip_destbuff,0,sizeof(zip_destbuff));
    d_stream.next_in = s;
    d_stream.avail_in = iSLen;
    d_stream.next_out =zip_destbuff;
    d_stream.avail_out = iDLen;

    err = inflateInit2(&d_stream,-MAX_WBITS);
    if(err != Z_OK)
        return err ;
    d_stream.next_in += 10L;
    err = inflate(&d_stream,Z_FINISH);
    if(err != Z_STREAM_END)
    {
        inflateEnd(&d_stream);
        return err == Z_OK ? Z_BUF_ERROR:err;
    }
    *knlen = d_stream.total_out;
    err = inflateEnd(&d_stream);
    return err;
}

//int wte_file(char *desbuff,int len)
//{
//    //暂时先这样放着//
//    FILE *fp;
//    strcat(fsbname,"1");
//    fp=fopen(fsbname,"wb");
//    if(!fp)
//    {
//        printf("cant open file\n");
//        sleep(2);
//        exit(0);
//    }
//    fwrite(desbuff,len,1,fp);
//    fclose(fp);
//    return 1;
//}


int adget_content_lenght(struct half_stream *svb)
{
//    struct half_stream *cs,*ss;
//    ss = &a_tcp->server;
//    cs = &a_tcp->client;
    int sum=0;
    char *ssh=NULL,*ssg=NULL;
    ssh = strstr(svb->data,"Content-Length: ");
    if(ssh)
    {
        ssg=strstr(ssh,": ");
        ssg=ssg+2;
        if(ssg)
            ssh=strstr(ssg,"\x0d\x0a");
        //  printf("%d\n",gg-ssg);
        //把字符串数字转化为数据串//
        int ysb;
        //我抓到很多0的 content length ,但一次都没有出现过，这代码可能有bug//
        int val=1;
        for(ysb=ssh-ssg-1; ysb>=0; ysb--)
        {
            sum+=val* (*(ssg+ysb)-48);
            val*=10;
        }
        printf("SUM %d \n",sum);
    }
    return sum;
}

int dect_qm_attchment(struct tcp_stream *a_tcp)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    char *ssg=NULL,*ssh=NULL;
    char fname[200];
    memset(fname,0,sizeof(fname));

    ssg = strstr(cs->data,"Content-Disposition: attachment;"); //没有检查data 是否有足够的长度,函数外检查//
    if(ssg) //附件下载标准字眼
    {
        char *ssb1 =NULL;
        printf("Found attchment\n");
        ssb1 = strstr(cs->data,"filename=\"");

////        //TEST-----------//
////        FILE *fp;
////        fp = fopen("attch//TEST.txt","wb+");
////        fwrite(cs->data,cs->count_gd,1,fp);
////        fclose(fp);
////        //---------------------------//

        if(ssb1)
        {
            ssb1 += 10;
            char *ssb2 =NULL;
            ssb2 = strstr(ssb1,"\"\x0d\x0a");

            int irt;
            for(irt=0; irt<ssb2-ssb1; irt++)
                fname[irt] = *(ssb1+irt);
            fname[irt]=0;

        }
        else
        {
            ssb1 = memmem(cs->data,cs->count_gd,"filename*=UTF-8''",17);
            if(ssb1)
            {
                ssb1 += 17;
                char *ssb2 =NULL;
                ssb2 = strstr(ssb1,"\x0d\x0a");

                int irt;
                for(irt=0; irt<ssb2-ssb1; irt++)
                    fname[irt] = *(ssb1+irt);
                fname[irt]=0;
            }
        }

        if(ssb1)
        {
            printf("attchment down load Fname %s\n",fname);
            ssh = NULL;
            ssh = strstr(cs->data,"Content-Length: ");
            if(ssh)
            {
                ssg=strstr(ssh,": ");
                ssg=ssg+2;
                if(ssg)
                    ssh=strstr(ssg,"\x0d\x0a");
                //  printf("%d\n",gg-ssg);
                //把字符串数字转化为数据串//
                int ysb;
                int sum=0;
                int val=1;
                for(ysb=ssh-ssg-1; ysb>=0; ysb--)
                {
                    sum+=val* (*(ssg+ysb)-48);
                    val*=10;
                }
                printf("SUM %d \n",sum);
                if(sum<cs->count_gd)
                {


                    //开始写文件//
                    ssg= strstr(ssh,"\x0d\x0a\x0d\x0a");
                    if(ssg)
                    {
                        ssg+=4;
                        char fnamebuff[1024];
                        memset(fnamebuff,0,1024);
                        strcat(fnamebuff,"auditFile/download/");
                        strcat(fnamebuff,fname);
                        FILE *fp;
                        fp=fopen(fnamebuff,"wb+");
                        if(!fp)
                        {
                            printf("Can't open file \n");
                            sleep(10);
                        }
                        //sleep(10);
                        fwrite(ssg,sum,1,fp);         //size_t == unsigned long
                        fclose(fp);

                        char sip[17];
                        memset(sip,0,sizeof(sip));
                        struct in_addr faddr;
                        faddr.s_addr = (in_addr_t) a_tcp->addr.saddr;
                        strcpy(sip,inet_ntoa(faddr));

                        //sql语句//
                        audit_insert_down_attch(sip,fnamebuff);
                        //int audit_insert_down_attch(char *sip,char *path);
                        return 1;
                    }
                }
            } //ssh
        }//ssb1

    }//ssg = strstr(cs->data,"Content-Disposition: attachment;");
    return 0;
}
int dect_qm_upload(struct tcp_stream *a_tcp)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    if(ss->count_gd > 30)
    {
        char *ssb1=NULL;
        ssb1 = strstr(ss->data,"POST /cgi-bin/uploadfile");
        if(ssb1)
        {
            //以后可以把里面的内容封装成函数//
            //printf("file upload\n");
            char *ssb2 =NULL;
            ssb2 = strstr(ssb1,"mail.qq.com");
            if(ssb2) //QQ邮箱附件上传//
            {
                //printf("mail.qq.com\n");
                ssb1 =NULL;
                ssb1 = strstr(ssb2,"filename=\"");
                if(ssb1)
                {
                    ssb1 += 10;
                    ssb2 = strstr(ssb1,"\"\x0d\x0a");
                    char fname[200];
                    int irt;
                    for(irt=0; irt<ssb2-ssb1; irt++)
                        fname[irt] = *(ssb1+irt);
                    fname[irt]=0;
                    //printf("%s\n",fname);
                    //printf("filename\n");
                    char *ssg,*ssh;
                    ssg=NULL;
                    ssh=NULL;
                    ssg = strstr(ssb2,"\x0d\x0a\x0d\x0a");
                    ssg+=4;
                    ssh = memmem(ssb2,(ss->count_gd - (ssb2 -ss->data)),"\x0d\x0a\x2d\x2d\x2d",5);
                    if(ssh)
                    {
                        char fnamebuff[1024];
                        memset(fnamebuff,0,1024);
                        strcat(fnamebuff,"auditFile/upload/");
                        strcat(fnamebuff,fname);


                        FILE *fp;
                        fp=fopen(fnamebuff,"wb+");
                        if(!fp)
                        {
                            printf("Can't open file \n");
                            sleep(10);
                        }
                        unsigned long  lll= (gulong)(ssh-ssg);
                        //uint64_t hhh = (uint64_t)lll;
                        printf("File size %ld\n",lll);
                        //sleep(10);
                        fwrite(ssg,lll,1,fp);         //size_t == unsigned long
                        fclose(fp);

                        //sql语句//
                        char sip[17];
                        memset(sip,0,sizeof(sip));
                        struct in_addr faddr;
                        faddr.s_addr = (in_addr_t) a_tcp->addr.saddr;
                        strcpy(sip,inet_ntoa(faddr));

                        //sql语句//
                        audit_insert_qm_up_attch(sip,fnamebuff);


                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}


int dect_baidu_tieba(struct tcp_stream *a_tcp)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    char *ssb1=NULL,*ssb2=NULL;
    if(ss->count_gd)
    {
        ssb1 = strstr(ss->data,"POST /f/commit/thread/add");
        if(ssb1)
        {
            printf("----------POST /f/commit/thread/add \n");

            ssb2 = strstr(ssb1,"Host: tieba.baidu.com");
            if(ssb2)
            {
                int sum =adget_content_lenght(ss);
                if(sum>0)
                {
                    ssb1 = NULL;
                    //charter set //
                    ssb2 =NULL ;
                    ssb2 = strstr(ss->data,"charset=UTF-8");
                    ssb1 = strstr(ss->data,"\x0d\x0a\x0d\x0a");
                    if(ssb1 && ssb2)
                    {


                        char sip[17];
                        memset(sip,0,sizeof(sip));
                        struct in_addr faddr;
                        faddr.s_addr = (in_addr_t) a_tcp->addr.saddr;
                        strcpy(sip,inet_ntoa(faddr));


                        char title[512];
                        memset(title,0,sizeof(title));
                        char content[1000];
                        memset(content,0,sizeof(content));

                        char host[100]= {"tieba.baidu.com"};



                        ssb1+=4;
                        if(ss->count_gd >= (ssb1 -ss->data) )
                        {
                            ssb2 =NULL;
                            ssb2 = memmem(ssb1,sum,"&title=",7);
                            if(ssb2)
                            {

                                char *ssb3=NULL;
                                ssb2 += 7;
                                int dis = ssb2 -ssb1;
                                //可以拿到讨论组的名字，到时再看看数据库需不需要在进行编程//
                                //kw = %%%%%//

                                ssb3 = memmem(ssb2,sum-dis,"&content=",9);
                                //读取标题标题  ssb1 - ssb2//
                                if(ssb3)
                                {
//                                    char sbsssbuff[2000];
//                                    memset(sbsssbuff,0,sizeof(sbsssbuff));
                                    int iii;
                                    for(iii=0; iii<(ssb3-ssb2); iii++)
                                        title[iii]=*(ssb2+iii);
                                    title[iii]=0;
                                    code_convert_email(title);  //sbsssbuff里面已经有了tiltle utf8



                                    //题目 utf 8 转化后写入数据库即可
                                    //wte_file(title,strlen(title));

                                    ssb3 += 9; //ssb1 指向content开头
                                    ssb2 =NULL;
                                    ssb2 =memmem(ssb1,sum,"&anonymous",10);
                                    if(ssb2)
                                    {
//                                        //wte_file(ssb3,ssb2-ssb3);
//                                        memset(sbsssbuff,0,sizeof(sbsssbuff));

                                        for(iii=0; iii<ssb2-ssb3; iii++)
                                            content[iii]=*(ssb3+iii);
                                        content[iii]=0;

//                                        printf("%s\n",sbsssbuff);
//                                        sleep(10);
//                                        wte_file(sbsssbuff,strlen(sbsssbuff));
                                        code_convert_email(content);

                                        //内容 utf 8 转化后写入数据库即可
                                        //wte_file(content,strlen(content));

                                        printf("%s\n",sip);
                                        audit_insert_BD_tieba(sip,title,content,host);

                                        // int audit_insert_BD_tieba(char *sip,char *title,char *content,char *host)  //url? host?


                                    }

                                }
                                else
                                {
                                    printf("-------------we cant found &content=\n");
                                    //sleep(10);
                                }
                            }
                            else
                            {
                                printf("-------------we cant found &prefix=&title==\n");
                                //sleep(10);
                            }

                        }
                    }
                }
            }
        }
    }
    return 0;
}

int dect_qmread_gzip(char *desbuff,int len,struct tcp_stream *a_tcp )
{
    char *ssb1=NULL;
    ssb1 = memmem(desbuff,160,"\x51\x51\xd3\xca\xcf\xe4\x3c\x2f\x74\x69\x74\x6c\x65\x3e",14);
    if(ssb1)
    {
        //        ssb2 = strstr(ssb1,"\x0d\x0a");
        //ssb2 = memmem(desbuff,len,"\x0d\x0a\x64\x6f\x63\x75\x6d\x65\x6e\x74\x2e\x64\x6f\x6d\x61\x69\x6e\x3d\x22\x71\x71\x2e\x63\x6f\x6d\x22\x3b\x0d\x0a",29);
//        ssb2 = memmem(ssb1,40,"\x0d\x0a\x64\x6f\x63\x75\x6d\x65\x6e\x74",10);
//
//        if(ssb2)
//        {
//            ssb1=NULL;
//            ssb1=memmem(ssb2,20,"\x2e\x64\x6f\x6d\x61\x69\x6e\x3d\x22\x71\x71\x2e\x63\x6f\x6d\x22\x3b",17);
//            if(ssb1)
//            {
//                printf("-----------------FOUND QQMAIL ------------------\n");
        //暂时先这样放着//
        FILE *fp=NULL;
        char sip[17];
        memset(sip,0,sizeof(sip));
        struct in_addr faddr;
        faddr.s_addr = (in_addr_t) a_tcp->addr.saddr;
        strcpy(sip,inet_ntoa(faddr));

        //文件命名方式   ip_时间

        time_t ttt;
        ttt = time(NULL);
        struct tm  *stm = localtime(&ttt);

        char vtme[40];
        memset(vtme,0,sizeof(vtme));

        strftime(vtme,sizeof(vtme),"-%F-%R",stm);

        char filepath[100];
        memset(filepath,0,sizeof(100));
        strcat(filepath,"auditFile/email_read/");
        strcat(filepath,sip);
        strcat(filepath,vtme);
        strcat(filepath,".html");

//        strcat(fsbname,"1");
        fp=fopen(filepath,"wb");
        if(!fp)
        {
            printf("cant open file\n");
        }
        else
        {
            fwrite(desbuff,len,1,fp);

            fclose(fp);
        }

        //mysql处理
        //audit_insert_qm_gzip_browse(char *sip,char *path);

        audit_insert_qm_gzip_browse(sip,filepath);
//            }

////            return 1;
//        }
    }
    return 1;
}

int dect_http_url(struct tcp_stream *a_tcp)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    if(ss->count_gd>30)
    {
        char *get=NULL,*url=NULL;
        char userip[17];
        memset(userip,0,17);
        get = memmem(ss->data,ss->count_gd,"GET",3);
        if(get)
        {
            url=http_get_info(ss->data,"Referer",16);
            if(url)
            {
                struct in_addr faddr;
                memset(&faddr,0,sizeof(struct in_addr));
                faddr.s_addr = (in_addr_t)a_tcp->addr.saddr;
                strncpy(userip,inet_ntoa(faddr),sizeof(userip));
                userip[16]=0;
                printf("ip %s  url %s\n",userip,url);

                audit_insert_http_url(userip,url);

                return 1;
            }

        }

    }
    return 0;

}

int dect_gzip(struct tcp_stream *a_tcp,int clght)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    //测试一下转换函数是否能够成功转换//
    char *ssg,*ssb2;
    if(clght <= cs->count_gd && clght > 0)
    {
        //char *dnf;
        ssg = strstr(cs->data,"Content-Type: text/html;");
        ssb2 = strstr(cs->data,"Content-Encoding: gzip");
        //dnf = strstr(cs->data,"Domain=mail.qq.com;");
        if(ssg && ssb2 /*&& dnf*/) //text html 文本 并且被gzip 加压//
        {
            ssg = strstr(cs->data,"\x0d\x0a\x0d\x0a");
            if(ssg)
            {
                printf("Content-Type: text/html;    Content-Encoding: gzip  \n");
                ssg+=4;
                unsigned long llls =0;
                Byte *asdf =(Byte *) ssg;
                //int slen = (int )(cs->count_gd - (ssg - cs->data));
                //int UnzipHttpFile(Byte *s,int iSLen,Byte *d,unsigned long * iDLen)
                int err = UnzipHttpFile(asdf, clght,&llls); //全局zip_buff 储存了解压结果
                if(err ==  Z_OK)
                {
                    printf("Z_OK \n");
                    printf("---------------------After unzip ,len = %ld\n",llls);
                    //printf("%s\n",zip_destbuff);
                    dect_qmread_gzip((char *)zip_destbuff,llls,a_tcp);

//                    //尝试一下打印到文件看看
//                    FILE *fp;
//                    strcat(fsbname,"1");
//                    fp=fopen(fsbname,"wb");
//                    if(!fp)
//                    {
//                        printf("cant open file\n");
//                    }
//                    fwrite(zip_destbuff,llls,1,fp);
//                    fclose(fp);
                    return 1;
//////                                    sleep(5);
//////                                    exit(0);
                    //
                }
                else if (err == Z_BUF_ERROR)
                {
                    printf("---------------Z_BUF_ERROR\n");
                }
                else
                {
                    printf("---------------UNKNOWN Z_ERR %d \n",err);
                }

                return 1;   //找到gzip 关键字 即正确//
            }
        }
    }
    return 0;
}

int dect_baidu_search(struct tcp_stream *a_tcp)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;
    if(ss->count_gd>30)
    {
        char *ssb=NULL,*bbs=NULL;
        bbs = memmem(ss->data,ss->count_gd,"Host: www.baidu.com",19);
        if(bbs)
        {
//            printf("BD _Search\n");
//            sleep(10);
            ssb = strstr(ss->data,"wd=%");
            if(ssb)     //然后拿字符串
            {
                ssb+=3;
                bbs=NULL;
                bbs = strstr(ssb,"&rsv");
                if(bbs)
                {
//                    printf("%s\n",ssb);
//                    sleep(4);
                    char sbsssbuff[2000];
                    memset(sbsssbuff,0,sizeof(sbsssbuff));
                    int iii;
                    for(iii=0; iii<(bbs - ssb); iii++)
                        sbsssbuff[iii] = *(ssb+iii);
                    //内容 utf 8 转化后写入数据库即可
                    // wte_file(sbsssbuff,strlen(sbsssbuff));
                    code_convert_email(sbsssbuff);
                    // wte_file(sbsssbuff,strlen(sbsssbuff));
//                    printf("BD _Search\n");
//                    sleep(10);
                    //search

                    char sip[17];
                    memset(sip,0,sizeof(sip));
                    struct in_addr faddr;
                    faddr.s_addr = (in_addr_t) a_tcp->addr.saddr;
                    strcpy(sip,inet_ntoa(faddr));

                    audit_insert_BD_sech(sip,sbsssbuff);

                    return 1;
                }
            }
        }
    }
    return 0;
}


void http_test_callback(struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
    struct half_stream *cs,*ss;
    ss = &a_tcp->server;
    cs = &a_tcp->client;

    if (a_tcp->nids_state == NIDS_JUST_EST)
    {
        if(a_tcp->addr.dest == 80)
        {
            //---------//
            a_tcp->tcp_num = num_tcp;
            num_tcp ++;
            //---------//
            a_tcp->client.collect++; // we want data received by a client
            a_tcp->server.collect++; // and by a server, too
        }
        return;
    }
    if (a_tcp->nids_state == NIDS_DATA)
    {
        nids_discard(a_tcp,0);
        if(ss->count_new)
        {
            ss->count_gd += ss->count_new;

////            printf("Server Count_new %d\n",ss->count_new);
////            printf("Server Count %d\n",ss->count_gd);
//                   // if(ss->count_new)
            //测试结果表明 流确实存在，但连接上以后，就会一直使用同一个流
            //我们没有探测出其他流，TCP重组还有可能有漏洞，但最好不要发生//
//            char *ssb1=NULL;
//            ssb1 = strstr(ss->data,"POST /f/commit/thread/add");
//            if(ssb1)
//            {
//                //printf("----------POST /f/commit/thread/add \n");
//                wte_file(ss->data,ss->count_gd);
//            }

        }
        if(cs->count_new)
        {

//            printf("Client Count_new %d\n",cs->count_new);
//            printf("Client Count %d\n",cs->count_gd);
            char *ssb1,*ssb2;
            ssb1 = strstr(cs->data+cs->count_gd,"200 OK");
            int flag=0;
            if(ssb1)
            {
                printf("200OK\n");

                int alen = adget_content_lenght(cs);
                if(alen != 0)   //做内容检测
                {

                    ssb2 = memmem(cs->data,cs->count_gd,"Content-Type:",13);
                    //ssb1 = memmem(ssb2,(cs->count_gd - (ssb2 - cs->data))-1,"\x0d\x0a",2);
                    if(ssb2)
                    {
                        ssb1 = strstr(ssb2,"\x0d\x0a");
                        printf("------------");

                        dect_http_url(a_tcp);
                        //ss->data//

                        int i;
                        for(i=0; i<ssb1-ssb2; i++)
                            printf("%c",*(ssb2+i));
                        printf("\n");
                        if(!flag)
                            flag = dect_baidu_search(a_tcp);
                        if(!flag)
                            flag = dect_gzip(a_tcp,alen); //qqmail 也在里面探寻了//
                        if(!flag)
                            flag = dect_qm_attchment(a_tcp);

                    }
                }
                else
                {
//                    char *yyy1=NULL;
//                    yyy1 = memmem(ss->data,ss->count_gd,"POST",4);
//                    if(yyy1)
//                        wte_file(ss->data,ss->count_gd);
                    flag = dect_baidu_tieba(a_tcp);
                    if(!flag)
                        flag = dect_qm_upload(a_tcp);
                }



                //nids_discard(a_tcp,cs->count_gd);  //这里没有把新来的 200 ok 清除掉的//
                //清空服务器流

                memmove(ss->data, ss->data + ss->count_gd, ss->count - ss->offset - ss->count_gd);
                ss->offset += ss->count_gd;
                ss->count_gd = 0;



                memmove(cs->data,cs->data + cs->count_gd,cs->count - cs->offset - cs->count_gd );
                cs->offset += cs->count_gd;
                cs->count_gd = 0;

            }
            cs->count_gd += cs->count_new;
        }


        return ;
    }
    else
    {
//        if (a_tcp->nids_state == NIDS_RESET)
//        {
//            printf("RESET ---%d \n",a_tcp->tcp_num);
//        }
//        if (a_tcp->nids_state == NIDS_TIMED_OUT)
//        {
//            printf("NIDS_TIMED_OUT ---%d \n",a_tcp->tcp_num);
//        }
//        if (a_tcp->nids_state == NIDS_EXITING)
//        {
//            printf("NIDS_EXITING ---%d \n",a_tcp->tcp_num);
//        }
//        if(a_tcp->nids_state == NIDS_CLOSE)
//        {
//            printf("NIDS_CLOSE  --- %d \n",a_tcp->tcp_num);
//        }
        if(cs->count_gd>0)
        {
            char *ssb1;
            ssb1 = strstr(cs->data,"200 OK");
            if(ssb1)
            {
                printf("DNANANANA-  --------- -- -200OK\n");

                int alen = adget_content_lenght(cs);
                if(alen != 0)   //做内容检测
                {
                    int flag=0;

                    if(!flag)
                        flag = dect_baidu_search(a_tcp);
                    if(!flag)
                        flag = dect_qm_attchment(a_tcp);
                    if(!flag)
                        flag = dect_baidu_tieba(a_tcp);
                    if(!flag)
                        flag = dect_qm_upload(a_tcp);
//                    if(!flag)
//                        flag = dect_gzip(a_tcp,alen);
                }
            }
        }

    }
    return;
}




















