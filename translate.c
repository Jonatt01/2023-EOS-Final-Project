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

int whichmode(char* timing){
    if(strcmp(timing,"morning")==0){
        return 0;
    }
    else if(strcmp(timing,"afternoon")==0){
        return 1;
    }
    else if(strcmp(timing,"night")==0){
        return 2;
    }
}

void remove_spaces(char* s) {
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}