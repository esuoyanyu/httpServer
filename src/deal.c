/*
 * deal.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */
#include "deal.h"

static const char requestType[][SIZE] = {"GET"};
static const char http[] = "<html>\
<title> Dir Browser\
</title>\
<meta http-equiv=\"Content-Language\" content=\"utf8\">\
<style type=""text/css""> a:link{text-decoration:none;}\
</style>\
<body bgcolor=""ffffff"" font-family=Arial color=#fff font-size=14px>\r\n";

void ret405(const int cfd)
{
#ifdef DEBUG
	printf("*************************************\n");
	printf("%s\n%s\n",__func__,RETURN405);
	printf("*************************************\n");
#endif
	wirteloop(RETURN405,strlen(RETURN405),cfd);
}

char pathBase[URLSIZE];

//path[0] 服务器的相对地址
//path[1] 网页的绝对地址
static int sethttppage(const char *const pagePath,const char *const path,const int fd)
{
	DIR *dir = NULL;
	struct stat dirName;
	char buff[BUFFSIZE]; //缓存每一项的内容
	char image[BUFFSIZE]; //设置图标
	int num = 1;
	struct dirent *dirInfo = NULL;

	dir = opendir(path); //打开目录

	const char *tt = strstr(path,pathBase); //寻找前戳

	if(tt)
		tt = path + strlen(pathBase); //去掉前缀

#ifdef DEBUG
	printf("%s %d\npath=%s pagePath=%s\n",__func__,__LINE__,path,pagePath);
#endif

	//清空缓冲区
	memset(buff,0,sizeof(buff));
	memset(image,0,sizeof(image));

	wirteloop(http,strlen(http),fd); //把缓冲区信息写到文件

	while((dirInfo = readdir(dir)))
	{
		stat(dirInfo->d_name,&dirName);//获取信息

		if(dirInfo->d_name[0] == '.')
			continue;

		if(S_ISDIR(dirName.st_mode))
	         sprintf(image,"<img src=""dir.png"" width=""24px"" height=""24px"">");
		else
	         sprintf(image,"<img src=""file.png"" width=""24px"" height=""24px"">");

		sprintf(buff,"%s<p><pre>%-2d""<a href=%s%s"">%-15s</a></pre></p>\r\n",buff,num++,tt,dirInfo->d_name,dirInfo->d_name);

		wirteloop(buff,strlen(buff),fd); //把缓冲区信息写到文件
		memset(buff,0,sizeof(buff)); //缓存每一项的内容
	}
	wirteloop("</body></html>",strlen("</body></html>"),fd); //写入文件尾部
	close(fd);
	return getFileSzie(pagePath);
}

static int dealDir(const dealInfoPtr info)
{
	char page[URLSIZE];
	sprintf(page,"%s/page%ld.http",pathBase,(unsigned long int)pthread_self()); //产生网页文件的名称
	int fd = open(page,O_CREAT | O_RDWR,0777); //打开文件 若不存在则创建

	if(-1 == fd)
		return -1;

	if(info->path[strlen(info->path) - 1] != '/') //是目录但是没有最后‘/’ 则添加
		strcat(info->path,"/");

	int len = sethttppage(page,info->path,fd); //生成网页 并返回文件大小

	if(-1 == len)
		return -1;

	if(httpServerFileType(info->server,"text/html;charset=utf8",len))
		return -1;
	if(httpSeverStatusAndCmd(info->server,"OK",OK))
		return -1;

	info->server->lenth = len;

	char temp[BUFFSIZE * 2];
	memset(temp,0,sizeof(temp));
	if(setHttpSeverHead(info->server,temp))
		return -1;

	if(wirteloop(temp,strlen(temp),info->fd)) //发送http头
		return -1;

	if(writefd(info->fd,page)) //发送数据
		return -1;

	unlink(page);

	return 0;
}

static int dealFile(const dealInfoPtr info)
{
	ssize_t len;
	len = getFileSzie(info->path);

	if(-1 == len)
	{
#ifdef DEBUG
		printf("%s %d\n 文件大小获取失败\n\n",__func__,__LINE__);
#endif
		return -1;
	}

	if(!isRead(info->path)) //可以读
	{
		httpServerFileType(info->server,"applicaiton/octet-stream",len);

#ifdef DEBUG
		printf("%s %d\n file size = %d\nthis file is read\n",__func__,__LINE__,len);
#endif

		httpSeverStatusAndCmd(info->server,"OK",OK);
		char temp[BUFFSIZE];
		memset(temp,0,sizeof(temp));
		setHttpSeverHead(info->server,temp);
#ifdef DEBUG
		printf("%s %d len=%d\n%s\n",__func__,__LINE__,len,temp);
#endif

		wirteloop(temp,strlen(temp),info->fd);
		writefd(info->fd,info->path);
	}
	else //没有读权限 返回405
	{
		ret405(info->fd);
	}
	return 0;
}

