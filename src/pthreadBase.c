/*
 * pthreadBase.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#include "pthreadBase.h"

int pthreadFunc(struct pthreadArg *argv) //线程处理函数
{
	struct queue node;
	while(1)
	{
		workQueueGet(argv->work,&node);

		switch(node.events)
		{
		case EPOLLIN: dealInputDate(node.fd); break;
		case EPOLLOUT:// dealOutputDate(node.fd); break;
		case EPOLLHUP:// dealClosSocket(node.fd); break;
			 break;
		}
	}

	return 0;
}

int pthreadCreatrLoop(struct pthreadArg *argv) //创建线程池
{
	pthread_attr_t attr;

	pthread_attr_init(&attr); //初始化线程属性
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED); //设置为分离的线程

	argv->tid = (pthread_t *)malloc(sizeof(pthread_t) * argv->tidNum);

	int i;
	for(i = 0; i < argv->tidNum; i++) //创建线程池
	{
		pthread_create(&argv->tid[i],&attr,(void *(*) (void *))pthreadFunc,(void*)argv);
#ifdef DEBUG
		fprintf(stderr,"%s tid = %ld\n",__func__,argv->tid[i]);
#endif
	}

	pthread_attr_destroy(&attr); //释放线程属性

	return 0;
}
