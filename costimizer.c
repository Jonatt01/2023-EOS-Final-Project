# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define BUFFERSIZE 1024
# define MAXUSERNUM 10

void setmode(int connfd, int* user_mode, int user, int mode){
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};
    int msglen = 0;

    memset(snd,0,BUFFERSIZE);// set the snd char array to zero
    memset(rcv,0,BUFFERSIZE);// set the rcv char array to zero
    
    msglen = sprintf(snd,"Enter the Bedroom setting.\nTemperature of airconditioner : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);

    *(user_mode + 36*user + 12*mode) = atoi(rcv);
    printf("changed air conditioner temperature : %d\n",*(user_mode + 36*user + 12*mode));

}