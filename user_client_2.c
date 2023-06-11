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


    // while(1){

    // Send data to the server
    //char* message = "Hello, server!";
    char message[MAX_BUFFER_SIZE];
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);

    //scanf(" %s",message);
//*********************************start of registeration*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"register");
    // write(sockfd,message,msglen+1);
    // printf("Sent register to server: %s\n", message);

    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer); // enter your id
    // scanf("%s",message);
    // write(sockfd, message, strlen(message)+1);

    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer); // enter your password
    // scanf("%s",message);
    // write(sockfd, message, strlen(message)+1);

    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer); // successful

    // sleep(1);
//*********************************end of registeration*********************************//

//*********************************testing for authentication*********************************//
    
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);
    msglen = sprintf(message,"login");
    write(sockfd,message,msglen+1);
    printf("Sent login to server: %s\n", message);

    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);
    //fgets(message,sizeof(message),stdin);
    scanf("%s",message);
    ssize_t num_bytes_written = write(sockfd, message, strlen(message)); // send the user id to server
    if (num_bytes_written == -1) {
        perror("Error: Failed to send data to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Sent user id to server: %s\n", message);

    // read the enter password to server
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);

    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);
    // fgets(message,sizeof(message),stdin);
    scanf("%s",message);
    num_bytes_written = write(sockfd, message, strlen(message));
    if (num_bytes_written == -1) {
        perror("Error: Failed to send data to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Sent password to server: %s\n", message);        


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

//*********************************start of personal setting*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"setmode | user Jonathan | night");
    // write(sockfd,message,msglen+1);

    // //********************set temp of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set fan of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the level of fan
    // write(sockfd,message,msglen+1);

    // //********************set curtain of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // open or close of curtain
    // write(sockfd,message,msglen+1);

    // //********************set temp of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of light
    // write(sockfd,message,msglen+1);

    // //********************set fan of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the level of fan
    // write(sockfd,message,msglen+1);

    // //********************set curtain of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // open or close of curtain
    // write(sockfd,message,msglen+1);

    // //********************set light of kitchen********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of light
    // write(sockfd,message,msglen+1);

    // //********************set temp of bathroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of air conditioner
    // write(sockfd,message,msglen+1);

//*********************************end of personal setting*********************************//

//*********************************start of mode command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"mode night | user Jonathan");
    // write(sockfd,message,msglen+1);
    // printf("Sent setmode to server: %s\n", message);

    // sleep(1);
//*********************************end of mode command*********************************//

    
//*********************************start of emergency command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"emergency earthquake");
    // write(sockfd,message,msglen+1);
    // printf("Sent emregency to server: %s\n", message);

    // sleep(1);
//*********************************end of emergency command*********************************//   

//*********************************start of control device command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"control device | user Jonathan | bedroom light 3 | livingroom light 2");
    // write(sockfd,message,msglen+1);
    // printf("Sent control to server: %s\n", message);

    // sleep(1);
//*********************************end of control device command*********************************//   

//*********************************start of reservation command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"reservation | user Jonathan | bedroom airconditioner 30 | livingroom light 2");
    // write(sockfd,message,msglen+1);
    // printf("Sent reservation to server: %s\n", message);

    // sleep(1);
//*********************************end of reservation command*********************************// 

//*********************************start of preference setting*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"preference | user Jonathan");
    // write(sockfd,message,msglen+1);

    // //********************set temp of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set fan of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the level of fan
    // write(sockfd,message,msglen+1);

    // //********************set curtain of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // open or close of curtain
    // write(sockfd,message,msglen+1);

    // //********************set temp of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of light
    // write(sockfd,message,msglen+1);

    // //********************set fan of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the level of fan
    // write(sockfd,message,msglen+1);

    // //********************set curtain of living room********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // open or close of curtain
    // write(sockfd,message,msglen+1);

    // //********************set light of kitchen********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of light
    // write(sockfd,message,msglen+1);

    // //********************set temp of bathroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the temperature of air conditioner
    // write(sockfd,message,msglen+1);

    // //********************set light of bedroom********************//
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("%s",buffer);

    // scanf("%s",message); // the brightness of air conditioner
    // write(sockfd,message,msglen+1);
    
    // sleep(1);
//*********************************end of personal setting*********************************//    

//*********************************start of room command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"room | user Jonathan | bedroom comfort | kitchen comfort");
    // write(sockfd,message,msglen+1);
    // printf("Sent control to server: %s\n", message);

    // sleep(1);
//*********************************end of room command*********************************//  


//*********************************start of control device command*********************************//
    // memset(message,0,MAX_BUFFER_SIZE);
    // memset(buffer,0,MAX_BUFFER_SIZE);
    // msglen = sprintf(message,"control device | user Jonathan | bedroom light comfort | livingroom light comfort");
    // write(sockfd,message,msglen+1);
    // printf("Sent control to server: %s\n", message);

    // sleep(1);
//*********************************end of control device command*********************************//  
    
    // }
    

    close(sockfd);

    return 0;
}
