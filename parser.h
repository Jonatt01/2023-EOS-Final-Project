# ifndef PARSER_H
# define PARSER_H

# include "list_operation.h"

/**
* the set mode procedure (after confirming the username and mode)
*/
Node* setmode_parser(int user, int mode, int* mode_table);

/**
* convert the emergency command to struct
*/
Node* emergency_parser();

/**
* convert the normal control command to struct
*/
Node* control_parser(int* ischange, int* change, int user);

# endif