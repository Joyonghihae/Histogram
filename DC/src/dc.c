// FILE          : dc.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dc.h"

int main(int argc, char *argv[]) {

    int read;
    signal (SIGINT, alarmHandler);

    if (argc != 4) {
        exit(1);
    }

    int semid;
    int shmID = atoi(argv[3]); 
    int dp1_pid = atoi(argv[2]);
    int dp2_pid = atoi(argv[1]); 
    printf("share memory: %d\n", shmID);
    printf("dp-1 process ID:%d\n", dp1_pid);
    printf("dp-2 process ID:%d\n", dp2_pid);

    // DC application will attach itself to the shared memory
    char* shm_ptr = (char*) shmat(shmID, NULL, 0);
    if (shm_ptr == (char*) -1) {
        perror("shmat");
        exit(1);
    }

    //initialize semaphore
    semid = semget(shmID, 1, 0666 | IPC_CREAT);
    if (semid == -1)
	{
	  printf ("Couldn't get semaphores!\n");
	  exit (1);
	}
    int counts[20] = {0};

    while(1){
        // wait for semaphore
        if (semop(semid, &acquire_operation, 1) == -1) {
            perror ("Can't end critical region\n");
            break;
        }
         

        char check = shm_ptr[read];
        read++;

        if(read >= 256){
            read = 0;
        }

        counts[check - 'A']++;

        for (int i = 0; i < 20; i++) {
            printf("%c-%03d ", i + 'A', counts[i]);
            for (int j = 0; j < counts[i]; j++) {
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


        sleep(10);
    }

    shmdt (shm_ptr);
    shmctl (shmID, IPC_RMID, 0);

   return 0;
}




