#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
# include <sys/shm.h>
#include "reservation_signal.h"
#include "list_operation.h"

#define MSG_TYPE 1
#define MSG_Q_KEY 1111

#define RELAY 0
#define RESERVATION 1
#define CALCULATE 2

#define OPEN 1
#define CLOSE 0

#define STATUS_SIZE 12 * sizeof(int)

# define AIRCONDITION_WATT 5
# define LIGHT_WATT 1
# define FAN_WATT 2

int reservation_device_id = 0;
int reservation_operation = -1;
int reservation_data[4];

pid_t reservaion_child;
key_t msgQ_key;
int msg_queue_id;

// msgQ message format
struct message
{
    long msg_type;
    int data[4];
};

#define DEVICE_ID 0
#define LEVEL 1
#define TEMP 2
#define DURATION 3

void signal_handler(int signum)
{
    if (signum == SIGALRM)
    {
        key_t dev_status_key = 1234;
        int status_shm_id;
        int *device_status;
        int *status_shm;

        if ((status_shm_id = shmget(dev_status_key, STATUS_SIZE, IPC_CREAT | 0666)) < 0)
        {
            perror("shmget");
            exit(-1);
        }
        if ((status_shm = shmat(status_shm_id, NULL, 0)) == (int *)-1)
        {
            perror("shmat");
            exit(-1);
        }
        
        // signal send 給 relay
        // 將要signal給Relay的資料，放進msgQ中
        msgQ_key = MSG_Q_KEY;
        msg_queue_id = msgget(msgQ_key, IPC_CREAT | 0666); // get msgQ
        if (msg_queue_id == -1)
        {
            perror("msgget error");
        }
        struct message msg;
        msg.msg_type = MSG_TYPE;
        msg.data[DEVICE_ID] = reservation_data[DEVICE_ID];
        msg.data[LEVEL] = reservation_data[LEVEL];
        msg.data[TEMP] = reservation_data[TEMP];
        msg.data[DURATION] = reservation_data[DURATION];

        if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
        //printf("設備 %d 預定完成，狀態已設定為 %d !\n", reservation_device_id, reservation_operation);
    }
}

int get_command_type(Node *head)
{

    if (head->task.device != 0 && head->task.reservation == 0)
    {
        return RELAY;
    }
    if (head->task.reservation == 1 && head->task.device != 0)
    {
        return RESERVATION;
    }
    if (head->task.calculate == 1)
    {
        return CALCULATE;
    }
}

int get_reservation_operation(Node *head)
{
    if (head->task.level > 0 || head->task.temp > 0)
    {
        return OPEN;
    }
    else
    {
        return CLOSE;
    }
}

void dispatcher(Node *head, int* status_shm)
{
    while (head != NULL)
    {
        printf("Strating dispatch ..\n");
        // 判斷進入哪個流程，分為 relay、reservation、calculate
        int command_type = -1;
        command_type = get_command_type(head);
        reservation_operation = get_reservation_operation(head);
        printf("command type = %d\n",command_type);
        printf("device_id = %d\n",head->task.device);
        switch (command_type)
        {
        case RELAY:
            printf("command for RELAY\n");
            msgQ_key = MSG_Q_KEY;
            msg_queue_id = msgget(msgQ_key, IPC_CREAT | 0666); // get msgQ
            if (msg_queue_id == -1)
            {
                perror("msgget error");
            }

            struct message msg;
            // 儲存要送給Relay的資料，放進msgQ中
            msg.msg_type = MSG_TYPE;
            msg.data[DEVICE_ID] = head->task.device;
            msg.data[LEVEL] = head->task.level;
            msg.data[TEMP] = head->task.temp;
            msg.data[DURATION] = head->task.duration;
            printf("msg.data[DEVICE_ID] = %d\n",msg.data[DEVICE_ID]);
            printf("msg.data[LEVEL] = %d\n",msg.data[LEVEL]);
            printf("msg.data[TEMP] = %d\n", msg.data[TEMP]);
            printf("msg.data[DURATION] = %d\n",msg.data[DURATION]);
           
            if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1)
            {
                perror("msgsnd error");
                exit(1);
            }
            printf("msgQ command send!\n");
            break;
        case RESERVATION:
            reservaion_child = fork();
            if (reservaion_child == 0)
            {
                reservation_device_id = head->task.device;
                reservation_data[DEVICE_ID] = head->task.device;
                reservation_data[LEVEL] = head->task.level;
                reservation_data[TEMP] = head->task.temp;
                reservation_data[DURATION] = head->task.reservation_time;
                device_reservation(head->task.device, head->task.reservation_time, status_shm, reservation_operation);
                exit(EXIT_SUCCESS);
            }
            else
            {
                // do nothing
            }
            break;
        case CALCULATE:
            break;

        default:
            break;
        }

        removeHeadNode(&head);
    }
}
