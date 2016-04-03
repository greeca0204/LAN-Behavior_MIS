#include "ip_fragment.h"
#include "tcp_cbna.h"


char *
test_malloc(int x)
{
    char *ret = malloc(x);

    if (!ret)
        printf("test_malloc");

    return ret;
}



static struct hostfrags **fragtable;
static struct hostfrags *this_host;
static int numpack = 0;
static int hash_size;
static int timenow;
static unsigned int time0;
static struct timer_list *timer_head = 0, *timer_tail = 0;
int dev_addon=16;
int sk_buff_size=168;
static int timenow;

struct timer_list
{
    struct timer_list *prev;
    struct timer_list *next;
    int expires;
    void (*function)();
    unsigned long data;
    // struct ipq *frags;
};


//每个IP都有IP分片包。这个是每个IP都有的维护ip分片的结构体
struct hostfrags
{
    struct ipq *ipqueue;//这里维护IP碎片队列
    int ip_frag_mem;
    u_int ip;//主机对应的IP地址
    //很明显，下面三行告诉我们，这是哈希表的一个元素
    int hash_index;
    struct hostfrags *prev;
    struct hostfrags *next;
};
//下面这个就是维护所有IP的哈希表了。
static struct hostfrags **fragtable;

//这个是每个IP下的被分片的IP包--IP碎片队列
/* Describe an entry in the "incomplete datagrams" queue. */
struct ipq
{
    unsigned char *mac;        /* pointer to MAC header                */
    struct ip *iph;        /* pointer to IP header                 */
    int len;            /* total length of original datagram    */
    short ihlen;            /* length of the IP header              */
    short maclen;            /* length of the MAC header             */
    struct timer_list timer;    /* when will this queue expire?         */
    struct ipfrag *fragments;    /* linked list of received fragments    */
    struct hostfrags *hf;
    struct ipq *next;        /* linked list pointers                 */
    struct ipq *prev;
    // struct device *dev;    /* Device - for icmp replies */
};

//IP碎片的定义
/* Describe an IP fragment. */
struct ipfrag
{
    int offset;            /* offset of fragment in IP datagram    */
    int end;            /* last byte of data in datagram        */
    int len;            /* length of this fragment              */
    struct sk_buff *skb;        /* complete received fragment           */
    unsigned char *ptr;        /* pointer into real fragment data      */
    struct ipfrag *next;        /* linked list pointers                 */
    struct ipfrag *prev;
};

//存储IP数据报
struct sk_buff
{
    char *data;
    int truesize;
};

//格式化出错信息到指定字符串里
static void
panic(char *str)
{
    fprintf(stderr, "%s", str);
    exit(1);
}


/* Memory Tracking Functions 修改链表指针 */
static void
atomic_sub(int ile, int *co)
{
    *co -= ile;
}

static void
atomic_add(int ile, int *co)
{
    *co += ile;
}

//释放一个skb的空间
static void
kfree_skb(struct sk_buff * skb, int type)
{
    (void)type;
    free(skb);
}



//释放一个ipfrag节点
static void
frag_kfree_skb(struct sk_buff * skb, int type)
{
    if (this_host)
        //IPQ队列中已经到达的IP分片总长度自动减
        //少truesize大小，然后释放sk_buff所占的内存
        atomic_sub(skb->truesize, &this_host->ip_frag_mem);
    kfree_skb(skb, type);
}

//释放当前指针所指数据
static void
frag_kfree_s(void *ptr, int len)
{
    if (this_host)
        atomic_sub(len, &this_host->ip_frag_mem);
    free(ptr);
}


//添加新单元到链表timer_list  ，
//为IP分片启动一个新的定时器
static void
add_timer(struct timer_list * x)
{
    //不是第一个节点时的操作
    if (timer_tail)
    {
        timer_tail->next = x;
        x->prev = timer_tail;
        x->next = 0;
        timer_tail = x;
    }
    //时间链表加入第一个单元时
    else
    {
        x->prev = 0;
        x->next = 0;
        timer_tail = timer_head = x;
    }
}


//删除timer_list中的x 元素
static void
del_timer(struct timer_list * x)
{
//x有前驱节点
    if (x->prev)
        x->prev->next = x->next;
    //x是头结点
    else
        timer_head = x->next;

    if (x->next)
        x->next->prev = x->prev;
    else
        timer_tail = x->prev;
}

