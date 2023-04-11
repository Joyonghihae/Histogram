// FILE          : dc.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dc.h"
 int shmID;
 int dp1_pid;
 int dp2_pid;
 char* shm_ptr;
 int shmid;


int main(int argc, char *argv[]) {

    int read;
    char buffer[SHM_SIZE];
    int x;

    if (argc != 4) {
        exit(1);
    }

    shmID = atoi(argv[3]); 
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[1]); 
    printf("share memory: %d\n", shmID);
    printf("dp-1 process ID:%d\n", dp1_pid);
    printf("dp-2 process ID:%d\n", dp2_pid);
    
    signal(SIGINT, int_handler);
    signal(SIGALRM, allPowerfulSignalHandler);
    
   
    if ((shmid = shmget (shmID, SHM_SIZE, 0)) == -1) 
	{
		// note: you could try setting up your own fork/exec here
		// to launch the 2nd app!!

		printf ("(DP) Shared-Memory doesn't exist. run the PRODUCER!\n");
		return 2;
	}

    printf ("(DC) Our Shared-Memory ID is %d\n", shmid);

    // DC application will attach itself to the shared memory
    char *shm_ptr = (char *) shmat(shmid, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    // read semaphore ID from shared memory
    int* semid_ptr = (int*) (shm_ptr + SHM_SIZE - sizeof(int));
    int semid = *semid_ptr;


        // read current buffer index from shared memory
    unsigned int* index_ptr = (unsigned int*) (shm_ptr + SHM_SIZE - 2 * sizeof(int));
    unsigned int index = *index_ptr;


    while(1){
        // wait for semaphore
        if (semop(semid, &acquire_operation, 1) == -1) {
            perror ("Can't end critical region\n");
            break;
        }
        

        // access the current index of the shared memory buffer
        index = *index_ptr;
        
        if(index == 256)
        {
            index = 0;
        }

        
        for (int i = 0; i < 20; i++) {
            printf("%c-%03d ", i + 'A', buffer[i]);
            for (int j = 0; j < buffer[i]; j++) {
                if (j % 100 == 0) {
                    printf("*");
                } else if (j % 10 == 0) {
                    printf("+");
                } else {
                    printf("-");
                }
            }
            printf("\n");
        }

        index += 1;

        // update the current index of the shared memory buffer
        *index_ptr = index;

        if (semop (semid, &release_operation, 1) == -1) 
        {
            printf ("RELEASE\n");
            break;
        }
        sleep(10);
    }

    shmdt (semid_ptr);
    shmdt (shm_ptr);
   return 0;
}

void int_handler(int sig) {
    kill(dp1_pid, SIGINT);
    kill(dp2_pid, SIGINT);
    shmdt(shm_ptr);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(shmID, 0, IPC_RMID);
    printf("Shazam!!\n");
    exit(0);
}