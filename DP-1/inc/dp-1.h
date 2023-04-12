// FILE          : dp-1.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : This is a header file for DP-1
#include "../../Common/inc/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>


#define SHM_SIZE 272 

#ifndef __dp_1_H__
#define __dp_1_H__

struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };

unsigned short init_values[1] = { 1 };
#endif
