# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int whichuser(char* name){
    if(strcmp(name,"Jonathan")==0){
        return 0;
    }
    else
        return 1;
}

int which_mode(char* timing){
    if(strcmp(name,"morning")==0){
        return 0;
    }
    else if(strcmp(name,"afternoon")==0){
        return 1;
    }
    else(strcmp(name,"night")==0){
        return 2;
    }
}