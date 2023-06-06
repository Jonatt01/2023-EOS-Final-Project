#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 100
#define MSG_TYPE 1

struct message {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int main() {
    key_t key;
    int msg_queue_id;
    struct message msg;

    // 產生唯一的 key
    key = ftok(".", 'a');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 取得訊息佇列
    msg_queue_id = msgget(key, 0666);
    if (msg_queue_id == -1) {
        perror("msgget");
        exit(1);
    }

    while(1){
        // 傳送訊息給伺服器
        printf("請輸入要傳送的訊息：");
        fgets(msg.msg_text, MAX_MSG_SIZE, stdin);
        msg.msg_type = MSG_TYPE;

        if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    return 0;
}
