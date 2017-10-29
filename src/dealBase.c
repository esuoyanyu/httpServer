/*
 * dateBase.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#include "dealBase.h"

#include <unistd.h>
char pathBase[URLSIZE];

int getAbsPath(const char *const path,char *absPath)
{
#ifdef DEBUG
	printf("%s %d\n%s\n",__func__,__LINE__,pathBase);
#endif
	if(!path || !absPath)
	{
#ifdef DEBUG
		fprintf(stderr,"%s %d 路径不合法\n",__func__,__LINE__);
#endif
		return -1;
	}

	if(!strstr(path,pathBase))
	{
		strcpy(absPath,pathBase);
		strcat(absPath,path);
	}
	else
	{
		strcpy(absPath,path);
	}

	return 0;
}

int isDir(const char *const absPath)
{
	struct stat temp;
	stat(absPath,&temp);

	if(S_ISDIR(temp.st_mode)) return 0;

	return -1;
}

int isRead(const char *const absPath)
{
	struct stat temp;
	stat(absPath,&temp);

	if((temp.st_mode & S_IRWXO) || (temp.st_mode & S_IROTH) || (temp.st_mode & S_IRGRP) || (temp.st_mode & S_IRUSR))
		return 0;

	return -1;
}

ssize_t getFileSzie(const char *path)
{
	int fd = open(path,O_RDWR);

	if(fd < 0)
	{
#ifdef DEBUG
		fprintf(stderr,"%s %d 文件打开失败\n",__func__,__LINE__);
#endif
		return -1;
	}

	off_t start = lseek(fd,0,SEEK_CUR);
	off_t end = lseek(fd,0,SEEK_END);

	if(start < 0 || end < 0)
	{
#ifdef DEBUG
		fprintf(stderr,"%s %d 文件长度获取失败\n",__func__,__LINE__);
#endif
		return -1;
	}

	close(fd);

	return end - start;
}

int readloop(char *const buff,const int cfd,int size)
{
	int ret = -1;
	char *temp = buff;
	int off = 0;
	while(ret = read(cfd,temp + off,size),ret < 0)
		off += ret;

	return ret;
}

int wirteloop(const char *const buff,const int readNum,const int cfd)
{
	const char *writePrt = buff;
	int writeNum = 0;
	int writeOff = 0;
	int s = readNum;

	while(writeNum = write(cfd,writePrt + writeOff,s),writeNum > 0)
	{
		if(writeNum < 0)
		{
#ifdef DEBUG
			fprintf(stderr,"%s %d 写文件出错\n",__func__,__LINE__);
#endif
		return -1;
		}
		writeOff += writeNum;
		s = readNum - writeOff;
	}

	return 0;
}

int writefd(const int cfd,const char *const path)
{
	int fd = open(path,O_RDWR);
	char buff[BUFFSIZE + 1];
	memset(buff,0,sizeof(buff));
	int SizeOut;

	while(SizeOut = read(fd,buff,BUFFSIZE),SizeOut > 0)
	{
		if(SizeOut < 0)
		{
#ifdef DEBUG
			fprintf(stderr,"%s %d 读文件出错\n",__func__,__LINE__);
#endif
			return -1;
		}
#ifdef DEBUG
		printf("%s",buff);
#endif
		if(wirteloop(buff,SizeOut,cfd) < 0) return -1;

		memset(buff,0,sizeof(buff));
	}

	close(fd);

	return 0;
}



