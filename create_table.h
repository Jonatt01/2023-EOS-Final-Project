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
/**
* create device usage time table
*/
int* create_start_time_table(key_t key);
/**
* create device usage time table
*/
int* create_using_time_table(key_t key);

/**
* create preference table
*/
int* create_preference_table(key_t key);

int* create_temperature_table(key_t key);

int* create_expect_using_time_table(key_t key);

int* create_expect_watt_table(key_t key);

# endif