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

int whichdevice(char* place, char* device){

    if(strcmp(place,"bedroom")==0){
        if(strcmp(device,"airconditioner")==0)
            return 1;
        else if(strcmp(device,"light")==0)
            return 2;
        else if(strcmp(device,"fan")==0)
            return 3;
        else if(strcmp(device,"curtain")==0)
            return 4;
    }

    if(strcmp(place,"livingroom")==0){
        if(strcmp(device,"airconditioner")==0)
            return 5;
        else if(strcmp(device,"light")==0)
            return 6;
        else if(strcmp(device,"fan")==0)
            return 7;
        else if(strcmp(device,"curtain")==0)
            return 8;
    }

    if(strcmp(place,"kitchen")==0)
        return 9;

    if(strcmp(place,"bathroom")==0){
        if(strcmp(device,"airconditioner")==0)
            return 10;
        else if(strcmp(device,"light")==0)
            return 11;
    }

    if(strcmp(place,"all")==0)
        return 12;
}

int whichplace(char* place){
    if(strcmp(place,"bedroom")==0){
        return 0;
    }
    if(strcmp(place,"livingroom")==0){
        return 1;
    }
    if(strcmp(place,"kitchen")==0){
        return 2;
    }
    if(strcmp(place,"bathrooom")==0){
        return 3;
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
// 3*2 table print_table(table, 3, 2)
void print_int_table(int* array, int col, int row){
    printf("=================================\n");
    for(int i=0; i<col; i++){
        for(int j=0; j<row; j++){
            printf("%d ",*(array+i*row+j));
        }
        printf("\n");
    }
    printf("=================================\n");
}