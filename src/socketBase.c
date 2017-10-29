/*
 * socketBase.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */
#include "socketBase.h"

int getSocketId()
{
	int ret = -1;

	ret = socket(AF_INET,SOCK_STREAM,0);

	if(ret < 0)
	{
#ifdef DEBUG
		fprintf(stderr,"获取套接字ID失败\n");
#endif
		return ret;
	}

	return ret;
}

int initSocket(const int fid,const sysInfo *const info)
{
	int ret = -1;

	struct sockaddr_in *socketattr;

	socketattr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

	if(!socketattr)
	{
#ifdef DEBUG
		fprintf(stderr,"initSocket socketaddr 内存分配失败\n");
#endif
		return ret;
	}

	socketattr->sin_family = AF_INET;
	socketattr->sin_addr.s_addr = inet_addr(info->ip); //把点分十进制转换为网络地址
	socketattr->sin_port = htons((uint16_t)info->port); //端口号

	ret = bind(fid,(struct sockaddr*)socketattr,sizeof(struct sockaddr)); //声明

	if(-1 == ret)
	{
#ifdef DEBUG
		fprintf(stderr,"initSocket bind 声明失败\n");
#endif
		return ret;
	}

	ret = listen(fid,LINSTENMAX);

	if(-1 == ret)
	{
#ifdef DEBUG
		fprintf(stderr,"initSocket bind 监听失败\n");
#endif
		return ret;
	}
	return ret;
}

int getClinetId(const int fid)
{
	int cfd = -1;

	cfd = accept(fid, NULL,0);

	return cfd;
}

