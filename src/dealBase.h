/*
 * dateBase.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_DEALBASE_H_
#define SRC_DEALBASE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/errno.h>
#include "sysInfo.h"

int isDir(const char *const);
int isRead(const char *const);
ssize_t getFileSzie(const char *);
int writefd(const int,const char *const);
int wirteloop(const char *const,const int,const int);
int readloop(char *const,const int,int);
int getAbsPath(const char *const,char *const);

#endif /* SRC_DEALBASE_H_ */
