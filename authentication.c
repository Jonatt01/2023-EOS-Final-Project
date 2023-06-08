# include <stdio.h>
# include <stdlib.h>
# include <string.h> // memset
# include <unistd.h> // read write

# define BUFFERSIZE 1024
# define MAXUSERNUM 10

typedef struct user{
    char* name;
    char* password;
}User;

struct user users[MAXUSERNUM];

// return 0 means authentication failed, 1 means successful
int welcome(int connfd){

    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};
    int msglen = 0;

    users[0].name = "Jonathan";
    users[0].password = "12345";

    msglen = sprintf(snd,"Welcome to the smart home system!\nPlease enter your id : ");
    if ((write(connfd,snd,msglen+1))==-1){
        perror("Error: write()\n");
        exit(-1);
    }
    memset(snd,0,BUFFERSIZE);// set the snd char array to zero
    if ((read(connfd,rcv,BUFFERSIZE))==-1){
        perror("Error: read()\n");
        exit(-1);
    }
    printf("received user id : %s\n",rcv);
    char* name = {0};
    char* password = {0};
    // issue : error when compare with non0define index of users
    for(int i=0;i<MAXUSERNUM;i++){
        printf("receive id: %s .\n",rcv);
        if(strcmp(rcv,users[i].name)==0){
            name = users[i].name;
            password = users[i].password;
            break;
        }
    }
    printf("-----------------------\n");
    memset(rcv,0,BUFFERSIZE);// set the rcv char array to zero

    if(password == 0){
        msglen = sprintf(snd,"No user found!\n");
        return 0;
    }
    else{
        msglen = sprintf(snd,"Please enter your password : ");
        if ((write(connfd,snd,msglen+1))==-1){
            perror("Error: write()\n");
            exit(-1);
        }


        if ((read(connfd,rcv,BUFFERSIZE))==-1){
            perror("Error: read()\n");
            exit(-1);
        }
        printf("recv : %s .\n",rcv);
        printf("password : %s .\n",password);
        if(strcmp(rcv,password)==0){
            msglen = sprintf(snd,"Welcome %s!\n",name);
            if ((write(connfd,snd,msglen+1))==-1){
                perror("Error: write()\n");
                exit(-1);
            }
            return 1;
        }
        else{
            msglen = sprintf(snd,"Wrong password...\n");
            if ((write(connfd,snd,msglen+1))==-1){
                perror("Error: write()\n");
                exit(-1);
            }
            return 0;
        }
    }
}