/*
 * workQueue.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_WORKQUEUE_H_
#define SRC_WORKQUEUE_H_

#include <semaphore.h> //posix 无名
#include <pthread.h>
#include "queueBase.h"
#include "sysInfo.h"

struct workQueue
{
	pthread_mutex_t mutex;
	sem_t full;
	sem_t empty;
	struct queueInfo *queuei;
};

int workQueueInit(struct workQueue *work,struct queueInfo *const queuei);
int workQueueSet(struct workQueue *const work,const struct queue *const node);
int workQueueGet(struct workQueue *const,struct queue *const);

#endif /* SRC_WORKQUEUE_H_ */