//删除一个hostfrag节点
static void
rmthis_host()
{
    int hash_index = this_host->hash_index;

    if (this_host->prev)
    {
        this_host->prev->next = this_host->next;
        if (this_host->next)
            this_host->next->prev = this_host->prev;
    }
    else
    {
        fragtable[hash_index] = this_host->next;
        if (this_host->next)
            this_host->next->prev = 0;
    }
    free(this_host);
    this_host = 0;
}


//为指定结构分配大小合适的堆空间
static void *
frag_kmalloc(int size, int dummy)
{
    void *vp = (void *) malloc(size);
    (void)dummy;
    if (!vp)
        return NULL;
    //ip_frag_men 内存中ip分片包的实际大小自动增加size大小
    atomic_add(size, &this_host->ip_frag_mem);

    return vp;
}

static void
ip_free(struct ipq * qp)
{
    struct ipfrag *fp;
    struct ipfrag *xp;

    /* Stop the timer for this entry. */
    del_timer(&qp->timer);

    /* Remove this entry from the "incomplete datagrams" queue. */
    if (qp->prev == NULL)
    {
        this_host->ipqueue = qp->next;
        if (this_host->ipqueue != NULL)
            this_host->ipqueue->prev = NULL;
        else
            rmthis_host();
    }
    else
    {
        qp->prev->next = qp->next;
        if (qp->next != NULL)
            qp->next->prev = qp->prev;
    }

    //释放 每一个ipfrag节点
    /* Release all fragment data. */
    fp = qp->fragments;
    while (fp != NULL)
    {
        xp = fp->next;
        frag_kfree_skb(fp->skb, FREE_READ);
        frag_kfree_s(fp, sizeof(struct ipfrag));
        fp = xp;
    }
    /* Release the IP header. */
    frag_kfree_s(qp->iph, 64 + 8);

    /* Finally, release the queue descriptor itself. */
    frag_kfree_s(qp, sizeof(struct ipq));
}

//当IP包占用太多内存时(阀值256k)，就调用该函数
//释放那些太老的IPQ队列，直到小于192k
static void
ip_evictor(void)
{
    // fprintf(stderr, "ip_evict:numpack=%i\n", numpack);
    while (this_host->ip_frag_mem > IPFRAG_LOW_THRESH)
    {
        if (!this_host->ipqueue)
            panic("ip_evictor: memcount");
        ip_free(this_host->ipqueue);
    }
}


// get time
static int
jiffies()
{
    struct timeval tv;

    if (timenow)
        return timenow;
    gettimeofday(&tv, 0);
    timenow = (tv.tv_sec - time0) * 1000 + tv.tv_usec / 1000;//把时间转换为毫秒

    return timenow;
}




/*
  Find the correct entry in the "incomplete datagrams" queue for this
  IP datagram, and return the queue entry address if found.
*/
static struct ipq *
ip_find(struct ip * iph)
{
    struct ipq *qp;
    struct ipq *qplast;

    qplast = NULL;
    for (qp = this_host->ipqueue; qp != NULL; qplast = qp, qp = qp->next)
    {
        if (iph->ip_id == qp->iph->ip_id &&
                iph->ip_src.s_addr == qp->iph->ip_src.s_addr &&
                iph->ip_dst.s_addr == qp->iph->ip_dst.s_addr &&
                iph->ip_p == qp->iph->ip_p)
        {
            del_timer(&qp->timer);	/* So it doesn't vanish on us. The timer will
				   be reset anyway */
            return (qp);
        }
    }
    return (NULL);
}

/* Oops- a fragment queue timed out.  Kill it and send an ICMP reply. */
static void
ip_expire(unsigned long arg)
{
    struct ipq *qp;

    qp = (struct ipq *) arg;

    /* Nuke the fragment queue. */
    ip_free(qp);
}


//根据目的IP地址返回哈希值
static int
frag_index(struct ip * iph)
{
//将ip_dst.s_addr转化为主机字节地址
    unsigned int ip = ntohl(iph->ip_dst.s_addr);

    return (ip % hash_size);
}




//创建一个新的hostfrag节点
static void
hostfrag_create(struct ip * iph)
{
    struct hostfrags *hf = mknew(struct hostfrags);
    int hash_index = frag_index(iph);

    hf->prev = 0;
    hf->next = fragtable[hash_index];
    if (hf->next)
        hf->next->prev = hf;
    fragtable[hash_index] = hf;
    hf->ip = iph->ip_dst.s_addr;
    hf->ipqueue = 0;
    hf->ip_frag_mem = 0;
    hf->hash_index = hash_index;
    this_host = hf;
}

