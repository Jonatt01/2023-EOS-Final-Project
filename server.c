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

#define MAX_BUFFER_SIZE 1024
#define PORT 8081

int serverfd, clinetfd;
struct sockaddr_in server_addr, client_addr;
socklen_t client_len;
char rcvBuffer[MAX_BUFFER_SIZE];
char sendBuffer[MAX_BUFFER_SIZE];


pid_t childpid;

// void create_semaphore()
// {

// }
// void create_msgQueue()
// {
    
// }

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

    while (1)
    {

        clinetfd = accept(serverfd, (struct sockaddr *)&client_addr, &client_len);
        if (clinetfd == -1)
        {
            perror("Error: Failed to accept connection");
            continue;
        }

        printf("New connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        int authenticate = 0; // 0 : failed, 1 : successful
        authenticate = welcome(clinetfd);
        
        printf("authenticate result : %d\n",authenticate);
        if(authenticate == 0){
            printf("Wrong user password\n");
            printf("Close socket to client\n");
            close(clinetfd);
            break;
        }
        
        childpid = fork();
        if (childpid >= 0)
        {

            if (childpid == 0)
            {                
                // 子程序，處理client的command，Read data from the client
                while (1)
                {
                    memset(rcvBuffer, 0, MAX_BUFFER_SIZE);
                    memset(sendBuffer, 0, MAX_BUFFER_SIZE);
                    // read data frim client
                    read(clinetfd, rcvBuffer, MAX_BUFFER_SIZE);
                    if (strlen(rcvBuffer) > 0)
                    {
                        printf("Received message from client: %s\n", rcvBuffer);
                    }                  
                    strcat(sendBuffer,"received string .\n");
                    write(clinetfd, sendBuffer, strlen(sendBuffer) + 1);
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
