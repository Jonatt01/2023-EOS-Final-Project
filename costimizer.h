# ifndef COSTIMIZER_H
# define COSTIMIZER_H



/**
* the set mode procedure (after confirming the username and mode)
*/
void setmode(int* user_mode, int user, int mode, int* settable);

/**
* the set preference table
*/
void setpreference(int* preference, int user, int* settable);

void setusingtime(int* expect_using_time, int user, int* settable);

void setwatt(int* expect_watt, int user, int expect_watt_value);

# endif