//根据目的IP地址找hostfrag节点
static int
hostfrag_find(struct ip * iph)
{
    int hash_index = frag_index(iph);
    struct hostfrags *hf;

    this_host = 0;
    for (hf = fragtable[hash_index]; hf; hf = hf->next)
        if (hf->ip == iph->ip_dst.s_addr)
        {
            this_host = hf;
            break;
        }
    if (!this_host)
        return 0;
    else
        return 1;
}


static struct ipq *
ip_create(struct ip * iph)
{
    struct ipq *qp;
    int ihlen;

    qp = (struct ipq *) frag_kmalloc(sizeof(struct ipq), GFP_ATOMIC);
    if (qp == NULL)
    {
        // NETDEBUG(printk("IP: create: no memory left !\n"));
        printf("ip_create");
        return (NULL);
    }
    memset(qp, 0, sizeof(struct ipq));

    /* Allocate memory for the IP header (plus 8 octets for ICMP). */
    ihlen = iph->ip_hl * 4;
    qp->iph = (struct ip *) frag_kmalloc(64 + 8, GFP_ATOMIC);
    if (qp->iph == NULL)
    {
        //NETDEBUG(printk("IP: create: no memory left !\n"));
        printf("ip_create");
        frag_kfree_s(qp, sizeof(struct ipq));
        return (NULL);
    }
    //复制IP包头信息
    memcpy(qp->iph, iph, ihlen + 8);
    qp->len = 0;
    qp->ihlen = ihlen;
    qp->fragments = NULL;
    qp->hf = this_host;
//设置该ipq所对应分片包的失效期限
    /* Start a timer for this entry. */
    qp->timer.expires = jiffies() + IP_FRAG_TIME;	/* about 30 seconds     */
    qp->timer.data = (unsigned long) qp;	/* pointer to queue     */
    qp->timer.function = ip_expire;	/* expire function 超时时的处理函数     */
    add_timer(&qp->timer);

    /*

      此时，新建的ipq实例已经设置完闭，将ipq加入到this_host

      所指向的ipqueue双向链表中（因为this_host也是新建立的，

      所以这时该链表中仅有一个ipq链表）。

    */

    /* Add this entry to the queue. */
    qp->prev = NULL;
    qp->next = this_host->ipqueue;
    if (qp->next != NULL)
        qp->next->prev = qp;
    this_host->ipqueue = qp;

    return (qp);
}


/* See if a fragment queue is complete. */
static int
ip_done(struct ipq * qp)
{
    struct ipfrag *fp;
    int offset;

    /* Only possible if we received the final fragment. */
    if (qp->len == 0)
        return (0);

    /* Check all fragment offsets to see if they connect. */
    fp = qp->fragments;
    offset = 0;
    while (fp != NULL)
    {
        if (fp->offset > offset)
            return (0);		/* fragment(s) missing */
        offset = fp->end;
        fp = fp->next;
    }
    /* All fragments are present. */
    return (1);
}


