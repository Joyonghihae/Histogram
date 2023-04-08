#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SHM_SIZE 258 // for 256 char buffer and 2 indices

int main() {
    //shared memory var
    int shmID = 0;
    key_t shmem_key;
    pid_t p;
    unsigned int index = 0;

    //semaphore var
    int semid;

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

    //lanch DP-2
    p = fork();
    if (p == -1) 
    {
        return 1;
    }
    else if (p == 0) 	
    {
        // should we use semaphore here??? I don't know
        // pass sharedMemoryID
        execl ("./DP-2", "DP-2", shmID_str,NULL);
        exit(1);
       
    }


    //access the circular buffer and indices as needed
    // int* read_index ;
    // int* write_index ;
    // char* buffer = shm_ptr + (sizeof(int) * 2);

    // ?????????????????????????????idk???????????????/
    int max_idx = 0; 
    while(1)
    {

        // ???? idk if using semaphore here or not
        semid = semget (IPC_PRIVATE, 1, IPC_CREAT | 0666);

        // check if ther is enough space to write 20 characters
        if(index >= 235)
        {
            max_idx = 255-index;
            check = true;
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
        }
        index += i;
  
        sleep(2000);
    }





    // detach the shared memory segment from DP-1
    shmdt (shm_ptr);

    //Removing the Shared-Memory resource
    shmctl (shmID, IPC_RMID, 0);

    return 0;
}
