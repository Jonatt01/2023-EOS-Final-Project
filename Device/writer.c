#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#define NUM_GPIO_PINS 3
#define MAX_GPIO_NUMBER 4
#define BUF_SIZE 50
#define PATH_SIZE 20
int main(int argc, char **argv)
{
    char path[PATH_SIZE], buf[BUF_SIZE];
    int i = 0, fd = 0;
    //int id;

    // 讀取字符串 "/dev/etx_device" 並存儲在 path 中 , 大小為 sizeof(path) -1 (最後一個為結尾符)
    snprintf(path, sizeof(path), "/dev/etx_device");
    fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror("Error opening GPIO pin");
        exit(EXIT_FAILURE);
    }
    //id=strtol(argv[1],NULL,0);
    //printf("%d",id);
    printf("Set GPIO pins to output, logic level :\n");

    memset(buf,0,BUF_SIZE);
    for(int i = 1;i<=12;i++){
        strcat(buf,argv[i]);
        strcat(buf," ");
    }

    //buf[1] = '\0';
    printf("buffer = %s , ready to write.\n",buf);
    if (write(fd, buf, sizeof(buf)) < 0) {
        perror("write, set pin output");
        exit(EXIT_FAILURE);
    }

    printf("write succesfully\n");

    return EXIT_SUCCESS;
}