/*
  Build a new IP datagram from all its fragments.

  FIXME: We copy here because we lack an effective way of handling
  lists of bits on input. Until the new skb data handling is in I'm
  not going to touch this with a bargepole.
*/
//将一个所有分片已经到齐的的IP包组合好
static char *
ip_glue(struct ipq * qp)
{
    unsigned char *skb;
    struct ip *iph;
    struct ipfrag *fp;
    unsigned char *ptr;
    int count, len;

    /* Allocate a new buffer for the datagram. */
    len = qp->ihlen + qp->len;

    if (len > 65535)
    {
        // NETDEBUG(printk("Oversized IP packet from %s.\n", int_ntoa(qp->iph->ip_src.s_addr)));
//    nids_params.syslog(NIDS_WARN_IP, NIDS_WARN_IP_OVERSIZED, qp->iph, 0);
        ip_free(qp);
        return NULL;
    }
    if ((skb = (unsigned char *) malloc(len)) == NULL)
    {
        // NETDEBUG(printk("IP: queue_glue: no memory for gluing queue %p\n", qp));
//    nids_params.no_mem("ip_glue");
        ip_free(qp);
        return (NULL);
    }
    /* Fill in the basic details. */
    ptr = skb;
    memcpy(ptr, ((unsigned char *) qp->iph), qp->ihlen);
    ptr += qp->ihlen;
    count = 0;

    /* Copy the data portions of all fragments into the new buffer. */
    fp = qp->fragments;
    while (fp != NULL)
    {
        if (fp->len < 0 || fp->offset + qp->ihlen + fp->len > len)
        {
            //NETDEBUG(printk("Invalid fragment list: Fragment over size.\n"));
//      nids_params.syslog(NIDS_WARN_IP, NIDS_WARN_IP_INVLIST, qp->iph, 0);
            ip_free(qp);
            //kfree_skb(skb, FREE_WRITE);
            //ip_statistics.IpReasmFails++;
            free(skb);
            return NULL;
        }
        //将每个分片的数据连接到一起
        memcpy((ptr + fp->offset), fp->ptr, fp->len);
        count += fp->len;
        fp = fp->next;
    }
    /* We glued together all fragments, so remove the queue entry. */
    ip_free(qp);

    /* Done with all fragments. Fixup the new IP header. */
    iph = (struct ip *) skb;
    iph->ip_off = 0;
    //确保网络程序在linux和win下的可移植性
    iph->ip_len = htons((iph->ip_hl * 4) + count);
    // skb->ip_hdr = iph;
//将重组的ip 包首地址传递给调用函数ip_defrag
    return ((char *)skb);
}






//产生一个IP分片实体，并初始化相关变量
/* Create a new fragment entry. */
static struct ipfrag *
ip_frag_create(int offset, int end, struct sk_buff * skb, unsigned char *ptr)
{
    struct ipfrag *fp;

    fp = (struct ipfrag *) frag_kmalloc(sizeof(struct ipfrag), GFP_ATOMIC);
    if (fp == NULL)
    {
        // NETDEBUG(printk("IP: frag_create: no memory left !\n"));
        printf("ip_frag_create");
        return (NULL);
    }
    memset(fp, 0, sizeof(struct ipfrag));

    //用传来的参数初始化新建立的结构体
    /* Fill in the structure. */
    fp->offset = offset;
    fp->end = end;
    fp->len = end - offset;
    fp->skb = skb;
    fp->ptr = ptr;

    /* Charge for the SKB as well. */
    this_host->ip_frag_mem += skb->truesize;

    return (fp);
}




