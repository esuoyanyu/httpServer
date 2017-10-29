/*
 * misc.h
 *
 *  Created on: 2017年10月24日
 *      Author: chy
 */

#ifndef SRC_MISC_H_
#define SRC_MISC_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <unistd.h>


int stringti(const char *const);
int syslog(const char *const);

#endif /* SRC_MISC_H_ */
