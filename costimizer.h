# ifndef COSTIMIZER_H
# define COSTIMIZER_H



/**
* the set mode procedure (after confirming the username and mode)
*/
void setmode(int connfd, int* user_mode, int user, int mode);

/**
* the set preference table
*/
void setpreference(int connfd, int* preference, int user);

# endif