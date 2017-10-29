/*
 * epollBase.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */
#include "epollBase.h"

int epollInit(const int fidmax) //初始化epoll
{
	int ret = -1;

	ret = epoll_create(fidmax);

	if(!ret)
	{
#ifdef DEBUG
		fprintf(stderr,"dealSock epoll_create 创建失败\n");
#endif
		return ret;
	}

	return ret;
}

int epollAdd(const int epollId,const int fd,struct epoll_event* const env) //添加到epoll队列中
{
	int ret = -1;

	ret = epoll_ctl(epollId,EPOLL_CTL_ADD,fd,env); //添加到epoll队列中

	if(ret < 0)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 添加失败\n", __func__);
#endif
		return ret;
	}

	return ret;
}

int epollRemove(const int epollId,const int cfd,struct epoll_event* const env) //从队列中删除fd
{
	int ret = -1;

	ret = epoll_ctl(epollId,EPOLL_CTL_DEL,cfd,env); //删除epoll队列的fd

	if(ret < 0)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 删除失败\n", __func__);
#endif
		return ret;
	}

	return ret;
}

int epollChangeMod(const int epollId,const int fd,struct epoll_event* const env) //更改模式
{
	int ret = -1;
	ret = epoll_ctl(epollId,EPOLL_CTL_MOD,fd,env); //更改cfd模式

	if(!ret)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 改变模式失败\n", __func__);
#endif
		return ret;
	}

	return ret;
}

int epoll_for(struct workQueue *const work,const int sfd,const int epollId)
{
	struct epoll_event env[FDNUMMAX];
	struct queue evqueue;
	int cfd = -1;

	while(1)
	{
		int num = epoll_wait(epollId,env,FDNUMMAX - 1,-1);

		struct epoll_event* ev = env;

		for(;ev < env + num; ev++)
		{
			if(ev->data.fd == sfd)
			{
				cfd = getClinetId(sfd);

				struct epoll_event evn;
		    	evn.data.fd = cfd;
		    	evn.events = EPOLLIN | EPOLLRDHUP |EPOLLET;
		    	epollAdd(epollId,cfd,&evn);
#ifdef DEBUG
		    	printf("epoll is cfd = %d\n",cfd);
#endif
			}
			else if(ev->data.fd != sfd && ev->events == EPOLLIN)
			{
				evqueue.fd = ev->data.fd;
				evqueue.events = EPOLLIN;
				workQueueSet(work,&evqueue);
			}
			else if(ev->data.fd != sfd && ev->events == (EPOLLRDHUP | EPOLLIN))
			{
				epollRemove(epollId,ev->data.fd,ev);

#ifdef DEBUG
				printf("套接字关闭fd = %d\n",ev->data.fd);
#endif
			}
		}
	}

	return 0;
}

