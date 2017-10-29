/*
 * pthreadBade.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_PTHREADBASE_H_
#define SRC_PTHREADBASE_H_
#include <pthread.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include "workQueue.h"
#include "deal.h"

struct pthreadArg
{
	struct workQueue *work;
	pthread_t *tid;
	int tidNum;

};

int pthreadArgvatttr();
int pthreadFunc(struct pthreadArg *); //线程处理函数
int pthreadCreatrLoop(struct pthreadArg *argv); //创建线程池

#endif /* SRC_PTHREADBASE_H_ */
