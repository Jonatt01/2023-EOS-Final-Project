# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h> // read write
# include <sys/time.h> // gettimeofday
# include <sys/msg.h> // msg queue

# define BUFFERSIZE 1024

# define AIRCONDITION_WATT 5
# define LIGHT_WATT 1
# define FAN_WATT 2

# define MONEY_PER_WATT 5

#define MSG_TYPE 2


key_t msg_queue_key;
int msg_queue_id;

// msgQ message format
struct message
{
    long msg_type;
    char message[50];
};

void inquire_temperature(int connfd, int* temperature, int* ischeck){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    for(int i=0; i<4; i++){
        if(ischeck[i] == 1){
            if(i==0){

                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bedroom : %d\n", *(temperature));
                write(connfd,snd,msglen+1);

            }

            if(i==1){

                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in living room : %d\n", *(temperature + 1));
                write(connfd,snd,msglen+1);

            }

            if(i==3){

                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bathroom : %d\n", *(temperature + 2));
                write(connfd,snd,msglen+1);
            }            
        }
    }
}

void inquire_status(int connfd, int* ischeck, int* status){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    char *arr[] = {"Bedroom airconditioner", "Bedroom light", "Bedroom fan", "Bedroom cutain", "Living room airconditioner", "Living room light", "Living room fan", "Living room cutain", "Kitchen light", "Bathroom airconditioner", "Bathroom light", "Doors"};
    
    for(int i=0; i<12; i++){

        if(ischeck[i]==1){

            if( *(status+i) >= 1 ){
                sleep(0.00001);
                msglen = sprintf(snd,"%s is open.\n", arr[i]);
                write(connfd,snd,msglen+1);
                printf("%s.\n",snd);
                sleep(1);
            }
            else if( *(status+i) == 0 ){
                sleep(0.00001);
                msglen = sprintf(snd,"%s is close.\n", arr[i]);
                write(connfd,snd,msglen+1);
                printf("%s.\n",snd);
                sleep(1);
            }
        }
    }
}

void inquire_using_time(int connfd, int* ischeck, int* using_time, int* start_time, int* status){
    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    char *arr[] = {"Bedroom airconditioner", "Bedroom light", "Bedroom fan", "Bedroom cutain", "Living room airconditioner", "Living room light", "Living room fan", "Living room cutain", "Kitchen light", "Bathroom airconditioner", "Bathroom light", "Doors"};
    
    for(int i=0; i<12; i++){
        
        // do not need to check curtain using time
        if(i==3 | i==7) continue;

        if(ischeck[i]==1){
            if( *(status+i) >= 1 ){

                int total_time = 0;
                struct timeval now_system_time;

                gettimeofday(&now_system_time,NULL); // get current time

                total_time = *(using_time+i) + (int)now_system_time.tv_sec - *(start_time+i);
                sleep(0.00001);
                msglen = sprintf(snd,"%s using time : %d.\n", arr[i], total_time);
                write(connfd,snd,msglen+1);
            }
            else if( *(status+i) == 0 ){

                int total_time = 0;
                total_time = *(using_time+i);
                sleep(0.00001);
                msglen = sprintf(snd,"%s using time : %d.\n", arr[i], total_time);
                write(connfd,snd,msglen+1);
            }
        }
    }

}

void check_temperature(int connfd, int* temperature, int* preference, int user, int msg_queue_id){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};
    // printf("out msg queue id : %d\n",msg_queue_id);

    struct message msg;


    for(int i=0; i<4; i++){
        if(i==0){

            printf("Temperature in bedroom : %d, Temperature in preference: %d\n", *(temperature) , *(preference + 12*user) );

            if( *(temperature) > *(preference + 12*user)  & *(preference + 12*user) != 0){
                
                printf("Temperature in bedroom is too high.\n");

                msg.msg_type = MSG_TYPE;
                memset(msg.message,0,50);
                strcat(msg.message,"Temperature in bedroom is too high.\n");
                printf("message %s\n",msg.message);

                sleep(0.00001);
                printf("msg queue id : %d\n", msg_queue_id);
                // write to message queue
                if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long) , 0) == -1){
                    perror("msgsnd error");
                    exit(1);
                }
                printf("msg queue in i==0 command send!\n");
            }
        }

        if(i==1){
            
            printf("Temperature in living room : %d, Temperature in preference: %d\n", *(temperature + 1) , *(preference + 12*user + 4) );

            if( *(temperature + 1) > *(preference + 12*user + 4) & *(preference + 12*user + 4) != 0 ){
                
                printf("Temperature in living room is too high.\n");

                msg.msg_type = MSG_TYPE;
                memset(msg.message,0,50);
                strcat(msg.message,"Temperature in living room is too high.\n");

                sleep(0.00001);
                // write to message queue
                if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1){
                    perror("msgsnd error");
                    exit(1);
                }
                printf("msg queue in i==1 command send!\n");
            }
        }

        if(i==3){

            printf("Temperature in bathroom : %d, Temperature in preference: %d\n", *(temperature + 2) , *(preference + 12*user + 9) );

            if( *(temperature + 2) > *(preference + 12*user + 9) & *(preference + 12*user + 9) != 0 ){
                printf("Temperature in bathroom is too high.\n");

                msg.msg_type = MSG_TYPE;
                memset(msg.message,0,50);
                strcat(msg.message,"Temperature in bathroom is too high.\n");;

                sleep(0.00001);
                // write to message queue
                if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1){
                    perror("msgsnd error");
                    exit(1);
                }
                printf("msg queue in i==3 command send!\n");
            }
        }            
    }
}

