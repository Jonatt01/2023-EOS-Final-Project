# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h> // read write
# include <sys/time.h> // gettimeofday

# define BUFFERSIZE 1024



void inquire_temperature(int connfd, int* temperature, int* ischeck){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    for(int i=0; i<4; i++){
        if(ischeck[i] == 1){
            if(i==0){

                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bedroom : %d\n", *(temperature));
                write(connfd,snd,msglen+1);

            }

            if(i==1){

                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in living room : %d\n", *(temperature + 1));
                write(connfd,snd,msglen+1);

            }

            if(i==3){

                memset(snd,0,BUFFERSIZE);
                msglen = sprintf(snd,"Temperature in bathroom : %d\n", *(temperature + 2));
                write(connfd,snd,msglen+1);
            }            
        }
    }
}

void inquire_status(int connfd, int* status, int* ischeck){

    int msglen = 0;
    char snd[BUFFERSIZE] = {0},rcv[BUFFERSIZE] = {0};

    char *arr[] = {"Bedroom airconditioner", "Bedroom light", "Bedroom fan", "Bedroom cutain", "Living room airconditioner", "Living room light", "Living room fan", "Living room cutain", "Kitchen light", "Bathroom airconditioner", "Bathroom light", "Doors"};
    
    for(int i=0; i<12; i++){
        if(ischeck[i]==1){
            if( *(status+i) >= 1 ){
                msglen = sprintf(snd,"%s is open.\n", arr[i]);
                write(connfd,snd,msglen+1);
            }
            else if( *(status+i) == 0 ){
                msglen = sprintf(snd,"%s is close.\n", arr[i]);
                write(connfd,snd,msglen+1);
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

                msglen = sprintf(snd,"%s using time : %d.\n", arr[i], total_time);
                write(connfd,snd,msglen+1);
            }
            else if( *(status+i) == 0 ){

                int total_time = 0;
                total_time = *(using_time+i);

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

            memset(rcv,0,BUFFERSIZE);
            memset(snd,0,BUFFERSIZE);
            msglen = sprintf(snd,"Temperature in bedroom : %d\tTemperature in preference: %d\n", *(temperature) , *(preference + 12*user) );
            write(connfd,snd,msglen+1);

            if( *(temperature) > *(preference + 12*user) ){
                msglen = sprintf(snd,"Temperature in bedroom is too high.\n");
                write(connfd,snd,msglen+1);
            }
        }

        if(i==1){

            memset(rcv,0,BUFFERSIZE);
            memset(snd,0,BUFFERSIZE);
            msglen = sprintf(snd,"Temperature in living room : %d\tTemperature in preference: %d\n", *(temperature + 1) , *(preference + 12*user + 4) );
            write(connfd,snd,msglen+1);

            if( *(temperature + 1) > *(preference + 12*user + 4) ){
                msglen = sprintf(snd,"Temperature in living room is too high.\n");
                write(connfd,snd,msglen+1);
            }
        }

        if(i==3){

            memset(rcv,0,BUFFERSIZE);
            memset(snd,0,BUFFERSIZE);
            msglen = sprintf(snd,"Temperature in bathroom : %d\tTemperature in preference: %d\n", *(temperature + 2) , *(preference + 12*user + 9) );
            write(connfd,snd,msglen+1);

            if( *(temperature + 2) > *(preference + 12*user + 9) ){
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
        if(i==3 | i==7) continue;


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

            memset(snd,0,BUFFERSIZE);
            msglen = sprintf(snd,"Using time of %s is too long.\n",arr[i]);
            write(connfd,snd,msglen+1);
        }
    }    
}