/* Process an incoming IP datagram fragment. 重点      判定是不是新开头分片 */
static char *
ip_defrag(struct ip *iph, struct sk_buff *skb)
{
    struct ipfrag *prev, *next, *tmp;
    struct ipfrag *tfp;
    struct ipq *qp;
    char *skb2;
    unsigned char *ptr;
    int flags, offset;
    int i, ihl, end;

    /*
    当收到的IP包为分片数据，即标志字段中的MF为1，

    首先根据其目的地址计算其hash值hash_index，然后在

    fragtable[hash_index] 所指向的hostfrags双向链表中查找该分

    片包对应的hostfrags结点

    */
    if (!hostfrag_find(iph) && skb)
        /* 如果fragtable[hash_index]为空或者没有找到对应的

            hostfrags结点，则创建一个新hostfrags结点，之后

            将其加入到fragtable[hash_index] 所指向的双向链表

            中，并使fragtable[hash_index] 指向刚创建的hostfrags结

            点，并设置其成员如下：hostfrags.ip为该分片数

            据的目的地址；hostfrags.hash_index为根据目的地

            址计算所得的hash值；其余变量设置为0。为

            了以后访问该结点方便，设置全局变量this_host

            也指向这个新结点。此时该结点对应的ipq链表为空
          */
        hostfrag_create(iph);

    /* Start by cleaning up the memory. */
    if (this_host)
        if (this_host->ip_frag_mem > IPFRAG_HIGH_THRESH)
            ip_evictor();

    /* Find the entry of this IP datagram in the "incomplete datagrams" queue. */
    //在ipq链表中找该分片对应的结点
    if (this_host)
        /*  如果fragtable[hash_index]不为空且在fragtable[hash_index] 所指

          向的hostfrags双向链表中找到了该分片包对应的hostfrags结点

          （此时this_host指向该结点），则在this_host所对应的ipq链表

          中检查是否已经收到和当前IP分片包属于同一个IP包的

          分片数据：
          */
        qp = ip_find(iph);
    else
        qp = 0;

    /* Is this a non-fragmented datagram? */
    offset = ntohs(iph->ip_off);//转换
    flags = offset & ~IP_OFFSET;
    offset &= IP_OFFSET;//
    //如果找到了该节点且是非IP分片，
    //则释放包括ipq在内的整个ipfrag链表
    if (((flags & IP_MF) == 0) && (offset == 0))
    {
        if (qp != NULL)
            ip_free(qp);		/* Fragmented frame replaced by full
				   unfragmented copy */
        return 0;
    }
    offset <<= 3;			/* offset is in 8-byte chunks */
    ihl = iph->ip_hl * 4;

    /*
      If the queue already existed, keep restarting its timer as long as
      we still are receiving fragments.  Otherwise, create a fresh queue
      entry.
    */
    //如果找到，则判断是否是原IP包的第一个分片
    if (qp != NULL)
    {
        /* ANK. If the first fragment is received, we should remember the correct
           IP header (with options) */
        if (offset == 0)  //是第一个分片数据，则更新IP包头部信息
        {
            qp->ihlen = ihl;
            memcpy(qp->iph, iph, ihl + 8);
        }
        //将新到来的分片添加到时间链表中以防止超时
        del_timer(&qp->timer);//删除原来的超时计时器
        qp->timer.expires = jiffies() + IP_FRAG_TIME;	/* about 30 seconds */
        qp->timer.data = (unsigned long) qp;	/* pointer to queue */
        qp->timer.function = ip_expire;	/* expire function */
        add_timer(&qp->timer);
    }
    else
    {
        // 如果没找到则新建一个ipq结构的实例，设置其
        //变量后将它加入到this_host对应的ipq链表中
        /* If we failed to create it, then discard the frame. */
        //创建ipq结构实例并加到ipq链表中
        if ((qp = ip_create(iph)) == NULL)
        {
            kfree_skb(skb, FREE_READ);
            return NULL;
        }
    }
    /* Attempt to construct an oversize packet. */
    if (ntohs(iph->ip_len) + (int) offset > 65535)
    {
        // NETDEBUG(printk("Oversized packet received from %s\n", int_ntoa(iph->ip_src.s_addr)));
//    nids_params.syslog(NIDS_WARN_IP, NIDS_WARN_IP_OVERSIZED, iph, 0);
        kfree_skb(skb, FREE_READ);
        return NULL;
    }


    /*

    至此，保存IP分片数据的ipfrag结构链表的头结点已经找

    到或建立，下一步将该IP分片数据插入到分片链表中。

    */

    /* Determine the position of this fragment. */
    end = offset + ntohs(iph->ip_len) - ihl;
    //偏移＋ 该分片的长度 － 分片包头的长度＝ 该分片在整个ip包中
    /* Point into the IP datagram 'data' part. */
    ptr =(unsigned char *) (skb->data + ihl);

    /* Is this the final fragment? */
    if ((flags & IP_MF) == 0)
        qp->len = end;

    /*
      Find out which fragments are in front and at the back of us in the
      chain of fragments so far.  We must know where to put this
      fragment, right?
    */
    prev = NULL;
    //根据offset在链表中找要插入的位置
    for (next = qp->fragments; next != NULL; next = next->next)
    {
        if (next->offset >= offset)
            break;			/* bingo! */
        prev = next;
    }
    /*
      We found where to put this one.  Check for overlap with preceding
      fragment, and, if needed, align things so that any overlaps are
      eliminated.
    */
    //先检查当前分片数据和prev所指向的分片数据是否有重叠
    if (prev != NULL && offset < prev->end)
    {
        //调整当前分片对应的数据
//    nids_params.syslog(NIDS_WARN_IP, NIDS_WARN_IP_OVERLAP, iph, 0);
        i = prev->end - offset;
        offset += i;		/* ptr into datagram */
        ptr += i;			/* ptr into fragment data */
    }
    /*
      Look for overlap with succeeding segments.
      If we can merge fragments, do it.
    */
    for (tmp = next; tmp != NULL; tmp = tfp)
    {
        tfp = tmp->next;
        if (tmp->offset >= end)
            break;			/* no overlaps at all */
//    nids_params.syslog(NIDS_WARN_IP, NIDS_WARN_IP_OVERLAP, iph, 0);

        i = end - next->offset;	/* overlap is 'i' bytes */
        tmp->len -= i;		/* so reduce size of    */
        tmp->offset += i;		/* next fragment        */
        tmp->ptr += i;
        /*
          If we get a frag size of <= 0, remove it and the packet that it
          goes with. We never throw the new frag away, so the frag being
          dumped has always been charged for.
        */
        if (tmp->len <= 0)
        {
            if (tmp->prev != NULL)
                tmp->prev->next = tmp->next;
            else
                qp->fragments = tmp->next;

            if (tmp->next != NULL)
                tmp->next->prev = tmp->prev;

            next = tfp;		/* We have killed the original next frame */

            frag_kfree_skb(tmp->skb, FREE_READ);
            frag_kfree_s(tmp, sizeof(struct ipfrag));
        }
    }
    /* Insert this fragment in the chain of fragments. */
    tfp = NULL;
    tfp = ip_frag_create(offset, end, skb, ptr);

    /*
      No memory to save the fragment - so throw the lot. If we failed
      the frag_create we haven't charged the queue.
    */
    if (!tfp)
    {
        printf("ip_defrag");
        kfree_skb(skb, FREE_READ);
        return NULL;
    }
    /* From now on our buffer is charged to the queues. */
    tfp->prev = prev;
    tfp->next = next;
    if (prev != NULL)
        prev->next = tfp;
    else
        qp->fragments = tfp;

    if (next != NULL)
        next->prev = tfp;

    /*
      OK, so we inserted this new fragment into the chain.  Check if we
      now have a full IP datagram which we can bump up to the IP
      layer...
    */
    //判断是否已经收到完整的IP数据报
    if (ip_done(qp))
    {
        skb2 = ip_glue(qp);		/* glue together the fragments */
//    struct ip *iphtr;
//    iphtr=(struct ip *)skb2;
//    printf("new packet id: %d\n",iphtr->ip_id);
//    printf("new packet len: %d\n",noths((iphtr->ip_len));
        return (skb2);
    }
    return (NULL);
}



