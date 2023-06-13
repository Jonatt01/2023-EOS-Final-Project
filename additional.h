# ifndef ADDITIONAL_H
# define ADDITIONAL_H

/**
* calculate the bill and return to user
*/
void calculate_bill(int connfd, int* using_time);

/**
* check out the status and return to user
*/
void inquire_status(int connfd, int* status);

/**
* check out the using time and return to user
*/
void inquire_using_time(int connfd, int* using_time);

/**
* check if the temperature is higher than user comfort
*/
void check_the_temperature(int connfd, int* temperature, int* preference);

/**
* check if the total using time of devices is higher than user expected
*/
void check_using_time(int connfd, int* using_time, int* expect_using_time);

/**
* check if the total using watt of devices is higher than user expected
*/
void check_using_watt(int connfd, int* watt, int* expect_watt);
# endif