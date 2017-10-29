/*
 * sysInfo.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_SYSINFO_H_
#define SRC_SYSINFO_H_

#define BUFFSIZE 512 //buff的最大值
#define URLSIZE 512 //url最大值

#define FDNUMMAX 256 //epoll监听的最大值
#define LINSTENMAX 128

#define PATHREADNUM 64 //线程池最大值
#define WORKQUEUENUM PATHREADNUM * 2 //工作队列最大值
#define CONFIGPATH "../etc/httpServerConf" //配置文件路径
#define ETCCONFIGPATH "etc/httpServerConf" //配置文件路径

 //return 405
#define RETURN405 "HTTP/1.1 405 method not allowed\r\n\
Server:(linux\r\n\
Content-type:text/html;charset=UTF8\r\n\
Content-length:195\r\n\
Connection: keep-alive\r\n\r\n\
<html>\
<title> Dir Browser </title>\
<style type=""text/css""> a:link{text-decoration:none;} </style>\
<body bgcolor=""ffffff"" font-family=Arial color=#fff font-size=14px>\
405 method not allowed\
</body>\
</html>"

extern char pathBase[]; //根目录的相对服务器的路径

typedef struct
{
	char *ip;
	int port;
	char *root;
	char *syslog;
}sysInfo;

#endif /* SRC_SYSINFO_H_ */
