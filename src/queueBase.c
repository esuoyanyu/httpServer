/*
 * queue.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#include "queueBase.h"

int queueInit(struct queueInfo *queuei,const int num)
{
	if(!queuei)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 参数为空\n",__func__);
#endif
		return -1;
	}

	queuei->headPrt  = (struct queue *) malloc(sizeof(struct queue) * num);

	memset(queuei->headPrt,0,sizeof(struct queue) * num);

	queuei->head = queuei->tail = 0;

	queuei->queueSize = num;

	return 0;
}

char queueEmpty(const struct queueInfo *const queuei)
{
	if(!queuei)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 参数为空\n",__func__);
#endif
		return -1;
	}

	if(queuei->head == queuei->tail)
		return 0;

	return 1;
}

char queueFull(const struct queueInfo *const queuei)
{
	if(!queuei)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 参数为空\n",__func__);
#endif
		return -1;
	}

	if((queuei->tail + 1) % queuei->queueSize == queuei->head)
			return 0;

		return 1;
}

int queueAdd(struct queueInfo *const queuei,const struct queue *const node)
{

	if(!queuei || !node)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 参数为空\n",__func__);
#endif
		return -1;
	}

	if(!queueFull(queuei))
	{
#ifdef DEBUG
		fprintf(stderr,"%s 添加失败，队列已满\n",__func__);
#endif
		return -1;
	}

	if(!memcpy(&(queuei->headPrt[queuei->tail]),node,sizeof(struct queue)))
		return -1;

	queuei->tail = (queuei->tail + 1) % (queuei->queueSize);

	return 0;
}

int queueRemovSeek(struct queueInfo * const queuei,struct queue *const node)
{
	if(!queuei)
	{
#ifdef DEBUG
		fprintf(stderr,"%s 参数为空\n",__func__);
#endif
		return -1;
	}

	if(!queueEmpty(queuei))
	{
#ifdef DEBUG
		fprintf(stderr,"%s 队列为空\n",__func__);
#endif
		return -1;
	}

	if(node)
		if(!memcpy(node,&queuei->headPrt[queuei->head],sizeof(struct queue)))
			return -1;

	if(!memset(&queuei->headPrt[queuei->head],0,sizeof(struct queue)))
		return -1;

	queuei->head = (queuei->head + 1) % (queuei->queueSize);

	return 0;
}

