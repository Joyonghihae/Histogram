#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>

#define SHM_SIZE 258 // for 256 char buffer and 2 indices
#include "../../Common/sinc/common.h"

//!!! [256]and[257] is read and write buffer!
// -> update 256 and 257 every time
// -> if DP-1 and DP-2 that they have caught up or about to overtake then sleep mode

// Listen to SIGINT signal

// everything finished, remove printf statement

int main() {
    //shared memory var
    int shmID = 0;
    key_t shmem_key;
    pid_t p;
    //unsigned int index = 0;

    //semaphore var
    int semid;

    
    // Get message queue ID
    int msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }

    // get the secret key_code to the area of shared memory we will allocate 
	shmem_key = ftok (".", 'M');
    // cannot allocate key
	if (shmem_key == -1) 
	{ 
	  return 1;
	}

   //if shared mem exists, use it, otherwise allocate the block of memory
	if ((shmID = shmget (shmem_key, SHM_SIZE, 0)) == -1) 
	{
		//create new shared memory
		shmID = shmget (shmem_key, SHM_SIZE, IPC_CREAT | 0660);
		if (shmID == -1) 
		{
          // cannot allocate memory
		  return 2;
		}
	}

    // attach the shared memory segment to DP-1
    char* shm_ptr = (char*) shmat(shmID, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }


    // convert shared memory ID 
    char shmID_str[16] = "";
    sprintf(shmID_str, "%d", shmID);

    //SEMAPHORE
    semid = semget (IPC_PRIVATE, 1, IPC_CREAT | 0666);
	if (semid == -1)
	{
        printf("DP-1 semaphore error");
	    exit (1);
	}

    // Write semaphore ID to shared memory
    int* semid_ptr = (int*) (shm_ptr + SHM_SIZE - sizeof(int));
    *semid_ptr = semid;

    // initialize the semaphore
    if (semctl (semid, 0, SETALL, init_values) == -1) 
	{
        printf("DP-1 semaphore error");
	    exit (1);
	}
    printf("\nDP-1 Semaphore id : %d\n",semid);


    //lanch DP-2
    p = fork();
    if (p == -1) 
    {
        return 1;
    }
    else if (p == 0) 	
    {
        // pass sharedMemoryID
        execl ("../../DP-2/bin/DP-2", "DP-2", shmID_str,NULL);
        exit(1);
       
    }


    int max_idx = 0;
    
    int* index_ptr = (int*) (shm_ptr + SHM_SIZE - 2*sizeof(int));
    int index = *index_ptr;

    for(int x = 0; x<15;x++) // needs to change get SIGINT??
    {   
        
        if (semop (semid, &acquire_operation, 1) == -1)     // fix to ==1? 
        {
            printf ("DP-1 smop error\n");
	        exit (4);
        }

        // access the current index of the shared memory buffer
        index = *index_ptr;
       

        // check if ther is enough space to write 20 characters
        if(index == 256)
        {
            index = 0;
        }
        else if(index >= 235)
        {
            max_idx = 255-index;
        }
        else
        {
            max_idx = 20;
        }
        int i=0;
        //generate 20 random letters and write all 20 letter into the sharedMemory buffer
        for (i = 0; i < max_idx; i++) {
            char random_letter = (char) ((rand() % 20) + 'A');

            // Write the letter to the shared memory buffer
            shm_ptr[index+i] = random_letter;
            printf("%d : %c\n",index+i, random_letter);
        }
        index += i;


        // update the current index of the shared memory buffer
        *index_ptr = index;
  
        sleep(2);    //here? or after release semaphore?
        
	    //release the semaphore 
        if (semop (semid, &release_operation, 1) == -1) 
        {
            printf ("(USER1) GRRRRR.... Can't exit critical region\n");
            semctl (semid, 0, IPC_RMID);
            exit (6);
        }

    }





    // // detach the shared memory segment from DP-1
    // shmdt (shm_ptr);
    // shmdt (semid_ptr);

    //Removing the Shared-Memory resource
    shmctl (shmID, IPC_RMID, 0);

    return 0;
}

// void sigint_handler(int sig) {

    
//     shmdt(shm_ptr); // detach shared memory segment
//     shmctl(shmID, IPC_RMID, NULL); // remove shared memory resource
//     exit(0);
// }
