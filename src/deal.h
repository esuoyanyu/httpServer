/*
 * deal.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_DEAL_H_
#define SRC_DEAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include "sysInfo.h"
#include "dealBase.h"
#include "httpStringOper.h"

typedef struct
{
	int fd;
	char path[URLSIZE];
	httpHeadPtr clnet;
	httpServerHeadPtr server;
}dealInfo,*dealInfoPtr;

void ret405(const int); //返回405
int dealOutputDate(int); //处理输出
int dealInputDate(const int); //处理链接请求
int freeHttpHeadPtr(const httpHeadPtr headNode); //释放HttpHead
int freehttpServerHeadPtr(const httpServerHeadPtr); //释放httpServerHead

#endif /* SRC_DEAL_H_ */
