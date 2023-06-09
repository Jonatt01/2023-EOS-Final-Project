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