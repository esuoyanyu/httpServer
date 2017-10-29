/*
 * epollBase.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_EPOLLBASE_H_
#define SRC_EPOLLBASE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/epoll.h>
#include "sysInfo.h"
#include "workQueue.h"
#include "socketBase.h"

int epollInit(const int);
int epollAdd(const int,const int,struct epoll_event* const);
int epollRemove(const int,const int,struct epoll_event* const);
int epollChangeMod(const int,const int,struct epoll_event* const);
int epoll_for(struct workQueue *const,const int,const int);
#endif /* SRC_EPOLLBASE_H_ */
