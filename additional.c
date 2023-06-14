# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h> // read write
# include <sys/time.h> // gettimeofday

# define BUFFERSIZE 1024

# define AIRCONDITION_WATT 5
# define LIGHT_WATT 1
# define FAN_WATT 2

# define MONEY_PER_WATT 5


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

void check_temperature(int connfd, int* temperature, int* preference, int user){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    for(int i=0; i<4; i++){
        if(i==0){

            printf("Temperature in bedroom : %d, Temperature in preference: %d\n", *(temperature) , *(preference + 12*user) );

            if( *(temperature) > *(preference + 12*user) ){
                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bedroom is too high.\n");
                write(connfd,snd,msglen+1);
            }
        }

        if(i==1){
            
            printf("Temperature in living room : %d, Temperature in preference: %d\n", *(temperature + 1) , *(preference + 12*user + 4) );

            if( *(temperature + 1) > *(preference + 12*user + 4) ){
                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in living room is too high.\n");
                write(connfd,snd,msglen+1);
            }
        }

        if(i==3){

            printf("Temperature in bathroom : %d, Temperature in preference: %d\n", *(temperature + 2) , *(preference + 12*user + 9) );

            if( *(temperature + 2) > *(preference + 12*user + 9) ){
                sleep(0.00001);
                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bathroom is too high.\n");
                write(connfd,snd,msglen+1);
            }
        }            
    }
}

void check_using_time(int connfd, int* status, int* using_time, int* start_time, int* expect_use_time, int user){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

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

        if( total_time > *(expect_use_time + 12*user + i) ){
            sleep(0.00001);
            memset(snd,0,BUFFERSIZE);
            msglen = sprintf(snd,"Using time of %s is too long.\n",arr[i]);
            write(connfd,snd,msglen+1);
        }
    }    
}

void check_using_watt(int connfd, int* device_status, int* expect_watt, int user){

    int total_watt = 0;
    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

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
    if( total_watt > *(expect_watt + user) ){
        sleep(0.00001);
        memset(snd,0,BUFFERSIZE);
        msglen = sprintf(snd,"Using watt of is too high.\n");
        write(connfd,snd,msglen+1);
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