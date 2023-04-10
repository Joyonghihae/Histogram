#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>



int main(int argc, char *argv[]) {
    if (argc != 4) {
        exit(1);
    }

    int shmID = atoi(argv[3]); 
    int dp1_pid = atoi(argv[2]);
    int dp2_pid = atoi(argv[1]); 
    printf("HERE IS DC!!\n");

   return 0;
}
