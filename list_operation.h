# ifndef LIST_OPERATION_H
# define LIST_OPERATION_H

typedef struct Task {
    int priority;
    int user; // 1-10   
    int reservation; // 0 or 1
    int reservation_time; // minute
    int place; // useless
    int device; // 1-12
    int level; 
    int temp; // for air conditioner
    int duration;
    int calculate; // 0 or 1
} Task;

// struct of the linked list node
typedef struct Node {
    Task task;
    struct Node* next;
} Node;


Node* createNode();
void insertAtBeginning(Node** head,  Node* newNode);
void insertAtEnd(Node** head,  Node* newNode);
void displayList(Node* head);
void removeHeadNode(Node** head);

# endif