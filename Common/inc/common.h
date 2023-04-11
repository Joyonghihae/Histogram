// FILE          : common.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#ifndef __SEMAPHORE_STRUCT_H__
#define __SEMAPHORE_STRUCT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <signal.h>


#else
extern struct sembuf acquire_operation;
extern struct sembuf release_operation;

#endif
void alarmHandler(int signal_number);
void allPowerfulSignalHandler (int signal_number);