// FILE          : dp-2.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 
#include "../inc/dp-2.h"

int main(int argc, char *argv[]) {

    int shmID = 0;
    pid_t dc_pid;

    

    // shared ID does not exist
    if (argc < 2) {
    
        return 1;
    }

    shmID = atoi(argv[1]);
    int* semid_ptr = (int*) shmat(shmID, NULL, 0);
    int semid = *semid_ptr;

    printf("\nDP-2 Semaphore id : %d\n",semid);

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
        execlp("../../DC/bin/DC", "DC", dp2_pid_str, dp1_pid_str, shmID_str, NULL); // execute DC with arguments
        exit(1);
    }

    // attach to the block of shared memory
    char *shm = (char *) shmat(shmID, NULL, 0);

    for( int x = 0; x<10; x++)
    {
        if (semop (semid, &acquire_operation, 1) == -1) 
        {
            printf ("     (USER2) AHHHHH... Can't start critical region\n");
            break;
        }
        printf("YEEEEE\n");

        /*
      * release the semaphore - we're leaving the critical region
      */

     if (semop (semid, &release_operation, 1) == -1) 
     {
       printf ("     (USER2) RELEASE\n");
       break;
     }

    }



    // code to attach to shared memory and use circular buffer goes here

    shmdt (semid_ptr);
    return 0;

}