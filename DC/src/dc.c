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
  // this is where you could do period work!
  // like saving a file, calculating statistics, etc.
    //printf ("\n2 seconds have elapsed!\n\n");


    alarm(2);   // reset alarm
    
}


int main(int argc, char *argv[]) {
    
    int read;
    char buffer[SHM_SIZE];
    RANDOMDATA *data;
    signal(SIGINT, int_handler);
    signal(SIGALRM, alarm_handler);
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

    int readIndex = data->readWrite_idx[0]-1;
    int writeIndex = data->readWrite_idx[1];
  
    char arr[100] = {0};    // each time 20 * 100
    int j=0;

    int counter = 0;
    printf("DC semid address: %p \n", &data->semid);
    
    while(1){
        
        alarm(2); 
        printf("\nI WOKE UP!!\n");
        // wait for semaphore
        if (semop(data->semid, &acquire_operation, 1) == -1) {
            perror ("DC - Can't end critical region\n");
            
        }
        
        counter++;
        data->pos = data->readWrite_idx[0]; // if it reads until 11, then it should read from 12. -> readIndex = readWritd_idx[0]-1;
        int i= 0;

        for(i = 0; i < 20 ; i++)
        {
            if(data->pos == 256)
            {
                data->pos = 0;
            }
            arr[j] = data->randomChar[data->pos];
            data->readWrite_idx[0]=data->pos;
            printf("DC READ: %d: %c\n",data->pos,arr[j]);            
            data->pos++;
            j++;

        }

        //update
        data->readWrite_idx[0] = data->pos;

        printf("DC COUNTER: %d\n\n",counter);
        if(counter == 5)
        {
            histogram(arr);
            counter = 0;
        }


        if (semop (data->semid, &release_operation, 1) == -1) 
        {
            printf ("RELEASE\n");
        }
        
    }
    shmdt (data);
   return 0;
}

int histogram(char* arr)
{
    //system("clear");
    int j = 0;
    int check;
    int count[ASCII] = {0};
    for (int i = 0; i < ASCII; i++) {
        //Find A~T
        char letter = arr[j];
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
        j++;
    }
}

void int_handler(int sig) {
    kill(dp1_pid, SIGINT);
    kill(dp2_pid, SIGINT);
    shmctl(shmID, IPC_RMID, NULL);
    semctl(shmID, 0, IPC_RMID);
    printf("Shazam!!\n");
    exit(0);
}

