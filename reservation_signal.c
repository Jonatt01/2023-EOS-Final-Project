#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include "dispatcher.h"

#define OPEN 1
#define CLOSE 0

void device_reservation(int device_id, int reservation_time, int *status_shm, int reservation_operation)
{
    signal(SIGALRM, signal_handler); // 設定信號處理函數

    alarm(reservation_time);
    printf("設定裝置 %d , %d 秒後動作\n", device_id, reservation_time);

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
    
    printf("設備 %d 預定完成，狀態已設定為 %d !\n", device_id, reservation_operation);
}

