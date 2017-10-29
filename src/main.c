#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/unistd.h>
#include "deal.h"
#include "sysInfo.h"
#include "dealBase.h"
#include "sysConfig.h"
#include "queueBase.h"
#include "socketBase.h"
#include "epollBase.h"
#include "pthreadBase.h"
#include "misc.h"

int main(int argc, char *argv[])
{
	sysInfo info;
	int ret = sysInit(&info,pathBase);

#ifdef SYSLOG
	if(syslog(info.syslog))
	{
#ifdef RELEASE
		printf("%s%d日志文件路径没有配置\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}
#endif

	if(ret < 0)
	{
#ifdef RELEASE
		printf("%s%d文件配置失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	int sfd = getSocketId();
	if(initSocket(sfd,&info))
	{
#ifdef RELEASE
		printf("%s%d 套接字初始化失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	confree(&info); //释放系统配置信息

	int epollId = epollInit(FDNUMMAX); //初始化epoll

	if(-1 == epollId)
	{
#ifdef RELEASE
		printf("%s%d epoll初始化失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	//添加套节字ID到监听事件
	struct epoll_event evn;
	evn.data.fd = sfd;
	evn.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
	epollAdd(epollId,sfd,&evn);

	struct pthreadArg arg;
	struct queueInfo queues;
	struct workQueue work;

	if(queueInit(&queues,WORKQUEUENUM)) //初始化任务队列
	{
#ifndef RELEASE
		printf("%s%d queue初始化失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	if(workQueueInit(&work,&queues)) //初始化工作队列
	{
#ifndef RELEASE
		printf("%s%d work初始化失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	arg.work = &work;
	arg.work->queuei = &queues;
	arg.tidNum = PATHREADNUM;

	if(pthreadCreatrLoop(&arg)) //创建线程池
	{
#ifndef RELEASE
		printf("%s%d 线程池初始化失败\n",__func__,__LINE__);
#endif
		exit(EXIT_FAILURE);
	}

	epoll_for(&work,sfd,epollId);

	return 0;
}

