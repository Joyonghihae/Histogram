// FILE          : common.h
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
#include <signal.h>

<<<<<<< HEAD
#define CHAR_SIZE 256
void alarmHandler(int signal_number);
void allPowerfulSignalHandler (int signal_number);
typedef struct dataStruc
{
    int pos;
    int readWrite_idx[2];
   char randomChar[CHAR_SIZE];
   char character[CHAR_SIZE];
    int semid;
} RANDOMDATA;
=======

void alarmHandler(int signal_number);
void allPowerfulSignalHandler (int signal_number);
>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb
