#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <netdb.h>
#include <semaphore.h>

# include "authentication.h"
# include "create_table.h"
# include "translate.h"
# include "costimizer.h"

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

int serverfd, clinetfd;
struct sockaddr_in server_addr, client_addr;
socklen_t client_len;
char rcvBuffer[MAX_BUFFER_SIZE];
char sendBuffer[MAX_BUFFER_SIZE];


pid_t childpid;

// parameters for device status shared memory
key_t dev_status_key = 1234;
extern int status_shm_id; // defined in create_table.c
int* device_status;

key_t mode_key = 5678;
extern int mode_shm_id; // defined in create_table.c
int* user_mode;

// void create_semaphore()
// {

// }
// void create_msgQueue()
// {
    
// }
void interrupt_handler(int signum);

int socket_server()
{
    {
        // Create a TCP socket
        serverfd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverfd == -1)
        {
            perror("Error: Failed to create socket");
            exit(EXIT_FAILURE);
        }

        // Bind the socket to a local address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("Error: Failed to bind socket");
            exit(EXIT_FAILURE);
        }

        // Listen for incoming connections
        if (listen(serverfd, 5) == -1)
        {
            perror("Error: Failed to listen for connections");
            exit(EXIT_FAILURE);
        }

        printf("Server is listening on port %d...\n", PORT);
        client_len = sizeof(client_addr);
    }
    return serverfd;  
}


int main()
{
    serverfd = socket_server(); // open server

    // create shared status memory
    device_status = create_status_table(dev_status_key);
    
    // for(int i=0;i<12;i++)
    //     printf("%.2f ",*(device_status+i));
    // printf("\n");

    // create user specific mode table
    user_mode = create_mode_table(mode_key);

    while (1)
    {

        clinetfd = accept(serverfd, (struct sockaddr *)&client_addr, &client_len);
        if (clinetfd == -1)
        {
            perror("Error: Failed to accept connection");
            continue;
        }

        printf("New connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // int authenticate = 0; // 0 : failed, 1 : successful
        // authenticate = welcome(clinetfd);
        
        // printf("authenticate result : %d\n",authenticate);
        // if(authenticate == 0){
        //     printf("Wrong user password\n");
        //     printf("Close socket to client\n");
        //     close(clinetfd);
        //     continue;
        // }
        
        childpid = fork();
        if (childpid >= 0)
        {

            if (childpid == 0)
            {                
                // 子程序，處理client的command，Read data from the client
                int msglen = 0;
                while (1)
                {
                    memset(rcvBuffer, 0, MAX_BUFFER_SIZE);
                    memset(sendBuffer, 0, MAX_BUFFER_SIZE);
                    // read data from client
                    read(clinetfd, rcvBuffer, MAX_BUFFER_SIZE);
                    if (strlen(rcvBuffer) > 0)
                    {
                        printf("Received message from client: %s\n", rcvBuffer);
                    }                  

                    // if user request to set their own mode
                    if(strncmp(rcvBuffer,"setmode",7)==0){
                        printf("%d wants to set the user mode.\n",clinetfd);
                        
                        char username[64];
                        char tmp[64];
                        int mode = 0;
                        int user = 0;
                        char *token;

                        token=strtok(rcvBuffer,"|"); // setmode
                        token=strtok(NULL,"|"); // user Jonathan
                        sscanf(token," %s %s",tmp,username);
                        user = whichuser(username);
                    
                        token=strtok(NULL,"|"); // afternoon
                        mode = whichmode(token);
                        printf("\nmode : %d\n",mode);
                        printf(sendBuffer,"%s start to set mode%s\n",username,token);

                        setmode(clinetfd, user_mode, user, mode);
                        
                        // print_int_table(user_mode, 30, 12);

                    }
                    // if user want to set to specifc mode
                    else if(strncmp(rcvBuffer,"mode",4)==0){

                        char mode[64];
                        char username[64];
                        char tmp[64];
                        int user_index = 0;
                        int mode_index = 0;
                        char *token;

                        token=strtok(rcvBuffer,"|"); // mode afternoon
                        sscanf(token,"%s %s",tmp,mode);
                        mode_index = whichmode(mode);

                        token=strtok(NULL,"|"); // user Jonathan
                        sscanf(token," %s %s",tmp,username);
                        user_index = whichuser(username);

                        printf("%s wants to set to %s mode.\n",mode, username);


                    }
                }
            }
            else if (childpid > 0)
            {
                // Parent process,listening the clients connections...
            }
        }
        else
        {
            perror("fork error");
            exit(-1);
        }
        close(clinetfd);
    }
    close(serverfd);
    return 0;
}


void interrupt_handler(int signum){


    // delete shared memory (device status)
    if (shmdt(device_status) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(status_shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    // delete shared memory (user specific mode)
    if (shmdt(user_mode) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(mode_shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }


    close(clinetfd);
    close(serverfd);

    exit(EXIT_SUCCESS);

}