/*
 * misc.c
 *
 *  Created on: 2017年10月24日
 *      Author: chy
 */
#include "misc.h"

int stringti(const char *const str)
{
	int temp = 0;

	const char *p = NULL;
	for(p = str;*p;p++)
	{
		temp = temp * 10 + (*p - '0');
	}

	return temp;
}

int syslog(const char *const pathlog)
{
	if(!pathlog)
		return -1;

	printf("%s\n",pathlog);

	int fd = open(pathlog,O_RDWR | O_CREAT,0777);

	if(-1 == fd)
		return -1;

	dup2(fd,1); //标准输出重定向到日志文件

	return 0;
}

