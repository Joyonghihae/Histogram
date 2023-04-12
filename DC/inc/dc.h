// FILE          : dc.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : This is a header file for DC

#include "../../Common/inc/common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <signal.h>


#define DP1_PID
#define DP2_PID
#define SIZE 100
#define ASCII 20
#define SHM_SIZE 272 

#define CHAR_SIZE 256
#define SHM_SIZE 272 

<<<<<<< HEAD
#ifndef __dc_H__
#define __dc_H__
unsigned short init_values[1] = { 1 };

struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };

=======
void int_handler(int sig);
>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb

#ifndef __dc_H__
#define __dc_H__
unsigned short init_values[1] = { 1 };

struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };


#endif
