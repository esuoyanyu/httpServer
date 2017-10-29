/*
 * httpOper.c
 *
 *  Created on: 2017年10月22日
 *      Author: chy
 */

#include "httpStringOper.h"

int httpStringList(const char *const clent,char (*stringList)[COUNT])
{
	int ret = -1;

	if(!clent || !stringList)
	{
#ifdef DEBUG
		fprintf(stderr,"%s %d\n",__func__,__LINE__);
#endif
		return ret;
	}

	char *endAddr = NULL;

	if(!(endAddr = strstr(clent,"\r\n\r\n")))
	{
#ifdef DEBUG
		fprintf(stderr,"%s %d http头不完整\n",__func__,__LINE__);
#endif
		return ret;
	}

	const char *startAddr = NULL;
	const char *temp = NULL;

	for(startAddr = clent,temp = clent; temp != endAddr + 2; startAddr = temp)
	{
		temp = strstr(temp,"\r\n");

		if(!temp)
		{
	#ifdef DEBUG
			fprintf(stderr,"%s %d http头解析失败\n",__func__,__LINE__);
	#endif
			return ret;
		}

		strncpy(*stringList,startAddr,temp - startAddr);

#ifdef DEBUG
		printf("%s %d\n%s\n",__func__,__LINE__,*stringList);
#endif

		temp += 2;

		stringList++;
	}

	return 0;
}

int stringToInt(const char (*const table)[SIZE],const char *const str)
{
	const char (*temp)[SIZE] = NULL;
	for(temp = table; temp; temp++)
		if(!strcmp(*temp,str))
			return temp - table;

	return -1;
}

static char *findChar(const char *const str,const char findCh)
{
	if(findCh)
		return strchr(str,findCh);
	return (char*)(str + strlen(str));

}

static char *getStr(const char *const str,char *const ret,const char findC)
{

	char *charPtr = findChar(str,findC);

	if(ret && charPtr)
		strncpy(ret,str,charPtr - str );

	return charPtr;
}

int httpGetWord(const char (*stringList)[COUNT],const httpHeadPtr headNode)
{
	if(!stringList || !headNode)
	{
		return -1;
	}

	//获取请求类型
	char temp[URLSIZE];
	memset(temp,0,sizeof(temp));
	char *charPtr = getStr(*stringList++,temp,' ');
	headNode->requestType = (char *)malloc(sizeof(char) * strlen(temp));
	strcpy(headNode->requestType,temp);

	//获取URL
	memset(temp,0,sizeof(temp));
	charPtr = getStr(charPtr + 1,temp,' ');
	headNode->url = (char *)malloc(sizeof(char) * strlen(temp) + 1);
	strcpy(headNode->url,temp);

	//获取http协议版本
	headNode->httpTpye = (char *)malloc(sizeof(char) * strlen(charPtr) + 1);
	strcpy(headNode->httpTpye,charPtr + 1);

	//获取主机IP or 域名
	memset(temp,0,sizeof(temp));
	charPtr = getStr(*stringList++,NULL,' ');
	char *temps = charPtr + 1;
	charPtr = getStr(charPtr + 1,temp,':');
	if(charPtr)
	{
		headNode->hostAddr = (char *)malloc(sizeof(char) * strlen(temp));
		strcpy(headNode->hostAddr,temp);
		//获取端口号
		memset(temp,0,sizeof(temp));
		getStr(charPtr,temp,0);
		headNode->prot = stringti(temp+1);
	}
	else
	{
		headNode->hostAddr = (char *)malloc(sizeof(char) * strlen(temps));
		strcpy(headNode->hostAddr,temps);
	}

	//获取链接类型
	charPtr = getStr(*stringList++,NULL,' ');
	charPtr = getStr(charPtr,temp,0);
	headNode->connection = (char *)malloc(sizeof(char) * strlen(temp));
	strcpy(headNode->connection,temp);

	return 0;
}

int initHttpServerHead(const httpServerHeadPtr serverHttp,const httpHead const* head,const char *const serverName)
{
	serverHttp->httpTpye = (char *)malloc(sizeof(char) * strlen(head->httpTpye)); //http版本信息
	strcpy(serverHttp->httpTpye,head->httpTpye);

	serverHttp->serverName = (char *)malloc(sizeof(char) * strlen(serverName)); //http服务器的Name
	strcpy(serverHttp->serverName,serverName);

	serverHttp->connection = (char *)malloc(sizeof(char) * strlen(head->connection)); //http服务器链接的类型
	strcpy(serverHttp->connection,head->connection);

	return 0;
}

int httpSeverStatusAndCmd(const httpServerHeadPtr serverHttp,const char *const comd,const int statusHttp)//设置状态码 and 命令
{
	serverHttp->statusHttp = statusHttp; //http服务器返回的状态码

	serverHttp->comd = (char *)malloc(sizeof(char) * strlen(comd)); //http服务器返回的命令
	strcpy(serverHttp->comd,comd);

	return 0;
}

int httpServerFileType(const httpServerHeadPtr serverHttp,const char *const connectTpye,ssize_t len) //设置文件传输的格式 and 文件的大小
{
	serverHttp->connectTpye = (char *)malloc(sizeof(char) * strlen(connectTpye) + 1); //http服务器传输文件使用的类型
	memset(serverHttp->connectTpye,0,sizeof(connectTpye));
	strcpy(serverHttp->connectTpye,connectTpye);

	serverHttp->lenth = len; //http服务器传输文件使用的大小

	return 0;
}

int setHttpSeverHead(const httpServerHead const *info,char *headBuff)
{
	char buff[5][COUNT];

	sprintf(buff[0],"%s %d %s\r\n",info->httpTpye,info->statusHttp,info->comd); //协议版本号 状态码 命令
	sprintf(buff[1],"Server:(%s)\r\n",info->serverName); //服务器名字
	sprintf(buff[2],"Content-type:%s\r\n",info->connectTpye); //请求内容的传输格式
	sprintf(buff[3],"Content-length:%ld\r\n",info->lenth); //请求内容大小
	sprintf(buff[4],"Connection:%s\r\n",info->connection); //链接方式

	int i;
	for(i = 0; i < 5; i++)
		strcat(headBuff,buff[i]);

	strcat(headBuff,"\r\n");

	return 0;
}

