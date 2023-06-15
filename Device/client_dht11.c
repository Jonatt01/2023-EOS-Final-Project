#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define SERVER_IP "172.20.10.4"
//#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 9091
#define DEVICE_FILE "/dev/dht11"
#define DEVICE_FILE_2 "/dev/dht11_2"

int main() {
    // 创建 Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket creation failed");
        return 1;
    }

    // 设置服务器地址信息
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("invalid address");
        return 1;
    }

    // 连接服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connection failed");
        close(sockfd);
        return 1;
    }
    char buffer[3];
	char buffer2[3];
	char sendbuffer[256];
    while(1){
		memset(buffer,0,3);
		memset(buffer2,0,3);
		memset(sendbuffer,0,256);

		 // 打开 /dev/dht11 设备
		int device_fd = open(DEVICE_FILE, O_RDONLY);
		if (device_fd == -1) {
			perror("failed to open device");
			close(sockfd);
			return 1;
		}   
		
		int device_fd_2 = open(DEVICE_FILE_2, O_RDONLY);
		if (device_fd_2 == -1) {
			perror("failed to open device");
			close(sockfd);
			return 1;
		}                                                            

		// 读取设备数据

		ssize_t num_bytes = read(device_fd, buffer, sizeof(buffer) - 1);
		if (num_bytes == -1) {
			perror("failed to read device");
			close(device_fd);
			close(sockfd);
			return 1;
		}
		//~ strcat(sendbuffer,buffer);
		//~ strcat(sendbuffer," ");
		ssize_t num_bytes_2 = read(device_fd_2, buffer2, sizeof(buffer) - 1);
		if (num_bytes_2 == -1) {
			perror("failed to read device");
			close(device_fd_2);
			close(sockfd);
			return 1;
		}
		strcat(sendbuffer,buffer);
		strcat(sendbuffer," ");
		strcat(sendbuffer,buffer2);
		//buffer[num_bytes] = '\0'; // 添加字符串结尾符号
		printf("sendbuffer = %s\n",sendbuffer);
		// 发送数据到服务器
		if (send(sockfd, sendbuffer, 6, 0) == -1) {
			perror("failed to send data");
			close(device_fd);
			close(sockfd);
			return 1;
		}

		// 关闭设备和 Socket
		close(device_fd);
		sleep(1);
	
	}

   
    
    
    close(sockfd);

    return 0;
}
