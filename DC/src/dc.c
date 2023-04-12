// FILE          : dc.c
// PROJECT       : Histogram
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/dc.h"
<<<<<<< HEAD

 int shmID;
 int dp1_pid;
 int dp2_pid;
 int shmid;

void int_handler(int sig);
void alarm_handler(int signal_number);
=======

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
>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb


void alarm_handler(int signal_number)
{
  // this is where you could do period work!
  // like saving a file, calculating statistics, etc.
    printf ("\n2 seconds have elapsed!\n\n");

    alarm(2);	// reset alarm
    
}

int main(int argc, char *argv[]) {

    int read;
    char buffer[SHM_SIZE];
    int x;
<<<<<<< HEAD
    RANDOMDATA *data;
    signal(SIGINT, int_handler);
=======
    int count;

>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb
    if (argc != 4) {
        exit(1);
    }

    shmID = atoi(argv[3]); 
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[1]); 
    
    // attach to the shared memory segment
<<<<<<< HEAD
    data = (RANDOMDATA *) shmat(shmID, NULL, 0);
    if (data == NULL) {
=======
    char *shm_ptr = (char *) shmat(shmID, NULL, 0);
    if (shm_ptr == (char*) -1) {
>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb
        perror("shmat");
        exit(1);
    }
    int ind = data->readWrite_idx[1];

    // read semaphore ID from shared memory
<<<<<<< HEAD
    int semid = data->semid;
    printf("\nDC Semaphore id : %d\n",semid);

    int writeIndex = data->readWrite_idx[0];
    int readIndex = data->readWrite_idx[1];
  
    char arr[100] = {0};
    int j=0;
    int count[ASCII] = {0};
    int check;
    
    while(1){

        // wait for semaphore
        if (semop(data->semid, &acquire_operation, 1) == -1) {
            perror ("Can't end critical region\n");
            
        }

        signal(SIGALRM, alarm_handler);
        alarm(2); 

        data->pos = data->readWrite_idx[1];
        int i= 0;
        int left = 0;
        for (i = 0; i < ASCII; i++) {
            if(data->pos == 256)
            {
                data->pos = 0;
            }
            writeIndex = data->pos;
        
            arr[j] = data->randomChar[writeIndex];
            //Find A~T
            char letter = arr[j];
            printf("array: %c\n", arr[j]);
            count[letter - 'A']++;

            printf("%c-%03d ", i + 'A', count[i]);
                if(count[i] / 100 != 0){
                    check = count[i] / 100;
                    check = check / 10;
                    for(int z = 0; z < check; z++){
                        printf("*");
                    }
                    check = 0;
                    check = count[i] % 100;
                    for(int z =0; z < check; z++){
                        printf("+");
                    }
                    check = 0;
                    check = count[i] % 10;
                    for(int z =0; z < check; z++){
                        printf("-");
                    }
                    check = 0;
                }else if(count[i] / 10 != 0){
                    check = count[i] / 10;
                    for(int z = 0; z < check; z++){
                        printf("+");
                    }
                    check = 0;
                    check = count[i] % 10;
                    for(int z =0; z < check; z++){
                        printf("-");
                    }
                    check = 0;
                }else{
                    check = count[i];
                    for(int z = 0; z < check; z++){
                        printf("-");
                    }
                    check = 0;
                }

            printf("\n");  

            readIndex++;
            j++;
        }

        //*(data->character + 257) = readIndex;

        data->readWrite_idx[1] = data->pos;

        // update the current index of the shared memory buffer
=======
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

    
      


>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb

        if (semop (data->semid, &release_operation, 1) == -1) 
        {
            printf ("RELEASE\n");
            break;
        }
<<<<<<< HEAD
        
    }
    shmdt (data);
=======
    }

    
    shmdt (semid_ptr);
    shmdt (shm_ptr);
>>>>>>> e567d9b7416617177ee2b7fcbbe7c09e5dcd38cb
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


