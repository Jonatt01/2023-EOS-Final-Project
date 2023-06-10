#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

# include "list_operation.h"

int place_index[12] = {1,1,1,1,2,2,2,2,3,4,4,0};
int device_index[12] = {1,2,3,4,1,2,3,4,2,1,2,5};


Node* setmode_parser(int user, int mode, int* mode_table){
    Node* head = NULL;
    
    for(int i=0;i<12;i++){
        Node* newNode = createNode();

        newNode -> task.user = user;
        newNode -> task.device = i + 1;
        if(i == 0 | i == 4 | i == 9){
            newNode -> task.temp = *(mode_table + 36*user + 12*mode + i);
        }
        else{
            newNode -> task.level = *(mode_table + 36*user + 12*mode + i);
        }

        // insert to linked list
        insertAtEnd(&head, newNode);

    }
    // printf("In parser.c - the device value of the head : %d\n",head->task.device);
    return head;

}