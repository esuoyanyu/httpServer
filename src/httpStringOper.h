/*
 * httpStringOper.h
 *
 *  Created on: 2017年10月22日
 *      Author: chy
 */

#ifndef HTTPSTRINGOPER_H_
#define HTTPSTRINGOPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysInfo.h"
#include "misc.h"

#define SIZE 16
#define COUNT 256 //列
#define ROW 128 //行
//请求类型
#define GET 0

//链接类型
#define KEEPLIVE 0
#define CLOSE 1

//状态码
#define OK 200
#define MNA 405

typedef struct
{
	char *requestType;
	char *url;
	char *httpTpye;
	char *hostAddr;
	int prot;
	char *connection;
}httpHead,*httpHeadPtr;

typedef struct
{
	char *httpTpye;
	char *serverName;
	char *connectTpye;
	char *connection;
	char *comd;
	unsigned int statusHttp;
	ssize_t lenth;
}httpServerHead,*httpServerHeadPtr;

int httpGetWord(const char (* const )[],const httpHeadPtr);
int httpStringList(const char *const ,char (*)[COUNT]);
int initHttpServerHead(const httpServerHeadPtr,const httpHead const*,const char *const);
int httpSeverStatusAndCmd(const httpServerHeadPtr,const char *const,const int);//设置状态码 and 命令
int httpServerFileType(const httpServerHeadPtr,const char *const,ssize_t); //设置文件传输的格式 and 文件的大小
int setHttpSeverHead(const httpServerHead const *info,char *);
int stringToInt(const char (*const)[SIZE],const char *const);
#endif /* HTTPSTRINGOPER_H_ */
