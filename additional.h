# ifndef ADDITIONAL_H
# define ADDITIONAL_H

/**
* calculate the bill and return to user
*/
void calculate_bill(int connfd);

/**
* check out the status and return to user
*/
void inquire_status(int connfd);


void inquire_using_time(int connfd);

/**
* check if the temperature is higher than user comfort
*/
void check_the_temperature(int connfd);
# endif