static int dealGetRequest(const dealInfoPtr info) //处理客户端get请求
{
	if(!isDir(info->path)) //如果是目录 则 显示目录信息
	{
#ifdef DEBUG
		printf("%s %d\nthis is DIR\n",__func__,__LINE__);
#endif
		if(dealDir(info))
		{
			ret405(info->fd);
			return -1;
		}
	}
	else//如果是文件检查读权限然后下载
	{
#ifdef DEBUG
		printf("%s %d\nthis is file\n",__func__,__LINE__);
#endif
		if(dealFile(info) < 0)
		{
			ret405(info->fd);
			return -1;
		}
	}

	close(info->fd);
	return 0;
}

static int defDeal(const dealInfoPtr info) //处理默认请求
{
	ret405(info->fd);

	return 0;
}

static int dealRequest(const dealInfoPtr info) //处理请求
{
	int requestTpye = stringToInt(requestType,info->clnet->requestType);

	switch(requestTpye)
	{
	case GET: dealGetRequest(info);
#ifdef DEBUG
	printf("%s %d\nthis is get\n",__func__,__LINE__);
#endif
	break;
	default: defDeal(info);
#ifdef DEBUG
	printf("%s %dthis is def\n",__func__,__LINE__);
#endif
	}

	return 0;
}

int freeHttpHeadPtr(const httpHeadPtr headNode)
{
	if(!headNode)
		return -1;

	if(headNode->connection) free(headNode->connection);

	if(headNode->hostAddr) free(headNode->hostAddr);

	if(headNode->httpTpye) free(headNode->httpTpye);

	if(headNode->requestType) free(headNode->requestType);

	if(headNode->url) free(headNode->url);

	return 0;
}

int freehttpServerHeadPtr(const httpServerHeadPtr serverHttp)
{
	if(!serverHttp)
		return -1;

	if(serverHttp->connection) free(serverHttp->connection);

	if(serverHttp->comd) free(serverHttp->comd);

	if(serverHttp->httpTpye) free(serverHttp->httpTpye);

	if(serverHttp->connectTpye) free(serverHttp->connectTpye);

	if(serverHttp->serverName) free(serverHttp->serverName);

	return 0;
}


int dealInputDate(const int cfd)
{
	char buff[BUFFSIZE * 8];
	char buffs[ROW][COUNT];
	memset(buff,0,sizeof(buff));
	memset(buffs,0,sizeof(buffs));

	readloop(buff,cfd,BUFFSIZE * 8); //读取浏览器发送的数据

#ifdef DEBUG
	printf("%s\n",buff);
#endif

	if(httpStringList(buff,buffs)) //分解头文件信息
	{
#ifdef DEBUG
	printf("%s %d 分解头文件信息\n",__func__,__LINE__);
#endif
		ret405(cfd);
		return -1;
	}

	httpHead headNode;
	if(httpGetWord(buffs,&headNode)) //解析头文件 并 填充httpHead结构体
	{
#ifdef DEBUG
	printf("%s %d 解析头文件 并 填充httpHead结构体\n",__func__,__LINE__);
#endif
		freeHttpHeadPtr(&headNode); //释放HttpHead结构体
		ret405(cfd);
		return -1;
	}

	httpServerHead serverHttp;
	if(initHttpServerHead(&serverHttp,&headNode,"linux")) //填充返回头文件httpServerHead结构体
	{
#ifdef DEBUG
	printf("%s %d 填充返回头文件httpServerHead结构体\n",__func__,__LINE__);
#endif
		freeHttpHeadPtr(&headNode); //释放HttpHead结构体
		freehttpServerHeadPtr(&serverHttp); //释放httpServerHeadPtr结构体
		ret405(cfd);
		return -1;
	}

	char path[FDNUMMAX];
	memset(path,0,sizeof(path));
	if(getAbsPath(headNode.url,path)) //获取绝对路径
	{
#ifdef DEBUG
	printf("%s %d 获取绝对路径失败\n",__func__,__LINE__);
#endif
		freeHttpHeadPtr(&headNode); //释放HttpHead结构体
		freehttpServerHeadPtr(&serverHttp); //释放httpServerHeadPtr结构体
		ret405(cfd);
		return -1;
	}
#ifdef DEBUG
	printf("%s %d\npath=%s\npathABS=%s\n",__func__,__LINE__,headNode.url,path);
#endif

	dealInfo info = {
			.fd = cfd,
			.clnet = &headNode,
			.server = &serverHttp,
	};

	strcpy(info.path,path);
	if(dealRequest(&info)) //处理链接请求
	{
#ifdef DEBUG
	printf("%s %d 处理链接请求失败\n",__func__,__LINE__);
#endif
		ret405(cfd);
		return -1;
	}

	freeHttpHeadPtr(&headNode); //释放HttpHead结构体
	freehttpServerHeadPtr(&serverHttp); //释放httpServerHeadPtr结构体

	return 0;
}

