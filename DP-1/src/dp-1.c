// FILE          : dp-1.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dp-1.h"

RANDOMDATA *data;
int semID = 0;
int main() {
    //shared memory var
    key_t shmem_key;
    pid_t p;
    int shmID = 0;

    //unsigned int index = 0;
    RANDOMDATA *data;
    signal (SIGINT, int_handler);
    timer_t timerID;
  
    // get the secret key_code to the area of shared memory we will allocate 
   shmem_key = ftok (".", 'M');
    // cannot allocate key
   if (shmem_key == -1) 
   { 
     return 1;
   }



    // try to attach to existing shared memory
    shmID = shmget(shmem_key, sizeof(RANDOMDATA), 0);
   
    if (shmID == -1) { 
        // create new shared memory
        shmID = shmget(shmem_key, sizeof(RANDOMDATA), IPC_CREAT | 0660);
        if (shmID == -1) { 
            
            return 2;
        }
    }
     printf("shared id: %d\n", shmID);
    // attach the shared memory segment to DP-1
    data = (RANDOMDATA*) shmat(shmID, NULL, 0);
    if (data == NULL) {
        perror("shmat error");
        exit(1);
    }
   
    // convert shared memory ID 
    char shmID_str[16] = "";
    sprintf(shmID_str, "%d", shmID);

    //SEMAPHORE
    semID = semget (IPC_PRIVATE, 1, IPC_CREAT | 0666);
   if (semID == -1)
   {
        printf("DP-1 semaphore error");
       exit (1);
   }

    data->semid = semID;
    printf("semid address: %p \n", &data->semid);
    printf("pos address: %p \n", &data->pos);

    // initialize the semaphore
    if (semctl (semID, 0, SETALL, init_values) == -1) 
   {
        perror("DP-1 semaphore error");
       exit (1);
   }


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
    int readIndex = data->readWrite_idx[0];
    int writeIndex = data->readWrite_idx[1];

    
    printf("DP1 SEMAPHORE ID : %d\n\n",semID);


    // initialize data to blank
   for (int x = 0; x < SHM_SIZE; x++) 
   {
     data->randomChar[x] = 0;
   }
    for (int x = 0; x < 2; x++) 
   {
     data->readWrite_idx[x] = 0;
   }
    data->pos = 0;

    //write = 20
    //dp -> read and write -> 

    while(1) // needs to change get SIGINT??
    {   
        //signal(SIGINT, int_handler);
        
        if (semop (semID, &acquire_operation, 1) == -1) 
        {
            printf ("DP-1 smop error\n");
           exit (4);
        }
        data->pos = data->readWrite_idx[1];
        printf("DP-1 SEMA WRITING SEMA : %d \n\n",semID);
         printf("[DP1] data update: %d\n", data->pos);
        if(data->pos > 255)
        {  
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
        printf("1SEMAPHORE ID: %d \n\n",semID);
        //generate 20 random letters and write all 20 letter into the sharedMemory buffer
        for ( i = 0; i < max_idx; i++) {
   
            // ===========FIX : CHECK THE NUMBER 30 LATER =condition  =============================
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
        
       
        sleep(2);    //here? or after release semaphore?

       //release the semaphore 
        if (semop (semID, &release_operation, 1) == -1) 
        {
            printf("\n DP-1 RELEASE ERROR\n");
            semctl (semID, 0, IPC_RMID);
            exit (6);
        }
    }




    // detach the shared memory segment from DP-1
    // shmdt (shm_ptr);
    // shmdt (semid_ptr);
    // shmctl(shmID, IPC_RMID, 0);
    return 0;
}


void int_handler(int sig) {
    
    // Detach from shared memory
    shmdt(data);
    semctl (semID, 0, IPC_RMID, 0);
    // Exit with no statement
    exit(0);
    
}