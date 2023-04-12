// FILE          : dp-2.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : This is a header file for DP-2


#include "../../Common/inc/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define SHM_SIZE 272 
#define str_size 16

#ifndef __SEMAPHORE_STRUCT_H__
#define __SEMAPHORE_STRUCT_H__

struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };

unsigned short init_values[1] = { 1 };

#else

extern struct sembuf acquire_operation;
extern struct sembuf release_operation;
#endif

void int_handler(int sig);
