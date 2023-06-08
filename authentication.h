# ifndef AUTHENTICATION_H
# define AUTHENTICATION_H



/**
* send welcome information to client and Check the user id and password
*/
int welcome(int connfd);

/**
* if first msg is "register <user>", then he would want to sign up a account
*/
void add_user(char* name, int password);

// int find
# endif