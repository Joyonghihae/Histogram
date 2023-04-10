#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SHM_SIZE 258 // for 256 char buffer and 2 indices
#define str_size 16
#include "../inc/common.h"





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

    // attach to the shared memory segment
    char *shm_ptr = (char *) shmat(shmID, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    // read semaphore ID from shared memory
    int* semid_ptr = (int*) (shm_ptr + SHM_SIZE - sizeof(int));
    int semid = *semid_ptr;
    printf("\nDP-2 Semaphore id : %d\n",semid);

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


    // read current buffer index from shared memory
    unsigned int* index_ptr = (unsigned int*) (shm_ptr + SHM_SIZE - 2 * sizeof(int));
    unsigned int index = *index_ptr;


    while(signal(SIGINT,sigint_handler))
    {
        if (semop (semid, &acquire_operation, 1) == -1) 
        {
            printf ("  (USER2) AHHHHH... Can't start critical region\n");
            break;
        }
        // access the current index of the shared memory buffer
        index = *index_ptr;
        
        if(index == 256)
        {
            index = 0;
        }
        
        
        // generate random letter and write to buffer
        char random_letter = (char) ((rand() % 20) + 'A');
        shm_ptr[index] = random_letter;
        printf("%d : %c\n", index, random_letter);

        index += 1;

        // update the current index of the shared memory buffer
        *index_ptr = index;

        // sleep 1/20 second
        usleep(50000); 

        /*
	   * release the semaphore - we're leaving the critical region
	   */
 
	  if (semop (semid, &release_operation, 1) == -1) 
	  {
	    printf ("     (USER2) RELEASE\n");
	    break;
	  }

    }



    // shmdt (semid_ptr);
    // shmdt (shm_ptr);
    return 0;



}

// void sigint_handler(int sig) {
//     shmdt(shm_ptr); // detach shared memory segment
//     exit(0);
// }