// FILE          : dp-2.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 
#include "../inc/dp-2.h"

// void int_handler(int sig) {
    
//     // Detach from shared memory
//     shmdt(shm_ptr);
//     shmdt(semid_ptr);
    
//     // Exit with no statement
//     exit(0);
    
// }
RANDOMDATA *data;

int semID = 0;
int main(int argc, char *argv[]) {

    pid_t dc_pid;

      int shmID = 0;
    // shared ID does not exist
    if (argc < 2) {
    
        return 1;
    }

    shmID = atoi(argv[1]);
    printf("\nDP2 share: %d\n\n",shmID);

    signal (SIGINT, int_handler);
    pid_t dp2_pid = getpid();   
    pid_t dp1_pid = getppid(); 

    // attach to the shared memory segment
    data = (RANDOMDATA *) shmat(shmID, NULL, 0);
    if (data == NULL) {
        printf ("(CONSUMER) Cannot attach to Shared-Memory!\n");
        exit(1);
    }
 
    // read semaphore ID from shared memory
    semID = data->semid;

    printf("semid address: %p \n", &data->semid);
    printf("pos address: %p \n", &data->pos);
     printf("\nDP-2 Semaphore id : %d\n",semID);
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
        execl("../../DC/bin/DC", "DC", dp2_pid_str, dp1_pid_str, shmID_str, NULL); // execute DC with arguments
        exit(1);
    }


    // read current buffer index from shared memory
    // unsigned int* index_ptr = (unsigned int*) (shm_ptr + SHM_SIZE - 2 * sizeof(int));
    // unsigned int index = *index_ptr;
    
    int readIndex = data->readWrite_idx[0];
    int writeIndex = data->readWrite_idx[1];




    while(1)
    {
        //signal(SIGINT, int_handler);

        if (semop (data->semid, &acquire_operation, 1) == -1) 
        {
            printf ("DP-2 smop error\n");
            exit (4);
        }
        // access the current index of the shared memory buffer
        //data->readWrite_idx[1] = data->pos;
        if(data->pos == 256)
        {
            data->pos = 0;
        }
                    
        // ===========FIX : CHECK THE NUMBER 30 LATER  =============================
        // if(writeIndex - readIndex <= 30 || readIndex - writeIndex >=230)
        // {   
        //     usleep(50000);
        //     continue; // IS THIS RIGHT?
        // }
        //========================== FIX ================================================
        
        writeIndex = data->pos;

        // generate random letter and write to buffer
        char random_letter = (char) ((rand() % 20) + 'A');
        data->randomChar[writeIndex]= random_letter;
        printf("%d!!!!!!!!!!!!!!!!!!!!! : %c dp-2\n",data->pos, random_letter);

        data->pos++;
        printf("[DP2] data update: %d\n", data->pos);
        // update the current index of the shared memory buffer
        data->readWrite_idx[1] = data->pos;

        /*
      * release the semaphore - we're leaving the critical region
      */
 
     if (semop (data->semid, &release_operation, 1) == -1) 
     {
       printf ("     (USER2) RELEASE\n");
       break;
     }

     // sleep 1/20 second
        usleep(50000); 

    }

    // shmdt (semid_ptr);
    // shmdt (shm_ptr);
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