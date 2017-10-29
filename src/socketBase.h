/*
 * socketBase.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */
#ifndef SRC_SOCKETBASE_H_
#define SRC_SOCKETBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sysInfo.h"

struct socketPack{
	char *id;
	unsigned char flag;
};

int getSocketId(void);
int initSocket(const int,const sysInfo *const);
int getClinetId(const int);

#endif
