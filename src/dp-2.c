#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define str_size 16

int main(int argc, char *argv[]) {

    int shmID = 0;
    pid_t dc_pid;

    

    // shared ID does not exist
    if (argc < 2) {
    
        return 1;
    }

    shmID = atoi(argv[1]);

    pid_t dp2_pid = getpid();   
    pid_t dp1_pid = getppid(); 

    // launch DC
    dc_pid = fork();
    
    // cannot fork
    if (dc_pid == -1) {
        exit(1);
    } else if (dc_pid == 0) {
        // child process (DC)
        char dp2_pid_str[str_size];
        char dp1_pid_str[str_size];
        char shmID_str[str_size];

        sprintf(dp2_pid_str, "%d", dp2_pid); 
        sprintf(dp1_pid_str, "%d", dp1_pid); 
        sprintf(shmID_str, "%d", shmID); 
        execlp("./DC", "DC", dp2_pid_str, dp1_pid_str, shmID_str, NULL); // execute DC with arguments
        exit(1);
    }

    // attach to the block of shared memory
    char *shm = (char *) shmat(shmID, NULL, 0);
  

    // while(1)
    // {

    // }



    // code to attach to shared memory and use circular buffer goes here


    return 0;

}