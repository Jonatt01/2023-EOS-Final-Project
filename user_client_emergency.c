#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int msglen = 0;

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

    char message[MAX_BUFFER_SIZE];
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);

//*********************************testing for authentication*********************************//
    
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);
    msglen = sprintf(message,"login");
    write(sockfd,message,msglen+1);
    printf("Sent login to server: %s\n", message);

    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);
    printf("Jonathan\n");
    ssize_t num_bytes_written = write(sockfd, "Jonathan", strlen("Jonathan")); // send the user id to server


    // read the enter password to server
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);

    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    num_bytes_written = write(sockfd, "123", strlen("123"));     


    // Receive successed or failed response from the server
    memset(buffer,0,MAX_BUFFER_SIZE);
    ssize_t num_bytes_read = read(sockfd, buffer, MAX_BUFFER_SIZE);
    if (num_bytes_read == -1) {
        perror("Error: Failed to receive data from server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("%s", buffer);
//*********************************end of authentication*********************************//

//*********************************start of emergency command*********************************//
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);
    msglen = sprintf(message,"emergency earthquake");
    write(sockfd,message,msglen+1);
    printf("Sent emregency to server: %s\n", message);

    sleep(1);
//*********************************end of emergency command*********************************//   
}