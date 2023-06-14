# ifndef ADDITIONAL_H
# define ADDITIONAL_H


/**
* check out the status and return to user
*/
void inquire_status(int connfd, int* status, int* ischeck);

/**
* check out the using time and return to user
*/
void inquire_using_time(int connfd, int* ischeck, int* using_time, int* start_time, int* status);

/**
* check out the temperature is higher than user comfort
*/
void inquire_temperature(int connfd, int* temperature, int* ischeck);


/**
* check if the temperature is higher than user comfort
*/
void check_temperature(int connfd, int* temperature, int* preference, int user);

/**
* check if the total using time of devices is higher than user expected (recommand)
*/
void check_using_time(int connfd, int* status, int* using_time, int* start_time, int* expect_use_time, int user);

/**
* check if the total using watt of devices is higher than user expected (recommand)
*/
void check_using_watt(int connfd, int* watt, int* expect_watt, int user);





/**
* calculate the bill and return to user (will be called by dispatcher)
*/
void calculate_bill(int connfd, int* using_time, int* start_time, int* watt, int* status);

# endif