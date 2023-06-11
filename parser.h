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

/**
* convert the reservation command to struct
*/
Node* reservation_parser(int* ischange, int* change, int user, int time);

/**
* convert the room comfort command to struct
*/
<<<<<<< HEAD
Node* room_preference_parser();
=======
Node* room_preference_parser(int* ischange, int user, int* preference);
>>>>>>> 3f5d8542b3e2ab6da36a1b68546ac28649c12c11

# endif