#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/msg.h>
#include <semaphore.h>
#include <fcntl.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

void* recommend_thread();
void* command_thread();

int id;
key_t key;
sem_t *wait_signal_sem;

// msgQ message format
struct message
{
    long msg_type;
    char message[50];
};

struct message msg_queue;
int main()
{

    wait_signal_sem = sem_open("/WT", O_CREAT, 0666, 0);
    if(wait_signal_sem == SEM_FAILED){
        perror("wait_signal_sem init failed:");  
        return -1;  
    }

    pthread_t threads[2];
    // 創建thread
    pthread_create(&threads[0], NULL, command_thread, NULL);
    pthread_create(&threads[1], NULL, recommend_thread, NULL);


    // 等待線程完成
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}

void* command_thread(){
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

//*********************************start of reading message queue id*********************************//

    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    // printf("msg queue id : %s\n",buffer);
    id = atoi(buffer);

    sem_post(wait_signal_sem);

//*********************************end of reading message queue id*********************************//

//*********************************start of set expect watt command*********************************//
    memset(message,0,MAX_BUFFER_SIZE);
    memset(buffer,0,MAX_BUFFER_SIZE);
    msglen = sprintf(message,"expect watt | user Jonathan");
    write(sockfd,message,msglen+1);
    printf("Sent set expect watt to server: %s\n", message);

    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("22\n"); // set watt
    write(sockfd,"22",strlen("22")+1);

    sleep(1);
//*********************************end of set expect watt command*********************************//

//*********************************Brain start of setting expect time command*********************************// 

    memset(message,0,MAX_BUFFER_SIZE);
    msglen = sprintf(message,"expect time | user Jonathan");
    write(sockfd,message,msglen+1);
    printf("Sent set expect time to server: %s\n", message);

    //********************set temp of bedroom********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("240\n"); // set airconditioner time
    write(sockfd,"240",strlen("240")+1);

    //********************set light of bedroom********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set fan of bedroom********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set temp of living room********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set light of living room********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set fan of living room********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set light of kitchen********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);

    //********************set temp of bathroom********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);
    //********************set light of bedroom********************//
    memset(buffer,0,MAX_BUFFER_SIZE);
    read(sockfd, buffer, MAX_BUFFER_SIZE);
    printf("%s",buffer);

    printf("360\n");
    write(sockfd,"360",strlen("360")+1);
    
    sleep(1);

//*********************************Brain end of setting expect time command*********************************// 

    sem_close(wait_signal_sem);
    sem_unlink("/WT");
}


void* recommend_thread(){
        
    sem_wait(wait_signal_sem);

    // printf("id in recommend thread : %d",id);

    while(1){
        if (msgrcv(id, &msg_queue, sizeof(struct message) - sizeof(long), 2, 0) == -1)
        {
            perror("msgrcv error");
            exit(1);
        }
        printf("--------------------------RECOMMENDATION--------------------------\n");
        printf("message : %s",msg_queue.message);
        printf("------------------------------------------------------------------\n");
    }

}