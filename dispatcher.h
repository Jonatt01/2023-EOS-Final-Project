# ifndef DISPATCHER_H
# define DISPATCHER_H
# include "list_operation.h"

void signal_handler(int signum);
int get_command_type(Node *head);
int get_reservation_operation(Node *head);
void dispatcher(Node *head,int* status_shm);
#endif