void check_using_time(int connfd, int* status, int* using_time, int* start_time, int* expect_use_time, int user, int msg_queue_id){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};
    struct message msg;

    char *arr[] = {"Bedroom airconditioner", "Bedroom light", "Bedroom fan", "Bedroom cutain", "Living room airconditioner", "Living room light", "Living room fan", "Living room cutain", "Kitchen light", "Bathroom airconditioner", "Bathroom light", "Doors"};
    
    for(int i=0; i<12; i++){
        
        int total_time = 0;

        // do not need to check curtain using time
        if(i==3 | i==7 | i==11) continue;


        if( *(status+i) >= 1 ){


            struct timeval now_system_time;

            gettimeofday(&now_system_time,NULL); // get current time

            total_time = *(using_time+i) + (int)now_system_time.tv_sec - *(start_time+i);

            printf("%s using time : %d.\texpect using time : %d\n", arr[i] ,total_time, *(expect_use_time + 12*user + i) );

        }
        else if( *(status+i) == 0 ){

            total_time = *(using_time+i);

            printf("%s using time : %d.\texpect using time : %d\n", arr[i] ,total_time, *(expect_use_time + 12*user + i) );
        }

        if( total_time == *(expect_use_time + 12*user + i) ){

            sleep(0.00001);

            msg.msg_type = MSG_TYPE;
            memset(msg.message,0,50);
            memset(snd,0,BUFFERSIZE);
            sprintf(snd,"Using time of %s is too long.\n",arr[i]);
            strcat(msg.message,snd);
            printf("message %s\n",msg.message);

            sleep(0.00001);
            printf("msg queue id : %d\n", msg_queue_id);

            // write to message queue
            if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long) , 0) == -1){
                perror("msgsnd error");
                exit(1);
            }
            printf("msg queue in using time command send. i = %d!\n",i);
        }
    }    
}

void check_using_watt(int connfd, int* device_status, int* expect_watt, int user, int msg_queue_id){

    int total_watt = 0;
    int msglen = 0;

    struct message msg;

    for(int i=0; i<12; i++){
        
        int now_watt = 0;
        if( i==0 | i==4 | i==9 ) now_watt = AIRCONDITION_WATT;
        else if( i==1 | i==5 | i==8 | i==10) now_watt = LIGHT_WATT;
        else if( i==2 | i==6) now_watt = FAN_WATT;

        // do not need to check curtain using time
        if(i==3 | i==7) continue;

        total_watt += *(device_status + i)*now_watt;

    } 
    printf("total watt: %d, expect watt: %d\n", total_watt, *(expect_watt + user));
    if( total_watt > *(expect_watt + user) & *(expect_watt + user) != 0 ){

        printf("Using watt of is too high.\n");

        msg.msg_type = MSG_TYPE;
        memset(msg.message,0,50);
        strcat(msg.message,"Using watt of is too high.\n");

        sleep(0.00001);
        // write to message queue
        if (msgsnd(msg_queue_id, &msg, sizeof(struct message) - sizeof(long), 0) == -1){
            perror("msgsnd error");
            exit(1);
        }
        printf("msg queue in watt send!\n");
        
    }
}

void calculate_bill(int connfd, int* using_time, int* start_time, int* status){
    
    int bill = 0;
    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    for(int i=0; i<12; i++){
        
        int total_time = 0;

        int now_watt = 0;
        if( i==0 | i==4 | i==9 ) now_watt = AIRCONDITION_WATT;
        else if( i==1 | i==5 | i==8 | i==10) now_watt = LIGHT_WATT;
        else if( i==2 | i==6) now_watt = FAN_WATT;

        if(i==3 | i==7) continue;

        if( *(status+i) >= 1 ){

            struct timeval now_system_time;

            gettimeofday(&now_system_time,NULL); // get current time
            total_time = *(using_time+i) + (int)now_system_time.tv_sec - *(start_time+i);
        }
        else if( *(status+i) == 0 ){
            total_time = *(using_time+i);
        }

        bill += total_time * now_watt * MONEY_PER_WATT;
        printf("bill: %d\n", bill);
    }
    printf("Total bill: %d\n", bill);

    sleep(0.00001);
    memset(snd,0,BUFFERSIZE);
    msglen = sprintf(snd,"Total bill: %d\n", bill);
    write(connfd,snd,msglen+1);
}