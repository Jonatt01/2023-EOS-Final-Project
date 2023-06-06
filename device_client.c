#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error: Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Error: Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error: Failed to connect to server");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", "127.0.0.1", PORT);


    while(1){

        // Send data to the server
        //char* message = "Hello, server!";
        char message[MAX_BUFFER_SIZE];
        memset(message,0,MAX_BUFFER_SIZE);
        memset(buffer,0,MAX_BUFFER_SIZE);

        // Receive data from the server
        ssize_t num_bytes_read = read(sockfd, buffer, MAX_BUFFER_SIZE);
        if (num_bytes_read == -1) {
            perror("Error: Failed to receive data from server");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("received command : %s\n",buffer);
        // sprintf(message,"Device received msg Ack\n");
        // ssize_t num_bytes_written = write(sockfd, message, strlen(message));
        // if (num_bytes_written == -1) {
        //     perror("Error: Failed to send data to server");
        //     close(sockfd);
        //     exit(EXIT_FAILURE);
        // }

        // printf("Sent message to server: %s\n", message);
    }    

    close(sockfd);

    return 0;
}
