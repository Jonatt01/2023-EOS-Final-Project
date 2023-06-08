#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/shm.h>
#include <unistd.h> // for close
#define MAX_BUFFER_SIZE 1024
#define DEVICE_PORT 9090
#define STATUS_PORT 9091
#define SM_SIZE 256

/*     創建 Msg Queue     */

key_t msgQkey;
int msgQid;
struct message {
    long msg_type;
    char msg_text[MAX_BUFFER_SIZE];
};
struct message msgQ;

/*     創建 Shared memory     */
key_t shmKey;
int* shmemArr;
int shmid;


int serverSocket, clientSocket;
int sockfd, connfd;

void interrupt_handler(int signum){

    // 刪除 Msg Queue
    if (msgctl(msgQid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
    printf("MsgQ deleted!\n");

    // 刪除 Shared memory
    if (shmdt(shmemArr) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    exit(EXIT_SUCCESS);

    close(clientSocket);
    close(serverSocket);
    close(sockfd);

}

void* status_thread(void* arg)
{
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];
    int addrLen = sizeof(clientAddr);
    
    // 建立 Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("錯誤：無法建立 Socket");
        exit(1);
    }
    
    // 設定 Server 的 IP 和 Port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(STATUS_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // 綁定 Socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("錯誤：無法綁定 Socket");
        exit(1);
    }
    
    // 監聽連線請求
    if (listen(sockfd, 5) < 0) {
        perror("錯誤：無法監聽連線請求");
        exit(1);
    }
    
    printf("等待連線...\n");
    
    // 接受連線請求
    connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
    if (connfd < 0) {
        perror("錯誤：無法接受連線請求");
        exit(1);
    }

    /*                Shared Memory                */
    {

        shmKey = ftok(".", 'S');
        if (shmKey == -1) {
            perror("ftok");
            exit(1);
        }

        shmid = shmget(shmKey, SM_SIZE * sizeof(int), IPC_CREAT | 0666);
        if (shmid == -1) {
            perror("shmget error");
            exit(1);
        }

        shmemArr = (int*)shmat(shmid, NULL, 0);
        if (shmemArr == (int*)-1) {
            perror("shmat error");
            exit(1);
        }

    }
     /*                Shared Memory                */
    
    signal(SIGINT,interrupt_handler);
    // 接收訊息並處理
    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);
        
        // 接收訊息
        if (recv(connfd, buffer, MAX_BUFFER_SIZE, 0) < 0) {
            perror("錯誤：接收訊息失敗");
            exit(1);
        }
        
        if(strlen(buffer) > 0){
            printf("接收到的訊息：%s", buffer);
        }

        //將狀態寫到shared memory
        
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }
    }
    
    // 關閉 Socket
    close(sockfd);
}

void* command_thread(void* arg)
{
    struct sockaddr_in serverAddr, clientAddr;
    char commandBuffer[MAX_BUFFER_SIZE];
    
    {   // 獲得 msgQ 參數
        // 產生唯一的 key
        msgQkey = ftok(".", 'a');
        if (msgQkey == -1) {
            perror("ftok");
            exit(1);
        }
        // 創建訊息佇列
        msgQid = msgget(msgQkey, IPC_CREAT | 0666);
        if (msgQid == -1) {
            perror("msgget");
            exit(1);
        }
    }    
    
    {   // 建立和devices的socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            perror("錯誤：無法建立 Socket");
            exit(1);
        }

        // 設定 Server 的 IP 和 Port
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(DEVICE_PORT);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        // 綁定 Socket
        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("錯誤：無法綁定 Socket");
            exit(1);
        }

        // 監聽連線請求
        if (listen(serverSocket, 5) < 0) {
            perror("錯誤：無法監聽連線請求");
            exit(1);
        }

        printf("Wait for device connection...\n");

        // 接受連線請求
        socklen_t addrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            perror("錯誤：無法接受連線請求");
            exit(1);
        }
        else{
            printf("Device connection successful!\n");
        }

    }
    
    // 接收msgQ裡面的device command
    signal(SIGINT,interrupt_handler);
    while (1) {
        // 接收指令
        if (msgrcv(msgQid, &msgQ, sizeof(struct message) - sizeof(long), 1, 0) == -1) {
            perror("msgrcv error");
            exit(1);
        }
        printf("接收到的指令：%s\n", msgQ.msg_text);
        // 複製msgQ裡面的指令內容到socket buffer
        strncpy(commandBuffer,msgQ.msg_text,MAX_BUFFER_SIZE);
        // 將msgQ接收到的指令透過socket轉送給device
        if (send(clientSocket, commandBuffer, strlen(commandBuffer), 0) < 0) {
            perror("錯誤：發送訊息失敗");
            exit(1);
        }
    }
    exit(EXIT_SUCCESS);
}

int main() {

    pthread_t threads[2];
    // 創建thread
    pthread_create(&threads[0], NULL, command_thread, NULL);
    pthread_create(&threads[1], NULL, status_thread, NULL);

    // 等待線程完成
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    
    return 0;
}
