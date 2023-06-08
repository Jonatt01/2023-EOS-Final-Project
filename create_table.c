# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h> // key_t
# include <sys/shm.h>

# define STATUS_SIZE 12*sizeof(float)

int status_id;
float *status_shm; // float because of the temperature might be float


/*
** create the device status table
*/
void create_status_table(key_t key){

    if ((status_id = shmget(key, STATUS_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(-1);
    }
    if ((status_shm = shmat(status_id, NULL, 0)) == (float *) -1)
    {
        perror("shmat");
        exit(-1);
    }
    memset(status_shm,0,STATUS_SIZE);
}

int main(){
    key_t status_key = 1234; // shared memory key of status
    create_status_table(status_key);

    // for(int i=0;i<12;i++)
    //     printf("%f ",*(status_shm+i));
    // printf("\n");
}
