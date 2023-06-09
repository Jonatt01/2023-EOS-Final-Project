# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h> // key_t
# include <sys/shm.h>

# define STATUS_SIZE 12*sizeof(float)
# define MODE_SIZE 30*12*sizeof(float)


int status_shm_id;
float *status_shm; // float because of the temperature might be float

float* create_status_table(key_t key){

    if ((status_shm_id = shmget(key, STATUS_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(-1);
    }
    if ((status_shm = shmat(status_shm_id, NULL, 0)) == (float *) -1)
    {
        perror("shmat");
        exit(-1);
    }
    memset(status_shm,0,STATUS_SIZE);
    return status_shm;
}

int mode_shm_id;
float *mode_shm;

float* create_mode_table(key_t key){
    if ((mode_shm_id = shmget(key, MODE_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(-1);
    }
    if ((mode_shm = shmat(mode_shm_id, NULL, 0)) == (float *) -1)
    {
        perror("shmat");
        exit(-1);
    }
    memset(mode_shm,0,MODE_SIZE);
    return mode_shm;
}