//先是判断是否为分片的函数
int
ip_defrag_stub(struct ip *iph, struct ip **defrag)
{
    int offset, flags, tot_len;
    struct sk_buff *skb;

    numpack++;
    //先处理超时事件
    timenow = 0;//刷新时间
    while (timer_head && timer_head->expires < jiffies())
    {
        this_host = ((struct ipq *) (timer_head->data))->hf;
        timer_head->function(timer_head->data);
    }

    //然后计算分片的偏移
    offset = ntohs(iph->ip_off);
    flags = offset & ~IP_OFFSET;
    offset &= IP_OFFSET;

    //此包不是分片
    if (((flags & IP_MF) == 0) && (offset == 0))
    {
        ip_defrag(iph, 0);
        return IPF_NOTF;
    }

    //此包是分片，先申请一个sk_buff把分片的数据保存起来，然后交给defrag函数
    tot_len = ntohs(iph->ip_len);
    skb = (struct sk_buff *) malloc(tot_len + sizeof(struct sk_buff));
    if (!skb)
        printf("ip_defrag_stub");
    skb->data = (char *) (skb + 1);
    memcpy(skb->data, iph, tot_len);
    skb->truesize = tot_len + 16 + dev_addon;
    skb->truesize = (skb->truesize + 15) & ~15;
    skb->truesize += sk_buff_size;

    //如果集齐了一个ip包的所有分片ip_defrag将返回合并后的ip包，此时返回IPF_NEW，进行下一步的ip包处理
    //否则，返回IPF_ISF，跳过ip包处理
    if ((*defrag = (struct ip *)ip_defrag((struct ip *) (skb->data), skb)))
    {
        //      iphtr = (struct ip *)ip_defrag((struct ip *) (skb->data), skb);

//      printf("new packet len:  %d\n", iphtr->ip_len);
        return IPF_NEW;
    }


    return IPF_ISF;
}






int ip_frag_init(int n)
{
    struct timeval tv;

    gettimeofday(&tv, 0);
    time0 = tv.tv_sec;
    fragtable = (struct hostfrags **) malloc(n * sizeof(struct hostfrags *));
    if (!fragtable)
        printf("ip_frag_init");
    memset(fragtable, 0, n * sizeof(struct hostfrags *));
    hash_size = n;

    return 0;
}





