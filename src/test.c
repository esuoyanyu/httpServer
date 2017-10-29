/*
 * test.c
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#include "test.h"

int testNet()
{
	int ret  = -1;
	ret = getSocketId();

	sysInfo info;
	char pathBase[URLSIZE];
	sysInit(&info,pathBase);

	if(ret < 0)
	{
		printf("%s%d文件配置失败\n",__func__,__LINE__);
		exit(EXIT_FAILURE);
	}

	initSocket(ret,&info);

	int cid;
	cid  = getClinetId(ret);

	char buff[BUFFSIZE];

	read(cid,buff,sizeof(buff) - 1);

	printf("%s\n",buff);

	return 0;
}

int testEpoll()
{
	int epollId = -1;
	epollId = epollInit(FDNUMMAX);

	int sfd = getSocketId();

	struct epoll_event evn;

	evn.data.fd = sfd;

	sysInfo info;
	char pathBase[URLSIZE];
	int ret = sysInit(&info,pathBase);

	if(ret < 0)
	{
		printf("%s%d文件配置失败\n",__func__,__LINE__);
		exit(EXIT_FAILURE);
	}

	initSocket(ret,&info);

	evn.events = EPOLLIN | EPOLLRDHUP;

	epollAdd(epollId,sfd,&evn);

	struct epoll_event env[FDNUMMAX];
	while(1)
	{
		int num = epoll_wait(epollId,env,FDNUMMAX - 1,-1);
		struct epoll_event* ev = env;
		for(;ev < env + num; ev++)
		{
			if(ev->data.fd == sfd)
			{
				int cfd = -1;

				cfd = getClinetId(sfd);

				struct epoll_event evn;
		    	evn.data.fd = cfd;
		    	evn.events = EPOLLIN | EPOLLRDHUP;
		    	epollAdd(epollId,cfd,&evn);

		    	printf("epoll is cfd = %d\n",cfd);
			}
			else if(ev->data.fd != sfd && ev->events == EPOLLIN)
			{
				char buff[BUFFSIZE];
				memset(buff,0,sizeof(buff));
				read(ev->data.fd,buff,sizeof(buff) - 1);
				printf("%s\n",buff);
			}
			else if(ev->data.fd != sfd && ev->events == (EPOLLIN | EPOLLRDHUP))
			{
				epollRemove(epollId,ev->data.fd,ev);
				printf("套接字关闭fd = %d\n",ev->data.fd);
			}
		}
	}
	return 0;
}

int testQueue() //测试队列
{
	struct queueInfo queuetest;

	queueInit(&queuetest,2);

	struct queue node = {
			.fd = 256,
			.events = 222,
	};

	queueAdd(&queuetest,&node);

	struct queue node1;

	queueRemovSeek(&queuetest,&node1);

	printf("fd = %d enevs = %d\n",node1.fd,node1.events);

	printf("fd = %d enevs = %d\n",node1.fd,node1.events);

	struct queue node2 = {
				.fd = 256,
				.events = 222,
		};

	queueAdd(&queuetest,&node2);

	memset(&node1,0,sizeof(struct queue));
	queueRemovSeek(&queuetest,&node1);
	printf("head = %d tail = %d\n",queuetest.head,queuetest.tail);

	queueRemovSeek(&queuetest,&node1);

	printf("fd = %d enevs = %d\n",node1.fd,node1.events);

	printf("fd = %d enevs = %d\n",node1.fd,node1.events);

	return 0;
}

int testPthread() //测试线程池
{
	struct pthreadArg arg;
	struct queueInfo queues;
	struct workQueue work;

	workQueueInit(&work,&queues);

	queueInit(&queues,20);

	arg.work = &work;
	arg.work->queuei = &queues;
	arg.tidNum = 10;

	pthreadCreatrLoop(&arg);

	sleep(2);

	struct queue env;
	env.fd = getpid();
	env.events = 12345;
	workQueueSet(arg.work,&env);

	sleep(2);

	while(1);

	return 0;
}

int testString()
{
	char ttemp[] = "GET /doing HTTP/1.1\r\nHost: 10.1.18.4:2048\r\nConnection: keep-alive\r\nUser-Agent:Mozilla/5.0 (Windows NT 6.2; rv:40.0) Gecko/20100101 Firefox/40.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3\r\nAccept-Encoding: gzip, deflate\r\nReferer: http://10.1.18.4/\r\n\r\n";

	char t[20][COUNT];
	memset(t,0,sizeof(t));

	httpStringList(ttemp,t);

	httpHead headNode;

	httpGetWord(t,&headNode);

	printf("connection=%s\n",headNode.connection);
	printf("hostAddr=%s\n",headNode.hostAddr);
	printf("httpTpye=%s\n",headNode.httpTpye);
	printf("requestType=%s\n",headNode.requestType);
	printf("url=%s\n",headNode.url);
	printf("prot=%d\n",headNode.prot);
	printf("************************************************\n");

	httpServerHead serverHttp;

	initHttpServerHead(&serverHttp,&headNode,"linux");
	httpSeverStatusAndCmd(&serverHttp,"OK",OK);
	httpServerFileType(&serverHttp,"hahahha",2048);

	printf("httpTpye=%s\n",serverHttp.httpTpye);
	printf("cmd=%s\n",serverHttp.comd);
	printf("connectTpye=%s\n",serverHttp.connectTpye);
	printf("connection=%s\n",serverHttp.connection);
	printf("serverName=%s\n",serverHttp.serverName);
	printf("statusHttp=%d\n",serverHttp.statusHttp);
	printf("statusHttp=%ld\n",serverHttp.lenth);
	printf("************************************************\n");

	char buff[BUFFSIZE];
	memset(buff,0,sizeof(buff));
	setHttpSeverHead(&serverHttp,buff);

	printf("%s\n",buff);

	return 0;

}

int testDealBase()
{
	char path[256];

	getAbsPath("/Ubuntu",path);

	int fd = open("/home/chy/testss",O_RDWR | O_CREAT,0777);

	printf("absPath=%s\n",path);

	if(!isDir(path)) printf("is dir\n");
	else printf("is file\n");

	if(!isRead(path))
		printf("is read\n"); //有问题
	else
		printf("no read\n");

	printf("%ld\n",getFileSzie(path));

	writefd(fd,path);

	close(fd);

	return 0;
}

int testdeal()
{
	int fd = open("/home/chy/testss",O_RDWR | O_CREAT,0777);
	dealInputDate(fd);

	return 0;
}

void testconf()
{
	sysInfo info;
	char pathBase[URLSIZE];
	memset(pathBase,0,sizeof(pathBase));

	int ret = sysInit(&info,pathBase);

	if(!ret)
		printf("%s%d %s\n",__func__,__LINE__,pathBase);
}

