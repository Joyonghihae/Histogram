// FILE          : dc.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dc.h"

int shmID;
int dp1_pid;
int dp2_pid;
int shmid;

void alarm_handler(int signal_number)
{

  alarm (2);	// reset alarm

  // reactivate signal handler for next time ...

  signal (signal_number, alarm_handler);
}


int main(int argc, char *argv[]) {

    int read;
    char buffer[SHM_SIZE];
    int x;
    int count;

    if (argc != 4) {
        exit(1);
    }

    shmID = atoi(argv[3]); 
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[1]); 
    
    // attach to the shared memory segment
    char *shm_ptr = (char *) shmat(shmID, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    // read semaphore ID from shared memory
    int* semid_ptr = (int*) (shm_ptr + SHM_SIZE - sizeof(int));
    int semid = *semid_ptr;
    printf("\nDC Semaphore id : %d\n",semid);


    int writeIndex = *(shm_ptr + 256); //DP
    int readIndex = *(shm_ptr + 257);  //DC
  
    char arr[100] = {0};
    int j, counter=0;
    while(1)
    {
        // signal(SIGINT, int_handler);
        signal(SIGALRM, alarm_handler);
        printf("\n\nI WOKE UP!!!\n\n");

        //wait for semaphore
        if (semop(semid, &acquire_operation, 1) == -1) {
            printf("semaphore error\n");
            break;
        }

        int i= 0;
        int left = 0;
        for (i = 0; i < 20; i++) {
            if(readIndex == 256)
            {
                readIndex = 0;
            }
        
            arr[j] = shm_ptr[readIndex];
            printf("\n arr[%d] : %c\n",j,arr[j]);
            readIndex++;
            j++;
            if(j==99)
            {
                printf("\n\nPRINT\n\n");
                j=0;
            }
        }
        printf("\nREAD INDEX = %d\n WRITE INDEX = %d\n\n",readIndex,writeIndex);
        counter++;

        // read

        if(counter == 5)
        {
            // PRINT CODE HERE
            counter = 0;
        }

    
      



        if (semop (semid, &release_operation, 1) == -1) 
        {
            printf ("RELEASE\n");
            break;
        }
    }

    
    shmdt (semid_ptr);
    shmdt (shm_ptr);
   return 0;
}

void int_handler(int sig) {
    kill(dp1_pid, SIGINT);
    kill(dp2_pid, SIGINT);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(shmID, 0, IPC_RMID);
    printf("Shazam!!\n");
    exit(0);
}