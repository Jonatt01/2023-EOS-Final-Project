#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

int deviceID = 0;
int device_status = 0; // 0表示設備關閉，1表示設備開啟

// 設定信號處理函數，當接收到信號時執行
void signal_handler(int signum)
{
    if (signum == SIGALRM)
    {
        device_status = 1; // 設備開啟
        printf("設備 %d ,已開啟！\n",deviceID);
    }
}

int device_reservation()
{
    signal(SIGALRM, signal_handler); // 設定信號處理函數

    int hour = 0;
    int minute = 0;
    int sec = 0;
    printf("請輸入device id , 預約時間（秒）：");
    scanf("%d %d",&deviceID, &sec);

    alarm(sec);
    printf("設定裝置 %d , %d 秒後動作\n", deviceID,sec);

    // 等待設備開啟
    while (device_status == 0)
    {
        sleep(1);
    }

    return 1;
}
