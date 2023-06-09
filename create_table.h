# ifndef CREATE_TABLE_H
# define CREATE_TABLE_H



/**
* create a shared device status table
*/
int* create_status_table(key_t key);

/**
* create user mode table
*/
int* create_mode_table(key_t key);
# endif