# include <stdio.h>
# include <stdlib.h>
# include <string.h> // memset
# include <unistd.h> // read write

# define BUFFERSIZE 1024
# define MAXUSERNUM 10

typedef struct user{
    char id[20];
    char password[20];
}User;

User users[MAXUSERNUM];
int numUsers = 0;

char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};
int msglen = 0;


// return 0 means authentication failed, 1 means successful
int welcome(int connfd){

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

    for(int i=0;i<numUsers;i++){
        if(strcmp(rcv,users[i].id)==0){
            name = users[i].id;
            password = users[i].password;
            break;
        }
    }

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
        // printf("recv : %s .\n",rcv);
        // printf("password : %s .\n",password);
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

void signup(char* id, int connfd){

    if (numUsers >= MAXUSERNUM){

        memset(snd,0,BUFFERSIZE);
        msglen = sprintf(snd,"Maximum number of users reached.\n");
        write(connfd,snd,msglen+1);

        printf("Maximum number of users reached.\n");
        close(connfd);
        return;
    }

    // Check if the user ID already exists
    for (int i = 0; i < numUsers; i++){
        if (strcmp(users[i].id, id) == 0){

            msglen = sprintf(snd,"User ID '%s' already exists.\n",id);
            write(connfd,snd,msglen+1);

            printf("User ID '%s' already exists.\n", id);
            close(connfd);
            return;
        }
    }

    memset(snd,0,BUFFERSIZE);
    msglen = sprintf(snd,"Please set your password : ");
    write(connfd,snd,msglen+1);

    memset(rcv,0,BUFFERSIZE);
    read(connfd,rcv,BUFFERSIZE);
    
    // Register the new user
    strcpy(users[numUsers].id, id);
    strcpy(users[numUsers].password, rcv);
    numUsers++;

    memset(snd,0,BUFFERSIZE);
    msglen = sprintf(snd,"User registered successfully.\n");
    if ((write(connfd,snd,msglen+1))==-1){
        perror("Error: write()\n");
        exit(-1);
    }
    printf("User registered successfully.\n");

}

void deleteUser(char* id, int connfd, int* preference, int* mode){
    int foundIndex = -1;

    // Find the user index based on the ID
    for (int i = 0; i < numUsers; i++){
        if (strcmp(users[i].id, id) == 0){
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1){
        printf("User ID '%s' not found.\n", id);
        return;
    }

    // Shift the remaining users to remove the user from the table
    for (int i = foundIndex; i < numUsers - 1; i++){
        strcpy(users[i].id, users[i + 1].id);
        strcpy(users[i].password, users[i + 1].password);
    }
    memset(users[numUsers - 1].id,0,20);
    memset(users[numUsers - 1].password,0,20);

    // adjust the preference table
    for(int i = foundIndex; i < numUsers - 1; i++){
        for(int j = 0; j < 12; j++){
            *(preference + i*12 + j) = *(preference + (i+1)*12 + j);
        }
    }
    for(int j = 0; j<12; j++){
        *(preference + (numUsers - 1)*12 + j) = 0;
    }
    
    // adjust the mode table
    for(int i = foundIndex; i< numUsers - 1; i++){
        for(int j = 0; j < 12; j++){
            *(mode + i*36 + j) = *(mode + (i+1)*36 + j);
            *(mode + i*36 + 12 + j) = *(mode + (i+1)*36 + 12 + j);
            *(mode + i*36 + 24 + j) = *(mode + (i+1)*36 + 12 + j);
        }
    }
    for(int j = 0; j < 12; j++){
        *(mode + (numUsers - 1)*36 + j) = 0;
        *(mode + (numUsers - 1)*36 + 12 + j) = 0;
        *(mode + (numUsers - 1)*36 + 24 + j) = 0;
    }

    numUsers--;

    memset(snd,0,BUFFERSIZE);
    msglen = sprintf(snd,"Successfully delete user %s.\n", id);
    write(connfd,snd,msglen+1);


    printf("User deleted successfully.\n");
}


void printUserTable(User* users){

    printf("User ID\t\tPassword\n");
    printf("-------\t\t--------\n");

    for (int i = 0; i < 10; i++){
        printf("%s\t\t%s\n", users[i].id, users[i].password);
    }
    printf("\n");
}