# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define BUFFERSIZE 1024
# define MAXUSERNUM 10

void setmode(int* user_mode, int user, int mode, int* settable){
    for(int i=0; i<12; i++){
        *(user_mode + 36*user + 12*mode + i) = settable[i];
    }

}

void setpreference(int* preference, int user,int* settable){

    for(int i=0; i<12; i++){
        *(preference + user*12 + i) = settable[i];
    }
    
}

void setusingtime(int* expect_using_time, int user, int* settable){
    for(int i=0; i<12; i++){
        *(expect_using_time + user*12 + i) = settable[i];
    }
}

void setwatt(int* expect_watt, int user, int* settable){
    for(int i=0; i<12; i++){
        *(expect_watt + user*12 + i) = settable[i];
    }   
}