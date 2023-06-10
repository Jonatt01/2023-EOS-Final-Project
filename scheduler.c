# include <stdio.h>
# include <stdlib.h>

# include "list_operation.h"

void scheduler(Node** task_list_head, Node* head){

    // if it is emergency, the task list consist of only one task that is open the door
    if(head-> task.priority == 0)
        insertAtBeginning(task_list_head, head);
    else{
        insertAtEnd(task_list_head, head);
    }

}