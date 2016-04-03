#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct ad_list  *lshdr;
void display(AList *lst)
{
    struct ANode *phdr=lst->head;
    while(phdr)
    {
        if(phdr->dip)
        printf("FIP %s\n",phdr->dip);
        if(phdr->data)
        printf("F data %s \n",phdr->data);
        phdr=phdr->next;
    }
}
/* 1.初始化线性表，即置单链表的表头指针为空 */
void initList()
{
   lshdr =(struct ad_list * ) malloc(sizeof(struct ad_list));
   lshdr->head = NULL;
   lshdr->len=0;
  //  printf("initList函数执行，初始化成功\n");
}

/* 6.检查单链表是否为空，若为空则返回１，否则返回０ */
int isEmptyList(AList *lst)
{
    struct ANode *pHead = lst->head;
    if(pHead == NULL)
    {
       // printf("isEmptyList\n");
        return 1;
    }
   // printf("NOT EmptyList\n");
    return 0;
}
 int Insert_ip_to_List(AList *lst , char * xip)
 {
        //防止重复节点//
        struct ANode *node ;
        node =getElemNode(lst, xip);
        if(node)
        {
            return 1;
            printf("Found in 31 line\n");
        }

        node = (struct ANode *)malloc(sizeof(struct ANode ));
        if(!node)
        return 0;

        memset(node->dip,0,12);
        strcpy(node->dip,xip);
        memset(node->data,0,128);
        node -> next = lst->head ;
        lst->head = node;
//        //display(Head);
//        if(getElemNode(Head,xip))
//        {
//            printf("success insert\n");
//        }
        return 1;
 }
 int Insert_f_to_list(AList *lst,char *dip,char *fbuff)
 {
     struct ANode *pHead=NULL;
     pHead = getElemNode(lst,dip);
     if(pHead)
     {
        memset(pHead->data,0,128);
        strcpy(pHead->data,fbuff);
        printf("Here we insert file name \n");
        return 1;
     }
     return 0;
 }

/* 从单链表中查找具有给定值x的第一个元素，若查找成功则返回该结点的存储地址，否则返回NULL */
struct ANode *getElemNode(AList *lst, char *xip)
{
    struct ANode *pHead;
    pHead = lst->head;
    if(NULL == pHead)
    {
       // printf("getElemAddr函数执行，链表为空\n");
        return NULL;
    }
    if(xip  == NULL)
    {
       // printf("getElemAddr函数执行，给定值X不合法\n");
        return NULL;
    }
//(pHead->element != x)//
    while( (strcmp(pHead->dip,xip)!=0) && (NULL != pHead->next)) //判断是否到链表末尾，以及是否存在所要找的元素
    {
        pHead = pHead->next;
    }
    if( (strcmp(pHead->dip,xip)!=0) && (pHead != NULL))
    {
      //  printf("getElemAddr函数执行，在链表中未找到x值\n");
        return NULL;
    }
    return pHead;//返回元素的地址
}



/* 删除一个节点 */
int  DelElemNode(AList *lst, char *xip)
{
    struct ANode *pHead,*pev;
    pHead = lst->head;
    if(NULL == pHead)
    {

        return 0;
    }
    if(strcmp(pHead->dip,xip)==0)
    {
        lst->head = (lst->head) ->next;
        free(pHead);
        pHead = NULL;
        return 1;
    }
    while(  (strcmp(pHead->dip,xip)!=0)  && (NULL != pHead->next) ) //判断是否到链表末尾，以及是否存在所要找的元素
    {
        pev = pHead;
        pHead = pHead->next;
    }
    if( (strcmp(pHead->dip,xip)!=0)  && (pHead != NULL))
    {
      //  printf("getElemAddr函数执行，在链表中未找到x值\n");
        return 0;
    }
    pev ->next = pHead->next;
    free(pHead);
    pHead=NULL;
    return 1;//返回元素的地址
}
int desylist()
{
    struct ANode * ptr,*sss;
    ptr = lshdr->head;
    while(ptr)
    {
        sss=ptr->next;
        free(ptr);
        ptr = sss;
    }
    ptr = NULL;
    sss = NULL;
    free(lshdr);
    lshdr = NULL;
    return 1;
}
