/*
 * test.h
 *
 *  Created on: 2017年10月18日
 *      Author: chy
 */

#ifndef SRC_TEST_H_
#define SRC_TEST_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/unistd.h>
#include "deal.h"
#include "sysInfo.h"
#include "dealBase.h"
#include "sysConfig.h"
#include "queueBase.h"
#include "socketBase.h"
#include "epollBase.h"
#include "pthreadBase.h"
#include "misc.h"

void testconf();
int testdeal();
int testDealBase();
int testString();
int testNet();
int testQueue();
int testEpoll();
int testPthread();
int test();

#endif /* SRC_TEST_H_ */
