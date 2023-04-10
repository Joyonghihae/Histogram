// FILE          : dp-2.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : This is a header file for DP-2

#ifndef __dp_2_H__
#define __dp_2_H__

#include "../../Common/inc/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <signal.h>

#define str_size 16
struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };


#endif