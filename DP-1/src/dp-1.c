// FILE          : dp-1.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dp-1.h"

// void int_handler(int sig) {
    
//     // Detach from shared memory
//     shmdt(shm_ptr);
//     shmdt(semid_ptr);
    
//     // Exit with no statement
//     exit(0);
    
// }

typedef struct dataStruc
{
    unsigned int pos;
    int readWrite_idx[2];
	char randomChar[CHAR_SIZE];
    int semid;
} RANDOMDATA;


int main() {
    //shared memory var
    int shmID = 0;
    key_t shmem_key;
    pid_t p;
    RANDOMDATA *data;
    //unsigned int index = 0;

    //signal (SIGINT, allPowerfulSignalHandler);
    
  
    // get the secret key_code to the area of shared memory we will allocate 
	shmem_key = ftok (".", 'M');
    // cannot allocate key
	if (shmem_key == -1) 
	{ 
	  return 1;
	}



    // try to attach to existing shared memory
    shmID = shmget(shmem_key, SHM_SIZE, 0);
    printf("shared id: %d", shmID);
    if (shmID == -1) { 
        // create new shared memory
        shmID = shmget(shmem_key, SHM_SIZE, IPC_CREAT | 0660);
        if (shmID == -1) { 
            
            return 2;
        }
    }

    // attach the shared memory segment to DP-1
    data = (RANDOMDATA*) shmat(shmID, NULL, 0);
    if (data == NULL) {
        printf("ERRROROROR");
        exit(1);
    }
   

    printf("1here2\n");
    // convert shared memory ID 
    char shmID_str[16] = "";
    sprintf(shmID_str, "%d", shmID);

    //SEMAPHORE
    data->semid = semget (IPC_PRIVATE, 1, IPC_CREAT | 0666);
	if (data->semid == -1)
	{
        printf("DP-1 semaphore error");
	    exit (1);
	}

  printf("1here3\n");
    // Write semaphore ID to shared memory
    // int* semid_ptr = (int*) (data + SHM_SIZE - sizeof(int));
    // *semid_ptr = semid;
   
    // initialize the semaphore
    if (semctl (data->semid, 0, SETALL, init_values) == -1) 
	{
        printf("DP-1 semaphore error");
	    exit (1);
	}
    printf("\nDP-1 Semaphore id : %d\n",data->semid);


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
    printf("here?\n");
    // int* index_ptr = (int*) (data + SHM_SIZE - 2*sizeof(int));
    //int index = 0;


    int writeIndex = data->readWrite_idx[0];
    int readIndex = data->readWrite_idx[1];

    /*
	 * initialize our data to blanks, as initially we have no randomized coin 
         * toss data! and initialize the position to the first location
	 * in our circular buffer.
	 */
    printf("\n\n AGAIN\n\n");
	for (int x = 0; x < SHM_SIZE; x++) 
	{
	  data->randomChar[x] = 0;
	}
    for (int x = 0; x < 2; x++) 
	{
	  data->readWrite_idx[x] = 0;
	}
    data->pos = 0;
    printf("!!!%d\n",data->readWrite_idx[1]);

    while(1) // needs to change get SIGINT??
    {   
        //signal(SIGINT, int_handler);

        if (semop (data->semid, &acquire_operation, 1) == -1)     // fix to ==1? 
        {
            printf ("DP-1 smop error\n");
	        exit (4);
        }

        // access the current index of the shared memory buffer
        data->pos = data->readWrite_idx[1];
        if(data->pos > 255)
        {        printf("here?3: %d\n",data->pos);
            data->pos = 0;
        }
        // check if ther is enough space to write 20 characters
        else if(data->pos >= 235)
        {
            max_idx = 256-(data->pos);
        }
        else
        {
            max_idx = 20;
        }
        int i = 0;
        //generate 20 random letters and write all 20 letter into the sharedMemory buffer
        for ( i = 0; i < max_idx; i++) {
            // set Write index to current index
            //writeIndex = data->pos;
            
            // ===========FIX : CHECK THE NUMBER 30 LATER  =============================
            // if(writeIndex - readIndex <= 30 || readIndex - writeIndex >=230)
            // {   
            //     break; // IS THIS RIGHT?
            // }
            //========================== FIX ================================================
            char random_letter = (char) ((rand() % 20) + 'A');

            // Write the letter to the shared memory buffer
            data->randomChar[data->pos+i]= random_letter;
            printf("%d : %c\n",data->pos+i, data->randomChar[data->pos+i]);
            
        }
        data->pos += i;
       
        data->readWrite_idx[1] = data->pos;


        // update the current index of the shared memory buffer
        data->readWrite_idx[1] = data->pos;
  
        sleep(2);    //here? or after release semaphore?
        
	    //release the semaphore 
        if (semop (data->semid, &release_operation, 1) == -1) 
        {
            printf ("(USER1) GRRRRR.... Can't exit critical region\n");
            semctl (data->semid, 0, IPC_RMID);
            exit (6);
        }

    }




    // detach the shared memory segment from DP-1
    // shmdt (shm_ptr);
    // shmdt (semid_ptr);
    // shmctl(shmID, IPC_RMID, 0);
    return 0;
}

