// FILE          : dc.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/header.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        exit(1);
    }

     printf("dc\n");
    int shmID = atoi(argv[3]); 
    int dp1_pid = atoi(argv[2]);
    int dp2_pid = atoi(argv[1]); 


   return 0;
}
