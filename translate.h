# ifndef TRANSLATE_H
# define TRANSLATE_H

/**
* translate user name to index of table
*/
int whichuser(char* name, User* users);

/**
* translate morning/afternoon/night to index of table
*/
int whichmode(char* timing);

int whichdevice(char* place, char* device);

int whichplace(char* place);

void remove_spaces(char* s);

void print_int_table(int* array, int col, int row);
# endif