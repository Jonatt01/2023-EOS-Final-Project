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
    
    msglen = sprintf(snd,"Start of the Bedroom setting.\nTemperature of airconditioner : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36) = atoi(rcv);
    printf("Air conditioner temperature : %d\n",*(user_mode + 36*user + 12*mode));

    msglen = sprintf(snd,"Brightness of lights (0-5) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 1) = atoi(rcv);
    printf("Lights brightness : %d\n",*(user_mode + 36*user + 12*mode + 1));

    msglen = sprintf(snd,"Level of fan (0-3) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 2) = atoi(rcv);
    printf("Fan level : %d\n",*(user_mode + 36*user + 12*mode + 2));

    msglen = sprintf(snd,"Curtain (0: close, 1: open): ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 3) = atoi(rcv);
    printf("Changed curtain level : %d\n",*(user_mode + 36*user + 12*mode + 3));

    // living room
    msglen = sprintf(snd,"Start of the Living room setting.\nTemperature of airconditioner : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 4) = atoi(rcv);
    printf("Air conditioner temperature : %d\n",*(user_mode + 36*user + 12*mode + 4));

    msglen = sprintf(snd,"Brightness of lights (0-5) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 5) = atoi(rcv);
    printf("Lights brightness : %d\n",*(user_mode + 36*user + 12*mode + 5));

    msglen = sprintf(snd,"Level of fan (0-3) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 6) = atoi(rcv);
    printf("Fan level : %d\n",*(user_mode + 36*user + 12*mode + 6));

    msglen = sprintf(snd,"Curtain (0: close, 1: open): ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 7) = atoi(rcv);
    printf("Changed curtain level : %d\n",*(user_mode + 36*user + 12*mode + 7)); 

    // kitchen
    msglen = sprintf(snd,"Start of the Kitchen setting.\nBrightness of lights (0-5) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 8) = atoi(rcv);
    printf("Lights brightness : %d\n",*(user_mode + 36*user + 12*mode + 8));

    // bathroom
    msglen = sprintf(snd,"Start of the Bathroom setting.\nTemperature of airconditioner : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 9) = atoi(rcv);
    printf("Air conditioner temperature : %d\n",*(user_mode + 36*user + 12*mode + 9));

    msglen = sprintf(snd,"Brightness of lights (0-5) : ");
    write(connfd,snd,msglen+1);
    read(connfd,rcv,BUFFERSIZE);
    *(user_mode + mode*12 + user*36 + 10) = atoi(rcv);
    printf("Lights brightness : %d\n",*(user_mode + 36*user + 12*mode + 10));   

}