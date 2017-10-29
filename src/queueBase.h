/*
 * queueBase.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_QUEUEBASE_H_
#define SRC_QUEUEBASE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "sysInfo.h"

struct queue
{
	int fd;
	uint32_t events;
	char buff[BUFFSIZE];
};

struct queueInfo
{
	struct queue *headPrt;
	int head;
	int tail;
	int queueSize;
};

int queueInit(struct queueInfo *queuei,const int num);
char queueEmpty(const struct queueInfo *const );
char queueFull(const struct queueInfo *const);
int queueAdd(struct queueInfo *const queuei,const struct queue *const node);
int queueRemovSeek(struct queueInfo * const queuei,struct queue *const node);
#endif /* SRC_QUEUEBASE_H_ */
