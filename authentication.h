# ifndef AUTHENTICATION_H
# define AUTHENTICATION_H

typedef struct user{
    char id[20];
    char password[20];
}User;


/**
* send welcome information to client and Check the user id and password
*/
int welcome(int connfd);

/**
* NOT DONE : if first msg is "register <user> <password>", then he would want to sign up a account
*/
void signup(char* name, int connfd);


void deleteUser(char* id, int connfd);

void printUserTable(User* users);
# endif