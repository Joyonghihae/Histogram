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

void int_handler(int sig);
void alarm_handler(int signal_number);


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
    RANDOMDATA *data;
    signal(SIGINT, int_handler);
    if (argc != 4) {
        exit(1);
    }

    shmID = atoi(argv[3]); 
    dp1_pid = atoi(argv[2]);
    dp2_pid = atoi(argv[1]); 
    
    // attach to the shared memory segment
    data = (RANDOMDATA *) shmat(shmID, NULL, 0);
    if (data == NULL) {
        perror("shmat");
        exit(1);
    }
    int ind = data->readWrite_idx[1];

    // read semaphore ID from shared memory
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

        if (semop (data->semid, &release_operation, 1) == -1) 
        {
            printf ("RELEASE\n");
            break;
        }
        
    }
    shmdt (data);
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


