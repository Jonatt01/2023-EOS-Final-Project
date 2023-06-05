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

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

int sockfd, newsockfd;
pid_t childpid;

int socket_server()
{
    
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[MAX_BUFFER_SIZE];
    char sendBuffer[MAX_BUFFER_SIZE];
    {
        // Create a TCP socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            perror("Error: Failed to create socket");
            exit(EXIT_FAILURE);
        }

        // Bind the socket to a local address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("Error: Failed to bind socket");
            exit(EXIT_FAILURE);
        }

        // Listen for incoming connections
        if (listen(sockfd, 5) == -1)
        {
            perror("Error: Failed to listen for connections");
            exit(EXIT_FAILURE);
        }

        printf("Server is listening on port %d...\n", PORT);
        client_len = sizeof(client_addr);
    }

    while (1)
    {

        newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newsockfd == -1)
        {
            perror("Error: Failed to accept connection");
            continue;
        }

        printf("New connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        childpid = fork();
        if (childpid >= 0)
        {

            if (childpid == 0)
            {
                
                // 子程序，處理client的command，Read data from the client
                while (1)
                {
                    memset(buffer, 0, MAX_BUFFER_SIZE);
                    memset(sendBuffer, 0, MAX_BUFFER_SIZE);
                    // read data frim client
                    read(newsockfd, buffer, MAX_BUFFER_SIZE);
                    if (strlen(buffer) > 0)
                    {
                        printf("Received message from client: %s\n", buffer);
                    }                  
                    strcat(sendBuffer,"received string .\n");
                    write(newsockfd, sendBuffer, strlen(sendBuffer) + 1);
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
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}


int main()
{
    socket_server();
}
