#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 9090
#define MAX_BUFFER_SIZE 50
#define PATH_SIZE 20

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[50];
    char path[PATH_SIZE];
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

        int fd = 0;
        //int id;

        // 讀取字符串 "/dev/etx_device" 並存儲在 path 中 , 大小為 sizeof(path) -1 (最後一個為結尾符)
        // snprintf(path, sizeof(path), "/dev/etx_device");
        // fd = open(path, O_WRONLY);
        // if (fd < 0) {
        //     perror("Error opening GPIO pin");
        //     exit(EXIT_FAILURE);
        // }

        // printf("buffer = %s , ready to write.\n",buffer);
        // if (write(fd, buffer, sizeof(buffer)) < 0) {
        //     perror("write, set pin output");
        //     exit(EXIT_FAILURE);
        // }

        // printf("write succesfully\n");


    }    

    close(sockfd);

    return 0;
}
