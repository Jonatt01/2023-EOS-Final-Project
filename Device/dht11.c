#include <wiringPi.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define DHT_PIN 31


int readDHT(int data[5], int pin) {
    unsigned char bits[5];
    unsigned char cnt = 7;
    unsigned char idx = 0;

    // 初始化数据数组
    for (int i = 0; i < 5; ++i)
        data[i] = 0;

    // 发送起始信号
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    // 等待传感器响应
    unsigned int loopCnt = 10000;
    while (digitalRead(pin) == LOW) {
        if (--loopCnt == 0)
            return -1;
    }

    loopCnt = 10000;
    while (digitalRead(pin) == HIGH) {
        if (--loopCnt == 0)
            return -1;
    }

    // 读取数据
    for (int i = 0; i < 40; ++i) {
        loopCnt = 10000;
        while (digitalRead(pin) == LOW) {
            if (--loopCnt == 0)
                return -1;
        }

        unsigned int t = micros();

        loopCnt = 10000;
        while (digitalRead(pin) == HIGH) {
            if (--loopCnt == 0)
                return -1;
        }

        if ((micros() - t) > 40)
            bits[idx] |= (1 << cnt);

        if (cnt == 0) {
            cnt = 7;
            ++idx;
        } else {
            --cnt;
        }
    }

    // 将数据从位数组转换为整数数组
    data[0] = bits[0];
    data[1] = bits[1];
    data[2] = bits[2];
    data[3] = bits[3];
    data[4] = bits[0] + bits[1] + bits[2] + bits[3];

    return 0;
}

int main() {
    if (wiringPiSetup() == -1) {
        perror("wiringPiSetup failed");
        return 1;
    }

    int fd = open("/dev/dht11", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    while (1) {
        int data[5] = {0};
        int result = readDHT(data, DHT_PIN);

        if (result == 0 && data[2] > 15 && data[2] < 35) {
			
            char buffer[32];
            printf("Temp:%d\n", data[2]);
            sprintf(buffer, "%d\n", data[2]);
            lseek(fd,0,SEEK_SET);
            write(fd, buffer, strlen(buffer));
        } else {
            //printf("Failed to read DHT11\n");
        }

        delay(1000);
    }

    close(fd);

    return 0;
}
