#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "dispatcher.h"

#define OPEN 1
#define CLOSE 0

void device_reservation(int device_id, int duration, int *status_shm, int reservation_operation)
{
    signal(SIGALRM, signal_handler); // 設定信號處理函數

    alarm(duration);
    printf("設定裝置 %d , %d 秒後動作\n", device_id, duration);

    // 等待設備開啟

    if (reservation_operation == OPEN)
    {
        while (status_shm[device_id - 1] == CLOSE) // 等待狀態變為 1 (OPEN)
        {
            sleep(1);
        }
    }
    else
    {
        while (status_shm[device_id - 1] == OPEN) // 等待狀態變為 0 (CLOSE)
        {
            sleep(1);
        }
    }
}

