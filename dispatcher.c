#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "reservation_signal.h"

#define MSG_TYPE 1
#define MSG_Q_KEY 1111

#define RELAY 0
#define RESERVATION 1
#define CALCULATE 2

#define OPEN 1
#define CLOSE 0

#define STATUS_SIZE 12*sizeof(int)

int reservation_device_id = 0;
int reservation_operation = -1;
pid_t reservaion_child;
key_t msgQ_key;

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

typedef struct Task
{
    int user;
    int priority;
    int device;
    int level;
    int place;
    int temp;
    int reservation;
    int duration;
    int calculate;
} Task;

typedef struct Node
{
    Task task;
    struct Node *next;
} Node;


void signal_handler(int signum)
{
    if (signum == SIGALRM)
    {
        key_t dev_status_key = 1234;
        int status_shm_id;
        int* device_status;
        int *status_shm;

        if ((status_shm_id = shmget(dev_status_key, STATUS_SIZE, IPC_CREAT | 0666)) < 0)
        {
            perror("shmget");
            exit(-1);
        }
        if ((status_shm = shmat(status_shm_id, NULL, 0)) == (int *) -1)
        {
            perror("shmat");
            exit(-1);
        }
        
        // 設備開啟，寫進共享內存
        if(reservation_operation == OPEN)
        {
            status_shm[reservation_device_id - 1] = OPEN;      
        }
        else if(reservation_operation == CLOSE)
        {
            status_shm[reservation_device_id - 1] = CLOSE;
        }
        //device_status = 1; 
        printf("設備 %d 預定完成，狀態已設定為 %d !\n",reservation_device_id,reservation_operation);
    }
}


int get_command_type(Node *head){

    if(head->task.device != 0)
    {
        return RELAY;
    }
    if(head->task.reservation == 1)
    {
        return RESERVATION;
    }
    if(head->task.calculate == 1)
    {
        return CALCULATE;
    }

}

int get_reservation_operation(Node *head)
{
    if(head->task.level > 0 || head->task.temp > 0){
        return OPEN;
    }
    else{
        return CLOSE;
    }
}

void dispatcher(Node *head)
{

    // 判斷進入哪個流程
    int command_type = -1;
    get_command_type(head);
    reservation_operation = get_reservation_operation(head);

    
    switch (command_type)
    {
    case RELAY:

        msgQ_key = MSG_Q_KEY;
        int msg_queue_id = msgget(msgQ_key, 0666); // get msgQ
        if (msg_queue_id == -1)
        {
            perror("msgget error");
        }

        struct message msg;
        msg.msg_type = MSG_TYPE;
        msg.data[DEVICE_ID] = head->task.device;
        msg.data[LEVEL] = head->task.level;
        msg.data[TEMP] = head->task.temp;
        msg.data[DURATION] = head->task.duration;

        if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
        break;
    case RESERVATION:        

        reservaion_child = fork();
        if(reservaion_child == 0){
            reservation_device_id = head->task.device;
            device_reservation(head->task.device,head->task.duration);
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

}

