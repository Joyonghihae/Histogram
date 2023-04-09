// FILE          : header.h
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>


#define SHM_SIZE 258 // for 256 char buffer and 2 indices
#define str_size 16
struct sembuf semid_dp1 = { 0, -1, SEM_UNDO };
struct sembuf semid_dp2 = { 0, -1, SEM_UNDO };
unsigned short init_values[1] = { 1 };
