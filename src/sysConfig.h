/*
 * setSys.h
 *
 *  Created on: 2017年10月23日
 *      Author: chy
 */

#ifndef SRC_SYSCONFIG_H_
#define SRC_SYSCONFIG_H_
#define WORDSIZE 10

//const char config[][WORDSIZE] = {"ip","port","rootfs"};

#define IP 0
#define PORT 1
#define ROOTFS 2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "sysInfo.h"
#include "misc.h"

int confree(sysInfo *const);
int sysInit(sysInfo *const,char *const);

#endif /* SRC_SYSCONFIG_H_ */
