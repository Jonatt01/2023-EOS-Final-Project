#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "dispatcher.h"

int deviceID = 0;
int device_status = 0; // 0表示設備關閉，1表示設備開啟

int device_reservation(int device_id,int duration)
{
    signal(SIGALRM, signal_handler); // 設定信號處理函數

    alarm(duration);
    printf("設定裝置 %d , %d 秒後動作\n", deviceID,duration);

    // 等待設備開啟
    while (device_status == 0)
    {
        sleep(1);
    }

    return 1;
}
