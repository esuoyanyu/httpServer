/*
 * setSys.c
 *
 *  Created on: 2017年10月23日
 *      Author: chy
 */
#include "sysConfig.h"

const char config[][WORDSIZE] = {"ip","port","rootfs","syslog"};

int initConf()
{
	int fd = open(CONFIGPATH,O_RDONLY);

	if(fd < 0)
		fd = open(ETCCONFIGPATH,O_RDONLY);

	return fd;
}

static int checkStr(const char *const str)
{
	if(str[strlen(str) - 1] == '\n' && strlen(str) == 1)
		return 0;

	int i = 0;
	for(i = 0; i < strlen(str) - 1 && str[i] == ' '; i++);

	if(i == strlen(str) - 2) return 0;

	return -1;
}

static int getKeyWord(const char *const str,char *const ret)
{
	if(!checkStr(str))
		return -1;

	char *ss = NULL;

	const char (*temp)[WORDSIZE] = NULL;

	for(temp = config; temp < config + sizeof(config) / sizeof(char*); temp++)
	{
		ss = strstr(str,*temp); //寻找对应的配置表
		if(ss)
		{
			char *s = NULL;
			s = strchr(ss,'='); //寻找‘=’
			if(s)
			{
				char *t = NULL;
				t = strstr(s + 1,"\n"); //寻找‘\\n’

				if(t) //寻找‘/n’
					strncpy(ret,s + 1,t - s - 1 );
				else
					strcpy(ret,s + 1);
#ifdef DEBUG
				printf("%s %d\n%s\n",__func__,__LINE__,ret);
#endif
				return temp - config;
			}
		}
	}

#ifdef RELEASE
	printf("%s%d:配置文件中有错误\n",__func__,__LINE__);
#endif

	return -1;
}

int confree(sysInfo *const info)
{
	if(!info)
	{
#ifdef DEBUG
		fprintf(stderr,"%s%d info is NULL\n",__func__,__LINE__);
#endif
		return -1;
	}

	if(info->ip) free(info->ip);

	if(info->root) free(info->root);

	if(info->syslog) free(info->syslog);

	return 0;
}

static int readConfig(const int fd,sysInfo *const info)
{
	FILE *file = NULL;
	file = fdopen(fd,"r");

	if(!file)
	{
#ifdef DEBUG
		fprintf(stderr,"%s%d 文件描述符转换失败\n",__func__,__LINE__);
#endif
		return -1;
	}
	char line[URLSIZE];
	char *readStr = NULL;
	int ret = 0;

#ifdef DEBUG
	printf("%s%d\n",__func__,__LINE__);
#endif

	while(readStr = fgets(line,sizeof(line),file),readStr)
	{
		if(strchr(line,'#'))
			continue;

		char word[URLSIZE];
		memset(word,0,sizeof(word));

		switch(getKeyWord(line,word))
		{
		case 0: info->ip = (char*)malloc(sizeof(char) * strlen(word));if(info->ip) strcpy(info->ip,word); else ret = -1; break;
		case 1: info->port = stringti(word); break;
		case 2: info->root = (char*)malloc(sizeof(char) * strlen(word));if(info->root) strcpy(info->root,word); else ret = -1; break;
		case 3: info->syslog = (char*)malloc(sizeof(char) * strlen(word));if(info->syslog) strcpy(info->syslog,word); else ret = -1; break;
		default:
#ifdef DEBUG
			printf("%s%d:配置文件解析失败\n",__func__,__LINE__);
#endif
			break;
		}
	}

	return ret;
}



int sysInit(sysInfo *const info,char *const temp)
{
	int fd = initConf();

#ifdef DEBUG
	printf("%s%d fd = %d\n",__func__,__LINE__,fd);
#endif

	if(fd < 0)
		return -1;

	int ret = readConfig(fd,info);

	if(ret < 0)
		return -1;

	strcpy(temp,info->root);

	return ret;
}

