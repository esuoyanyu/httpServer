#include "workQueue.h"

int workQueueInit(struct workQueue *work,struct queueInfo *const queuei)
{
	pthread_mutex_init(&work->mutex,NULL);
	sem_init(&work->full,0,0); //初始化信号量
	sem_init(&work->empty,0,queuei->queueSize); //初始化信号量
	work->queuei = queuei;

	return 0;
}

int workQueueSet(struct workQueue *const work,const struct queue *const node) //向工作队列添加任务
{
	int ret = -1;
	sem_wait(&work->empty);

	pthread_mutex_lock(&work->mutex);
	ret = queueAdd(work->queuei,node);
	pthread_mutex_unlock(&work->mutex);

	sem_post(&work->full);

	return ret;
}

int workQueueGet(struct workQueue *const work,struct queue *const node)
{
	int ret = -1;

	sem_wait(&work->full);

	pthread_mutex_lock(&work->mutex);
	ret = queueRemovSeek(work->queuei,node);
	pthread_mutex_unlock(&work->mutex);

	sem_post(&work->empty);

	return ret;
}
